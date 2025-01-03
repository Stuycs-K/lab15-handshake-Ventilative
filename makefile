all: client1 server1
client1: basic_client.o pipe_networking.o
	gcc -o client1 basic_client.o pipe_networking.o

server1: persistent_server.o pipe_networking.o
	gcc -o server1 persistent_server.o pipe_networking.o

basic_client.o: basic_client.c pipe_networking.h
	gcc -c basic_client.c

persistent_server.o: persistent_server.c pipe_networking.h
	gcc -c persistent_server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	gcc -c pipe_networking.c

clean:
	rm *.o
	rm *~
client: client1
	@./client1
server: server1
	@./server1
