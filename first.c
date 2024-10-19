#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "controllers/admin_controller.h"
#include "controllers/employee_controller.h"
#include "controllers/customer_controller.h"
#define PORT 8000
#define MAX_CLIENTS 10  // Maximum number of concurrent clients

// Function to handle each client connection
void *client_handler(void *client_socket);

int main(int argc, char const *argv[]) {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;
    pthread_t thread_id;  // For creating threads

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Define the address for the server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    // Infinite loop to keep the server open and handle multiple clients
    while (1) {
        // Accept a new client connection
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            continue;  // Continue the loop to accept the next client
        }

        printf("New client connected!\n");

        // Create a new thread for each client
        if (pthread_create(&thread_id, NULL, client_handler, (void *)&client_socket) != 0) {
            perror("Failed to create thread");
            close(client_socket);
            continue;
        }

        // Detach the thread so it can clean up after itself
        pthread_detach(thread_id);
    }

    // Close the server socket when done (this point will not be reached in practice)
    close(server_fd);
    return 0;
}

// Function to handle each client in a separate thread
void *client_handler(void *client_socket) {
	printf("connected with client"); 
   int socket = *(int *)client_socket;

    // Display the menu to the client
    const char *menu = "Welcome! Please select your role:\n"
                       "1. Customer\n"
                       "2. Bank Employee\n"
                       "3. Manager\n"
                       "4. Admin\n"
                       "Enter your choice (1-4): ";
    
    send(socket, menu, strlen(menu), 0);
	printf("sent to client\n");
    char choice[10];  // Buffer for the client's choice
    ssize_t bytes_read = read(socket, choice, sizeof(choice) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        close(socket);
        return NULL;  // Handle the error
    }
    
    choice[bytes_read] = '\0';  // Null-terminate the string

    // Determine the role based on the choice
    int role_choice = atoi(choice);
	printf("%d",role_choice);
    switch (role_choice) {
        case 1:
            handle_customer(socket);
            break;
        case 2:
            handle_employee(socket);
            break;
        case 3:
            handle_manager(socket);
            break;
        case 4:
            handle_admin(socket);
            break;
        default:
            const char *invalid_choice = "Invalid choice. Please try again.\n";
            send(socket, invalid_choice, strlen(invalid_choice), 0);
            break;
    }

 

    // Close the client socket when done
    close(socket);
    printf("Client disconnected.\n");
    return NULL;
}
