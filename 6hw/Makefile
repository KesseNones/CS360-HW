CPP = gcc
FLAGS = -Wall -g

EXEC = 6EXE
OBJS = assignment6.o

default:${EXEC}

clean:
	rm -f ${EXEC}
	rm -f *.o

run: ${EXEC}
	./${EXEC} 

valgrun: ${EXEC}
	valgrind ./${EXEC}


${EXEC}:${OBJS}
	${CPP} ${FLAGS} -o ${EXEC} ${OBJS} -lm -lpthread

.c.o:
	${CPP} ${FLAGS} -c $<

assignment5.o: assignment6.c
