#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace std;

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    char buffer[1024];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    bind(server_fd, (sockaddr*)&server, sizeof(server));

    // Listen for connections
    listen(server_fd, 5);

    cout << "Server is running...\n";

    while (true) {
        client_fd = accept(server_fd, (sockaddr*)&client, &len);

        if (fork() == 0) {   // Child process
            close(server_fd);

            recv(client_fd, buffer, sizeof(buffer), 0);
            cout << "Client: " << buffer << endl;

            char reply[] = "Hello from Server";
            send(client_fd, reply, strlen(reply) + 1, 0);

            close(client_fd);
            return 0;
        }

        // Parent closes connected socket and continues accepting clients
        close(client_fd);
    }

    close(server_fd);
    return 0;
}