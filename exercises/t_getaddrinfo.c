#include "unp.h"
int main(int argc, char *argv[]) {
    int s;
    struct addrinfo hint, *result, *rp;

    if(argc != 3 || strcmp(argv[1], "--help") == 0)
        err_quit("%s hostname service\n", argv[0]);

    memset(&hint, 0, sizeof(hint));
    hint.ai_socktype = 0;
    hint.ai_protocol = 0;
    hint.ai_family = AF_UNSPEC;
    hint.ai_flags = AI_CANONNAME;

    if((s = getaddrinfo(argv[1], argv[2], &hint, &result)) != 0)
        err_quit("%s", gai_strerror(s));

    printf("CANONNAME:      %s\n\n", result->ai_canonname);
    for(rp = result; rp != NULL; rp = rp->ai_next) {

        printf("Family:     %s\n", rp->ai_family == AF_INET ? "AF_INET":
                rp->ai_family == AF_INET6 ? "AF_INET6": "???");
        printf("SOCKET:     %s\n", rp->ai_socktype == SOCK_STREAM ? "SOCK_STREAM":
                rp->ai_socktype == SOCK_DGRAM ? "SOCK_DGRAM":
                rp->ai_socktype == SOCK_RAW ? "SOCK_RAW": "???");
        printf("PROTOCOL:   %s\n", rp->ai_protocol == IPPROTO_TCP? "IPPROTO_TCP":
                rp->ai_protocol == IPPROTO_UDP ? "IPPROTO_UDP":
                rp->ai_protocol == 0? "0": "???");
        printf("ADDRESS:    %s\n", Sock_ntop(rp->ai_addr, sizeof(*rp->ai_addr)));
        printf("\n");
    }

    freeaddrinfo(result);

    return 0;
}
