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

   
    while (flock(fd, LOCK_EX | LOCK_NB) == -1) {
        if (errno == EAGAIN) { 
            printf("File is locked, waiting to save customers...\n");
            usleep(100000); 
            continue; 
        } else {
            perror("Error locking the customer data file");
            close(fd);
            return -1;
        }
    }

   
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
            
            
            flock(fd, LOCK_UN);
            close(fd);
            return -1;
        }
    }

    
    if (flock(fd, LOCK_UN) == -1) {
        perror("Error unlocking the customer data file");
        close(fd);
        return -1;
    }

    close(fd);
    printf("Customers saved successfully.\n");
    return 1; 
}

int load_all_customers(Customer *customers, int max_customers) {
    FILE *file = fopen(CUSTOMER_DATA_FILE, "r");
    if (file == NULL) {
        perror("Error opening customer data file");
        return -1;
    }

    char line[256];
    int num_customers = 0;

    while (fgets(line, sizeof(line), file) && num_customers < max_customers) {
        
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
    return num_customers;  
}


int validate_customer_credentials(const char *name, const char *password) {
    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    if (num_customers == -1) {
        printf("Error loading customers.\n");
        return -1;
    }

   
    for (int i = 0; i < num_customers; i++) {
        printf("Comparing with Customer %d - Name: '%s', Password: '%s', Status: %d\n", 
               customers[i].id, customers[i].name, customers[i].password, customers[i].status);

        if (strcmp(name, customers[i].name) == 0 && strcmp(password, customers[i].password) == 0) {
            if(customers[i].activeAccount==0){
                return -3;
            }
            if (customers[i].status == 1) {
                return -2; 
            }
            login_customer(customers[i].id);
            return customers[i].id; 
        }
    }
    return -1; 
}


void login_customer(int id) {
    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == id) {
            customers[i].status = 1; 
            save_customers(customers, num_customers);  
            return;
        }
    }
}

int logout_customer(int id) {
    printf("Call came for logging out this customer %d\n", id);
    fflush(stdout);

    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

   
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == id) {
            if (customers[i].status == 0) {
                return -1; 
            }
            customers[i].status = 0; 
            save_customers(customers, num_customers);  
            printf("Customer logged out successfully. Current status: %d\n", customers[i].status);
            fflush(stdout);
            return 0; 
        }
    }
    return -1; 
}

int customer_status(int id) {
    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == id) {
            if (customers[i].status == 1) {
                return 1; 
            }
            return -1; 
        }
    }

    return -1;  
}

double check_balance(int customer_id) {
    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == customer_id) {
            printf("Customer ID: %d, Name: %s, Balance: %.2f\n", 
                    customers[i].id, customers[i].name, customers[i].balance);
            return customers[i].balance;  
        }
    }

    printf("Customer with ID %d not found.\n", customer_id);
    return -1; 
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
                
                strcpy(customers[i].password, new_password);
                save_customers(customers, num_customers); 
                return 1; 
            } else {
                printf("Current password for customer %d is incorrect.\n", customer_id);
                return -1;
            }
        }
    }
    
    printf("Customer with ID %d not found.\n", customer_id);
    return 0; 
}
