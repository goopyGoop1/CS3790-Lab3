#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>   // for close()
#include <fcntl.h>    // for open()
#include <sys/stat.h> // for mkfifo()
#include <vector>
#include <cmath>
using namespace std;

// Function to check if a number is prime
bool is_prime(int num) {
    if (num <= 1) return false;// 1 or less are not prime numbers
    if (num == 2) return true;  // 2 is the only even prime number
    if (num % 2 == 0) return false;// Other even numbers are not prime
    // Check only odd divisors up to the square root of the number
    for (int i = 3; i <= sqrt(num); i += 2) {
        if (num % i == 0) return false;  // If divisible, it's not prime
    }
    return true;// If no divisors found, it’s prime
}

// Function to compute primes in a given range
vector<int> compute_primes(int lower, int upper) {
    vector<int> primes;
    for (int i = lower; i <= upper; ++i) {
        if (is_prime(i)) {
            primes.push_back(i);
        }
    }
    return primes;
}

// Function to parse the request
bool parse_request(const string& range, int& lower, int& upper) {
    istringstream ss(range);
    string command;
    // Check if the command is "RANGE" followed by two integers
    if (ss >> command >> lower >> upper && command == "RANGE") {
        return true;// Valid request format
    }
    
    return false;   // Invalid request format
}

int main() {

	remove("/tmp/prime_pipe");
    const char* pipe_name = "/tmp/prime_pipe"; // Single named pipe

    // Create the named pipe (FIFO)
    if (mkfifo(pipe_name, 0666) == -1) {
        perror("Error creating named pipe");  // Print error if pipe creation fails
        return 1;
    }

    cout << "Prime Server is listening on " << pipe_name << "...\n";

    while (true) {
        // Open the pipe for reading
        int pipe_fd = open(pipe_name, O_RDONLY);
        if (pipe_fd == -1) {
            perror("Error opening named pipe for reading");
            return 1;
        }

        // Read the request from the client
        char buffer[80];
        ssize_t bytes_read = read(pipe_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';  // Null-terminate the string
            string range(buffer);  // Convert buffer to a string
            int lower = 0, upper = 0;  // Initialize range boundaries

            cout << "Received request: " << range << "\n";

            // Parse the request
            string response;
            if (parse_request(range, lower, upper)) {
                vector<int> primes = compute_primes(lower, upper);  // If valid, compute the primes in the specified range

                // Convert the prime numbers to a string
                ostringstream response_stream;
                for (const int& prime : primes) {
                    response_stream << prime << " "; // Add each prime to the response
                }
                if (primes.empty()) {
                    response_stream << "No primes found in the range.";
                }
                response = response_stream.str();
            } else {
                // Malformed request
                response = "Malformed request";
            }

            close(pipe_fd); // Close the pipe after reading

            // Open the same pipe for writing the response back to the client
            pipe_fd = open(pipe_name, O_WRONLY);
            if (pipe_fd != -1) {
               size_t pos = 0;
                while (pos < response.size()) {
                    string chunk = response.substr(pos, 80); // Get next 80 chars
                    write(pipe_fd, chunk.c_str(), chunk.size());
                    pos += 80; // Move to the next chunk
                    
                }
                close(pipe_fd); // Close the pipe after writing
            } else {
                perror("Error opening named pipe for writing");
            }
        } else {
            perror("Error reading from pipe");
        }
    }

    // Remove the named pipe when done (cleanup)
    unlink(pipe_name);
    return 0;
}
