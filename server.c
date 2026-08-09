// server.c - Cross-platform version
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    time_t rawtime;
    struct tm *timeinfo;
    char time_str[100];
    
    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("✅ Server running on http://0.0.0.0:%d\n", PORT);
    printf("📝 Returns plain text (ANSI red text on white bg)\n");
    
    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }
        
        read(new_socket, buffer, BUFFER_SIZE);
        
        // Get current time
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);
        
        // Plain text response with ANSI color codes (red text on white background)
        char response[1024];
        snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain; charset=utf-8\r\n"
            "Connection: close\r\n"
            "\r\n"
            "\033[31;47mHello World from C!\033[0m\n"
            "\033[31;47mServer: Pure C Web Server on Render\033[0m\n"
            "\033[31;47mDate: %s\033[0m\n"
            "\033[31;47mMessage: Plain text with ANSI colors!\033[0m\n"
            "\033[31;47mHosted on: Render.com\033[0m\n"
            "\033[31;47mLanguage: C\033[0m\n"
            "\033[31;47mStatus: Running\033[0m\n",
            time_str
        );
        
        write(new_socket, response, strlen(response));
        close(new_socket);
    }
    
    return 0;
}