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

    if (rank == 0) {
        double soma_total = soma_local;
        for (int i = 1; i < size; i++) {
            double recv;
            MPI_Recv(&recv, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            soma_total += recv;
        }
        printf("Pi (integração p2p, N=%d): %.15f\n", N, soma_total * h);
    } else {
        MPI_Send(&soma_local, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}