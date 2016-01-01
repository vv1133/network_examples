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

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while (0)

int main(int argc, const char *argv[])
{
	int sock;
	int size = 1024;
	char *buffer;

	if (argc > 1) {
		size = atoi(argv[1]);
		printf("size = %d\n", size);
	}
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

	printf("write...\n");
	writelen = send(sock, buffer, size, 0);

	printf("read...\n");
	while (readlen < writelen) {
		int templen = recv(sock, buffer, size, 0);
		if (templen <= 0) {
			ERR_EXIT("read error");
		}
		readlen += templen;
	}

	printf("close\n");
	close(sock);

	return 0;
}
