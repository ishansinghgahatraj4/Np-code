#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;


#define PORT 8000


int main()
{
    int sock;

    sockaddr_in server;


    sock = socket(AF_INET,
                  SOCK_STREAM,
                  0);


    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);

    server.sin_addr.s_addr=
        inet_addr("127.0.0.1");


    connect(sock,
            (sockaddr*)&server,
            sizeof(server));


    char msg[]="Hello Server";


    send(sock,
         msg,
         strlen(msg),
         0);


    close(sock);


    return 0;
}