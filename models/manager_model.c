#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "manager_model.h"
#include "bank_model.h" 
#include "customer_model.h" 

#define MAX_MANAGERS 100
#define MANAGER_DATA_FILE "/home/rishika-gupta/ssProject/manager_data.txt"
static Manager managers[MAX_MANAGERS];

void add_manager(const char *name,const char *password, int emp_id) {
    Employee employees[100];
    int num_employees = load_all_employees(employees, 100);

    
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

   
    int num_managers = load_all_managers(managers, MAX_MANAGERS);

   
    int new_manager_id = get_max_manager_id() + 1;

    
    Manager new_manager;
    new_manager.id = new_manager_id;
    strncpy(new_manager.name, name, sizeof(new_manager.name));
    strncpy(new_manager.password, password, sizeof(new_manager.password));  
    strncpy(new_manager.employee_name, emp->name, sizeof(new_manager.employee_name));  // Use employee's name
    new_manager.applications_pending = 0;
    new_manager.status = 0;
    new_manager.emp_id = emp_id;

    
    managers[num_managers] = new_manager;
    num_managers++;

    
    save_all_managers(managers, num_managers);

    emp->manager_id = new_manager.id;
    save_all_employees(employees, num_employees);

    printf("Manager added: ID = %d, Name = %s, Employee Name = %s, Employee ID = %d\n", 
           new_manager.id, new_manager.name, new_manager.employee_name, new_manager.emp_id);
}


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


int validate_manager_credentials(const char *name, const char *password) {
    Manager managers[MAX_MANAGERS];
    int num_managers = load_all_managers(managers, MAX_MANAGERS);

    if (num_managers == -1) {
        printf("Error loading managers.\n");
        return -1;  
    }

   
    for (int i = 0; i < num_managers; i++) {
        printf("Comparing with Manager %d - Name: '%s', Password: '%s', Status: %d\n", 
               managers[i].id, managers[i].name, managers[i].password, managers[i].status);

      
        if (strcmp(name, managers[i].name) == 0 && strcmp(password, managers[i].password) == 0) {
            
           
            if (managers[i].status == 1) {
                return -2;  
            }

            
            login_manager(managers[i].id);  
            return managers[i].id;  
        }
    }

    return -1;  
}

void login_manager(int manager_id) {
    Manager managers[MAX_MANAGERS];
    int num_managers = load_all_managers(managers, MAX_MANAGERS);

    for (int i = 0; i < num_managers; i++) {
        if (managers[i].id == manager_id) {
            managers[i].status = 1;  
            break;
        }
    }

    
    save_all_managers(managers, num_managers);
}


int logout_manager(int id) {
    printf("Call came for logging out this manager %d\n", id);
    fflush(stdout);

    Manager managers[MAX_MANAGERS];
    int num_managers = load_all_managers(managers, MAX_MANAGERS);

    for (int i = 0; i < num_managers; i++) {
        if (managers[i].id == id) {
            if (managers[i].status == 0) {
                return -1; 
            }
            managers[i].status = 0; 
            save_all_managers(managers, num_managers); 
            printf("Manager logged out successfully. Current status: %d\n", managers[i].status);
            fflush(stdout);
            return 0; 
        }
    }
    return -1; 
}


int manager_status(int id) {
    Manager managers[MAX_MANAGERS];
    int num_managers = load_all_managers(managers, MAX_MANAGERS);

    for (int i = 0; i < num_managers; i++) {
        if (managers[i].id == id) {
            printf("manger ID %d, manager status %d\n",managers[i].id,managers[i].status);
            fflush(stdout);
            if (managers[i].status == 1) {
                return 1; 
            }
            return -1;  
        }
    }

    return -1;  
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
               
                strcpy(managers[i].password, new_password);
                
                save_all_managers(managers, num_managers); 
                return 1; 
            } else {
                printf("Current password for manager %d is incorrect.\n", manager_id);
                return -1; 
            }
        }
    }
    
    printf("Manager with ID %d not found.\n", manager_id);
    return 0; 
}


int activation_change(int act, int cust_id) {
    Customer customers[100];
    int num_customers = load_all_customers(customers, 100);

    printf("Call came to change activation status for customer ID: %d to %d\n", cust_id, act);
    fflush(stdout);

    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == cust_id) {
            customers[i].activeAccount = act;

            printf("Customer ID: %d, New activation status: %d\n", cust_id, customers[i].activeAccount);
            fflush(stdout);

            save_customers(customers, num_customers);
            return 1; 
        }
    }

    printf("Customer with ID %d not found.\n", cust_id);
    return 0; 
}
