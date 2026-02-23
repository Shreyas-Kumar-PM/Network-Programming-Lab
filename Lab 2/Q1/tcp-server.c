// TCP Server - Handles search, sort, odd/even operations
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define PORT 3388
#define BUF 1024

// Comparison functions for qsort - return negative/zero/positive for less/equal/greater
int asc(const void *a, const void *b) { 
    return *(int*)a - *(int*)b;  // Cast void* to int* then dereference
}

int desc(const void *a, const void *b) { 
    return *(int*)b - *(int*)a; 
}

// Parse space-separated integers into array, returns count
int parse(char *s, int *arr) {
    int n = 0;
    // strtok() splits string by delimiter, returns tokens one by one
    for (char *t = strtok(s, " "); t; t = strtok(NULL, " "))
        arr[n++] = atoi(t);  // atoi() converts string to integer ("123" -> 123)
    return n;
}

int main() {
    int sock, client, n, arr[100];
    socklen_t len;  // socklen_t is unsigned int type for socket address lengths
    struct sockaddr_in server, caddr;
    char buf[BUF];

    // socket() creates endpoint, SOCK_STREAM = TCP
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    // Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);  // htons() converts host byte order to network byte order
    server.sin_addr.s_addr = INADDR_ANY;  // INADDR_ANY = accept connections on any interface (0.0.0.0)
    
    // bind() assigns address to socket
    bind(sock, (struct sockaddr*)&server, sizeof(server));
    
    // listen() marks socket as passive, ready to accept connections (backlog=1)
    listen(sock, 1);
    printf("Listening on port %d...\n", PORT);

    // accept() blocks until client connects, returns new socket for that client
    len = sizeof(caddr);
    client = accept(sock, (struct sockaddr*)&caddr, &len);
    printf("Client connected.\n");

    while (1) {
        // Receive operation choice
        memset(buf, 0, BUF);  // memset() fills buffer with 0s to clear previous data
        recv(client, buf, BUF, 0);  // recv() receives data from connected socket
        char op = buf[0];

        if (op == '5') break;

        // Receive array size
        memset(buf, 0, BUF); 
        recv(client, buf, BUF, 0); 
        n = atoi(buf);  // atoi() converts "5" -> 5
        
        // Receive and parse array elements
        memset(buf, 0, BUF); 
        recv(client, buf, BUF, 0); 
        parse(buf, arr);

        if (op == '1') {
            // Search operation
            memset(buf, 0, BUF); 
            recv(client, buf, BUF, 0);
            int target = atoi(buf), pos = -1;
            
            // Linear search through array
            for (int i = 0; i < n; i++)
                if (arr[i] == target) { 
                    pos = i + 1;  // 1-indexed position
                    break; 
                }
            // sprintf() formats string like printf but stores in buffer
            sprintf(buf, pos > 0 ? "Found at position %d" : "Not found", pos);
        }
        else if (op == '2' || op == '3') {
            // qsort() is standard library quicksort
            // qsort(array, count, element_size, comparison_function)
            qsort(arr, n, sizeof(int), op == '2' ? asc : desc);
            
            // Build result string
            sprintf(buf, "Sorted: ");
            for (int i = 0; i < n; i++) 
                sprintf(buf + strlen(buf), "%d ", arr[i]);  // Append to existing string
        }
        else if (op == '4') {
            // Split into even and odd numbers
            sprintf(buf, "Even: ");
            for (int i = 0; i < n; i++) 
                if (arr[i] % 2 == 0)  // % is modulo operator (remainder after division)
                    sprintf(buf + strlen(buf), "%d ", arr[i]);
            
            strcat(buf, "\nOdd: ");  // strcat() concatenates strings
            for (int i = 0; i < n; i++) 
                if (arr[i] % 2 != 0)
                    sprintf(buf + strlen(buf), "%d ", arr[i]);
        }
        
        // send() transmits data to connected client
        send(client, buf, strlen(buf), 0);
    }
    
    // close() closes sockets and releases resources
    close(client);
    close(sock);
    return 0;
}
