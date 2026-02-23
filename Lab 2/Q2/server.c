#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define MAXSIZE 4096

int main(void)
{
    int sockfd, retval;
    socklen_t actuallen;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE], filename[MAXSIZE], filecontent[MAXSIZE * 10];
    FILE *fp;
    int alphabets = 0, digits = 0, spaces = 0, lines = 0, others = 0;
    int filesize;

    /* Create a UDP socket
     * AF_INET: IPv4 address family
     * SOCK_DGRAM: UDP (connectionless, datagram-based)
     * 0: Default protocol for SOCK_DGRAM (UDP)
     */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        printf("\nSocket creation error");
        exit(0);
    }

    /* Configure server address structure
     * sin_family: Address family (IPv4)
     * sin_port: Port number in network byte order
     * sin_addr.s_addr: Local IP address
     */
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3392);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Bind the socket to the configured address and port */
    retval = bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    if (retval == -1)
    {
        printf("Binding error");
        close(sockfd);
        exit(0);
    }

    actuallen = sizeof(clientaddr);

    while (1)
    {
        /* Receive filename from client */
        recedbytes = recvfrom(sockfd, filename, sizeof(filename), 0, (struct sockaddr*)&clientaddr, &actuallen);

        if (recedbytes == -1)
        {
            close(sockfd);
            exit(0);
        }

        /* Check if client wants to stop */
        if (strcmp(filename, "stop") == 0)
        {
            break;
        }

        /* Check if file exists */
        fp = fopen(filename, "r");
        if (fp == NULL)
        {
            strcpy(buff, "File not present");
            sentbytes = sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&clientaddr, actuallen);
            continue;
        }

        /* Read file content */
        fseek(fp, 0, SEEK_END);
        filesize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        fread(filecontent, sizeof(char), sizeof(filecontent), fp);
        fclose(fp);

        /* Count characters in file */
        alphabets = 0;
        digits = 0;
        spaces = 0;
        lines = 0;
        others = 0;

        for (int i = 0; i < filesize; i++)
        {
            if ((filecontent[i] >= 'a' && filecontent[i] <= 'z') || (filecontent[i] >= 'A' && filecontent[i] <= 'Z'))
            {
                alphabets++;
            }
            else if (filecontent[i] >= '0' && filecontent[i] <= '9')
            {
                digits++;
            }
            else if (filecontent[i] == ' ')
            {
                spaces++;
            }
            else if (filecontent[i] == '\n')
            {
                lines++;
            }
            else
            {
                others++;
            }
        }

        /* Send file content and statistics to client */
        sentbytes = sendto(sockfd, filecontent, sizeof(filecontent), 0, (struct sockaddr*)&clientaddr, actuallen);

        sprintf(buff, "%d", filesize);
        sentbytes = sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&clientaddr, actuallen);

        sprintf(buff, "%d", alphabets);
        sentbytes = sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&clientaddr, actuallen);

        sprintf(buff, "%d", lines);
        sentbytes = sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&clientaddr, actuallen);

        sprintf(buff, "%d", spaces);
        sentbytes = sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&clientaddr, actuallen);

        sprintf(buff, "%d", digits);
        sentbytes = sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&clientaddr, actuallen);

        sprintf(buff, "%d", others);
        sentbytes = sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&clientaddr, actuallen);
    }

    close(sockfd);

    return 0;
}
