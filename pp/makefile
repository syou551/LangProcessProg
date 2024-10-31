# MYLIBDIR=
# MYLIB=
# CFLAGS=-I${MYLIBDIR} -L${MYLIBDIR}
OBJS=counter.o fileHandle.o id-list.o main.o scan.o parser.c

all: pp

pp: ${OBJS}
	${CC} -o $@ $^
#	${CC} ${CFLAGS} -o $@ $^ ${MYLIB}

test: pp
	gcc -coverage -c *.c
	gcc -coverage -o tc *.o
	./test.sh
	gcov -b *.gcda

retest:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	make test

clean:
	${RM} *.o pp *~