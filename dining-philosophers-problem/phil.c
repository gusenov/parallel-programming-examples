// Решение классической задачи синхронизации «Обедающие философы».

// Программа в виде Linux-приложения, для выполнения различных частей которой
// создаются и запускается потоки управления, а для синхронизации доступа к
// требуемым ресурсам используются соответствующие объекты ОС.

#include <stdbool.h>
#include <locale.h>
#include <stdlib.h>
#include <wchar.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
#include <semaphore.h>

// Количество философов (потоков) / вилок (разделяемых ресурсов):
#define CNT 5
#define MAX_BUFFER 1000
#define MAX_DIGITS 9

// В пансионе отдыхают и предаются размышлениям 5 философов (потоки),
// пронумерованные от 1 до 5:
int philosophers_no[CNT] = { 1, 2, 3, 4, 5 };

// Начальное состояние CNT философов:
// (true - свободен; false - в столовой)
int philosophers[CNT] = { true, true, true, true, true};

// Начальное состояние CNT вилок:
// (true - вилка свободна; false - вилка поднята)
int forks[CNT] = { true, true, true, true, true};

// Синхронизирующие объекты ОС для синхронизации доступа к требуемым ресурсам:
pthread_mutex_t forks_mutex;
pthread_mutex_t philosophers_mutex;

// Величина временного промежутка для поглощения пищи:
struct timespec time_value;

sem_t s;

void* dinner(void* arg);
void put_down_both_forks(int rfork, int lfork);

int input_time_value() {
  int val;

  wchar_t str[MAX_BUFFER];
  int len = sizeof(str) / sizeof(wchar_t);

  wchar_t* ch = NULL;

  do {
    wprintf(L"Введите величину временного промежутка (с) для поглощения пищи (макс. %d цифр)>", MAX_DIGITS);
    fgetws(str
      , len  // мак. количество символов для чтения, включая нуль-символ.
      , stdin
    );
    str[wcscspn(str, L"\n")] = '\0';
    wprintf(L"Вы ввели: '%ls', длина: %d символов.\n", str, wcslen(str));

    // Устойчивость к некорректному пользовательскому вводу:

    if (wcslen(str) == 0) {
      wprintf(L"Пустая строка!\n");
      continue;
    } else  if (*str == L' ') {
      wprintf(L"Пробел в начале строки!\n");
      continue;
    } else if (wcslen(str) > MAX_DIGITS) {
      wprintf(L"Длина строки больше %d символов!\n", MAX_DIGITS);
      continue;
    }

    val = (int)wcstol(str, &ch, 10);

    if (val < 0) {
      wprintf(L"Отрицательное значение времени на поглощение пищи философом!\n");
      continue;
    } if (*ch != '\0') {
      wprintf(L"Некорректный пользовательский ввод!\n");
      continue;
    } else {
      wprintf(L"Значение времени на поглощение пищи философом = %d с\n", val);
      break;
    }
  } while (true);

  return val;
}

int write_output(int philosopher, const wchar_t* format, ...) {
    sem_wait(&s);

    va_list args;
    va_start(args, format);

    // Отступ:
    for (int i = 0; i < (philosopher - 1) * 32; ++i)
      wprintf(L" ");

    int result = vwprintf(format, args);

    va_end(args);

    sem_post(&s);

    return result;
}

int main(int argc, char* argv[]) {
  setlocale(LC_ALL, "");
  srand((long)time(NULL));

  sem_init(&s, 0, 1);

  // Величина временного промежутка для поглощения пищи устанавливается пользователем:
  time_value.tv_sec = input_time_value();

  pthread_t threads[CNT];

  while (true) {  // "вечный" цикл.
    // Функционирование программы, может
    // быть завершено только путем принудительного снятия процесса с выполнения.

    // Появление философа в столовой является случайной
    // величиной с равномерным законом распределения:
    int i = rand() % CNT;  // сгенерировать псевдослучайное число в диапазоне [0, 5).

    pthread_mutex_lock(&philosophers_mutex);
    if (philosophers[i]) {
      // Для того чтобы пообедать,
      // философ входит в столовую и садится на стул со своим номером:
      philosophers[i] = false;
      pthread_mutex_unlock(&philosophers_mutex);
      write_output(philosophers_no[i], L"Философ %d идёт в столовую.\n", philosophers_no[i]);
      if (pthread_create(threads + i, 0, dinner, philosophers_no + i) != 0) {
        perror("pthread_create() failure.");
        exit(1);
      }
    } else {
      pthread_mutex_unlock(&philosophers_mutex);
      usleep(rand() % 3000000);  // приостановка в микросекундах.
    }

  }

  return EXIT_SUCCESS;
}

void* dinner(void* arg) {
  // Номер (нач. с единицы) стула:
  int chair = *(int*)arg;

  // Индекс (нач. с нуля) правой вилки по отношению к стулу:
  int rfork = chair - 1;

  // Индекс (нач. с нуля) левой вилки по отношению к стулу:
  int lfork = (chair - 1 != 0 ? chair - 1 : CNT) - 1;

  // Есть философ сможет только в том случае,
  // если свободны две вилки – справа и слева от его тарелки.
  // При выполнении этого условия философ поднимает одновременно обе вилки:

  bool had_dinner = false;
  while (!had_dinner) {  // философу приходится ждать освобождения обеих вилок.


    // Заблокировать другим философам доступ к вилкам:
    pthread_mutex_lock(&forks_mutex);

    // Дождавшись, проверяем:
    //  а) свободна ли вилка справа от стула философа?
    //  б) свободна ли вилка слева от стула философа?
    if (forks[rfork] && forks[lfork]) {
      // Т.к. вилка справа и слева от стула философа свободна,
      // то поднимаем их одновременно:
      forks[rfork] = forks[lfork] = false;

      // Разблокировать другим философам доступ к вилкам:
      pthread_mutex_unlock(&forks_mutex);

    } else {
      // Разблокировать другим философам доступ к вилкам:
      pthread_mutex_unlock(&forks_mutex);

      write_output(chair, L"Философ %d ожидает вилки…\n", chair);
      usleep(rand() % 3000000);  // приостановка в микросекундах.

      continue;
    }


    write_output(chair, L"Философ %d поглощает пищу…\n", chair);

    // Философ поглощает пищу в течение какого-то заданного времени:
    nanosleep(&time_value, NULL);
    // Приостановка выполнения потока на заданный промежуток выполняется
    // вызовом функции nanosleep().
    // int nanosleep(const struct timespec* req, struct timespec* rem);

    // Результат выполнения выводится на терминал/консоль:
    write_output(chair, L"Философ %d пообедал и ушёл.\n", chair);
    had_dinner = true;


    // Заблокировать другим философам доступ к вилкам:
    pthread_mutex_lock(&forks_mutex);

    // Пообедав, философ кладет обе вилки на стол одновременно и уходит:
    forks[rfork] = forks[lfork] = true;

    // Разблокировать другим философам доступ к вилкам:
    pthread_mutex_unlock(&forks_mutex);
  }

  pthread_mutex_lock(&philosophers_mutex);
  philosophers[chair - 1] = true;
  pthread_mutex_unlock(&philosophers_mutex);

  pthread_exit(0);
}
