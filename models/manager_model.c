#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "manager_model.h"
#include "bank_model.h" 
#include "customer_model.h" // Assuming you have access to Employee struct and loading functions

#define MAX_MANAGERS 100
#define MANAGER_DATA_FILE "/home/rishika-gupta/ssProject/manager_data.txt"
static Manager managers[MAX_MANAGERS];

void add_manager(const char *name,const char *password, int emp_id) {
    Employee employees[100];
    int num_employees = load_all_employees(employees, 100);

    // Find the employee with the given emp_id
    Employee *emp = NULL;
    for (int i = 0; i < num_employees; i++) {
        if (employees[i].id == emp_id) {
            emp = &employees[i];
            break;
        }
    }

    if (!emp) {
        printf("Employee with ID %d not found.\n", emp_id);
        return;
    }

    // Load all existing managers
    int num_managers = load_all_managers(managers, MAX_MANAGERS);

    // Get new manager ID
    int new_manager_id = get_max_manager_id() + 1;

    // Create the new manager entry
    Manager new_manager;
    new_manager.id = new_manager_id;
    strncpy(new_manager.name, name, sizeof(new_manager.name));
    strncpy(new_manager.password, password, sizeof(new_manager.password));  
    strncpy(new_manager.employee_name, emp->name, sizeof(new_manager.employee_name));  // Use employee's name
    new_manager.applications_pending = 0;
    new_manager.status = 0;
    new_manager.emp_id = emp_id;

    // Add the new manager to the array
    managers[num_managers] = new_manager;
    num_managers++;

    // Save the updated list of managers back to the file
    save_all_managers(managers, num_managers);

    emp->manager_id = new_manager.id;
    save_all_employees(employees, num_employees);

    printf("Manager added: ID = %d, Name = %s, Employee Name = %s, Employee ID = %d\n", 
           new_manager.id, new_manager.name, new_manager.employee_name, new_manager.emp_id);
}

// Function to save all managers to file
void save_all_managers(Manager managers[], int num_managers) {
    int fd = open(MANAGER_DATA_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening manager data file");
        return;
    }

    char buffer[256];
    for (int i = 0; i < num_managers; i++) {
        int length = snprintf(buffer, sizeof(buffer), "%d,%s,%s,%s,%d,%d,%d\n", 
                              managers[i].id, managers[i].name, managers[i].password, 
                              managers[i].employee_name, managers[i].applications_pending, 
                              managers[i].status, managers[i].emp_id);
        if (write(fd, buffer, length) == -1) {
            perror("Error writing to manager data file");
            close(fd);
            return;
        }
    }

    close(fd);
    printf("Manager data saved successfully.\n");
}

// Function to load all managers from file
int load_all_managers(Manager managers[], int max_managers) {
    int fd = open(MANAGER_DATA_FILE, O_RDONLY);
    if (fd == -1) {
        perror("Error opening manager data file");
        return -1;
    }

    char buffer[1024];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("Error reading manager data file");
        close(fd);
        return -1;
    }
    buffer[bytes_read] = '\0';
    close(fd);

    char *line = strtok(buffer, "\n");
    int index = 0;
    while (line != NULL && index < max_managers) {
        sscanf(line, "%d,%49[^,],%49[^,],%49[^,],%d,%d,%d", 
               &managers[index].id, managers[index].name, managers[index].password, 
               managers[index].employee_name, &managers[index].applications_pending, 
               &managers[index].status, &managers[index].emp_id);
        printf("Loaded Manager - ID: %d, Name: %s, Employee Name: %s, Employee ID: %d\n", 
               managers[index].id, managers[index].name, managers[index].employee_name, 
               managers[index].emp_id);
        line = strtok(NULL, "\n");
        index++;
    }

    return index;
}

// Function to get the max manager ID
int get_max_manager_id() {
    int num_managers = load_all_managers(managers, MAX_MANAGERS);
    int max_id = 0;
    for (int i = 0; i < num_managers; i++) {
        if (managers[i].id > max_id) {
            max_id = managers[i].id;
        }
    }
    return max_id;
}

