Chad Lab 3 CS3790 
Here’s a README file for the `server.cpp` and `client.cpp` files.

---

# Prime Number Server-Client Program

## Overview

This project contains a server-client program in C++ that communicates via a named pipe (`FIFO`). The server accepts a range of integers from the client, calculates the prime numbers within that range, and returns the result. The client sends the request with a specified range and displays the prime numbers received from the server.

## Files

- **`server.cpp`**: The server-side program that listens for requests from the client to compute prime numbers in a specified range. It parses requests, computes primes, and returns the result back to the client in chunks if the response is over 80 characters.
- **`client.cpp`**: The client-side program that sends a request to the server with a range of numbers and then reads and displays the list of primes returned by the server.

## Requirements

- C++11 or later
- POSIX-compliant system (Linux or macOS) for named pipes functionality

## Compilation

To compile both the server and client:

```bash
g++ server.cpp -o server
g++ client.cpp -o client
```

## Usage

### 1. Run the Server

Start the server first, as it will listen for incoming requests from clients.

```bash
./server
```

### 2. Run the Client

After starting the server, open a new terminal window and run the client. The client requires two integer arguments: `lower_bound` and `upper_bound` for the range of numbers.

```bash
./client <lower_bound> <upper_bound>
```

For example:

```bash
./client 10 50
```

This will send a request to the server to calculate prime numbers between 10 and 50.

## Explanation of Code

### `server.cpp`

1. **Prime Calculation**:
   - The function `is_prime(int num)` checks if a number is prime.
   - `compute_primes(int lower, int upper)` returns a vector of prime numbers within the given range.
   
2. **Request Handling**:
   - `parse_request(const string& range, int& lower, int& upper)` checks if the request is in the format `RANGE lower upper`.
   
3. **Communication**:
   - The server reads a request from the pipe, parses it, computes the primes, and writes back the result in chunks if the response exceeds 80 characters.
   
4. **Named Pipe**:
   - The named pipe is created at `/tmp/prime_pipe`. It is opened for reading requests and writing responses.

### `client.cpp`

1. **Input Validation**:
   - The client checks for correct arguments and validates that `lower_bound` is less than or equal to `upper_bound`.
   
2. **Request Formation**:
   - The client formats the request as `"RANGE lower_bound upper_bound"` and writes it to the named pipe.

3. **Reading Response**:
   - The client reads from the named pipe in a loop, ensuring it captures responses longer than 80 characters.

## Example

Running the following commands in two terminals will demonstrate the server-client interaction:

1. Terminal 1 (Server):
   ```bash
   ./server
   ```

2. Terminal 2 (Client):
   ```bash
   ./client 10 50
   ```

The output will display the list of prime numbers between 10 and 50.

## Cleanup

The server automatically removes the named pipe `/tmp/prime_pipe` upon termination.

--- 

This README provides instructions and an overview to help understand and run the server-client program. Adjust paths and commands as necessary for your setup.
