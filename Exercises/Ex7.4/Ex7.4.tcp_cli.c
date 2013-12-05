/* Use standard echo server; baseline measurements for nonblocking version */
#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd, optval;
	struct sockaddr_in	servaddr, cliaddr;
	if (argc != 5)
		err_quit("usage: tcpcli laddr lport srvaddr srvport");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	optval = 1;
	Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval,
											sizeof(optval));
	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons((short int)atoi(argv[2]));
	Inet_pton(AF_INET, argv[1], &cliaddr.sin_addr);
	Bind(sockfd, (const SA *)&cliaddr, sizeof(cliaddr));

	sleep(10); /* Sleep to let another instance to bind */

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons((short int)atoi(argv[4]));
	Inet_pton(AF_INET, argv[3], &servaddr.sin_addr);

	while(connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		continue;
	str_cli(stdin, sockfd);
	exit(0);
}
