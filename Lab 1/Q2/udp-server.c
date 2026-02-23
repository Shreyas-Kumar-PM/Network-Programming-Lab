// UDP Server - Checks palindrome, length, and vowel count
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>

#define BUF 1024

// Check if string is palindrome by comparing from both ends
int is_palindrome(char *s) {
    int len = strlen(s);  // strlen() returns string length (excluding null terminator)
    for (int i = 0; i < len / 2; i++)
        if (s[i] != s[len - 1 - i])  // Compare first with last, second with second-last, etc.
            return 0;
    return 1;
}

int main() {
    int sock;
    socklen_t len;
    struct sockaddr_in server, client;
    char buf[BUF], result[BUF];

    // socket() with SOCK_DGRAM creates UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    // Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(3388);  // htons() converts to network byte order
    server.sin_addr.s_addr = INADDR_ANY;  // INADDR_ANY accepts on all network interfaces
    
    // bind() attaches socket to port
    bind(sock, (struct sockaddr*)&server, sizeof(server));
    printf("Listening on port 3388...\n");

    while (1) {
        len = sizeof(client);
        memset(buf, 0, BUF);  // memset() zeros out buffer
        
        // recvfrom() receives datagram and captures sender address in 'client'
        recvfrom(sock, buf, BUF, 0, (struct sockaddr*)&client, &len);

        if (strcmp(buf, "Halt") == 0) break;  // strcmp() compares strings, returns 0 if equal

        // Count vowels using array indexed by vowel type
        int v[5] = {0};  // Initialize array to zeros: a, e, i, o, u
        for (int i = 0; buf[i]; i++) {  // Loop until null terminator
            char c = tolower(buf[i]);  // tolower() converts 'A'->'a', leaves 'a' as 'a'
            if (c == 'a') v[0]++;
            else if (c == 'e') v[1]++;
            else if (c == 'i') v[2]++;
            else if (c == 'o') v[3]++;
            else if (c == 'u') v[4]++;
        }

        // sprintf() formats string like printf but writes to buffer
        // strlen() returns length, used with %lu (unsigned long) format specifier
        sprintf(result, "%s palindrome\nLength: %lu\nVowels - A:%d E:%d I:%d O:%d U:%d",
                is_palindrome(buf) ? "Is" : "Not", strlen(buf), v[0], v[1], v[2], v[3], v[4]);
        
        // sendto() sends datagram to specific client address
        sendto(sock, result, strlen(result), 0, (struct sockaddr*)&client, sizeof(client));
    }
    close(sock);
    return 0;
}
