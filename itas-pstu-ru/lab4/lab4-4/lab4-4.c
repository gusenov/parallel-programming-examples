#include "mpi.h"
#include <stdio.h>

int main(int argc,char *argv[])
{
  int myrank, i;
  int count = 5, root = 1;
  MPI_Group MPI_GROUP_WORLD, subgroup;
  int ranks[4] = {1, 3, 5, 7};
  MPI_Comm subcomm;
  int sendbuf[5] = {1, 2, 3, 4, 5};
  int recvbuf[5];

  MPI_Init(&argc, &argv);

  // Создается подгруппа, состоящая из процессов с рангами 1, 3, 5 и 7:
  MPI_Comm_group(MPI_COMM_WORLD, &MPI_GROUP_WORLD);
  MPI_Group_incl(MPI_GROUP_WORLD, 4, ranks, &subgroup);

  MPI_Group_rank(subgroup, &myrank);

  // И соответствующий ей коммуникатор:
  MPI_Comm_create(MPI_COMM_WORLD, subgroup, &subcomm);

  if(myrank != MPI_UNDEFINED)
  {
    // Редукция (суммирование) по процессам, входящим в новую группу:
    MPI_Reduce(
      &sendbuf,  // адрес буфера передачи.
      &recvbuf,  // адрес начала буфера результатов (используется только в процессе-получателе root).
      count,  // количество элементов в буфере передачи.
      MPI_INT,  // тип данных в буфере передачи.
      MPI_SUM,  // операция приведения.
      root,  // ранг главного процесса.
      subcomm  // коммуникатор.
    );
    if (myrank == root)
    {
      printf("Reduced values");
      for(i = 0; i < count; i++)
      {
        printf(" %i ", recvbuf[i]);
      }
    }
    printf("\n");
    MPI_Comm_free(&subcomm);
    MPI_Group_free(&MPI_GROUP_WORLD);
    MPI_Group_free(&subgroup);
  }

  MPI_Finalize();

  return 0;
}
