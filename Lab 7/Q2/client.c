// token_client.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {

    int sock = 0;
    struct sockaddr_in serv_addr;
    char *message = "Start Token Bucket Simulation";

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    send(sock, message,  strlen(message), 0);

    printf("Simulation request sent.\n");

    close(sock);

    return 0;
}