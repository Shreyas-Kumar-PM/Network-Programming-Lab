// token_server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

#define TOKEN_RATE 10.0      // KB per second
#define BUCKET_SIZE 50.0     // KB
#define PACKET_SIZE 15.0     // KB
#define INTERVAL 0.5         // seconds
#define MAX_TIME 5.0         // simulate first 5 seconds

int main() {

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Token Bucket Server Started...\n");
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    double tokens = BUCKET_SIZE;   // starts full
    double time = 0.0;
    double next_packet = 0.5;
    int queued_started = 0;

    printf("\nTime\tTokens Before\tStatus\t\tTokens After\n");

    while(time <= MAX_TIME) {

        time += INTERVAL;

        // Add tokens
        tokens += TOKEN_RATE * INTERVAL;
        if(tokens > BUCKET_SIZE)
            tokens = BUCKET_SIZE;

        if(time >= next_packet) {

            printf("%.1f\t%.2f\t\t", time, tokens);

            if(tokens >= PACKET_SIZE) {
                tokens -= PACKET_SIZE;
                printf("Sent\t\t%.2f\n", tokens);
            } else {
                printf("Queued\t\t%.2f\n", tokens);
                if(!queued_started) {
                    printf("\nPackets start getting queued at t = %.1f seconds\n", time);
                    queued_started = 1;
                }
            }

            next_packet += INTERVAL;
        }
    }

    printf("\n(i) Tokens left after 1.5 seconds ≈ 20 KB\n");
    printf("(iii) Maximum burst size when R = 20KBps is equal to bucket size = 50 KB\n");

    close(new_socket);
    close(server_fd);

    return 0;
}