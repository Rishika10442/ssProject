#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bank_model.h"
#include <sys/file.h> 
#include <unistd.h>  
#include <errno.h> 
#include "../models/customer_model.h"

#define CUSTOMER_DATA_FILE "/home/rishika-gupta/ssProject/customer_data.txt"

#define EMPLOYEE_DATA_FILE "/home/rishika-gupta/ssProject/employee_data.txt"

// Function to add a new employee to the file
void add_employee(const char *name, const char *password) {
    Employee employee;
    int fd;
    ssize_t bytes_read;
    char buffer[256];

    // Open the employee data file, or create it if it doesn't exist
    fd = open(EMPLOYEE_DATA_FILE, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Find the last employee's ID (by reading through the file)
    int last_id = 0;
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        char *line = strtok(buffer, "\n");
        while (line != NULL) {
            sscanf(line, "%d", &last_id);
            line = strtok(NULL, "\n");
        }
    }

    // Assign the new employee's details
    employee.id = last_id + 1;  // Set ID as last_id + 1
    strncpy(employee.name, name, sizeof(employee.name) - 1);
    strncpy(employee.password, password, sizeof(employee.password) - 1);
    employee.assignedLoans = 0;
    employee.rejectedLoans = 0;
    employee.acceptedLoans = 0;
    employee.status = 0;
    employee.manager_id = -1;


    // Prepare the employee data to write to file
    int length = snprintf(buffer, sizeof(buffer), "%d,%s,%s,%d,%d,%d,%d.%d\n",
                          employee.id, employee.name, employee.password, 
                          employee.assignedLoans, employee.rejectedLoans, 
                          employee.acceptedLoans,employee.status,employee.manager_id);

     ssize_t bytes_written = write(fd, buffer, strlen(buffer));
	printf("data is written %d \n",bytes_written);
    if (bytes_written == -1) {
        perror("Error writing to employee data file");
    } else {
        printf("Employee added: %s\n", buffer);
    }

    fsync(fd); 
    close(fd);
}


// Function to load all employees from the file
int load_all_employees(Employee employees[], int max_employees) {
    int fd = open(EMPLOYEE_DATA_FILE, O_RDONLY);
    if (fd == -1) {
        perror("Error opening employee data file");
        return -1;
    }

    char buffer[4096];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("Error reading employee data file");
        close(fd);
        return -1;
    }
    buffer[bytes_read] = '\0';
    close(fd);

    // Parse the file data into employee structures
    char *line = strtok(buffer, "\n");
    int index = 0;
    while (line != NULL && index < max_employees) {
        sscanf(line, "%d,%49[^,],%49[^,],%d,%d,%d,%d,%d", &employees[index].id, employees[index].name, employees[index].password, &employees[index].assignedLoans, &employees[index].rejectedLoans, &employees[index].acceptedLoans,&employees[index].status,&employees[index].manager_id);
        printf("Parsed Employee - ID: %d, Name: %s, Password: %s, Assigned Loans: %d, Rejected Loans: %d, Accepted Loans: %d, Status: %d, Manager ID: %d\n", 
           employees[index].id, employees[index].name, employees[index].password, 
           employees[index].assignedLoans, employees[index].rejectedLoans, 
           employees[index].acceptedLoans, employees[index].status, 
           employees[index].manager_id);

        line = strtok(NULL, "\n");
        index++;
    }

    return index;  // Return number of employees loaded
}

