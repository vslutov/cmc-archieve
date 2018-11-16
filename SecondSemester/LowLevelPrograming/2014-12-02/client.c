#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include <stdio.h>

int
main(void)
{
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr = {AF_INET, 8080, 0};
    connect(sd, &serv_addr, sizeof(serv_addr));
    char buffer[1024];
    recv(sd, buffer, sizeof(buffer), 0);
    buffer[1023] = 0;
    printf("%s\n", buffer);
    return 0;
}

