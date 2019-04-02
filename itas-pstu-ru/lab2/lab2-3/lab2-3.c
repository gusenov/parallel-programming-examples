#include "mpi.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
  int myrank, size, message;
  int TAG = 0;
  MPI_Status status;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  message = myrank;

  // При запуске чётного числа процессов, те из них, которые имеют чётный ранг,
  // отправляют сообщение следующим по величине ранга процессам:

  if ((myrank % 2) == 0)
  {
    if ((myrank + 1) != size)

    // Вызов процедуры стандартного блокирующего двухточечного обмена:
    MPI_Send(
      &message,  // начальный адрес данных.
      1,  // число элементов в буфере.
      MPI_INT,  // тип данных каждого из элементов буфера.
      myrank + 1,  // ранг получателя в коммуникаторе comm.
      TAG,  // тег сообщения.
      MPI_COMM_WORLD  // коммуникатор.
    );

  }
  else
  {
    if (myrank != 0)
      // Вызов процедуры стандартного блокирующего двухточечного обмена:
      MPI_Recv(
        &message,  // адрес начала буфера принимаемых данных.
        1,  // число принимаемых элементов.
        MPI_INT,  // тип данных каждого из элементов буфера.
        myrank - 1,  // ранг отправителя в коммуникаторе comm.
        TAG,  // тег сообщения.
        MPI_COMM_WORLD,  // коммуникатор.
        &status  // структура статуса принимаемого сообщения.
      );

    printf("I %i received from %i\n", myrank, message);
  }

  MPI_Finalize();

  return 0;
}
