# MYLIBDIR=
# MYLIB=
# CFLAGS=-I${MYLIBDIR} -L${MYLIBDIR}
OBJS=fileHandle.o main.o scan.o parser.o idList.o prettyPrint.o

all: cr

cr: ${OBJS}
	${CC} -o $@ $^
#	${CC} ${CFLAGS} -o $@ $^ ${MYLIB}

test: cr
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
	${RM} *.o cr *~