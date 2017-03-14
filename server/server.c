#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>

#define PORT 9001
#define QUEUE_MAX_COUNT 5
#define BUFF_SIZE 1024

//void parse_argments()
//{
//
//}

int main(int argc, char* argv[])
{
    char host[48];
    unsigned long port = 1234;

    char *opt_str = "h:p:cd";
    char option;

//    while((option = getopt(argc, argv, opt_str)) != -1)
//    {
//        printf("optind %d\n", optind);
//        printf("optarg: %s\n", optarg);
//        printf("optopt: %d\n", optopt);
//        printf("opterr: %d\n", opterr);
//        switch(option)
//        {
//            case 'h':
//                strncpy(host, optarg, strlen(optarg) + 1);
//                break;
//            case 'p':
//                port = strtoul(optarg, NULL, 0);
//                break;
//            default:
//                break;
//        }
//    }
    int arg_index;
    static int tmp = 0;
    static const struct option long_options[] =
    {
        {"host", required_argument, NULL, 'h'},
        {"port", required_argument, NULL, 'p'},
        {"ccc", no_argument, NULL, 'c'},
        {NULL, 0, NULL, 0}
    };
    while((option = getopt_long(argc, argv, opt_str, long_options, &arg_index)) != -1)
    {
        printf("optind %d\n", optind);
        printf("optarg: %s\n", optarg);
        printf("optopt: %d\n", optopt);
        printf("opterr: %d\n\n", opterr);
        switch(option)
        {
            case 'h':
                printf("host:\n");
                strncpy(host, optarg, strlen(optarg) + 1);
                break;
            case 'p':
                printf("port:\n");
                port = strtoul(optarg, NULL, 0);
                break;
            default:
                break;
        }
    }

    int server_fd;
    int client_fd;

    int local_file_fd;
    int ret_wr = 0;

//    u_short port = PORT;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    struct sockaddr_in server_addr;

    char buff[BUFF_SIZE];
    char recv_buf[BUFF_SIZE];
    int recv_len = 0;
    char buffer[BUFF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1)
    {
        perror("socket:");
        exit(-1);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    printf("%s", INADDR_ANY);
    unsigned long host_addr = inet_addr(host);
    memcpy(&server_addr.sin_addr, &host_addr, sizeof(host_addr));
    //server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind:");
        exit(-1);
    }

    if(listen(server_fd, QUEUE_MAX_COUNT) < 0)
    {
        perror("listen:");
        exit(-1);
    }

    printf("This server listen on %d\n", port);

    while(1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if(client_fd < 0)
        {
            perror("accept:");
            exit(-1);
        }

        local_file_fd = open("./test.file", O_RDWR|O_CREAT, S_IRUSR|S_IXUSR);
        if(local_file_fd < 0)
        {
            perror("open:");
            goto FAIL;
        }

        printf("accept a client request successfully\n");
        do{
            recv_len = recv(client_fd, recv_buf, BUFF_SIZE, 0);
            if(recv_len == 0)
            {
                close(local_file_fd);
                break;
            }
            if(recv_len < 0)
            {
                close(local_file_fd);
                perror("recv:");
                goto FAIL;
            }

            ret_wr = write(local_file_fd, recv_buf, recv_len);
            if(ret_wr != recv_len)
            {
                perror("write:");
                exit(-1);
            }

            printf("%d\n", strlen(buffer));
            snprintf(buffer, BUFF_SIZE, "%s", recv_buf);
            memset(recv_buf, 0, strlen(recv_buf));
            printf("receive content is: %s\n", buffer);

        }while(1);

//        printf("receive content is : %s", recv_buf);

        sprintf(buff, "hello world");
        send(client_fd, buff, strlen(buff), 0);

    }

    return 0;

FAIL:
    close(client_fd);
    close(server_fd);
    return -1;
}
