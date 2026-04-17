#include <stdio.h>

int main() {
    int n = 1000000;
    double width = 1.0 / n;
    double sum = 0.0;

    #pragma omp simd reduction(+:sum)
    for (int i = 0; i < n; i++) {
        double x = (i + 0.5) * width;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = sum * width;
    printf("Estimated Pi = %.15f\n", pi);

    return 0;
}