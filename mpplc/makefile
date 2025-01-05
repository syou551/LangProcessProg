# MYLIBDIR=
# MYLIB=
# CFLAGS=-I${MYLIBDIR} -L${MYLIBDIR}
OBJS=fileHandle.o main.o scan.o parser.o idList.o makeCode.o prettyPrint.o

all: mpplc

mpplc: ${OBJS}
	${CC} -o $@ $^
#	${CC} ${CFLAGS} -o $@ $^ ${MYLIB}

test: ${OBJS}
	gcc -coverage -c fileHandle.c main.c scan.c parser.c idList.c
	gcc -coverage -o cr $^
	bash test.sh
	gcov -b *.gcda

retest:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	make clean
	make test

clean:
	${RM} *.o mpplc *~
