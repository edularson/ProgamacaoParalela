#include <stdio.h>
#include <mpi.h>

#define N 100000

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_n = N / size;
    long dentro = 0;

    for (int i = 0; i < local_n; i++) {
        unsigned int seed = (unsigned int)((rank * local_n + i) * 1099087573UL);
        seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;
        double x = (seed & 0x7FFFFFFF) / (double)0x7FFFFFFF;
        seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;
        double y = (seed & 0x7FFFFFFF) / (double)0x7FFFFFFF;
        if (x * x + y * y <= 1.0) dentro++;
    }

    long total = 0;
    MPI_Reduce(&dentro, &total, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("Pi (Monte Carlo coletiva, N=%d): %.15f\n", N, 4.0 * total / N);

    MPI_Finalize();
    return 0;
}