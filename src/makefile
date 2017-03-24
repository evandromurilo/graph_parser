C = gcc
CFLAGS = -I ../lib 
OUT = -o ../bin/graphs

PROG = graphs
OBJS = graphs.o ../lib/hash.o ../lib/m_basics.o
SRCS = graphs.c ../lib/hash.c ../lib/m_basics.c
DEPS = ../lib/m_basics.h ../lib/hash.h graphs.h

%.o: %.c $(DEPS)
	$(CC) $(DEBUG) -g -c -o $@ $< $(CFLAGS)

$(PROG): $(OBJS)
	$(CC) $(DEBUG) $(OUT) $(CFLAGS) $(OBJS)

run:
	../bin/graphs test.txt
