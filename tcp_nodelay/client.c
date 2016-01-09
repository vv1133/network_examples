#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<netinet/tcp.h>

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while (0)

#define OPTION_NORMAL 0
#define OPTION_BUFFERING 1
#define OPTION_NO_DELAY 2
int get_args(int argc, char *argv[])
{
	int option = OPTION_NORMAL;
	int ch;
	opterr = 0;

	while ((ch = getopt(argc, argv, "bn")) != -1) {
		switch(ch) {
		case 'b':
			option = OPTION_BUFFERING;
			printf("buffering\n");
			break;
		case 'n':
			option = OPTION_NO_DELAY;
			printf("no delay\n");
			break;
		default:
			option = OPTION_NORMAL;
		}
	}
	return option;
}

int main(int argc, char *argv[])
{
	int sock;
	int size = 1024;
	char *buffer;
	int option = get_args(argc, argv);

	buffer = malloc(size);
	if (buffer == NULL)
		ERR_EXIT("malloc error");
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket error");

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("connect error");

	int writelen, readlen;
	int on = 1;

	if (option == OPTION_NORMAL) {
		printf("write...\n");
		writelen = send(sock, buffer, size/2, 0);
		usleep(100);
		printf("write...\n");
		writelen = send(sock, buffer+size/2, size/2, 0);
	} else if (option == OPTION_BUFFERING) {
		printf("write...\n");
		writelen = send(sock, buffer, size, 0);
	} else if (option == OPTION_NO_DELAY) {
		printf("set tcp nodelay\n");
		setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *)&on, sizeof(on));
		printf("write...\n");
		writelen = send(sock, buffer, size/2, 0);
		usleep(100);
		printf("write...\n");
		writelen = send(sock, buffer+size/2, size/2, 0);
	}

	printf("read...\n");
	readlen = recv(sock, buffer, size, 0);

	printf("close\n");
	close(sock);

	return 0;
}
