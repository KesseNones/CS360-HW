CPP = gcc
FLAGS = -Wall -g

EXEC = 5EXE
OBJS = assignment5.o

default:${EXEC}

clean:
	rm -f ${EXEC}
	rm -f *.o

run: ${EXEC}
	./${EXEC} 

valgrun: ${EXEC}
	valgrind ./${EXEC}


${EXEC}:${OBJS}
	${CPP} ${FLAGS} -o ${EXEC} ${OBJS} -lm #FOR MATH LIBRARY

.c.o:
	${CPP} ${FLAGS} -c $<

assignment5.o: assignment5.c
