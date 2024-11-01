# MYLIBDIR=
# MYLIB=
# CFLAGS=-I${MYLIBDIR} -L${MYLIBDIR}
OBJS=fileHandle.o main.o scan.o parser.o prettyPrint.o

all: pp

pp: ${OBJS}
	${CC} -o $@ $^
#	${CC} ${CFLAGS} -o $@ $^ ${MYLIB}

test: pp
	gcc -coverage -c *.c
	gcc -coverage -o pp *.o
	bash test.sh
	gcov -b *.gcda

retest:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	make clean
	make test

clean:
	${RM} *.o pp *~