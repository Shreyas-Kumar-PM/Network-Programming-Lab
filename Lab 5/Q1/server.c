// Lab 5 Q1: Concurrent Server - terminate if 3rd client connects
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5004
#define MAX 256

int main() {
    int sockfd, cli_fd[2], temp_fd;
    struct sockaddr_in servaddr, cliaddr[3];
    socklen_t len = sizeof(cliaddr[0]);
    char buf[MAX], result[MAX] = "Manipal";
    int client_count = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);

    printf("Concurrent Server on port %d. Waiting for clients...\n", PORT);

    while (1) {
        temp_fd = accept(sockfd, (struct sockaddr*)&cliaddr[client_count], &len);

        if (client_count == 2) {
            printf("Third client attempted to connect. Terminating server.\n");
            close(temp_fd);
            close(sockfd);
            exit(0);
        }

        cli_fd[client_count] = temp_fd;
        printf("Client %d connected from %s:%d\n",
               client_count + 1,
               inet_ntoa(cliaddr[client_count].sin_addr),
               ntohs(cliaddr[client_count].sin_port));

        client_count++;

        if (client_count == 2)
            break;
    }

    // Receive strings from both clients
    for (int i = 0; i < 2; i++) {
        memset(buf, 0, MAX);
        recv(cli_fd[i], buf, MAX, 0);
        strcat(result, " ");
        strcat(result, buf);
    }

    char final[MAX * 2];
    sprintf(final, "%s\nClient1: %s:%d\nClient2: %s:%d",
            result,
            inet_ntoa(cliaddr[0].sin_addr), ntohs(cliaddr[0].sin_port),
            inet_ntoa(cliaddr[1].sin_addr), ntohs(cliaddr[1].sin_port));

    printf("Result:\n%s\n", final);

    for (int i = 0; i < 2; i++) {
        send(cli_fd[i], final, strlen(final), 0);
        close(cli_fd[i]);
    }

    close(sockfd);
    return 0;
}
