#include	"unp.h"

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	socklen_t		len;
	struct sockaddr	*preply_addr;

	preply_addr = Malloc(servlen);

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		len = servlen;
		n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);

		/* Liu Jiaming's Note:	Dec 06, 2013
		 * As tested on Mac OS X 10.9, even if the (addr, port) tuple for
		 * pservaddr and preply_addr does match, memcmp returns a non-zero
		 * value, indicating that additional field(s) has been filled from
		 * the recvfrom() call by the kernel. So, for maximum portability,
		 * I use the sock_cmp_addr() and sock_cmp_port() functions to verify
		 * returned socket address/port.
		 * */

		/* if (len != servlen || memcmp(pservaddr, preply_addr, len) != 0) { */
		if (len != servlen || sock_cmp_addr(pservaddr, preply_addr, len) != 0
						   || sock_cmp_port(pservaddr, preply_addr, len) != 0) {
			printf("reply from %s (ignored)\n",
					Sock_ntop(preply_addr, len));
			continue;
		}

		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);
	}
}
