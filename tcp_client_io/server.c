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

int main(void)
{
	int listenfd;
	if ((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket error");

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int on = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		ERR_EXIT("setsockopt error");

	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("bind error");

	if (listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen error");

	int conn;
	while (1) {
		struct sockaddr_in peeraddr;
		socklen_t peerlen = sizeof(peeraddr);
		if ((conn = accept(listenfd, (struct sockaddr *)&peeraddr, &peerlen)) < 0)
			ERR_EXIT("accept error");
		printf("recv connect ip=%s port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

		char recvbuf[1024];
		char *ack = "ack\n";

		memset(recvbuf, 0, sizeof(recvbuf));
		read(conn, recvbuf, sizeof(recvbuf));
		fputs(recvbuf, stdout);
		sleep(3);
		write(conn, ack, strlen(ack));
	}
	close(conn);
	close(listenfd);

	return 0;
}
