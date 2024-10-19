
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "loan_model.h"
#include "manager_model.h"
#include "bank_model.h"


#define MAX_LOAN_HISTORY 100
#define MAX_LOANS 100
#define LOAN_DATA_FILE "/home/rishika-gupta/ssProject/loan_data.txt"
#define MAX_MANAGERS 50
#define MAX_EMPLOYEES 100

// Function to get the last loan ID
int get_last_loan_id() {
    int last_id = 0;
    FILE *file = fopen(LOAN_DATA_FILE, "r");
    if (file == NULL) {
        return 0;  // Start with ID 1 if the file doesn't exist
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int current_id;

        // Try to parse the loan ID (the sixth field in the line)
        if (sscanf(line, "%*[^,],%*lf,%*d,%*d,%*d,%d,%*d", &current_id) == 1) {
            printf("Debug: Successfully parsed loan ID: %d from line: '%s'\n", current_id, line);
            if (current_id > last_id) {
                last_id = current_id;
            }
        } else {
            printf("Debug: Failed to parse loan ID from line: '%s'\n", line);
        }
    }

    fclose(file);
    printf("Debug: Last loan ID found: %d\n", last_id);
    return last_id;
}

// Function to log a loan to the file (with file locking)
int log_loan(Loan* loan) {
    int fd = open(LOAN_DATA_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
    
    if (fd == -1) {
        perror("Error opening loan file");
        return 0;
    }

    // Acquire a lock on the file
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;  // Write lock

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error acquiring file lock");
        close(fd);
        return 0;
    }

    // Log the loan data to the file
    FILE *file = fdopen(fd, "a");
    if (file) {
        fprintf(file, "%s,%.2f,%d,%d,%d,%d,%d\n",
                loan->desc,
                loan->amount,
                loan->custID,
                loan->manID,
                loan->empID,
                loan->loanID,
                loan->loanStatus);
        fclose(file);
    }

    // Release the lock and close the file
    close(fd);
    return 1;
}


// Function to apply for a loan
int apply_loan(const char *subject, double money, int custID) {
    Loan new_loan;
    Manager managers[MAX_MANAGERS];
    int total_managers = load_all_managers(managers, MAX_MANAGERS);

    // Check if there are available managers
    if (total_managers == 0) {
        printf("No managers available\n");
        return 0;
    }

    // Assign a random manager ID
    int rand_index = rand() % total_managers;
    new_loan.manID = managers[rand_index].id;

    // Populate loan details
    strcpy(new_loan.desc, subject);
    new_loan.amount = money;
    new_loan.custID = custID;
    new_loan.empID = -1;             // Default empID
    new_loan.loanID = get_last_loan_id() + 1; // Increment loan ID
    new_loan.loanStatus = -1;        // Loan is pending

    // Save loan to file
    if (log_loan(&new_loan)) {
        printf("Loan applied successfully. Loan ID: %d\n", new_loan.loanID);
        return 1;
    }

    return 0;
}

// Function to get the application history for a customer
int applications_history(int custID, Loan loans[], int max_loans) {
    FILE *file = fopen(LOAN_DATA_FILE, "r");
    if (file == NULL) {
        perror("Failed to open loan data file");
        return -1;  // Error case
    }

    char line[256];
    int loan_count = 0;

    while (fgets(line, sizeof(line), file)) {
        Loan temp_loan;

        // Parse the loan data from the file, making sure the order matches the structure
        if (sscanf(line, "%[^,],%lf,%d,%d,%d,%d,%d", temp_loan.desc, &temp_loan.amount, 
                   &temp_loan.custID, &temp_loan.manID, &temp_loan.empID, 
                   &temp_loan.loanID, &temp_loan.loanStatus) == 7) {
            
            // Check if the customer ID matches
            if (temp_loan.custID == custID) {
                if (loan_count < max_loans) {
                    loans[loan_count++] = temp_loan;  // Add to the array
                }
            }
        } else {
            printf("Debug: Failed to parse loan record from line: '%s'\n", line);
        }
    }

    fclose(file);
    return loan_count;  // Return the number of loans found
}

