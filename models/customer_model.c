#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bank_model.h"
#include <sys/file.h> 
#include <unistd.h>  
#include <errno.h> 
#include "customer_model.h"
#define CUSTOMER_DATA_FILE "/home/rishika-gupta/ssProject/customer_data.txt"

int save_customers(Customer *customers, int num_customers) {
    int fd = open(CUSTOMER_DATA_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening customer data file for writing");
        return -1;
    }

    // Attempt to acquire an exclusive lock on the file
    while (flock(fd, LOCK_EX | LOCK_NB) == -1) {
        if (errno == EAGAIN) { // If lock is unavailable
            printf("File is locked, waiting to save customers...\n");
            usleep(100000); // Sleep for 100ms before retrying
            continue; // Try to acquire the lock again
        } else {
            perror("Error locking the customer data file");
            close(fd);
            return -1;
        }
    }

    // Save each customer to the file
    for (int i = 0; i < num_customers; i++) {
        char buffer[256];
        int length = snprintf(buffer, sizeof(buffer), "%d,%s,%s,%.2f,%d,%d\n",
                              customers[i].id, 
                              customers[i].name, 
                              customers[i].password, 
                              customers[i].balance, 
                              customers[i].status, 
                              customers[i].activeAccount);
        if (write(fd, buffer, length) == -1) {
            perror("Error writing to customer data file");
            
            // Release the file lock
            flock(fd, LOCK_UN);
            close(fd);
            return -1;
        }
    }

    // Release the file lock after writing
    if (flock(fd, LOCK_UN) == -1) {
        perror("Error unlocking the customer data file");
        close(fd);
        return -1;
    }

    close(fd);
    printf("Customers saved successfully.\n");
    return 1; // Success
}

// Function to load all customers from the file
int load_all_customers(Customer *customers, int max_customers) {
    FILE *file = fopen(CUSTOMER_DATA_FILE, "r");
    if (file == NULL) {
        perror("Error opening customer data file");
        return -1;
    }

    char line[256];
    int num_customers = 0;

    while (fgets(line, sizeof(line), file) && num_customers < max_customers) {
        // Parse each line to populate the customer struct
        sscanf(line, "%d,%49[^,],%49[^,],%lf,%d,%d", 
               &customers[num_customers].id, 
               customers[num_customers].name, 
               customers[num_customers].password, 
               &customers[num_customers].balance, 
               &customers[num_customers].status, 
               &customers[num_customers].activeAccount);
        num_customers++;
    }

    fclose(file);
    return num_customers;  // Return the number of customers loaded
}

// Function to validate customer credentials
int validate_customer_credentials(const char *name, const char *password) {
    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    if (num_customers == -1) {
        printf("Error loading customers.\n");
        return -1;
    }

    // Iterate through all customers and compare credentials
    for (int i = 0; i < num_customers; i++) {
        printf("Comparing with Customer %d - Name: '%s', Password: '%s', Status: %d\n", 
               customers[i].id, customers[i].name, customers[i].password, customers[i].status);

        if (strcmp(name, customers[i].name) == 0 && strcmp(password, customers[i].password) == 0) {
            if(customers[i].activeAccount==0){
                return -3;
            }
            if (customers[i].status == 1) {
                return -2; // Customer already logged in
            }
            login_customer(customers[i].id);
            return customers[i].id; // Valid credentials
        }
    }
    return -1; // Invalid credentials
}

// Function to mark a customer as logged in by ID
void login_customer(int id) {
    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == id) {
            customers[i].status = 1; // Mark customer as logged in
            save_customers(customers, num_customers);  // Save changes to file
            return;
        }
    }
}

// Function to log out a customer by ID
int logout_customer(int id) {
    printf("Call came for logging out this customer %d\n", id);
    fflush(stdout);

    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    // Iterate through all customers and find the one to log out
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == id) {
            if (customers[i].status == 0) {
                return -1; // Customer already logged out
            }
            customers[i].status = 0; // Mark customer as logged out
            save_customers(customers, num_customers);  // Save changes to file
            printf("Customer logged out successfully. Current status: %d\n", customers[i].status);
            fflush(stdout);
            return 0; // Success
        }
    }
    return -1; // Customer not found
}

// Function to check if a customer is logged in by ID
int customer_status(int id) {
    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == id) {
            if (customers[i].status == 1) {
                return 1;  // Customer is logged in
            }
            return -1;  // Customer is logged out
        }
    }

    return -1;  // Customer not found
}

// Function to check the balance of a customer by ID
double check_balance(int customer_id) {
    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    // Iterate over the customers to find the one with the given ID
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == customer_id) {
            printf("Customer ID: %d, Name: %s, Balance: %.2f\n", 
                    customers[i].id, customers[i].name, customers[i].balance);
            return customers[i].balance;  // Return the balance
        }
    }

    printf("Customer with ID %d not found.\n", customer_id);
    return -1;  // Return -1 if customer is not found
}

int change_customer_password(int customer_id, const char *current_password, const char *new_password) {
    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    printf("Call came for changing password for customer %d\n", customer_id);
    fflush(stdout);

    for (int i = 0; i < num_customers; i++) {
        printf("Comparing with Customer %d - password: '%s'\n", 
               customers[i].id, customers[i].password);
        if (customers[i].id == customer_id) {
            if (strcmp(current_password, customers[i].password) == 0) {
                // Change the password
                strcpy(customers[i].password, new_password);
                // Save the updated customers back to the file
                save_customers(customers, num_customers); 
                return 1; // Password changed successfully
            } else {
                printf("Current password for customer %d is incorrect.\n", customer_id);
                return -1; // Current password is incorrect
            }
        }
    }
    
    printf("Customer with ID %d not found.\n", customer_id);
    return 0; // Customer not found
}
