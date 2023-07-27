// tcp-client-demo.cc

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock == -1) {
        std::cout << "Failed to create socket, error message: " << strerror(errno) << std::endl;
        return -1;
    }

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(10086);
    server_addr.sin_addr.s_addr = inet_addr("172.21.8.48");

    if (connect(client_sock, (const sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        std::cout << "Failed to connect to server, error message: " << strerror(errno) << std::endl;
        close(client_sock);
        return -1;
    }

    const char *hello_msg = "Hello, here is client";
    ssize_t bytes_sent = send(client_sock, hello_msg, strlen(hello_msg), 0);
    if (bytes_sent <= 0) {
        std::cout << "Failed to send data to server, error message: " << strerror(errno) << std::endl;
        close(client_sock);
        return -1;
    }

    char buffer[1024] = {0};
    // memset(buffer, 0, sizeof(buffer));
    ssize_t bytes_recv = recv(client_sock, buffer, sizeof(buffer), 0);
    if (bytes_recv == -1) {
        std::cout << "Failed to receive data from server, error message: " << strerror(errno) << std::endl;
        close(client_sock);
        return -1;
    }

    std::cout << "Received message from server: " << buffer << std::endl;

    close(client_sock);

    return 0;
}
