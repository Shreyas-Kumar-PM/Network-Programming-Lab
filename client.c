// hamming_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {

    int sock = 0;
    struct sockaddr_in serv_addr;
    char data[100];

    printf("Enter binary data: ");
    scanf("%s", data);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    send(sock, data, strlen(data), 0);

    printf("Data sent to server.\n");

    close(sock);

    return 0;
}