// Function to validate manager credentials
int validate_manager_credentials(const char *name, const char *password) {
    Manager managers[MAX_MANAGERS];
    int num_managers = load_all_managers(managers, MAX_MANAGERS);

    if (num_managers == -1) {
        printf("Error loading managers.\n");
        return -1;  // Error loading managers
    }

    // Iterate through all managers and compare credentials
    for (int i = 0; i < num_managers; i++) {
        printf("Comparing with Manager %d - Name: '%s', Password: '%s', Status: %d\n", 
               managers[i].id, managers[i].name, managers[i].password, managers[i].status);

        // Validate the manager name and password
        if (strcmp(name, managers[i].name) == 0 && strcmp(password, managers[i].password) == 0) {
            
            // Check if the manager is already logged in
            if (managers[i].status == 1) {
                return -2;  // Manager already logged in
            }

            // If credentials are valid and manager is not logged in, log in the manager
            login_manager(managers[i].id);  // Assuming login_manager is the function that logs in the manager
            return managers[i].id;  // Valid credentials, return the manager's ID
        }
    }

    return -1;  // Invalid credentials
}

void login_manager(int manager_id) {
    Manager managers[MAX_MANAGERS];
    int num_managers = load_all_managers(managers, MAX_MANAGERS);

    // Iterate through the managers to find the matching ID and set the status to logged in (1)
    for (int i = 0; i < num_managers; i++) {
        if (managers[i].id == manager_id) {
            managers[i].status = 1;  // Mark the manager as logged in
            break;
        }
    }

    // Save the updated manager data back to the file
    save_all_managers(managers, num_managers);
}

// Function to log out a manager by ID
int logout_manager(int id) {
    printf("Call came for logging out this manager %d\n", id);
    fflush(stdout);

    Manager managers[MAX_MANAGERS];
    int num_managers = load_all_managers(managers, MAX_MANAGERS);

    // Iterate through all managers and find the one to log out
    for (int i = 0; i < num_managers; i++) {
        if (managers[i].id == id) {
            if (managers[i].status == 0) {
                return -1; // Manager already logged out
            }
            managers[i].status = 0; // Mark manager as logged out
            save_all_managers(managers, num_managers);  // Save changes to file
            printf("Manager logged out successfully. Current status: %d\n", managers[i].status);
            fflush(stdout);
            return 0; // Success
        }
    }
    return -1; // Manager not found
}

// Function to check if a manager is logged in by ID
int manager_status(int id) {
    Manager managers[MAX_MANAGERS];
    int num_managers = load_all_managers(managers, MAX_MANAGERS);

    for (int i = 0; i < num_managers; i++) {
        if (managers[i].id == id) {
            printf("manger ID %d, manager status %d\n",managers[i].id,managers[i].status);
            fflush(stdout);
            if (managers[i].status == 1) {
                return 1;  // Manager is logged in
            }
            return -1;  // Manager is logged out
        }
    }

    return -1;  // Manager not found
}

int change_manager_password(int manager_id, const char *current_password, const char *new_password) {
    Manager managers[MAX_MANAGERS];
    int num_managers = load_all_managers(managers, MAX_MANAGERS);

    printf("Call came for changing password for manager %d\n", manager_id);
    fflush(stdout);

    for (int i = 0; i < num_managers; i++) {
        printf("Comparing with Manager %d - password: '%s'\n", 
               managers[i].id, managers[i].password);
        if (managers[i].id == manager_id) {
            if (strcmp(current_password, managers[i].password) == 0) {
                // Change the password
                strcpy(managers[i].password, new_password);
                // Save the updated managers back to the file
                save_all_managers(managers, num_managers); 
                return 1; // Password changed successfully
            } else {
                printf("Current password for manager %d is incorrect.\n", manager_id);
                return -1; // Current password is incorrect
            }
        }
    }
    
    printf("Manager with ID %d not found.\n", manager_id);
    return 0; // Manager not found
}


int activation_change(int act, int cust_id) {
    Customer customers[100];
    int num_customers = load_all_customers(customers, 100);

    printf("Call came to change activation status for customer ID: %d to %d\n", cust_id, act);
    fflush(stdout);

    // Iterate through all customers to find the one with the matching ID
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == cust_id) {
            // Change the activeAccount field based on the provided act value
            customers[i].activeAccount = act;

            printf("Customer ID: %d, New activation status: %d\n", cust_id, customers[i].activeAccount);
            fflush(stdout);

            // Save the updated customers back to the file
            save_customers(customers, num_customers);
            return 1; // Successfully updated
        }
    }

    printf("Customer with ID %d not found.\n", cust_id);
    return 0; // Customer not found
}
