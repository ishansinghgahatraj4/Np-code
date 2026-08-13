#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdlib>
#include <fstream>
#include <ctime>

using namespace std;

// Function to create daemon process
void createDaemon()
{
    // Step 1: Create child process
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    // Parent exits
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    // Step 2: Create a new session
    if (setsid() < 0)
    {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }

    // Step 3: Fork again to prevent terminal access
    pid = fork();

    if (pid < 0)
    {
        perror("Second fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    // Step 4: Change working directory
    chdir("/");

    // Step 5: Set file permissions
    umask(0);

    // Step 6: Close standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Redirect standard descriptors to /dev/null
    open("/dev/null", O_RDONLY); // stdin
    open("/dev/null", O_WRONLY); // stdout
    open("/dev/null", O_RDWR);   // stderr
}


int main()
{
    createDaemon();

    // Daemon work starts here
    while (true)
    {
        ofstream logfile("/tmp/mydaemon.log", ios::app);

        if (logfile.is_open())
        {
            time_t now = time(nullptr);

            logfile << "Daemon running at: "
                    << ctime(&now);

            logfile.close();
        }

        sleep(10); // Perform task every 10 seconds
    }

    return 0;
}