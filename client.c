#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8000

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char command[1024];

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Set up the server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

   /* // Convert address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }*/

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
	printf("connected to server");
    // Read the welcome message from the server
    valread = read(sock, buffer, 1024);
    printf("%s", buffer);

    // Interact with the server
    while (1) {
        // Clear buffer and take input from user
        memset(buffer, 0, sizeof(buffer));
        memset(command, 0, sizeof(command));
        printf("> ");
        fgets(command, sizeof(command), stdin);

        // Remove the newline character from the input
        command[strcspn(command, "\n")] = 0;

        // Send the command to the server
        send(sock, command, strlen(command), 0);

        // Read the response from the server
        valread = read(sock, buffer, 1024);
        printf("%s", buffer);

        // Break the loop if the command is `exit`
        if (strcmp(command, "exit") == 0) {
            break;
        }
    }

    // Close the socket
    close(sock);
    return 0;
}
