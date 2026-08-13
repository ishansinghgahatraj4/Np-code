#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>

using namespace std;

#define PORT 8000

int main()
{
    int serverSocket, clientSocket;
    char buffer[1024];

    sockaddr_in serverAddr, clientAddr;
    socklen_t len = sizeof(clientAddr);


    serverSocket = socket(AF_INET, SOCK_STREAM, 0);


    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);


    bind(serverSocket,
         (sockaddr*)&serverAddr,
         sizeof(serverAddr));


    listen(serverSocket,5);


    cout<<"Blocking server running...\n";


    while(true)
    {
        // Blocking accept
        clientSocket =
        accept(serverSocket,
               (sockaddr*)&clientAddr,
               &len);


        cout<<"Client connected\n";


        // Blocking recv
        recv(clientSocket,
             buffer,
             sizeof(buffer),
             0);


        cout<<"Client message: "
            <<buffer<<endl;


        char reply[]="Data received";

        send(clientSocket,
             reply,
             strlen(reply),
             0);


        close(clientSocket);
    }


    close(serverSocket);

    return 0;
}