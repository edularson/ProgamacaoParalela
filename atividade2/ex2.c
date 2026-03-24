/*
gcc-15 -O2 -fopenmp -o ex2 ex2.c -lm
./ex2
*/

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <math.h>
 #include <omp.h>
 #include <stdbool.h>
 
 #define LIMIT 1000
 
 int main(void) {
     printf("crivo de eratostenes\n");
     printf("intervalo   : [2, %d]\n", LIMIT);
     printf("threads OMP : %d\n\n", omp_get_max_threads());
 
     bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
     if (!is_prime) { fprintf(stderr, "erro de alocacao\n"); return 1; }
 
     memset(is_prime, true, (LIMIT + 1) * sizeof(bool));
     is_prime[0] = false;
     is_prime[1] = false;
 
     int limit_sqrt = (int)sqrt((double)LIMIT);

     for (int p = 2; p <= limit_sqrt; p++) {
 
         if (!is_prime[p]) continue;
 
         #pragma omp parallel for schedule(static)
         for (int j = p * p; j <= LIMIT; j += p) {
             is_prime[j] = false;
         }
     }
 
     int count = 0;
     #pragma omp parallel for reduction(+:count) schedule(static)
     for (int i = 2; i <= LIMIT; i++) {
         if (is_prime[i]) count++;
     }
 
     int *primes = (int *)malloc(count * sizeof(int));
     if (!primes) { fprintf(stderr, "erro de alocacao\n"); free(is_prime); return 1; }
 
     int idx = 0;
     for (int i = 2; i <= LIMIT; i++) {
         if (is_prime[i]) primes[idx++] = i;
     }
 
     printf("qntd de num primos encontrados [2, %d]: %d\n\n", LIMIT, count);
     printf("num primos encontrados:\n");
 
     for (int i = 0; i < count; i++) {
         printf("%4d", primes[i]);
         if ((i + 1) % 10 == 0) printf("\n");
     }
     if (count % 10 != 0) printf("\n");
 
     free(is_prime);
     free(primes);
     return 0;
 }