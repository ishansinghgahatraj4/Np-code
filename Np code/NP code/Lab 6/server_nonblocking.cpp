#include <iostream>
#include <unistd.h>
#include <fcntl.h>
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

    socklen_t len =
        sizeof(clientAddr);


    serverSocket =
        socket(AF_INET,
               SOCK_STREAM,
               0);



    serverAddr.sin_family =
        AF_INET;

    serverAddr.sin_addr.s_addr =
        INADDR_ANY;

    serverAddr.sin_port =
        htons(PORT);



    bind(serverSocket,
         (sockaddr*)&serverAddr,
         sizeof(serverAddr));


    listen(serverSocket,5);



    // Make socket non-blocking
    fcntl(serverSocket,
          F_SETFL,
          O_NONBLOCK);



    cout<<"Non-blocking server running...\n";


    while(true)
    {

        clientSocket =
        accept(serverSocket,
               (sockaddr*)&clientAddr,
               &len);


        if(clientSocket < 0)
        {
            cout<<"No client yet\n";
            sleep(2);
            continue;
        }


        fcntl(clientSocket,
              F_SETFL,
              O_NONBLOCK);



        int n =
        recv(clientSocket,
             buffer,
             sizeof(buffer),
             0);



        if(n > 0)
        {
            buffer[n]='\0';

            cout<<"Client: "
                <<buffer<<endl;
        }


        close(clientSocket);
    }


    return 0;
}