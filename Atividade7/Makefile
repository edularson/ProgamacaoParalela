CC = /opt/homebrew/opt/llvm/bin/clang
CFLAGS = -O2 -fopenmp

all: pi_integracao pi_monte_carlo

pi_integracao: pi_integracao.c
	$(CC) $(CFLAGS) -o pi_integracao pi_integracao.c

pi_monte_carlo: pi_monte_carlo.c
	$(CC) $(CFLAGS) -o pi_monte_carlo pi_monte_carlo.c

clean:
	rm -f pi_integracao pi_monte_carlo