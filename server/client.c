#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static void usage()
{
    fprintf(stderr,
            "usage:\n"
            "\t./client {send_data}\n");
}


int main(int argc, char* argv[])
{

    int port = 9001;
    char *host = "localhost";
    int retval = 0;

    int client_sock;
    unsigned long in_addr;
    struct sockaddr_in server_addr;
    struct hostent *host_port;

    char *send_buffer;
    if(argc < 2)
    {
        usage();
        return 2;
    }
    else
    {
        strncpy(send_buffer, argv[1], sizeof(send_buffer));
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    in_addr = inet_addr(host);
    if(in_addr != INADDR_NONE)
    {
        memcpy(&server_addr.sin_addr, &in_addr, sizeof(in_addr));
    }
    else
    {
        host_port = gethostbyname(host);
        if(host_port == NULL)
        {
            perror("gethostbyname:");
                exit(-1);
        }
        memcpy(&server_addr.sin_addr, host_port->h_addr, host_port->h_length);
    }

    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(client_sock < 0)
    {
        perror("socket:");
        exit(-1);
    }

    if(connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect:");
        exit(-1);
    }
    retval = write(client_sock, send_buffer, strlen(send_buffer));
    if(retval != 12)
    {
        perror("write:");
        exit(-1);
    }
    printf("return value is :\t%d\n\r",retval);

    return 0;

}
