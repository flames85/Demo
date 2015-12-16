#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 80

void do_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{
    int n;
    socklen_t len;
    char mesg[MAXLINE];

    for(;;)
    {
        len = clilen;
        /* waiting for receive data */
        n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);


        // 将sockaddr强制转换为 sockaddr_in
        struct sockaddr_in sin ;
        memcpy(&sin, pcliaddr, sizeof(sin));

        // 取得ip和端口号
        printf("peer port %d\n", ntohs(sin.sin_port));

        /* sent data back to client */
        sendto(sockfd, mesg, n, 0, pcliaddr, len);
    }
}

int main(int argc, char* argv[])
{
    /* check args */
    if(argc != 2)
    {
        printf("usage: udp_server port\n");
        exit(1);
    }
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); /* create a socket */

    /* init servaddr */
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    /* bind address and port to socket */
    if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("bind error");
        exit(1);
    }

    do_echo(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

    return 0;
}