// Function to get the application history for a manager
int applications_historyManager(int manID, Loan loans[], int max_loans) {
    FILE *file = fopen(LOAN_DATA_FILE, "r");
    if (file == NULL) {
        perror("Failed to open loan data file");
        return -1;  // Error case
    }

    char line[256];
    int loan_count = 0;

    while (fgets(line, sizeof(line), file)) {
        Loan temp_loan;

        // Parse the loan data from the file, making sure the order matches the structure
        if (sscanf(line, "%[^,],%lf,%d,%d,%d,%d,%d", 
                   temp_loan.desc, &temp_loan.amount, 
                   &temp_loan.custID, &temp_loan.manID, 
                   &temp_loan.empID, &temp_loan.loanID, 
                   &temp_loan.loanStatus) == 7) {
            
            // Check if the manager ID matches
            if (temp_loan.manID == manID) {
                if (loan_count < max_loans) {
                    loans[loan_count++] = temp_loan;  // Add to the array
                }
            }
        } else {
            printf("Debug: Failed to parse loan record from line: '%s'\n", line);
        }
    }

    fclose(file);
    return loan_count;  // Return the number of loans found
}

// Function to assign a loan to an employee
int assignLoan_ToEmployee(int loanID, int empID) {
   Loan loans[MAX_LOANS];
    int num_loans = load_all_loans(loans, MAX_LOANS);

    if (num_loans == -1) {
        printf("Error loading loans.\n");
        return;
    }

    // Search for the loan to update
    int found = 0;
    for (int i = 0; i < num_loans; i++) {
        if (loans[i].loanID == loanID) {
            loans[i].empID = empID; // Update empID
            found = 1;
            printf("Updated Loan ID %d with Employee ID %d.\n", loanID, empID);
            break;
        }
    }

    if (!found) {
        printf("Loan with ID %d not found.\n", loanID);
        return;
    }

    // Implement file locking for safe writing
    int fd = open(LOAN_DATA_FILE, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open loan data file for locking");
        return;
    }

    // Lock the file
    struct flock lock;
    lock.l_type = F_WRLCK; // Write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0; // Lock the entire file

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Failed to lock file");
        close(fd);
        return;
    }

    // Save updated loans back to the file
    save_all_loans(loans, num_loans);

    // Unlock the file
    lock.l_type = F_UNLCK; // Unlock
    fcntl(fd, F_SETLK, &lock);
    close(fd);


    // Now update the assignedLoans in the employee table
    Employee employees[MAX_EMPLOYEES]; // Assuming you have defined this structure and MAX_EMPLOYEES
    int num_employees = load_all_employees(employees, MAX_EMPLOYEES); // Load the employees

    // Find the employee and update their assigned loans count
    for (int i = 0; i < num_employees; i++) {
        if (employees[i].id == empID) {
            employees[i].assignedLoans += 1; // Increment the assigned loans
            // Save the updated employee record back to file
            save_all_employees(employees, num_employees); // Use the provided function
            break;
        }
    }

    return 0; // Success
}


// Function to load all loans into an array
int load_all_loans(Loan loans[], int max_loans) {
    FILE *file = fopen(LOAN_DATA_FILE, "r");
    if (file == NULL) {
        perror("Failed to open loan data file");
        return -1;
    }

    char line[256];
    int loan_count = 0;

    while (fgets(line, sizeof(line), file) && loan_count < max_loans) {
        Loan temp_loan;
        if (sscanf(line, "%[^,],%lf,%d,%d,%d,%d,%d", temp_loan.desc, &temp_loan.amount,
                   &temp_loan.custID, &temp_loan.manID, &temp_loan.empID,
                   &temp_loan.loanID, &temp_loan.loanStatus) == 7) {
            loans[loan_count++] = temp_loan; // Add to the array
        }
    }

    fclose(file);
    return loan_count; // Return the number of loans found
}

