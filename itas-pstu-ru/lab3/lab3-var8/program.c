// Программа, использущая функции неблокирующего (и буферизованного)
// двухточечного обмена, реализующую алгоритм передачи данных по кольцу:
// все процессора одновременно посылают и принимают сообщения.

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int *buffer;
  int myrank;
  int numprocs;
  MPI_Status status;
  MPI_Request request;
  int bufsize = 1;
  int tag = 0;
  int msg;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  int s1;
  MPI_Pack_size(bufsize, MPI_INT, MPI_COMM_WORLD, &s1);
  int size = s1 + MPI_BSEND_OVERHEAD * 1;

  buffer = (int*)malloc(size);

  MPI_Buffer_attach(buffer, size);

  msg = myrank;

  MPI_Ibsend(
      &msg,  // начальный адрес данных.
      bufsize,  // число элементов в буфере.
      MPI_INT,  // тип данных каждого из элементов буфера.
      myrank + 1 >= numprocs ? 0 : myrank + 1,  // ранг получателя в коммуникаторе comm.
      tag,  // тег сообщения.
      MPI_COMM_WORLD,  // коммуникатор.
      &request
    );

  MPI_Buffer_detach(&buffer, &size);

  free(buffer);
  buffer = NULL;

  MPI_Irecv(
    &msg,  // адрес начала буфера принимаемых данных.
    bufsize,  // число принимаемых элементов.
    MPI_INT,  // тип данных каждого из элементов буфера.
    myrank - 1 < 0 ? numprocs - 1 : myrank - 1,  // ранг отправителя в коммуникаторе comm.
    tag,  // тег сообщения.
    MPI_COMM_WORLD,  // коммуникатор.
    &request
  );

  MPI_Wait(&request, &status);

  printf("I %i received from %i\n", myrank, msg);

  MPI_Finalize();

  return 0;
}
