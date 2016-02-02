.PHONY: all clean
all: fork_server select_server client

fork_server:
	gcc -g fork_server.c -o fork_server

select_server:
	gcc -g select_server.c -o select_server

client:
	gcc client.c -o client

clean:
	-rm fork_server select_server client
