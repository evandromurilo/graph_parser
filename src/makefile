C = gcc
CFLAGS = -I ../lib -g
OUT = -o ../bin/graphs

PROG = graphs
OBJS = graphs.o ../lib/hash.o ../lib/m_basics.o ../lib/m_list.o
SRCS = graphs.c ../lib/hash.c ../lib/m_basics.c ../lib/m_list.c
DEPS = ../lib/m_basics.h ../lib/hash.h graphs.h ../lib/m_list.h structures.h

%.o: %.c $(DEPS)
	$(CC) $(DEBUG) -c -o $@ $< $(CFLAGS)

$(PROG): $(OBJS)
	$(CC) $(DEBUG) $(OUT) $(CFLAGS) $(OBJS)

graph_file_tester:
	gcc -g -o ../bin/graph_file_tester graph_file_tester.c ../lib/m_basics.c ../lib/m_list.c ../lib/hash.c

run:
	../bin/graphs test.txt
