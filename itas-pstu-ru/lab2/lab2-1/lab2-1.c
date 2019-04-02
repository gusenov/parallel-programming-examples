#include "mpi.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
  int myid, numprocs;


  // Вызовы процедур подключения к MPI:

  // Инициализация окружения MPI:
  MPI_Init(&argc, &argv);  // инициализирует окружение MPI, эта функция
                           // вызывается один раз в начале программы,
                           // до первого вызова других MPI-функций.


  // В MPI-программе параллельные процессы можно объединять в группы
  // коммуникаций – коммуникаторы. Процесс может входить в несколько
  // коммуникаторов, у каждого процесса свой номер, отличающийся от
  // коммуникатора к коммуникатору. Коммуникаторы могут пересекаться
  // (по процессам), входить друг в друга. При старте программы автоматически
  // создается коммуникатор MPI_COMM_WORLD, в который входят все процессы.
  // У каждого процесса два основных атрибута – коммуникатор и номер процесса
  // в коммуникаторе.

  // Определения количества процессов и ранга процесса:

  // Получить количество процессов в коммуникаторе:
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

  // Получить номер процесса в коммуникаторе:
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);


  fprintf(stdout, "Process %d of %d\n", myid, numprocs);

  // Выход из системы передачи сообщений:
  MPI_Finalize();  // выход из системы MPI; после вызова этой функции код
                   // программы может продолжить свое выполнение,
                   // но выполнять вызов MPI-функций уже нельзя.

  return 0;
}