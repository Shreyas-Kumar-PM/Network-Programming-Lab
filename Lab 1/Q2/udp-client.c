// UDP Client - Sends strings for palindrome/vowel analysis
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF 1024

int main() {
    int sock;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(server);
    char buf[BUF];

    // socket() with SOCK_DGRAM creates UDP socket (connectionless)
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    // Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(3388);  // htons() converts port to network byte order
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  // inet_addr() converts "x.x.x.x" to binary
    
    // Configure client address
    client.sin_family = AF_INET;
    client.sin_port = htons(3389);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // bind() assigns local address to socket (optional for UDP client)
    bind(sock, (struct sockaddr*)&client, sizeof(client));

    printf("Ready.\n");
    while (1) {
        // Get string input
        printf("Enter string ('Halt' to exit): ");
        scanf(" %[^\n]", buf);  // %[^\n] reads until newline (allows spaces)
        
        // sendto() sends datagram to specified address (UDP doesn't need connect)
        sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&server, sizeof(server));

        // strcmp() compares strings, returns 0 if equal
        if (strcmp(buf, "Halt") == 0) break;

        // Receive result
        memset(buf, 0, BUF);  // memset() fills memory with value (here 0 to clear buffer)
        // recvfrom() receives datagram and stores sender's address
        recvfrom(sock, buf, BUF, 0, (struct sockaddr*)&server, &len);
        printf("Result:\n%s\n", buf);
    }
    
    // close() releases socket resources
    close(sock);
    return 0;
}
