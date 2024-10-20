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
#define MAX_CLIENTS 10  

void *client_handler(void *client_socket);

int main(int argc, char const *argv[]) {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;
    pthread_t thread_id;  

    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

   
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

   
    while (1) {
        
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            continue; 
        }

        printf("New client connected!\n");

      
        if (pthread_create(&thread_id, NULL, client_handler, (void *)&client_socket) != 0) {
            perror("Failed to create thread");
            close(client_socket);
            continue;
        }

        
        pthread_detach(thread_id);
    }

    close(server_fd);
    return 0;
}

void *client_handler(void *client_socket) {
	printf("connected with client"); 
   int socket = *(int *)client_socket;

    const char *menu = "Welcome! Please select your role:\n"
                       "1. Customer\n"
                       "2. Bank Employee\n"
                       "3. Manager\n"
                       "4. Admin\n"
                       "Enter your choice (1-4): ";
    
    send(socket, menu, strlen(menu), 0);
	printf("sent to client\n");
    char choice[10];  
    ssize_t bytes_read = read(socket, choice, sizeof(choice) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        close(socket);
        return NULL;  
    }
    
    choice[bytes_read] = '\0';  

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

 

    
    close(socket);
    printf("Client disconnected.\n");
    return NULL;
}
