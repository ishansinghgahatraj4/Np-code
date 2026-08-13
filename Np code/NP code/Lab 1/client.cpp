#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Create socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);

serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Connect to server
    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    // Send message
    string message = "Hello Server";
    send(clientSocket, message.c_str(), message.length(), 0);

    char buffer[1024] = {0};

    // Receive reply
    recv(clientSocket, buffer, sizeof(buffer), 0);

    cout << "Server: " << buffer << endl;

    closesocket(clientSocket);

    WSACleanup();

    return 0;
}