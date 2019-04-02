#include "mpi.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
  int myrank, count = 25;
  char data[count];
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  if (myrank == 0)
  {
    strcpy(data, "Hi, Parallel Programmer!");

    // Вызов процедуры широковещательной рассылки:
    MPI_Bcast(
      data,  // адрес буфера.
      count,  // количество элементов данных в сообщении.
      MPI_CHAR,  // тип данных MPI.
      0,  // ранг главного процесса, выполняющего широковещательную рассылку.
      MPI_COMM_WORLD  // коммуникатор.
    );

    printf("Send %i: %s\n", myrank, data);
  }
  else
  {

    // Вызов процедуры широковещательной рассылки:
    MPI_Bcast(
      data,  // адрес буфера.
      count,  // количество элементов данных в сообщении.
      MPI_CHAR,  // тип данных MPI.
      0,  // ранг главного процесса, выполняющего широковещательную рассылку.
      MPI_COMM_WORLD  // коммуникатор.
    );

    printf("Received %i: %s\n", myrank, data);
  }

  MPI_Finalize();
  return 0;
}
