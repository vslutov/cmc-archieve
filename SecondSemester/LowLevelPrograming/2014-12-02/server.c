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
    struct sockaddr_in serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(8080);
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    bind(sd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    listen(sd, 5);

    struct sockaddr_in cli_addr;
    int cd = accept(sd, (struct sockaddr *) &cli_addr, sizeof(cli_addr));

    char buffer[1024] = "Hello, world!";
    recv(sd, buffer, sizeof(buffer), 0);
    buffer[1023] = 0;
    printf("%s\n", buffer);
    return 0;
}

