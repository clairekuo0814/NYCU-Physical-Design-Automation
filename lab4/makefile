OUT = Lab4
SOURCE = main.cpp channel_routing.cpp
HEADER = channel_routing.h 
OBJS = $(SOURCE:.cpp=.o)
CC = g++
CFLAGS = -std=c++11 -O3 

main: ${OBJS}
	${CC} ${OBJS} ${CFLAGS} -o ${OUT}
main.o: main.cpp ${HEADER}
	${CC} main.cpp ${CFLAGS} -c
channel_routing.o: channel_routing.cpp ${HEADER}
	${CC} channel_routing.cpp ${CFLAGS} -c
clean:
	rm -rf *.o ${OUT}