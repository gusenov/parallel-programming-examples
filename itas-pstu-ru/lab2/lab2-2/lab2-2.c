#include "mpi.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
  int numprocs, myrank;
  int myid;
  char message[20+2];
  MPI_Status status;
  int TAG = 0;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  // При запуске двух процессов один из них отправляет сообщение другому,
  // а тот его принимает:

  if (myrank == 0)
  {
    strcpy(message, "Hi, Second Processor!");

    // Вызов процедуры стандартного блокирующего двухточечного обмена:
    MPI_Send(
      message,  // начальный адрес данных.
      sizeof(message),  // число элементов в буфере.
      MPI_CHAR,  // тип данных каждого из элементов буфера.
      1,  // ранг получателя в коммуникаторе comm.
      TAG,  // тег сообщения.
      MPI_COMM_WORLD  // коммуникатор.
    );

  }
  else
  {
    // Вызов процедуры стандартного блокирующего двухточечного обмена:
    MPI_Recv(
      message,  // адрес начала буфера принимаемых данных.
      sizeof(message),  // число принимаемых элементов.
      MPI_CHAR,  // тип данных каждого из элементов буфера.
      0,  // ранг отправителя в коммуникаторе comm.
      TAG,  // тег сообщения.
      MPI_COMM_WORLD,  // коммуникатор.
      &status  // структура статуса принимаемого сообщения.
    );

    printf("Received: %s\n", message);
  }

  MPI_Finalize();
  return 0;
}
