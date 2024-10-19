#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "admin_model.h"
#include "../models/customer_model.h"

// Define the maximum number of admins
#define MAX_ADMINS 5
#define ADMIN_DATA_FILE "/home/rishika-gupta/ssProject/admin_data.txt"
static int admins_loaded = 0;

// Static array of admins
static Admin admins[MAX_ADMINS];

// Function to load admin data from a file using open/read
void load_admins_from_file() {
    if (admins_loaded) return; // If already loaded, do nothing

    int fd = open(ADMIN_DATA_FILE, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    ssize_t bytes_read;
    int admin_index = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0 && admin_index < MAX_ADMINS) {
        char *line = strtok(buffer, "\n");
        while (line != NULL && admin_index < MAX_ADMINS) {
            sscanf(line, "%d,%49[^,],%49[^,],%d", &admins[admin_index].id, admins[admin_index].username, admins[admin_index].password, &admins[admin_index].logged_in);
            line = strtok(NULL, "\n");
            admin_index++;
        }
    }

    if (bytes_read == -1) {
        perror("Error reading file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    admins_loaded = 1; // Mark as loaded
}

    

// Function to save admin data to a file using open/write
void save_admins_to_file() {
    int fd = open(ADMIN_DATA_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    for (int i = 0; i < MAX_ADMINS; i++) {
        // Prepare the data to be written
        int length = snprintf(buffer, sizeof(buffer), "%d,%s,%s,%d\n", admins[i].id, admins[i].username, admins[i].password, admins[i].logged_in);
        
        // Debug statement to show the data being saved
        printf("Saving Admin %d: ID=%d, Username=%s, Password=%s, Logged In=%d\n",
               i, admins[i].id, admins[i].username, admins[i].password, admins[i].logged_in);
        fflush(stdout); // Ensure immediate output

        // Write the data to the file
        if (write(fd, buffer, length) == -1) {
            perror("Error writing to file");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    close(fd);
}

//validating admin credentials
int validate_admin_credentials(const char *username, const char *password) {
load_admins_from_file(); 

   for (int i = 0; i < MAX_ADMINS; i++) {

printf("Comparing with Admin %d - Username: '%s', Password: '%s', Logged In: %d\n", 
               admins[i].id, admins[i].username, admins[i].password, admins[i].logged_in);
        

        if (strcmp(username, admins[i].username) == 0 && strcmp(password, admins[i].password) == 0) {
            if (admins[i].logged_in == 1) {
                return -2; // Admin already logged in
            }
		login_admin(admins[i].id);
            return admins[i].id; // Valid credentials
        }
    }
	printf("%s, %s",username,password);
    return -1; // Invalid credentials
}

// Function to mark an admin as logged in by ID
void login_admin(int id) {
    for (int i = 0; i < MAX_ADMINS; i++) {
        if (admins[i].id == id) {
            admins[i].logged_in = 1;
            save_admins_to_file();  // Save changes to file
            return;
        }
    }
}

// Function to log out an admin by ID
int logout_admin(int id) {
printf("Call came for logging out this admin %d\n", id);
    fflush(stdout);
    for (int i = 0; i < MAX_ADMINS; i++) {
        if (admins[i].id == id) {
		if(admins[i].logged_in ==0){return -1;}
            admins[i].logged_in = 0;
            save_admins_to_file();  // Save changes to file
		printf("done %d",admins[i].logged_in);
 fflush(stdout);             
return 0;
        }
    }
}
// Function to mark an admin as logged in by ID
int  admin_status(int id) {
load_admins_from_file();
    for (int i = 0; i < MAX_ADMINS; i++) {
        if (admins[i].id == id) {
            if(admins[i].logged_in == 1){
		return 1;
             }
            return -1;
        }

    }
return -1;
}

int change_admin_password(int admin_id,const char *current_password,const char *password){

load_admins_from_file();

printf("Call came for change password admin %d\n", admin_id);
    fflush(stdout);

for (int i = 0; i < MAX_ADMINS; i++) {
printf("Comparing with Admin %d - password: '%s'\n", 
               admins[i].id, admins[i].password);
        if (admins[i].id == admin_id) {
		if (strcmp(current_password, admins[i].password) == 0){
		strcpy(admins[i].password, password);
		save_admins_to_file();
		return 1;		
		}else{
		printf("Comparing with Admin %d - password: '%s'\n", 
               admins[i].id, admins[i].password);
        

		return -1;
			}
		}
}
return 0;
}
