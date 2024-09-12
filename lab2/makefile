OUT = Floorplan
SOURCE = main.cpp BST.cpp
HEADER = BST.h 
OBJS = $(SOURCE:.cpp=.o)
CC = g++
CFLAGS = -std=c++11 -O3 

main: ${OBJS}
	${CC} ${OBJS} ${CFLAGS} -o ${OUT}
main.o: main.cpp ${HEADER}
	${CC} main.cpp ${CFLAGS} -c
BST.o: BST.cpp ${HEADER}
	${CC} BST.cpp ${CFLAGS} -c
clean:
	rm -rf *.o ${OUT}