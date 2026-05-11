#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100000

int main() {
    long dentro = 0;

    #pragma omp target teams distribute parallel for reduction(+:dentro) map(tofrom:dentro)
    for (int i = 0; i < N; i++) {
        /* gerador congruencial linear por thread/iteração */
        unsigned int seed = (unsigned int)(i * 1099087573UL);

        /* dois números pseudo-aleatórios em [0,1) */
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        double x = (seed & 0x7FFFFFFF) / (double)0x7FFFFFFF;

        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        double y = (seed & 0x7FFFFFFF) / (double)0x7FFFFFFF;

        if (x * x + y * y <= 1.0)
            dentro++;
    }

    double pi = 4.0 * dentro / N;
    printf("Pi (Monte Carlo, N=%d): %.15f\n", N, pi);

    return 0;
}