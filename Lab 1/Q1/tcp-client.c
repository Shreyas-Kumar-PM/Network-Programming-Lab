// TCP Client - Menu-driven operations (search, sort, odd/even split)
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 3388
#define BUF 1024

int main() {
    int sock;
    struct sockaddr_in server;
    char buf[BUF], choice;
    
    // socket() creates endpoint for communication, returns file descriptor
    // AF_INET = IPv4, SOCK_STREAM = TCP connection
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    // Configure server address structure
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);  // htons() converts port to network byte order (big-endian)
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  // inet_addr() converts IP string to binary
    
    // connect() establishes connection to server
    connect(sock, (struct sockaddr*)&server, sizeof(server));
    printf("Connected to Server.\n");

    while (1) {
        // Display menu
        printf("\n1.Search 2.Sort Asc 3.Sort Desc 4.Odd/Even 5.Exit\nChoice: ");
        scanf("%s", buf);
        choice = buf[0];
        
        // send() transmits data over TCP connection
        // strlen() returns length of string (excluding null terminator)
        send(sock, buf, strlen(buf), 0);

        if (choice == '5') break;

        // Get array size and send
        printf("Count: "); 
        scanf("%s", buf); 
        send(sock, buf, strlen(buf), 0);
        
        // Get array elements and send
        printf("Numbers: "); 
        scanf(" %[^\n]", buf);  // %[^\n] reads entire line including spaces until newline
        send(sock, buf, strlen(buf), 0);
        
        // For search operation, get target number
        if (choice == '1') {
            printf("Search for: "); 
            scanf("%s", buf); 
            send(sock, buf, strlen(buf), 0);
        }

        // Receive and display result
        memset(buf, 0, BUF);  // memset() fills memory with constant byte (here 0 to clear buffer)
        recv(sock, buf, BUF, 0);  // recv() receives data from connected socket
        printf("Result: %s\n", buf);
    }
    
    // close() closes socket and frees resources
    close(sock);
    return 0;
}