// Function to save all employees back to the file
void save_all_employees(Employee employees[], int num_employees) {
    int fd = open(EMPLOYEE_DATA_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening employee data file");
        return;
    }

    char buffer[256];
    for (int i = 0; i < num_employees; i++) {
        // Print debug information for each employee
        printf("Parsed Employee - ID: %d, Name: %s, Password: %s, Assigned Loans: %d, Rejected Loans: %d, Accepted Loans: %d, Status: %d, Manager ID: %d\n", 
           employees[i].id, employees[i].name, employees[i].password, 
           employees[i].assignedLoans, employees[i].rejectedLoans, 
           employees[i].acceptedLoans, employees[i].status, 
           employees[i].manager_id);

        
        int length = snprintf(buffer, sizeof(buffer), "%d,%s,%s,%d,%d,%d,%d,%d\n", 
                              employees[i].id, employees[i].name, employees[i].password, 
                              employees[i].assignedLoans, employees[i].rejectedLoans, employees[i].acceptedLoans,employees[i].status,employees[i].manager_id);
        
        // Print the data being written to the file
        printf("Writing to file: %s", buffer);

        if (write(fd, buffer, length) == -1) {
            perror("Error writing to employee data file");
            close(fd);
            return;
        }
    }

    close(fd);
    printf("Employee data saved successfully.\n");
}


