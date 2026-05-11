#include <stdio.h>
#include <omp.h>

#define N 100000

int main() {
    double h = 1.0 / N;
    double soma = 0.0;

    #pragma omp target teams distribute parallel for reduction(+:soma) map(tofrom:soma)
    for (int i = 0; i < N; i++) {
        double x = (i + 0.5) * h;
        soma += 4.0 / (1.0 + x * x);
    }

    double pi = soma * h;
    printf("Pi (integração numérica, N=%d): %.15f\n", N, pi);

    return 0;
}