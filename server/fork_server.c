#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<signal.h>

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while (0)

void do_service(int);

int main(void)
{
	//signal(SIGCHLD, SIG_IGN);
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

	if (listen(listenfd, 10) < 0)
		ERR_EXIT("listen error");

	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);
	int conn;

	pid_t pid;

	while (1) {
		if ((conn = accept(listenfd, (struct sockaddr *)&peeraddr, &peerlen)) < 0)
			ERR_EXIT("accept error");
		printf("recv connect ip=%s port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

		pid = fork();
		if (pid == -1)
			ERR_EXIT("fork error");
		if (pid == 0) {
			printf("client close \n");
			close(listenfd);
			do_service(conn);
			exit(EXIT_SUCCESS);
		} else {
			close(conn);
		}
	}

	return 0;
}

void do_service(int conn)
{
	char recvbuf[1024];
	int size = 1024;
	int total = 0;
	while (1) {
		memset(recvbuf, 0, size);
		int ret = read(conn, recvbuf, size);
		if (ret == 0) {
			printf("client close\n");
			break;
		} else if (ret == -1)
			ERR_EXIT("read error");
		printf("len = %d\n", ret);
		total += ret;
		if (total >= size)
			break;
	}
	write(conn, "ack\n", 4);
}

