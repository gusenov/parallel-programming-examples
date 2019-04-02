#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int myid, myid_in_group, numprocs;

  MPI_Group group;

  int odd_process_count;
  int* odd_process_ranks;
  MPI_Group odd_group;
  MPI_Comm odd_comm;

  int even_process_count;
  int* even_process_ranks;
  MPI_Group even_group;
  MPI_Comm even_comm;

  int A, B;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);


  // Количество нечётных процессоров:
  odd_process_count = numprocs / 2;

  odd_process_ranks = (int*)malloc(odd_process_count * sizeof(int));

  // Количество чётных процессоров:
  if (numprocs % 2 == 0) {
    even_process_count = numprocs / 2;
  } else {
    even_process_count = numprocs / 2 + 1;
  }

  even_process_ranks = (int*)malloc(even_process_count * sizeof(int));

  for (int proc = 0, i = 0, j = 0; proc < numprocs; proc++)
  {
    if (proc % 2 == 0) {
      even_process_ranks[i++] = proc;
    } else {
      odd_process_ranks[j++] = proc;
    }
  }


  MPI_Comm_group(MPI_COMM_WORLD, &group);


  MPI_Group_incl(
    group,  // исходная группа.
    odd_process_count,  // процессы.
    odd_process_ranks,  // ранги процессов.
    &odd_group  // новая группа.
  );

  MPI_Comm_create(
    MPI_COMM_WORLD,  // другой коммуникатор.
    odd_group,  // подмножество процессов.
    &odd_comm  // новый коммуникатор.
  );
  // if (odd_comm == MPI_COMM_NULL)
  //   printf("odd_comm == MPI_COMM_NULL in %i\n", myid);


  MPI_Group_incl(
    group,  // исходная группа.
    even_process_count,  // процессы.
    even_process_ranks,  // ранги процессов.
    &even_group  // новая группа.
  );

  MPI_Comm_create(
    MPI_COMM_WORLD,  // другой коммуникатор.
    even_group,  // подмножество процессов.
    &even_comm  // новый коммуникатор.
  );
  // if (even_comm == MPI_COMM_NULL)
  //   printf("even_comm == MPI_COMM_NULL in %i\n", myid);

  // Передача с помощью коммуникационной функции MPI_Bcast(…)
  // переменной A – четным процессорам:
  if (even_comm != MPI_COMM_NULL) {
    MPI_Comm_rank(even_comm, &myid_in_group);

    if (myid_in_group == 0) {
      // На нулевом процессоре задана переменная A:
      A = 100;

      MPI_Bcast(
        &A,  // адрес буфера.
        1,  // количество элементов данных в сообщении.
        MPI_INT,  // тип данных MPI.
        0,  // ранг главного процесса, выполняющего широковещательную рассылку.
        even_comm  // коммуникатор.
      );

    } else {
      MPI_Bcast(&A, 1, MPI_INT, 0, even_comm);
      printf("%i received: %i\n", myid, A);
    }

    MPI_Comm_free(&even_comm);
    MPI_Group_free(&even_group);
  }

  // Передача с помощью коммуникационной функции MPI_Bcast(…)
  // переменной B – нечетным процессорам:
  else if (odd_comm != MPI_COMM_NULL) {
    MPI_Comm_rank(odd_comm, &myid_in_group);

    if (myid_in_group == 0) {
      // На первом процессоре задана переменная B:
      B = 500;

      MPI_Bcast(
        &B,  // адрес буфера.
        1,  // количество элементов данных в сообщении.
        MPI_INT,  // тип данных MPI.
        0,  // ранг главного процесса, выполняющего широковещательную рассылку.
        odd_comm  // коммуникатор.
      );

    } else {
      MPI_Bcast(&B, 1, MPI_INT, 0, odd_comm);
      printf("%i received: %i\n", myid, B);
    }

    MPI_Comm_free(&odd_comm);
    MPI_Group_free(&odd_group);
  }

  free(odd_process_ranks);
  free(even_process_ranks);

  MPI_Finalize();

  return EXIT_SUCCESS;
}
