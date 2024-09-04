CPP = gcc
FLAGS = -Wall -g

EXEC = 7EXE
OBJS = assignment7.o main.o

default:${EXEC}

clean:
	rm -f ${EXEC}
	rm -f *.o

run: ${EXEC}
	./${EXEC} 

valgrun: ${EXEC}
	valgrind ./${EXEC}


${EXEC}:${OBJS}
	${CPP} ${FLAGS} -o ${EXEC} ${OBJS} -lpthread

.c.o:
	${CPP} ${FLAGS} -c $<

assignment5.o: assignment7.c assignment7.h
main.o: main.c assignment7.h
