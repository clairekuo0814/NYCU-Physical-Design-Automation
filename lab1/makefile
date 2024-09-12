OUT = Lab1
SOURCE = main.cpp FM.cpp
HEADER = FM.h 
OBJS = $(SOURCE:.cpp=.o)
CC = g++
CFLAGS = -std=c++11 -O3 

main: ${OBJS}
	${CC} ${OBJS} ${CFLAGS} -o ${OUT}
main.o: main.cpp ${HEADER}
	${CC} main.cpp ${CFLAGS} -c
FM.o: FM.cpp ${HEADER}
	${CC} FM.cpp ${CFLAGS} -c
clean:
	rm -rf *.o ${OUT}