// hamming_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

// Function to check if number is power of 2
int isPowerOfTwo(int x) {
    return (x && !(x & (x - 1)));
}

// Calculate Hamming Code
void generateHamming(char data[]) {

    int m = strlen(data);
    int r = 0;

    // Calculate number of parity bits required
    while (pow(2, r) < (m + r + 1))
        r++;

    int totalBits = m + r;
    int hamming[totalBits + 1];

    int j = 0;
    for (int i = 1; i <= totalBits; i++) {
        if (isPowerOfTwo(i))
            hamming[i] = 0;  // parity bits placeholder
        else
            hamming[i] = data[j++] - '0';
    }

    // Calculate parity bits
    for (int i = 0; i < r; i++) {
        int pos = pow(2, i);
        int parity = 0;

        for (int k = 1; k <= totalBits; k++) {
            if (k & pos)
                parity ^= hamming[k];
        }
        hamming[pos] = parity;
    }

    printf("\nGenerated Hamming Code: ");
    for (int i = 1; i <= totalBits; i++)
        printf("%d", hamming[i]);

    // Introduce error at position 3 for demo
    printf("\nIntroducing error at position 3...\n");
    hamming[3] ^= 1;

    printf("Received Data: ");
    for (int i = 1; i <= totalBits; i++)
        printf("%d", hamming[i]);

    // Error detection
    int errorPos = 0;
    for (int i = 0; i < r; i++) {
        int pos = pow(2, i);
        int parity = 0;

        for (int k = 1; k <= totalBits; k++) {
            if (k & pos)
                parity ^= hamming[k];
        }

        if (parity)
            errorPos += pos;
    }

    if (errorPos == 0) {
        printf("\nNo error detected.\n");
    } else {
        printf("\nError detected at position: %d\n", errorPos);
        hamming[errorPos] ^= 1;

        printf("Corrected Data: ");
        for (int i = 1; i <= totalBits; i++)
            printf("%d", hamming[i]);
        printf("\n");
    }
}

int main() {

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Hamming Server Started...\n");
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    read(new_socket, buffer, 1024);

    printf("\nData received from client: %s\n", buffer);

    generateHamming(buffer);

    close(new_socket);
    close(server_fd);

    return 0;
}