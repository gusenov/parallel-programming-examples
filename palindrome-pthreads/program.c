// Программа, использующая библиотеку pthreads
// для распараллеливания работы на несколько потоков:

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <stdbool.h>
#include <pthread.h>

#define MAX_WORD_LEN 1000

typedef struct Data {
  const wchar_t* word;
  bool result;
} Data;

// Проверка корректности заданного слова, как функция потока:
void* checkWord(void* threadDataPtr) {

  Data* threadData = (Data*)threadDataPtr;

	bool hasEnglish = false, hasRussian = false;

  const wchar_t* word = threadData->word;
	int wordLen = wcslen(word);
	for (int i = 0; i < wordLen; ++i) {
		switch (word[i]) {
		case L',': case L'.': case L'!': case L'?': case L';': case L':': case L'-':
    case L'"': case L'(': case L')':
			wprintf(L"Ошибка! В слове присутствуют знаки препинания!\n");
      threadData->result = false;
      return NULL;
		default: break;
		}

		if (word[i] >= L'0' && word[i] <= L'9') {
			wprintf(L"Ошибка! В слове присутствуют цифры!\n");
      threadData->result = false;
			return NULL;
		} else if ((word[i] >= L'A' && word[i] <= L'Z')
            || (word[i] >= L'a' && word[i] <= L'z')) {
			hasEnglish = true;
		} else if ((word[i] >= L'а' && word[i] <= L'я')
            || (word[i] >= L'А' && word[i] <= L'Я')) {
			hasRussian = true;
		} else {
			wprintf(L"Ошибка! Некорректный ввод!\n");
      threadData->result = false;
			return NULL;
		}

		if (hasEnglish && hasRussian) {
			wprintf(L"Ошибка! Смешение латинских и кириллических символов!\n");
      threadData->result = false;
			return NULL;
		}
	}
  threadData->result = true;
  return NULL;
}

// Проверка того, что введенное слово — палиндром, как функция потока:
void* isPalindrome(void* threadDataPtr) {
  Data* threadData = (Data*)threadDataPtr;

  const wchar_t* str = threadData->word;

  int l = 0, h = wcslen(str) - 1;

  while (h > l) if (str[l++] != str[h--]) {
    threadData->result = false;
    return NULL;
  }

  threadData->result = true;
  return NULL;
}

// Процедура для печати значения типа pthread_t:
void wprintPt(pthread_t pt) {
  unsigned char* ptc = (unsigned char*)(void*)(&pt);
  wprintf(L"0x");
  for (size_t i = 0; i < sizeof(pt); ++i)
    wprintf(L"%02x", (unsigned)(ptc[i]));
}

int main(int argc, char* argv[]) {
  setlocale(LC_ALL, "");

  wchar_t word[MAX_WORD_LEN];
  int len = sizeof(word) / sizeof(wchar_t);

  do {
    // Сначала осуществляется ввод слова.
    // Ввод необходимых данных осуществляется в основном (начальном) потоке.
    // Программа принимает от пользователя некое слово:
		wprintf(L"Введите слово>");
    fgetws(word
        , len  // мак. количество символов для чтения, включая нуль-символ.
        , stdin
    );

		word[wcscspn(word, L"\n")] = '\0';
		wprintf(L"Вы ввели слово: '%ls'\n", word);

    // Запускаются необходимые потоки:

    Data data;
    data.word = word;
    pthread_t id;
    void* exitStatus;

    // Поток проверяет слово на корректность
    // (отсутствие знаков препинания, цифр,
    // смешения латинских и кириллических символов):
    pthread_create(&id, NULL, checkWord, &data);
    wprintf(L"Запущен поток с ID "); wprintPt(id);
    wprintf(L" для проверки слова на корректность…\n");

    pthread_join(id, &exitStatus);  // ожидание окончания работы потока.
    wprintf(L"Поток с ID "); wprintPt(id); wprintf(L" завершил свою работу.\n");

    if (data.result) {

      // Поток проверяет, является ли введенное слово палиндромом:
      pthread_create(&id, NULL, isPalindrome, &data);
      wprintf(L"Запущен поток с ID "); wprintPt(id);
      wprintf(L" для проверки является ли введенное слово палиндромом…\n");

      pthread_join(id, &exitStatus);  // ожидание окончания работы потока.
      wprintf(L"Поток с ID "); wprintPt(id); wprintf(L" завершил свою работу.\n");

      // Вывод результатов выполнения на терминал/консоль
      // осуществляется в основном (начальном) потоке:
      if (data.result) {
        wprintf(L"Строка '%ls' является палиндромом.\n", word);
      } else {
        wprintf(L"Строка '%ls' НЕ является палиндромом.\n", word);
      }

      break;
    }

  } while (true);

  return EXIT_SUCCESS;
}
