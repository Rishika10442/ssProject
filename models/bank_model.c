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


void add_employee(const char *name, const char *password) {
    Employee employee;
    int fd;
    ssize_t bytes_read;
    char buffer[256];


    fd = open(EMPLOYEE_DATA_FILE, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    
    int last_id = 0;
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        char *line = strtok(buffer, "\n");
        while (line != NULL) {
            sscanf(line, "%d", &last_id);
            line = strtok(NULL, "\n");
        }
    }

    
    employee.id = last_id + 1;  
    strncpy(employee.name, name, sizeof(employee.name) - 1);
    strncpy(employee.password, password, sizeof(employee.password) - 1);
    employee.assignedLoans = 0;
    employee.rejectedLoans = 0;
    employee.acceptedLoans = 0;
    employee.status = 0;
    employee.manager_id = -1;


    
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

    return index;  
}


void save_all_employees(Employee employees[], int num_employees) {
    int fd = open(EMPLOYEE_DATA_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening employee data file");
        return;
    }

    char buffer[256];
    for (int i = 0; i < num_employees; i++) {

        printf("Parsed Employee - ID: %d, Name: %s, Password: %s, Assigned Loans: %d, Rejected Loans: %d, Accepted Loans: %d, Status: %d, Manager ID: %d\n", 
           employees[i].id, employees[i].name, employees[i].password, 
           employees[i].assignedLoans, employees[i].rejectedLoans, 
           employees[i].acceptedLoans, employees[i].status, 
           employees[i].manager_id);

        
        int length = snprintf(buffer, sizeof(buffer), "%d,%s,%s,%d,%d,%d,%d,%d\n", 
                              employees[i].id, employees[i].name, employees[i].password, 
                              employees[i].assignedLoans, employees[i].rejectedLoans, employees[i].acceptedLoans,employees[i].status,employees[i].manager_id);
        

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



void modify_employee(int employee_id, const char *new_name, const char *new_password) {
    Employee employees[100];  
    int num_employees = load_all_employees(employees, 100);

    if (num_employees == -1) {
        printf("Failed to load employees.\n");
        return;
    }

    
    int found = 0;
    for (int i = 0; i < num_employees; i++) {
        if (employees[i].id == employee_id) {
            
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

    
    save_all_employees(employees, num_employees);
}


int validate_employee_credentials(const char *username, const char *password) {
    Employee employees[100];  
    int num_employees = load_all_employees(employees, 100);

    if (num_employees == -1) {
        printf("Failed to load employees.\n");
        return -1;  
    }

    for (int i = 0; i < num_employees; i++) {
        printf("Comparing with Employee %d - Username: '%s', Password: '%s', Status: %d\n", 
               employees[i].id, employees[i].name, employees[i].password, employees[i].status);
        
        if (strcmp(username, employees[i].name) == 0 && strcmp(password, employees[i].password) == 0) {
            if (employees[i].status == 1) {
                return -2; 
            }

            
            login_employee(employees[i].id);
            return employees[i].id; 
        }
    }

    printf("Invalid credentials for Username: %s, Password: %s\n", username, password);
    return -1; 
}


void login_employee(int id) {
    Employee employees[100];
    int num_employees = load_all_employees(employees, 100);

    if (num_employees == -1) {
        printf("Failed to load employees.\n");
        return;
    }

    for (int i = 0; i < num_employees; i++) {
        if (employees[i].id == id) {
            employees[i].status = 1;  
            save_all_employees(employees, num_employees);  
            printf("Employee with ID %d is now logged in.\n", id);
            return;
        }
    }
    printf("Employee with ID %d not found.\n", id);
}


int employee_logout(int id) {
    Employee employees[100];  
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
                return -1;  
            }
            employees[i].status = 0;  
            save_all_employees(employees, num_employees);  
            printf("Employee %d logged out successfully.\n", id);
            fflush(stdout);
            return 0;
        }
    }

    return -1;  
}


int employee_status(int id) {
    Employee employees[100];  
    int num_employees = load_all_employees(employees, 100);

    if (num_employees == -1) {
        printf("Failed to load employees.\n");
        return -1;  
    }

    for (int i = 0; i < num_employees; i++) {
          printf("Checking Employee ID: %d, Status: %d\n", employees[i].id, employees[i].status);
        fflush(stdout);
        if (employees[i].id == id) {
                    printf("Employee found with ID: %d, Status: %d\n", employees[i].id, employees[i].status);  // Debug statement
                    fflush(stdout);
            if (employees[i].status == 1) {
                return 1; 
            }
            return -1; 
        }
    }

    return -1; 
}


int get_last_customer_id() {
    int last_id = 0;
    FILE *file = fopen(CUSTOMER_DATA_FILE, "r");
    if (file == NULL) {
        printf("Customer data file not found. Starting with ID 1.\n");
        return 1; 
    }

    char line[512];  
    while (fgets(line, sizeof(line), file)) {
        int current_id;

        
        if (sscanf(line, "%d,", &current_id) == 1) {
            
            if (current_id > last_id) {
                last_id = current_id;
            }
        }
    }

    fclose(file);

    
    return last_id + 1;
}


void add_customer(const char *name, const char *password, double balance) {
    Customer new_customer;
    new_customer.id = get_last_customer_id(); 
    strncpy(new_customer.name, name, sizeof(new_customer.name) - 1);
    new_customer.name[sizeof(new_customer.name) - 1] = '\0'; 
    strncpy(new_customer.password, password, sizeof(new_customer.password) - 1);
    new_customer.password[sizeof(new_customer.password) - 1] = '\0'; 
    new_customer.balance = balance;
    new_customer.status = 0; 
    new_customer.activeAccount = 1; 

    
    int fd = open(CUSTOMER_DATA_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("Error opening customer data file");
        return;
    }

    
    while (flock(fd, LOCK_EX | LOCK_NB) == -1) { 
        if (errno == EAGAIN) {
            printf("File is locked, waiting to add customer...\n");
            usleep(100000); 
            continue; 
        } else {
            perror("Error locking the customer data file");
            close(fd);
            return;
        }
    }

    
    char buffer[256];
    int length = snprintf(buffer, sizeof(buffer), "%d,%s,%s,%.2f,%d,%d\n",
                          new_customer.id, new_customer.name, new_customer.password,
                          new_customer.balance, new_customer.status, new_customer.activeAccount);
    
    if (write(fd, buffer, length) == -1) {
        perror("Error writing to customer data file");
    }

    
    if (flock(fd, LOCK_UN) == -1) {
        perror("Error unlocking the customer data file");
    }

    close(fd);
    printf("Customer added successfully with ID: %d\n", new_customer.id);
}

void modify_customer(int id, const char *new_name, const char *new_password, double new_balance) {
    int fd = open(CUSTOMER_DATA_FILE, O_RDWR); 
    if (fd == -1) {
        perror("Error opening customer data file");
        return;
    }

    
    while (flock(fd, LOCK_EX | LOCK_NB) == -1) { 
        if (errno == EAGAIN) { 
            printf("File is locked, waiting to modify customer...\n");
            usleep(100000); 
            continue; 
        } else {
            perror("Error locking the customer data file");
            close(fd);
            return;
        }
    }

    
    char buffer[1024];
    ssize_t bytes_read;
    Customer customers[100];  
    int num_customers = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; 
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

    
    if (bytes_read == -1) {
        perror("Error reading customer data file");
        flock(fd, LOCK_UN); 
        close(fd);
        return;
    }

    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == id) {
            if (new_name != NULL) {
                strncpy(customers[i].name, new_name, sizeof(customers[i].name) - 1);
                customers[i].name[sizeof(customers[i].name) - 1] = '\0'; 
            }
            if (new_password != NULL) {
                strncpy(customers[i].password, new_password, sizeof(customers[i].password) - 1);
                customers[i].password[sizeof(customers[i].password) - 1] = '\0'; 
            }
            customers[i].balance = new_balance;
            printf("Customer ID: %d modified successfully.\n", id);
            break;
        }
    }

    lseek(fd, 0, SEEK_SET);
    ftruncate(fd, 0); 

    for (int i = 0; i < num_customers; i++) {
        int length = snprintf(buffer, sizeof(buffer), "%d,%s,%s,%.2f,%d,%d\n",
                              customers[i].id, customers[i].name, customers[i].password,
                              customers[i].balance, customers[i].status, customers[i].activeAccount);
        write(fd, buffer, length);
    }

   
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
                strcpy(employees[i].password, new_password);
                save_all_employees(employees, num_employees);
                return 1;
            } else {
                printf("Current password for employee %d is incorrect.\n", employee_id);
                return -1; 
            }
        }
    }

    printf("Employee with ID %d not found.\n", employee_id);
    return 0; 
    }
