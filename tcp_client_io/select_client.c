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

	char sendbuf[1024] = "I'm select\n";
	char recvbuf[1024] = {0};
	write(sock, sendbuf, strlen(sendbuf));

	int maxfd = sock;
	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(sock, &rset);

	while (1) {
		int nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
		if (nready == -1)
			ERR_EXIT("select error");
		if (nready == 0)
			continue;

		if (FD_ISSET(sock, &rset)) {
			read(sock, recvbuf, sizeof(recvbuf));
			fputs(recvbuf, stdout);
			break;
		}
	}

	close(sock);

	return 0;
}
