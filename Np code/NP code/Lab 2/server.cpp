#include <iostream>
#include <winsock2.h>

using namespace std;

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET s;

    sockaddr_in server, client;

    char buffer[1024];

    int len = sizeof(client);

    s = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(s, (sockaddr*)&server, sizeof(server));

    cout << "Server is waiting...\n";

    recvfrom(s, buffer, sizeof(buffer), 0,
             (sockaddr*)&client, &len);

    cout << "Client: " << buffer << endl;

    char reply[] = "Hello Client";

    sendto(s, reply, strlen(reply)+1, 0,
           (sockaddr*)&client, len);

    closesocket(s);

    WSACleanup();

    return 0;
}