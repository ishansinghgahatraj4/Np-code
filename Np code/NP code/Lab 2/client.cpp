#include <iostream>
#include <winsock2.h>

using namespace std;

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET s;

    sockaddr_in server;

    char buffer[1024];

    int len = sizeof(server);

    s = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    char msg[] = "Hello Server";

    sendto(s, msg, strlen(msg)+1, 0,
           (sockaddr*)&server, sizeof(server));

    recvfrom(s, buffer, sizeof(buffer), 0,
             (sockaddr*)&server, &len);

    cout << "Server: " << buffer << endl;

    closesocket(s);

    WSACleanup();

    return 0;
}