#CFLAGS = -W -Wall -g -O2 -pipe

objs = HLBS_algorithm.o read_tgff.o  ranking.o assign.o print_functions.o
CC = gcc
CFLAGS = -g -O0 

dag: $(objs)
	$(CC) -o test $(objs)

sched_DAG.o: HLBS_algorithm.c
	$(CC) -c HLBS_algorithm.c

read_tgff.o: read_tgff.c
	$(CC) -c read_tgff.c

ranking.o: ranking.c
	$(CC) -c ranking.c

assign.o: assign.c
	$(CC) -c assign.c 

print_functions.o: print_functions.c
	$(CC) -c print_functions.c

clean:
	rm -f dag $(objs)