// Function to save all loans back to the file
void save_all_loans(Loan loans[], int num_loans) {
    FILE *file = fopen(LOAN_DATA_FILE, "w");
    if (file == NULL) {
        perror("Failed to open loan data file for writing");
        return;
    }

    for (int i = 0; i < num_loans; i++) {
        fprintf(file, "%s,%.2f,%d,%d,%d,%d,%d\n",
                loans[i].desc, loans[i].amount, loans[i].custID,
                loans[i].manID, loans[i].empID, loans[i].loanID,
                loans[i].loanStatus);
    }

    fclose(file);
}


// Function to get the application history for an employee
int applications_historyEmployee(int empID, Loan loans[], int max_loans) {
    FILE *file = fopen(LOAN_DATA_FILE, "r");
    if (file == NULL) {
        perror("Failed to open loan data file");
        return -1;  // Error case
    }

    char line[256];
    int loan_count = 0;

    while (fgets(line, sizeof(line), file)) {
        Loan temp_loan;

        // Parse the loan data from the file, making sure the order matches the structure
        if (sscanf(line, "%[^,],%lf,%d,%d,%d,%d,%d", 
                   temp_loan.desc, &temp_loan.amount, 
                   &temp_loan.custID, &temp_loan.manID, 
                   &temp_loan.empID, &temp_loan.loanID, 
                   &temp_loan.loanStatus) == 7) {
            
            // Check if the employee ID matches
            if (temp_loan.empID == empID) {
                if (loan_count < max_loans) {
                    loans[loan_count++] = temp_loan;  // Add to the array
                }
            }
        } else {
            printf("Debug: Failed to parse loan record from line: '%s'\n", line);
        }
    }

    fclose(file);
    return loan_count;  // Return the number of loans found
}
int update_loan_status(int loanID, int sts, int emp_id) {
 Loan loans[MAX_LOANS];
    int num_loans = load_all_loans(loans, MAX_LOANS);  // Load loans from file
    if (num_loans == -1) {
        printf("Error loading loans.\n");
        
        return -1;
    }
    int found = 0;
    // Search for the loan by loanID and check empID
    for (int i = 0; i < num_loans; i++) {
        if (loans[i].loanID == loanID) {
            if (loans[i].empID != emp_id) {
                
                return -2;  // empID does not match
            }
            found = 1;
            loans[i].loanStatus = sts;  // Update the status
            printf("Updated loan ID %d to status %d\n", loanID, sts);
            break;
        }
    }

    if (!found) {
        printf("Loan with ID %d not found.\n", loanID);
        return;
    }

    // Implement file locking for safe writing
    int fd = open(LOAN_DATA_FILE, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open loan data file for locking");
        return;
    }

    // Lock the file
    struct flock lock;
    lock.l_type = F_WRLCK; // Write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0; // Lock the entire file

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Failed to lock file");
        close(fd);
        return;
    }

    // Save updated loans back to the file
    save_all_loans(loans, num_loans);

    // Unlock the file
    lock.l_type = F_UNLCK; // Unlock
    fcntl(fd, F_SETLK, &lock);
    close(fd);


    // Now update the assignedLoans in the employee table
    Employee employees[MAX_EMPLOYEES]; // Assuming you have defined this structure and MAX_EMPLOYEES
    int num_employees = load_all_employees(employees, MAX_EMPLOYEES); // Load the employees

    // Find the employee and update their assigned loans count
    for (int i = 0; i < num_employees; i++) {
        if (employees[i].id == emp_id) {
            employees[i].assignedLoans -= 1; // Increment the assigned loans
            // Save the updated employee record back to file
            if(sts==1){
                employees[i].acceptedLoans += 1;
            }else{
             employees[i].rejectedLoans += 1;   
            }
            save_all_employees(employees, num_employees); // Use the provided function
            break;
        }
    }

    return 0; // Success
}