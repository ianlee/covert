CC = gcc
CFLAGS = -Wall -g 
LDFLAGS = -lpthread

all: main
client: main_client

main: client.o server.o
	${CC} ${CFLAGS} main.cpp client.o server.o ${LDFLAGS} -o exe/covert

client.o : client.c client.h helpers.o
	${CC} ${CFLAGS} -c client.c

	
server.o : server.c server.h helpers.o
	${CC} ${CFLAGS} -c server.c

helpers.o : helpers.c helpers.h
	${CC} ${CFLAGS} -c helpers.c


clean:
	rm -rf *.o  *.cpp~ *.h~ exe/covert
