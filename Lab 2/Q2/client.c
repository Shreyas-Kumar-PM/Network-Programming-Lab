#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 4096

int main(void)
{
    int sockfd, retval;
    socklen_t actuallen;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE], filename[MAXSIZE], filecontent[MAXSIZE * 10];
    int filesize, alphabets, lines, spaces, digits, others;

    /* Create a UDP socket
     * AF_INET: IPv4 address family
     * SOCK_DGRAM: UDP (connectionless, datagram-based)
     * 0: Default protocol for SOCK_DGRAM (UDP)
     */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        printf("\nSocket Creation Error");
        exit(0);
    }

    /* Configure client address structure
     * sin_family: Address family (IPv4)
     * sin_port: Port number in network byte order
     * sin_addr.s_addr: Local IP address
     */
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(3393);
    clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Configure server address structure */
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3392);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Bind the socket to the client address */
    retval = bind(sockfd, (struct sockaddr*)&clientaddr, sizeof(clientaddr));

    if (retval == -1)
    {
        printf("Binding error");
        close(sockfd);
        exit(0);
    }

    actuallen = sizeof(serveraddr);

    while (1)
    {
        /* Get filename from user */
        printf("Enter filename (or 'stop' to exit): ");
        scanf("%s", filename);

        /* Send filename to server */
        sentbytes = sendto(sockfd, filename, sizeof(filename), 0, (struct sockaddr*)&serveraddr, actuallen);

        if (sentbytes == -1)
        {
            printf("Send error");
            close(sockfd);
            exit(0);
        }

        /* Check if user wants to stop */
        if (strcmp(filename, "stop") == 0)
        {
            break;
        }

        /* Receive response from server */
        recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&serveraddr, &actuallen);

        if (recedbytes == -1)
        {
            close(sockfd);
            exit(0);
        }

        if (strcmp(buff, "File not present") == 0)
        {
            printf("%s\n", buff);
            continue;
        }

        /* Receive file content and statistics from server */
        strcpy(filecontent, buff);

        recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&serveraddr, &actuallen);
        filesize = atoi(buff);

        recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&serveraddr, &actuallen);
        alphabets = atoi(buff);

        recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&serveraddr, &actuallen);
        lines = atoi(buff);

        recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&serveraddr, &actuallen);
        spaces = atoi(buff);

        recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&serveraddr, &actuallen);
        digits = atoi(buff);

        recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&serveraddr, &actuallen);
        others = atoi(buff);

        /* Display file content and statistics */
        printf("\nFile content:\n%s\n", filecontent);
        printf("File size: %d bytes\n", filesize);
        printf("Number of alphabets: %d\n", alphabets);
        printf("Number of lines: %d\n", lines);
        printf("Number of spaces: %d\n", spaces);
        printf("Number of digits: %d\n", digits);
        printf("Number of other characters: %d\n", others);
    }

    close(sockfd);

    return 0;
}
