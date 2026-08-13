#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define PORT 8000


// Function to handle each client
void handleClient(int clientSocket)
{
    char buffer[1024];

    while(true)
    {
        int bytesReceived = recv(clientSocket,
                                 buffer,
                                 sizeof(buffer),
                                 0);

        if(bytesReceived <= 0)
        {
            cout << "Client disconnected\n";
            break;
        }

        buffer[bytesReceived] = '\0';

        cout << "Client message: "
             << buffer << endl;


        char reply[] = "Message received from server";

        send(clientSocket,
             reply,
             strlen(reply),
             0);
    }

    close(clientSocket);
}


int main()
{
    int serverSocket;
    sockaddr_in serverAddr, clientAddr;

    socklen_t clientLen = sizeof(clientAddr);


    // Create socket
    serverSocket = socket(AF_INET,
                          SOCK_STREAM,
                          0);


    if(serverSocket < 0)
    {
        cout << "Socket creation failed\n";
        return 1;
    }


    // Server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);


    // Allow port reuse
    int opt = 1;
    setsockopt(serverSocket,
               SOL_SOCKET,
               SO_REUSEADDR,
               &opt,
               sizeof(opt));


    // Bind
    if(bind(serverSocket,
            (sockaddr*)&serverAddr,
            sizeof(serverAddr)) < 0)
    {
        cout << "Bind failed\n";
        return 1;
    }


    // Listen
    listen(serverSocket,5);


    cout << "Multithreaded TCP Server running on port "
         << PORT << endl;



    while(true)
    {
        // Accept client
        int clientSocket =
            accept(serverSocket,
                   (sockaddr*)&clientAddr,
                   &clientLen);


        cout << "New client connected\n";


        // Create new thread for client
        thread t(handleClient, clientSocket);


        // Allow thread to run independently
        t.detach();
    }


    close(serverSocket);

    return 0;
}