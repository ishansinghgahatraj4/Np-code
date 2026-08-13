#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>

using namespace std;


#define PORT 8000


int serverSocket;


void handler(int sig)
{
    sockaddr_in client;
    socklen_t len =
        sizeof(client);


    int clientSocket =
        accept(serverSocket,
               (sockaddr*)&client,
               &len);



    if(clientSocket >=0)
    {
        char buffer[1024];


        recv(clientSocket,
             buffer,
             sizeof(buffer),
             0);


        cout<<"Client message: "
            <<buffer<<endl;


        char reply[]="Received";

        send(clientSocket,
             reply,
             strlen(reply),
             0);


        close(clientSocket);
    }
}



int main()
{
    sockaddr_in serverAddr;


    signal(SIGIO,handler);


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



    // Enable signal driven I/O
    fcntl(serverSocket,
          F_SETOWN,
          getpid());


    fcntl(serverSocket,
          F_SETFL,
          O_ASYNC);


    cout<<"Signal driven server running...\n";


    while(true)
    {
        pause();
    }


    return 0;
}