// sender.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char data[100], buffer[200];
    int count = 0;
    char parity_type, parity_bit;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    printf("Enter binary data: ");
    scanf("%s", data);

    printf("Enter parity type (e = even, o = odd): ");
    scanf(" %c", &parity_type);

    // Count number of 1s
    for (int i = 0; i < strlen(data); i++) {
        if (data[i] == '1')
            count++;
    }

    // Generate parity bit
    if (parity_type == 'e')
        parity_bit = (count % 2 == 0) ? '0' : '1';
    else
        parity_bit = (count % 2 == 0) ? '1' : '0';

    // Append parity bit
    sprintf(buffer, "%s%c%c", data, parity_bit, parity_type);

    send(sockfd, buffer, strlen(buffer), 0);

    printf("Data sent: %s (Parity bit = %c)\n", data, parity_bit);

    close(sockfd);
    return 0;
}
