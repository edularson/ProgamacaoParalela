#include <stdio.h>
#include <math.h>
#include <omp.h>

/*
  funcao: f(x) = 4 / (1 + x^2)
*/
static double funcao(double x)
{
    return 4.0 / (1.0 + x * x);
}

double simpson_composta(double a, double b, int n)
{
    double h = (b - a) / n;
    double soma = funcao(a) + funcao(b);

    #pragma omp parallel for reduction(+:soma)
    for (int i = 1; i < n; i++) {
        double xi = a + i * h;

        if (i % 2 == 0) {
            soma += 2.0 * funcao(xi);
        } else {
            soma += 4.0 * funcao(xi);
        }
    }

    return (h / 3.0) * soma;
}

 int main(void)
 {
     const double a = 0.0;
     const double b = 1.0;
 
     const int n = 100000;
 
     double pi_aprox  = simpson_composta(a, b, n);
     double pi_real   = M_PI;
     double erro_abs  = fabs(pi_real - pi_aprox);
 
     printf("aproximacao de pi com a regra de simpson\n");
     printf("subdivisoes: n = %d\n", n);
     printf("\n");
     printf("pi aproximado  : %.15f\n", pi_aprox);
     printf("pi real: %.15f\n", pi_real);
     printf("erro absoluto  : %.2e\n",  erro_abs);
 
     return 0;
 }