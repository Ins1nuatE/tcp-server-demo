// tcp-server-demo.cc

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
    // Create IPv4 TCP socket
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        std::cout << "Failed to create socket, error message: " << strerror(errno) << std::endl;
        return -1;
    }

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(10086);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_sock, (const sockaddr *)&server_addr, sizeof(sockaddr_in)) == -1) {
        std::cout << "Failed to bind socket, error message: " << strerror(errno) << std::endl;
        close(server_sock);
        return -1;
    }

    if (listen(server_sock, 10) == -1) {
        std::cout << "Failed to listen socket, error message: " << strerror(errno) << std::endl;
        close(server_sock);
        return -1;
    }

    sockaddr_in client_addr = {};
    socklen_t client_addrlen = sizeof(client_addr);
    int client_sock = accept(server_sock, (sockaddr *)&client_addr, &client_addrlen);
    if (client_sock == -1) {
        std::cout << "Failed to accept client, error message: " << strerror(errno) << std::endl;
        close(server_sock);
        return -1;
    }

    char buffer[1024] = {0};
    // memset(buffer, 0, sizeof(buffer));
    ssize_t bytes_recv = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_recv == -1) {
        std::cout << "Failed to receive data from client, error message: " << strerror(errno) << std::endl;
        // shutdown(client_sock, SHUT_RDWR);
        // shutdown(server_sock, SHUT_RDWR);
        close(client_sock);
        close(server_sock);
        return -1;
    }

    std::cout << "Received message from client: " << buffer << std::endl;

    const char *respone_msg = "Hello, here is server.";
    ssize_t bytes_sent = send(client_sock, respone_msg, strlen(respone_msg), 0);
    if (bytes_sent <= 0) {
        std::cout << "Failed to send data to client, error message: " << strerror(errno) << std::endl;
        // shutdown(client_sock, SHUT_RDWR);
        // shutdown(server_sock, SHUT_RDWR);
        close(client_sock);
        close(server_sock);
        return -1;
    }

    std::cout << "Response hello to client" << std::endl;

    // shutdown(client_sock, SHUT_RDWR);
    // shutdown(server_sock, SHUT_RDWR);
    close(client_sock);
    close(server_sock);

    return 0;
}
