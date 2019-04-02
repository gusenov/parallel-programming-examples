#include <g2.h>
#include <g2_X11.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include "mpi.h"

void paint(int* solution) {
  int d = g2_open_X11(640, 640);
  int color;

  int w = 80;
  int h = 80;

  int N = 8;
  int x, y;
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      x = row * w;
      y = col * h;

      if ((row % 2 == 0) == (col % 2 == 0))
        color = g2_ink(d, 0.0, 0.0, 0.0);
      else
        color = g2_ink(d, 1.0, 1.0, 1.0);

      g2_pen(d, color);
      g2_filled_rectangle(d, x, y + h, x + w, y);

      if (row == solution[col]) {
        color = g2_ink(d, 1.0, 0.0, 0.0);
        g2_pen(d, color);
        g2_filled_triangle(d, x, y, x + w, y, x + w / 2, y + h);
      }
    }
  }

  sleep(10);
  g2_close(d);
}

// Функция проверки диагоналей, горизонтали:
int F(int *v) {
  int result = 0;
  int k1, k, i, j, l, x, y;
  for (i = 0; i < 8; i++) {
    k = v[i];
    k1 = i;
    for (l = i + 1; l < 8; l++) {
      if (k == v[l]) {
        result += 1;
      }
      y = abs(k - v[l]);
      x = abs(k1 - l);
      if (x == y) {
        result+=1;
      }
    }
  }
  return result;  // возвращаем результат.
}

// Функция копирования массива:
void CopyArray(int g[][8], int *a, int count) {
  int i;
  for (i = 0; i < 8; i++) {
    g[count][i] = a[i];  // в цикле переписываем матрицу в массив.
  }
}

int main(int argc, char* argv[])
{
  int size, rank;

  MPI_Init(&argc,&argv);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int count = 0;  // счетчик.

  // Размер доски, в дальнейшем в 8-ми циклах это и будет наша шахматная доска:
  int m[8];

  // Матрица размером 100 на 8, где 100 предположительное число решений,
  // а 8 число ферзей:
  int global[100][8];

  int a, b, c, d, e, f, g, h, rrr;
  rrr = rank;
  double t1, t2;

  t1 = MPI_Wtime();

  // В этих циклах устанавливаем по очереди всех ферзей,
  // то есть перебираем все варианты установки ферзей:
  for (a = rrr; a <= 7; a = a + size) {
    m[0] = a;
    for (b = 0; b <= 7; b++) {
      m[1] = b;
      for (c = 0; c <= 7; c++) {
        m[2] = c;
        for (d = 0; d <= 7; d++) {
          m[3] = d;
          for (e = 0; e <= 7; e++) {
            m[4] = e;
            for (f = 0; f <= 7; f++) {
              m[5] = f;
              for (g = 0; g <= 7; g++) {
                m[6] = g;
                for (h = 0; h <= 7; h++) {
                  m[7] = h;
                  if (F(m) == 0) {
                    count++;
                    CopyArray(global, m, count);
                  }  // if
                }  // h
              }  // g
            }  // f
          }  // e
        }  // d
      }  // c
    }  // b
  }  // a

  int allsol[8][92][8];
  MPI_Status stat;
  int counts[8];

  // Отсылка с разных процессоров решений на 0 процессор:
  if (rank != 0) {
    // Посылает сообщения:
    MPI_Send(global, (count + 1) * 8, MPI_INT, 0, 100, MPI_COMM_WORLD);
  }

  int i, j, k;

  if (rank == 0) {
    // 0 процессор узнает размер и количество принимаемых данных
    // и записывает их себе:
    for (i = 1; i < size; i++) {
      // Определяет параметры полученного сообщения без его чтения:
      MPI_Probe(
        i,  // номер процесса-отправителя.
        100,  // идентификатор сообщения.
        MPI_COMM_WORLD,  // коммуникатор.
        &stat  // атрибуты опрошенного сообщения.
      );

      // Определяет число фактически полученных элементов сообщения:
      MPI_Get_count(
        &stat,  // атрибуты принятого сообщения.
        MPI_INT,  // тип элементов принятого сообщения.
        &counts[i]  // число полученных элементов.
      );

      // Принимает сообщения:
      MPI_Recv(&allsol[i], counts[i], MPI_INT, i, 100, MPI_COMM_WORLD, &stat);
    }  // i

    // Затем он выводит все решения на экран:

    int tmp = 1;
    for (i = 1; i <= count; i++) {
      printf("%d)", tmp);
      for (j = 0; j < 8; j++) {
        printf(" %d ", global[i][j]);
      }  // j
      tmp++;
      printf("\n");
    }  // i

    for (k = 1; k < size; k++) {
      printf("--------------------------------------------------------- \n \n \n");
      for (i = 1; i < counts[k] / 8; i++) {
        printf("%d)", tmp);
        for (j = 0; j < 8; j++) {
          printf(" %d ", allsol[k][i][j]);
        }
        tmp++;
        printf("\n");
      }  // i
    }  // k

    int solution_count = tmp - 1;

    // Время выполнения задачи:
    printf("time=%f\n", t2 - t1);

    while (1) {
      printf("Выберите решение для изображения>");
      int choice;
      scanf("%d", &choice);

      if (choice <= 0 || choice > solution_count)
        break;

      tmp = 1;
      for (i = 1; i <= count; i++) {
        if (tmp == choice) {
          paint(global[choice]);
        }
        tmp++;
      }

      if (i > count)
      for (k = 1; k < size; k++) {
        for (i = 1; i < counts[k] / 8; i++) {
          if (tmp == choice) {
            paint(allsol[k][i]);
          }
          tmp++;
        }
      }
    }

  }

  MPI_Finalize();

  return EXIT_SUCCESS;
}
