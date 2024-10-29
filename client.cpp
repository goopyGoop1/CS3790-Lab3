#include <iostream>
#include <fstream>
#include <unistd.h>   // for close()
#include <fcntl.h>    // for open()
#include <sys/stat.h> // for mkfifo()
#include <cstring>    // for strcpy and strlen
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    const char* pipe_name = "/tmp/prime_pipe"; 

    // Check if the correct number of command-line arguments are provided
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <lower_bound> <upper_bound>" << endl; 
        return 1; 
    }

    // Convert command-line arguments to integers
    int lower_bound = atoi(argv[1]);
    int upper_bound = atoi(argv[2]);

    // Check if lower bound is greater than upper bound
    if (lower_bound > upper_bound) {
        cerr << "Usage: lower_bound must be less than or equal to upper_bound" << endl; 
        return 1; 
    }

    // Open the named pipe for writing
    int pipe_fd = open(pipe_name, O_WRONLY);
    if (pipe_fd == -1) {
        perror("Error opening named pipe for writing");
        return 1;
    }

    // Prepare the request in the form "RANGE XX YY"
    string range = "RANGE " + to_string(lower_bound) + " " + to_string(upper_bound);

    // Send the request to the server
    write(pipe_fd, range.c_str(), range.size());
    close(pipe_fd); // Close the pipe after sending the request

    // Open the named pipe for reading to get the response from the server
    pipe_fd = open(pipe_name, O_RDONLY);
    if (pipe_fd == -1) {
        perror("Error opening named pipe for reading");
        return 1;
    }

    // Read the response from the server
    char buffer[80];
    ssize_t bytes_read = read(pipe_fd, buffer, sizeof(buffer) - 1);
    while ((bytes_read = read(pipe_fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';  // Null-terminate each chunk
        cout << buffer;              // Print each chunk as it arrives
    }
    cout << endl;

    close(pipe_fd); // Close the pipe after reading

    return 0;
}

    
    