// Function to modify employee details
void modify_employee(int employee_id, const char *new_name, const char *new_password) {
    Employee employees[100];  // Assuming max 100 employees
    int num_employees = load_all_employees(employees, 100);

    if (num_employees == -1) {
        printf("Failed to load employees.\n");
        return;
    }

    // Find the employee by ID
    int found = 0;
    for (int i = 0; i < num_employees; i++) {
        if (employees[i].id == employee_id) {
            // Modify the employee's name and password
            if (new_name != NULL) {
                strncpy(employees[i].name, new_name, sizeof(employees[i].name) - 1);
            }
            if (new_password != NULL) {
                strncpy(employees[i].password, new_password, sizeof(employees[i].password) - 1);
            }
                    printf("Saving Employee - ID: %d, Name: %s, Password: %s, Assigned Loans: %d, Rejected Loans: %d, Accepted Loans: %d, stsus - %d, mID - %d\n",
               employees[i].id, employees[i].name, employees[i].password, employees[i].assignedLoans, employees[i].rejectedLoans, employees[i].acceptedLoans,employees[i].status,employees[i].manager_id);

            printf("Employee ID %d modified successfully.\n", employee_id);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Employee with ID %d not found.\n", employee_id);
        return;
    }

    // Save the modified employees back to the file
    save_all_employees(employees, num_employees);
}

// Function to validate employee credentials
int validate_employee_credentials(const char *username, const char *password) {
    Employee employees[100];  // Assuming max 100 employees
    int num_employees = load_all_employees(employees, 100);

    if (num_employees == -1) {
        printf("Failed to load employees.\n");
        return -1;  // Return -1 to indicate failure in loading employees
    }

    for (int i = 0; i < num_employees; i++) {
        printf("Comparing with Employee %d - Username: '%s', Password: '%s', Status: %d\n", 
               employees[i].id, employees[i].name, employees[i].password, employees[i].status);
        
        if (strcmp(username, employees[i].name) == 0 && strcmp(password, employees[i].password) == 0) {
            if (employees[i].status == 1) {
                return -2; // Employee already logged in
            }

            // Mark the employee as logged in
            login_employee(employees[i].id);
            return employees[i].id; // Valid credentials, return employee ID
        }
    }

    printf("Invalid credentials for Username: %s, Password: %s\n", username, password);
    return -1; // Invalid credentials
}

// Function to mark an employee as logged in by ID
void login_employee(int id) {
    Employee employees[100];
    int num_employees = load_all_employees(employees, 100);

    if (num_employees == -1) {
        printf("Failed to load employees.\n");
        return;
    }

    for (int i = 0; i < num_employees; i++) {
        if (employees[i].id == id) {
            employees[i].status = 1;  // Mark as logged in (1)
            save_all_employees(employees, num_employees);  // Save changes to file
            printf("Employee with ID %d is now logged in.\n", id);
            return;
        }
    }
    printf("Employee with ID %d not found.\n", id);
}

// Function to log out an employee by ID
int employee_logout(int id) {
    Employee employees[100];  // Assuming max 100 employees
    int num_employees = load_all_employees(employees, 100);

    if (num_employees == -1) {
        printf("Failed to load employees.\n");
        return -1;
    }

    printf("Call came for logging out this employee %d\n", id);
    fflush(stdout);

    for (int i = 0; i < num_employees; i++) {
        if (employees[i].id == id) {
            if (employees[i].status == 0) {
                return -1;  // Employee already logged out
            }
            employees[i].status = 0;  // Mark as logged out
            save_all_employees(employees, num_employees);  // Save changes to file
            printf("Employee %d logged out successfully.\n", id);
            fflush(stdout);
            return 0;
        }
    }

    return -1;  // Employee not found
}

// Function to check the login status of an employee by ID
int employee_status(int id) {
    Employee employees[100];  // Assuming max 100 employees
    int num_employees = load_all_employees(employees, 100);

    if (num_employees == -1) {
        printf("Failed to load employees.\n");
        return -1;  // Failed to load employees
    }

    for (int i = 0; i < num_employees; i++) {
          printf("Checking Employee ID: %d, Status: %d\n", employees[i].id, employees[i].status);
        fflush(stdout);
        if (employees[i].id == id) {
                    printf("Employee found with ID: %d, Status: %d\n", employees[i].id, employees[i].status);  // Debug statement
                    fflush(stdout);
            if (employees[i].status == 1) {
                return 1;  // Employee is logged in
            }
            return -1;  // Employee is not logged in
        }
    }

    return -1;  // Employee not found
}


int get_last_customer_id() {
    int last_id = 0;
    FILE *file = fopen(CUSTOMER_DATA_FILE, "r");
    if (file == NULL) {
        printf("Customer data file not found. Starting with ID 1.\n");
        return 1;  // If file doesn't exist, start with ID 1
    }

    char line[512];  // Use a larger buffer to accommodate longer lines
    while (fgets(line, sizeof(line), file)) {
        int current_id;

        // Ensure the line is not empty and starts with a number (customer ID)
        if (sscanf(line, "%d,", &current_id) == 1) {
            // Only update the last_id if a valid ID is extracted
            if (current_id > last_id) {
                last_id = current_id;
            }
        }
    }

    fclose(file);

    // Return the next available ID (last ID + 1)
    return last_id + 1;
}

// Function to add a new customer
void add_customer(const char *name, const char *password, double balance) {
    Customer new_customer;
    new_customer.id = get_last_customer_id(); // Increment ID for new customer
    strncpy(new_customer.name, name, sizeof(new_customer.name) - 1);
    new_customer.name[sizeof(new_customer.name) - 1] = '\0'; // Ensure null-termination
    strncpy(new_customer.password, password, sizeof(new_customer.password) - 1);
    new_customer.password[sizeof(new_customer.password) - 1] = '\0'; // Ensure null-termination
    new_customer.balance = balance;
    new_customer.status = 0; // Set active status (1 for active)
    new_customer.activeAccount = 1; // Active account by default

    // Open the customer data file with write access
    int fd = open(CUSTOMER_DATA_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("Error opening customer data file");
        return;
    }

    // Attempt to acquire an exclusive lock on the file
    while (flock(fd, LOCK_EX | LOCK_NB) == -1) { // LOCK_NB makes it non-blocking
        if (errno == EAGAIN) { // If lock is unavailable
            printf("File is locked, waiting to add customer...\n");
            usleep(100000); // Sleep for 100ms before retrying
            continue; // Try to acquire the lock again
        } else {
            perror("Error locking the customer data file");
            close(fd);
            return;
        }
    }

    // Now write the new customer data to the file
    char buffer[256];
    int length = snprintf(buffer, sizeof(buffer), "%d,%s,%s,%.2f,%d,%d\n",
                          new_customer.id, new_customer.name, new_customer.password,
                          new_customer.balance, new_customer.status, new_customer.activeAccount);
    
    if (write(fd, buffer, length) == -1) {
        perror("Error writing to customer data file");
    }

    // Release the file lock after writing
    if (flock(fd, LOCK_UN) == -1) {
        perror("Error unlocking the customer data file");
    }

    close(fd);
    printf("Customer added successfully with ID: %d\n", new_customer.id);
}

void modify_customer(int id, const char *new_name, const char *new_password, double new_balance) {
    int fd = open(CUSTOMER_DATA_FILE, O_RDWR); // Open file for reading and writing
    if (fd == -1) {
        perror("Error opening customer data file");
        return;
    }

    // Attempt to acquire an exclusive lock on the file
    while (flock(fd, LOCK_EX | LOCK_NB) == -1) { // LOCK_NB makes it non-blocking
        if (errno == EAGAIN) { // If lock is unavailable
            printf("File is locked, waiting to modify customer...\n");
            usleep(100000); // Sleep for 100ms before retrying
            continue; // Try to acquire the lock again
        } else {
            perror("Error locking the customer data file");
            close(fd);
            return;
        }
    }

    // Read the customer data from the file
    char buffer[1024];
    ssize_t bytes_read;
    Customer customers[100];  // Assuming a maximum of 100 customers
    int num_customers = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the buffer
        char *line = strtok(buffer, "\n");
        while (line != NULL && num_customers < 100) {
            sscanf(line, "%d,%49[^,],%49[^,],%lf,%d,%d", 
                   &customers[num_customers].id, 
                   customers[num_customers].name, 
                   customers[num_customers].password, 
                   &customers[num_customers].balance, 
                   &customers[num_customers].status, 
                   &customers[num_customers].activeAccount);
            num_customers++;
            line = strtok(NULL, "\n");
        }
    }

    // Check for read errors
    if (bytes_read == -1) {
        perror("Error reading customer data file");
        flock(fd, LOCK_UN); // Unlock the file before returning
        close(fd);
        return;
    }

    // Find and modify the specified customer
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == id) {
            // Update only the specified fields
            if (new_name != NULL) {
                strncpy(customers[i].name, new_name, sizeof(customers[i].name) - 1);
                customers[i].name[sizeof(customers[i].name) - 1] = '\0'; // Ensure null-termination
            }
            if (new_password != NULL) {
                strncpy(customers[i].password, new_password, sizeof(customers[i].password) - 1);
                customers[i].password[sizeof(customers[i].password) - 1] = '\0'; // Ensure null-termination
            }
            customers[i].balance = new_balance; // Update balance
            printf("Customer ID: %d modified successfully.\n", id);
            break;
        }
    }

    // Move the file pointer to the beginning and truncate the file
    lseek(fd, 0, SEEK_SET);
    ftruncate(fd, 0); // Clear the file

    // Write the modified data back to the file
    for (int i = 0; i < num_customers; i++) {
        int length = snprintf(buffer, sizeof(buffer), "%d,%s,%s,%.2f,%d,%d\n",
                              customers[i].id, customers[i].name, customers[i].password,
                              customers[i].balance, customers[i].status, customers[i].activeAccount);
        write(fd, buffer, length);
    }

    // Release the file lock
    if (flock(fd, LOCK_UN) == -1) {
        perror("Error unlocking the customer data file");
    }

    close(fd);
    printf("Customer data saved successfully.\n");
}


int change_employee_password(int employee_id, const char *current_password, const char *new_password) {
    Employee employees[100];
    int num_employees = load_all_employees(employees, 100);

    printf("Call came for changing password for employee %d\n", employee_id);
    fflush(stdout);

    for (int i = 0; i < num_employees; i++) {
        printf("Comparing with Employee %d - password: '%s'\n", 
               employees[i].id, employees[i].password);
        if (employees[i].id == employee_id) {
            if (strcmp(current_password, employees[i].password) == 0) {
                // Change the password
                strcpy(employees[i].password, new_password);
                // Save the updated employees back to the file
                save_all_employees(employees, num_employees);
                return 1; // Password changed successfully
            } else {
                printf("Current password for employee %d is incorrect.\n", employee_id);
                return -1; // Current password is incorrect
            }
        }
    }

    printf("Employee with ID %d not found.\n", employee_id);
    return 0; // Employee not found
}
