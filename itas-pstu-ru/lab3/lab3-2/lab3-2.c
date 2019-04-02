#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int *buf, source, i;
  int message[3] = {0, 1, 2};
  int myrank, data = 2014, count, tag = 0;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  if (myrank == 0)
  {
    MPI_Send(
      &data,  // начальный адрес данных.
      1,  // число элементов в буфере.
      MPI_INT,  // тип данных каждого из элементов буфера.
      2,  // ранг получателя в коммуникаторе comm.
      tag,  // тег сообщения.
      MPI_COMM_WORLD  // коммуникатор.
    );
  }

  else
  if (myrank == 1)
  {
    MPI_Send(
      &message,  // начальный адрес данных.
      3,  // число элементов в буфере.
      MPI_INT,  // тип данных каждого из элементов буфера.
      2,  // ранг получателя в коммуникаторе comm.
      tag,  // тег сообщения.
      MPI_COMM_WORLD  // коммуникатор.
    );
  }

  else
  {
    // Вызовы подпрограмм-пробников:

    // С помощью вызова подпрограммы MPI_Probe фиксируется поступление
    // (но не прием) сообщения:
    MPI_Probe(
      MPI_ANY_SOURCE,
      tag,
      MPI_COMM_WORLD,
      &status
    );

    // Затем определяется источник сообщения, его длина, выделяется буфер
    // подходящего размера и выполняется прием сообщения:

    source = status.MPI_SOURCE;

    // Определение размера полученного сообщения:
    MPI_Get_count(&status, MPI_INT, &count);
    // Аргумент datatype должен соответствовать типу данных,
    // указанному в операции передачи сообщения.

    if (NULL == (buf = (int*)malloc(count)))
    {
      printf("malloc failed!\n");
      return (-1);
    }

    // Стандартный блокирующий прием:
    MPI_Recv(
      &buf[0],  // адрес начала буфера принимаемых данных.
      count,  // число принимаемых элементов.
      MPI_INT,  // тип данных каждого из элементов буфера.
      MPI_ANY_SOURCE,  // ранг отправителя в коммуникаторе comm.
      tag,  // тег сообщения.
      MPI_COMM_WORLD,  // коммуникатор.
      &status  // структура статуса принимаемого сообщения.
    );

    for (i = 0; i < count; i++)
    {
      printf("%i receive from %i: %i\n", myrank, source, buf[i]);
    }

    free(buf);
  }

  MPI_Finalize();
  return 0;
}
