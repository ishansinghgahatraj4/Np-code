#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>

using namespace std;

volatile sig_atomic_t server_running = 1;

// Signal handler for termination signals
void handleTermination(int signal)
{
    cout << "\nTermination signal received. Shutting down server..." << endl;
    server_running = 0;
}

// Signal handler for child process cleanup
void handleChildExit(int signal)
{
    int status;

    // Clean up all terminated child processes
    while (waitpid(-1, &status, WNOHANG) > 0)
    {
        cout << "Child process cleaned up." << endl;
    }
}

// Simulated client handling function
void handleClient(int clientSocket)
{
    cout << "Child process handling client: " << getpid() << endl;

    // Simulate client work
    sleep(5);

    close(clientSocket);
    exit(0);
}

int main()
{
    int serverSocket;
    sockaddr_in serverAddress{};

    // Register signal handlers
    signal(SIGINT, handleTermination);
    signal(SIGTERM, handleTermination);
    signal(SIGCHLD, handleChildExit);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8001);

    // Bind socket
    if (bind(serverSocket, (sockaddr*)&serverAddress,
             sizeof(serverAddress)) < 0)
    {
        perror("Bind failed");
        return 1;
    }

    // Listen for clients
    listen(serverSocket, 5);

    cout << "Server running on port 8080..." << endl;

    while (server_running)
    {
        sockaddr_in clientAddress{};
        socklen_t clientLength = sizeof(clientAddress);

        int clientSocket = accept(
            serverSocket,
            (sockaddr*)&clientAddress,
            &clientLength
        );

        if (clientSocket < 0)
        {
            if (!server_running)
                break;

            perror("Accept failed");
            continue;
        }

        pid_t pid = fork();

        if (pid == 0)
        {
            // Child process
            close(serverSocket);
            handleClient(clientSocket);
        }
        else if (pid > 0)
        {
            // Parent process
            close(clientSocket);
        }
        else
        {
            perror("Fork failed");
        }
    }

    cout << "Closing server socket..." << endl;

    close(serverSocket);

    // Cleanup remaining child processes
    while (waitpid(-1, nullptr, WNOHANG) > 0)
    {
        cout << "Remaining child cleaned." << endl;
    }

    cout << "Server terminated successfully." << endl;

    return 0;
}