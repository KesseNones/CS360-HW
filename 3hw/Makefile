CPP = gcc
FLAGS = -Wall -g

EXEC = fileCount
OBJS = assignment3.o main.o

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

assignment3.o: assignment3.c assignment3.h  
main.o: main.c assignment3.h