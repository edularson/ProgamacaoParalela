#include <stdio.h>
#include <mpi.h>

#define N 100000

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double h = 1.0 / N;
    double soma_local = 0.0;

    for (int i = rank; i < N; i += size) {
        double x = (i + 0.5) * h;
        soma_local += 4.0 / (1.0 + x * x);
    }

    double soma_total = 0.0;
    MPI_Reduce(&soma_local, &soma_total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("Pi (integração coletiva, N=%d): %.15f\n", N, soma_total * h);

    MPI_Finalize();
    return 0;
}