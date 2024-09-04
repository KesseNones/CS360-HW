CPP = gcc
FLAGS = -Wall -g

EXEC = 4EXE
OBJS = assignment4.o

default:${EXEC}

clean:
	rm -f ${EXEC}
	rm -f *.o

run: ${EXEC}
	./${EXEC} 

valgrun: ${EXEC}
	valgrind ./${EXEC}


${EXEC}:${OBJS}
	${CPP} ${FLAGS} -o ${EXEC} ${OBJS}

.c.o:
	${CPP} ${FLAGS} -c $<

assignment4.o: assignment4.c