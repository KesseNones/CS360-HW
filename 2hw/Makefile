CPP = gcc
FLAGS = -Wall -g

EXEC = dictDive
OBJS = lineNum.o main.o

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

lineNum.o: lineNum.c lineNum.h  
main.o: main.c lineNum.h