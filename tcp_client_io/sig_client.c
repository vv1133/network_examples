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
#include<signal.h>

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while (0)

volatile int finish = 0;
int sock;
void do_sigio(int sig)  
{  
	char recvbuf[1024] = {0};

	read(sock, recvbuf, sizeof(recvbuf));
	fputs(recvbuf, stdout);
	close(sock);

	finish = 1;
}  

int main(void)
{
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket error");

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("connect error");

	struct sigaction sigio_action;
	memset(&sigio_action, 0, sizeof(sigio_action));
	sigio_action.sa_flags = 0;
	sigio_action.sa_handler = do_sigio;
	sigaction(SIGIO, &sigio_action, NULL);

	fcntl(sock, F_SETOWN, getpid());
	int flag = fcntl(sock, F_GETFL ,0);
	flag = flag | O_ASYNC | O_NONBLOCK;
	fcntl(sock, F_SETFL, flag);

	char sendbuf[1024] = "I'm sig\n";
	write(sock, sendbuf, strlen(sendbuf));

	while (finish == 0);

	return 0;
}
