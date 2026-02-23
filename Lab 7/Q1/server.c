// leaky_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUCKET_SIZE 10
#define OUTPUT_RATE 1
#define PACKET_SIZE 4

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    int arrival_times[] = {1,2,3,5,6,8,11,12,15,16,19};
    int n = sizeof(arrival_times)/sizeof(arrival_times[0]);

    int bucket = 0;
    int last_time = 0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Leaky Bucket Server Started...\n");
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    char buffer[1024] = {0};
    read(new_socket, buffer, 1024);

    printf("\nSimulating Leaky Bucket:\n");
    printf("Time\tBucketBefore\tStatus\t\tBucketAfter\n");

    for(int i=0; i<n; i++) {

        int current_time = arrival_times[i];

        // Leak the bucket
        int leaked = (current_time - last_time) * OUTPUT_RATE;
        bucket -= leaked;
        if(bucket < 0)
            bucket = 0;

        printf("%d\t%d\t\t", current_time, bucket);

        if(bucket + PACKET_SIZE <= BUCKET_SIZE) {
            bucket += PACKET_SIZE;
            printf("Conforming\t%d\n", bucket);
        } else {
            printf("Non-Conforming\t%d\n", bucket);
        }

        last_time = current_time;
    }

    close(new_socket);
    close(server_fd);
    return 0;
}