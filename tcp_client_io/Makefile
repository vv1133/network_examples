.PHONY: all clean
all: server block_client nonblock_client select_client sig_client

server:
	gcc server.c -o server

block_client:
	gcc block_client.c -o block_client

nonblock_client:
	gcc nonblock_client.c -o nonblock_client

select_client:
	gcc select_client.c -o select_client

sig_client:
	gcc sig_client.c -o sig_client

clean:
	-rm server block_client nonblock_client select_client sig_client
