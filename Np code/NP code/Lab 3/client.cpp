#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

int main() {
    int sock;
    struct sockaddr_in server;
    char buffer[1024];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    connect(sock, (sockaddr*)&server, sizeof(server));

    char msg[] = "Hello Server";

    send(sock, msg, strlen(msg) + 1, 0);

    recv(sock, buffer, sizeof(buffer), 0);

    cout << "Server: " << buffer << endl;

    close(sock);

    return 0;
}