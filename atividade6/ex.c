#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define M 2147483647L
#define A 48271L
#define C 0L
#define N_THREADS 8

long lcg_next(long x) {
    return (A * x + C) % M;
}

long power_mod(long base, long exp, long mod) {
    long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (__int128)result * base % mod;
        exp /= 2;
        base = (__int128)base * base % mod;
    }
    return result;
}

double leapfrog_pi(long seed, long n_samples) {
    long stride = N_THREADS;
    long local_seed = seed;

    for (int i = 0; i < omp_get_thread_num(); i++)
        local_seed = lcg_next(local_seed);

    long a_k = power_mod(A, stride, M);

    long count = 0;
    long x = local_seed;
    for (long i = 0; i < n_samples; i++) {
        double rx = (double)x / M;
        x = (__int128)a_k * x % M;
        double ry = (double)x / M;
        x = (__int128)a_k * x % M;
        if (rx * rx + ry * ry <= 1.0)
            count++;
    }
    return count;
}

double modified_leapfrog_pi(long seed, long n_samples) {
    int tid = omp_get_thread_num();
    long local_seed = (A * (seed + tid) + C) % M;

    long a_k = power_mod(A, N_THREADS, M);

    long count = 0;
    long x = local_seed;
    for (long i = 0; i < n_samples; i++) {
        double rx = (double)x / M;
        x = (__int128)a_k * x % M;
        double ry = (double)x / M;
        x = (__int128)a_k * x % M;
        if (rx * rx + ry * ry <= 1.0)
            count++;
    }
    return count;
}

int main() {
    long seed = 12345L;
    long total_samples = 100000000L;
    long samples_per_thread = total_samples / N_THREADS;

    double pi_leapfrog = 0.0;
    double pi_modified = 0.0;
    long total_count_lf = 0;
    long total_count_mlf = 0;

    omp_set_num_threads(N_THREADS);

    #pragma omp parallel reduction(+:total_count_lf)
    {
        total_count_lf += (long)leapfrog_pi(seed, samples_per_thread / 2);
    }
    pi_leapfrog = 4.0 * total_count_lf / (samples_per_thread / 2 * N_THREADS);

    #pragma omp parallel reduction(+:total_count_mlf)
    {
        total_count_mlf += (long)modified_leapfrog_pi(seed, samples_per_thread / 2);
    }
    pi_modified = 4.0 * total_count_mlf / (samples_per_thread / 2 * N_THREADS);

    printf("Leapfrog Pi:          %.8f\n", pi_leapfrog);
    printf("Leapfrog Modificado:  %.8f\n", pi_modified);
    printf("Pi real:              %.8f\n", 3.14159265358979);

    return 0;
}