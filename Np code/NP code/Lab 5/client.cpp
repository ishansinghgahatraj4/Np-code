#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

#define PORT 8080


int main()
{
    int sock;
    sockaddr_in serverAddr;

    char message[1024];
    char buffer[1024];


    // Create socket
    sock = socket(AF_INET,
                  SOCK_STREAM,
                  0);


    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    serverAddr.sin_addr.s_addr =
        inet_addr("127.0.0.1");


    // Connect
    connect(sock,
            (sockaddr*)&serverAddr,
            sizeof(serverAddr));


    while(true)
    {
        cout << "Enter message: ";

        cin.getline(message,1024);


        send(sock,
             message,
             strlen(message),
             0);


        int bytes =
        recv(sock,
             buffer,
             sizeof(buffer),
             0);


        buffer[bytes] = '\0';


        cout << "Server: "
             << buffer << endl;


        if(strcmp(message,"exit")==0)
            break;
    }


    close(sock);

    return 0;
}