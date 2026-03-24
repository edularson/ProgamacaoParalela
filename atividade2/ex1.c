/*
 gcc -O2 -fopenmp -o ex1 ex1.c -lm
 ./ex1
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <math.h>
 #include <time.h>
 #include <omp.h>
 
 #define N 100000000L
 
 static inline double f(double x) {
     return 4.0 / (1.0 + x * x);
 }
 
 double pi_sequencial(void) {
     double h   = 1.0 / (double)N;
     double sum = 0.0;
 
     for (long i = 0; i < N; i++) {
         double x = (i + 0.5) * h;
         sum += f(x);
     }
 
     return sum * h;
 }
 
 double pi_paralelo(void) {
     double h   = 1.0 / (double)N;
     double sum = 0.0;

     #pragma omp parallel for reduction(+:sum) schedule(static)
     for (long i = 0; i < N; i++) {
         double x = (i + 0.5) * h;
         sum += f(x);
     }
 
     return sum * h;
 }
 
 int main(void) {
     printf("calculo de PI por int num\n");
     printf("subdivisoes: %ld\n", N);
     printf("threads da OMP: %d\n\n", omp_get_max_threads());
 
     double t0  = omp_get_wtime();
     double pi_s = pi_sequencial();
     double t1  = omp_get_wtime();
 
    
     double t2  = omp_get_wtime();
     double pi_p = pi_paralelo();
     double t3  = omp_get_wtime();
 
     double ref  = M_PI;
 
     printf("resultado sequencial: %.15f  (erro: %.2e)  tempo: %.4f s\n",
            pi_s, fabs(pi_s - ref), t1 - t0);
     printf("resultado paralelo   : %.15f  (erro: %.2e)  tempo: %.4f s\n",
            pi_p, fabs(pi_p - ref), t3 - t2);
     printf("ref (M_PI)    : %.15f\n", ref);
     printf("speedup              : %.2fx\n", (t1 - t0) / (t3 - t2));
 
     return 0;
 }