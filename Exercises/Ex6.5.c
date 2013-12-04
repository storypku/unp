#include	"unp.h"
#if defined(SERV_PORT)
	#undef SERV_PORT
	#define SERV_PORT 19
#endif
int
main(int argc, char **argv)
{
	int					sockfd, i;
	struct sockaddr_in	servaddr;
	ssize_t nRead;
	char chr;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
	Shutdown(sockfd, SHUT_RD);

	printf("Shutdown SHUT_RD called\n");
	sleep(5);

	while(read(sockfd, &chr, 1) > 0) {
		printf("%d\n", i++);
	}

	printf("Encountered EOF (after all outstanding data read) as a result"
		   "\n  of shutdown(SHUT_RD). Now set an alarm of 3 seconds.\n");
	sleep(5);
	alarm(3);

	i = 0; errno = 0;
	while((nRead = read(sockfd, &chr, 1)) > 0) {
		printf("%d\n", i++);
	}

	printf("%ld, %s\n", (long)nRead, (errno == EINTR)? "EINTER":
										strerror(errno));
	close(sockfd);

	printf("Done with the experiment.\n");
	exit(0);
}
