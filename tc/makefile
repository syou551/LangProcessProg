# MYLIBDIR=
# MYLIB=
# CFLAGS=-I${MYLIBDIR} -L${MYLIBDIR}
OBJS=counter.o fileHandle.o id-list.o main.o scan.o

all: tc

tc: ${OBJS}
	${CC} -coverage -o $@ $^
#	${CC} ${CFLAGS} -o $@ $^ ${MYLIB}

test: tc
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
	${RM} *.o chat *~