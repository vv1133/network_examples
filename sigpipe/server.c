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
#include<signal.h>

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while (0)

int main(void)
{
//	signal(SIGPIPE, SIG_IGN);
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

	while (1) {
		int conn;
		struct sockaddr_in peeraddr;
		socklen_t peerlen = sizeof(peeraddr);
		if ((conn = accept(listenfd, (struct sockaddr *)&peeraddr, &peerlen)) < 0)
			ERR_EXIT("accept error");
		printf("recv connect ip=%s port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

		while (1) {
			char recvbuf[1024];
			int readlen, writelen;

			readlen = recv(conn, recvbuf, sizeof(recvbuf), 0);
			printf("read len = %d\n", readlen);
			if (readlen <= 0)
				break;
			writelen = send(conn, recvbuf, readlen, 0);
			printf("write1 len = %d\n", writelen);
			sleep(1);
			writelen = send(conn, recvbuf, readlen, 0);
			printf("write2 len = %d\n", writelen);
			writelen = send(conn, recvbuf, readlen, 0);
			printf("write3 len = %d\n", writelen);
			if (writelen < 0)
				perror("write error");
		}
		printf("close\n");
		close(conn);
	}
	close(listenfd);

	return 0;
}
