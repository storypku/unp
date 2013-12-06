#include "unp.h"
static void usageErr(char *progName) {
    fprintf(stderr, "%s [options]\n"
                    "   Available options:\n"
                    "       -t          TCP\n"
                    "       -u          UDP\n"
#ifdef IPPROTO_SCTP
                    "       -s          SCTP\n"
#endif
                    , progName);
    exit(EXIT_FAILURE);
}

static void printBufferSize(const int sockfd, const char *msg) {
    int optval;
    socklen_t optlen = sizeof(optval);
    if(msg != NULL && *msg != '\0')
        printf("%s\n", msg);
    if(getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &optval, &optlen) == -1)
        err_sys("getsockopt - SO_SNDBUF");
    printf("    Send buffer size: %ld\n", (long)optval);
    if(getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &optval, &optlen) == -1)
        err_sys("getsockopt - SO_RCVBUF");
    printf("    Receive buffer size: %ld\n", (long)optval);
    return;
}

int main(int argc, char *argv[]) {
    int sockfd, opt;
    int bits = 0;
    if(argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr(argv[0]);
    while ((opt = getopt(argc, argv, "tu"
#ifdef IPPROTO_SCTP
                    "s"
#endif
                    )) != -1) {
        switch(opt) {
            case 't':
                bits |= 0x01;
                break;
            case 'u':
                bits |= 0x02;
                break;
#ifdef IPPROTO_SCTP
            case 's':
                bits |= 0x04;
                break;
#endif
            default:
                usageErr(argv[0]);
        }
    }
    if(argc != optind)
        usageErr(argv[0]);
    if(bits&0x01) {
        sockfd = Socket(AF_INET, SOCK_STREAM, 0);
        printBufferSize(sockfd, "TCP");
    }
    if(bits&0x02) {
        sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
        printBufferSize(sockfd, "UDP");
    }
#ifdef IPPROTO_SCTP
    if(bits&0x04) {
        sockfd = Socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
        printBufferSize(sockfd, "SCTP");
    }
#endif
    exit(EXIT_SUCCESS);
}
