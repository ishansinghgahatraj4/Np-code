#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

using namespace std;

#define PORT 8080
#define MAX_CLIENTS 10

int main()
{
    int serverSocket, clientSocket;
    int clientSockets[MAX_CLIENTS];

    fd_set readfds;

    char buffer[1024];

    sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    // Initialize client sockets
    for(int i = 0; i < MAX_CLIENTS; i++)
        clientSockets[i] = 0;


    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(serverSocket < 0)
    {
        cout << "Socket creation failed\n";
        return 1;
    }


    // Server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);


    // Bind
    if(bind(serverSocket, 
            (sockaddr*)&serverAddr,
            sizeof(serverAddr)) < 0)
    {
        cout << "Bind failed\n";
        return 1;
    }


    // Listen
    listen(serverSocket, 5);

    cout << "Server running on port " << PORT << endl;


    while(true)
    {
        FD_ZERO(&readfds);

        // Add server socket
        FD_SET(serverSocket, &readfds);

        int max_sd = serverSocket;


        // Add client sockets
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
            clientSocket = clientSockets[i];

            if(clientSocket > 0)
                FD_SET(clientSocket, &readfds);

            if(clientSocket > max_sd)
                max_sd = clientSocket;
        }


        // Monitor sockets
        select(max_sd + 1, &readfds, NULL, NULL, NULL);



        // New client connection
        if(FD_ISSET(serverSocket, &readfds))
        {
            int newSocket = accept(serverSocket,
                            (sockaddr*)&clientAddr,
                            &addrLen);


            cout << "New client connected\n";


            // Add new socket to list
            for(int i = 0; i < MAX_CLIENTS; i++)
            {
                if(clientSockets[i] == 0)
                {
                    clientSockets[i] = newSocket;
                    break;
                }
            }
        }



        // Check client sockets
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
            clientSocket = clientSockets[i];


            if(FD_ISSET(clientSocket, &readfds))
            {
                int bytes = recv(clientSocket,
                                 buffer,
                                 sizeof(buffer),
                                 0);


                // Client disconnected
                if(bytes <= 0)
                {
                    cout << "Client disconnected\n";

                    close(clientSocket);
                    clientSockets[i] = 0;
                }


                else
                {
                    buffer[bytes] = '\0';

                    cout << "Client message: "
                         << buffer << endl;


                    char reply[] = "Message received";

                    send(clientSocket,
                         reply,
                         strlen(reply),
                         0);
                }
            }
        }
    }


    close(serverSocket);

    return 0;
}