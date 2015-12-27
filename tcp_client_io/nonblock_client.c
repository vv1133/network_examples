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
#include<fcntl.h>

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while (0)

int main(void)
{
	int sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket error");

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("connect error");
	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
		ERR_EXIT("fcntl error");

	char sendbuf[1024] = "I'm nonblock\n";
	char recvbuf[1024] = {0};
	write(sock, sendbuf, strlen(sendbuf));
	while (1) {
		int ret = read(sock, recvbuf, sizeof(recvbuf));
		if (ret > 0) {
			break;
		}
		sleep(1);
		printf("sleep...\n");
	}
	fputs(recvbuf, stdout);

	close(sock);

	return 0;
}
