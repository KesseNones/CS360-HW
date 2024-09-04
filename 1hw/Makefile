
CPP = gcc
FLAGS = -Wall -g

EXEC = wordpairs
OBJS = getWord.o crc64.o wordPairLinkedList.o hashTable.o main.o

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

getWord.o: getWord.c getWord.h
crc64.o: crc64.c crc64.h
wordPairLinkedList.o: wordPairLinkedList.c wordPairLinkedList.h
hashTable.o: hashTable.c hashTable.h 
main.o: main.c getWord.h crc64.h hashTable.h wordPairLinkedList.h