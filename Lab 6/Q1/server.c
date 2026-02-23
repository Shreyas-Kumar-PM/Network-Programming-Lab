// receiver.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000

int main() {
    int sockfd, newfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    char buffer[200];
    int count = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);

    printf("Receiver waiting on port %d...\n", PORT);

    newfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);

    int bytes = recv(newfd, buffer, sizeof(buffer) - 1, 0);
    buffer[bytes] = '\0';   // 🔥 FIX

    int n = strlen(buffer);
    char parity_type = buffer[n - 1];
    char parity_bit  = buffer[n - 2];
    buffer[n - 2] = '\0';   // remove parity + type

    // Count 1s in data
    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == '1')
            count++;
    }
    if (parity_bit == '1')
        count++;

    printf("Received data: %s\n", buffer);
    printf("Parity type: %c\n", parity_type);

    if ((parity_type == 'e' && count % 2 == 0) ||
        (parity_type == 'o' && count % 2 != 0))
        printf("Result: Data received correctly ✅\n");
    else
        printf("Result: Data is corrupted ❌\n");

    close(newfd);
    close(sockfd);
    return 0;
}