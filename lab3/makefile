OUT = Lab3
SOURCE = main.cpp MOS_SEQ.cpp
HEADER = MOS_SEQ.h 
OBJS = $(SOURCE:.cpp=.o)
CC = g++
CFLAGS = -std=c++11 -O3 

main: ${OBJS}
	${CC} ${OBJS} ${CFLAGS} -o ${OUT}
main.o: main.cpp ${HEADER}
	${CC} main.cpp ${CFLAGS} -c
MOS_SEQ.o: MOS_SEQ.cpp ${HEADER}
	${CC} MOS_SEQ.cpp ${CFLAGS} -c
clean:
	rm -rf *.o ${OUT}