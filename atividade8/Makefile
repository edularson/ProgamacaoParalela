CC = mpicc
CFLAGS = -O2

TARGETS = pi_integ_p2p pi_integ_col pi_mc_p2p pi_mc_col

all: $(TARGETS)

pi_integ_p2p: pi_integ_p2p.c
	$(CC) $(CFLAGS) -o pi_integ_p2p pi_integ_p2p.c

pi_integ_col: pi_integ_col.c
	$(CC) $(CFLAGS) -o pi_integ_col pi_integ_col.c

pi_mc_p2p: pi_mc_p2p.c
	$(CC) $(CFLAGS) -o pi_mc_p2p pi_mc_p2p.c

pi_mc_col: pi_mc_col.c
	$(CC) $(CFLAGS) -o pi_mc_col pi_mc_col.c

clean:
	rm -f $(TARGETS)
