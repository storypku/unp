/* File tcpcliserv/tsigpipe.c */
#include	"unp.h"
/* Running this program under Ubuntu 13.10 x86_64 generates the following:
 *
 * 		[story@ubuntu:unp]$ tsigpipe 127.0.0.1
 * 		SIGPIPE received
 * 		write error: [EPIPE] Broken pipe
 *
 * The initial sleep of 2s is to let the daytime server send its reply and
 * close its end of connection. Our first write sends a data segment to the
 * server, which responds with an RST (since the daytime server has completely
 * closed its socket). Note that our TCP allows us to write to a socket that 
 * has received a FIN. The second sleep lets the server's RST be received, and
 * our second write generates SIGEPIPE. Since our signal handler returns,
 * write returns an error of EPIPE.
 */
void
sig_pipe(int signo)
{
	printf("SIGPIPE received\n");
	return;
}

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);		/* daytime server */
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Signal(SIGPIPE, sig_pipe);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	sleep(2);
	Write(sockfd, "hello", 5);
	sleep(2);
	Write(sockfd, "world", 5);

	exit(0);
}
