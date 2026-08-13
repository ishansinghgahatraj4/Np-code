#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Create socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    // Listen
    listen(serverSocket, 1);

    cout << "Server waiting for client..." << endl;

    // Accept client
    SOCKET clientSocket;
    clientSocket = accept(serverSocket, NULL, NULL);

    char buffer[1024] = {0};

    // Receive message
    recv(clientSocket, buffer, sizeof(buffer), 0);

    cout << "Client: " << buffer << endl;

    // Send response
    string message = "Hello Client";
    send(clientSocket, message.c_str(), message.length(), 0);

    // Close sockets
    closesocket(clientSocket);
    closesocket(serverSocket);

    WSACleanup();

    return 0;
}