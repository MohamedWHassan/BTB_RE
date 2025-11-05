DISTANCE ?= 2 	# Distance between each branch in bytes, typical range (2 - 128)
BRANCHES ?= 512 # Number of branches in one loop iteration, typical range (512 - 4096)
all:
	gcc -o spy -g -O0 spy.c
	./spy 2000000 $(DISTANCE) $(BRANCHES)
	gcc -o T1ImNB -g -O0 T1ImNB.c
	perf stat -e r6:u,r8:u,r9:u,instructions:u,branch-instructions:u,branch-misses:u taskset -c 0 ./T1ImNB
