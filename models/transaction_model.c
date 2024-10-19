#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "transaction_model.h"
#include "customer_model.h"

#define TRANSACTION_DATA_FILE "/home/rishika-gupta/ssProject/transactions_data.txt"

// Function to acquire file lock
void acquire_file_lock(FILE *file) {
    int fd = fileno(file);
    struct flock lock;
    lock.l_type = F_WRLCK;  // Write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0; // Lock the whole file

    // Attempt to lock the file
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error acquiring file lock");
        exit(EXIT_FAILURE);
    }
}

// Function to release file lock
void release_file_lock(FILE *file) {
    int fd = fileno(file);
    struct flock lock;
    lock.l_type = F_UNLCK;  // Unlock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0; // Unlock the whole file

    // Release the lock
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error releasing file lock");
    }
}

// Function to get the last transaction ID
// Function to get the last transaction ID
// Function to get the last transaction ID
int get_last_transaction_id() {
    int last_id = 0;
    FILE *file = fopen(TRANSACTION_DATA_FILE, "r");
    
    if (file == NULL) {
        printf("Debug: Transaction data file not found. Starting with ID 0.\n");
        return 0;  // If the file doesn't exist, start with ID 0
    } else {
        printf("Debug: Successfully opened transaction data file.\n");
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int transactionID;

        // Trim newline character from the end of the line
       // line[strcspn(line, "\n")] = 0; 

        // Use sscanf to extract the last value (transactionID)
        // The format is: transactionType,fromId,toId,money,entryID,transactionID
        if (sscanf(line, "%*[^,],%*d,%*d,%*d,%*lf,%d", &transactionID) == 1) {
            printf("Debug: Read transaction ID: %d from line: '%s'\n", transactionID, line);
            if (transactionID > last_id) {
                last_id = transactionID; // Update the last ID if current is greater
            }
        } else {
            printf("Debug: Failed to read transaction ID from line: '%s'\n", line);
        }
    }

    fclose(file);
    printf("Debug: Last transaction ID found: %d\n", last_id); // Print the last ID found
    return last_id;  // Return the last ID found
}


// Function to log a transaction
void log_transaction(const Transaction *transaction) {
    FILE *file = fopen(TRANSACTION_DATA_FILE, "a");
    if (file == NULL) {
        perror("Error opening transaction data file");
        return;
    }

    // Acquire file lock before writing
    acquire_file_lock(file);

    fprintf(file, "%s,%d,%d,%d,%.2f,%d\n",
            transaction->transactionType,
            transaction->fromId,
            transaction->toId,
            transaction->entryID,
            transaction->money,
            transaction->transactionID);

    // Release the file lock after writing
    release_file_lock(file);
    fclose(file);
}

int deposit_money(double amount, int customer_id) {
    // Load all customers
    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    // Find the customer with the given ID
    Customer *customer = NULL;
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == customer_id) {
            customer = &customers[i];
            break;
        }
    }

    // If customer is not found, return 0
    if (customer == NULL) {
        printf("Customer ID %d not found.\n", customer_id);
        return 0; // Customer not found
    }

    // Update the customer's balance
    customer->balance += amount;

    // Create a transaction for the deposit
    Transaction new_transaction;

    // Populate the transaction details
    strcpy(new_transaction.transactionType, "DEPOSIT");
    new_transaction.fromId = customer_id;      // From customer ID
    new_transaction.toId = customer_id;        // To customer ID (same for deposit)
    new_transaction.money = amount;             // Amount to deposit
    new_transaction.entryID = customer_id;      // Entry ID is customer ID
    new_transaction.transactionID = get_last_transaction_id() + 1; // New transaction ID

    // Log the transaction
    log_transaction(&new_transaction);

    // Save updated customer data
    save_customers(customers, num_customers);  // Implement this function to save customers

    printf("Deposit successful: Customer ID: %d, Amount: %.2f, New Balance: %.2f\n", 
           customer_id, amount, customer->balance);
    return 1; // Return 1 for successful deposit
}

int withraw_money(double amount, int customer_id) {
    printf("Withraw request: Customer ID: %d, Amount: %.2f, \n", customer_id, amount);
    fflush(stdout);
    // Load all customers

    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    // Find the customer with the given ID
    Customer *customer = NULL;
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == customer_id) {
            customer = &customers[i];
            break;
        }
    }

    // If customer is not found, return 0
    if (customer == NULL) {
        printf("Customer ID %d not found.\n", customer_id);
        return 0; // Customer not found
    }

    // Update the customer's balance
    customer->balance -= amount;

    // Create a transaction for the deposit
    Transaction new_transaction;
    //int ix = get_last_transaction_id() ;
    // Populate the transaction details
    strcpy(new_transaction.transactionType, "WITHRAW");
    new_transaction.fromId = customer_id;      // From customer ID
    new_transaction.toId = customer_id;        // To customer ID (same for deposit)
    new_transaction.money = amount;             // Amount to deposit
    new_transaction.entryID = customer_id;      // Entry ID is customer ID
    new_transaction.transactionID = get_last_transaction_id() + 1; // New transaction ID
  //  printf("Withraw request: Transaction ID %d, \n", ix);
    printf("Withraw request: Transaction ID %d, \n", new_transaction.transactionID);
    // Log the transaction
    log_transaction(&new_transaction);

    // Save updated customer data
    save_customers(customers, num_customers);  // Implement this function to save customers

    printf("Withraw successful: Customer ID: %d, Amount: %.2f, New Balance: %.2f\n", 
           customer_id, amount, customer->balance);
    return 1; // Return 1 for successful deposit
}


int transfer_funds(double amount, int customer_id1,int customer_id2) {
    printf("Transfer request: Customer ID: %d, Amount: %.2f, \n", customer_id1, amount);
    fflush(stdout);
    // Load all customers

    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    // Find the customer with the given ID
    Customer *customer = NULL;
    Customer *customer2 = NULL;
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == customer_id1) {
            customer = &customers[i];
            break;
        }
    }

    // If customer is not found, return 0
    if (customer == NULL) {
        printf("Customer ID %d not found.\n", customer_id1);
        return 0; // Customer not found
    }

    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == customer_id2) {
            if(customers[i].activeAccount==0){return -2;}
            customer2 = &customers[i];
            break;
        }
    }
    // If customer is not found, return 0
    if (customer2 == NULL) {
        printf("Customer 2 ID %d not found.\n", customer_id2);
        return 0; // Customer not found
    }
    // Update the customer's balance
    customer->balance -= amount;
    customer2->balance += amount;

    // Create a transaction for the deposit
    Transaction new_transaction;
    //int ix = get_last_transaction_id() ;
    // Populate the transaction details
    strcpy(new_transaction.transactionType, "TRANSFER");
    new_transaction.fromId = customer_id1;      // From customer ID
    new_transaction.toId = customer_id2;        // To customer ID (same for deposit)
    new_transaction.money = amount;             // Amount to deposit
    new_transaction.entryID = customer_id1;      // Entry ID is customer ID
    new_transaction.transactionID = get_last_transaction_id() + 1; // New transaction ID
  //  printf("Withraw request: Transaction ID %d, \n", ix);
    printf("Transfer request: Transaction ID %d, \n", new_transaction.transactionID);
    // Log the transaction
    log_transaction(&new_transaction);

    // Save updated customer data
    save_customers(customers, num_customers);  // Implement this function to save customers

    printf("Transfer successful: Customer ID: %d, Amount: %.2f, New Balance: %.2f ,Customer 2 ID: %d, 2balance: %.2f,\n", 
           customer_id1, amount, customer->balance,customer_id2,customer2->balance);
    return 1; // Return 1 for successful deposit
}


// Function to get the transaction history for a customer dynamically
Transaction* get_transaction_history(int cust_id, int *num_transactions) {
    FILE *file = fopen(TRANSACTION_DATA_FILE, "r");
    if (file == NULL) {
        printf("Debug: Transaction data file not found.\n");
        *num_transactions = -1;  // Return -1 if the file doesn't exist
        return NULL;
    }

    printf("Debug: Successfully opened transaction data file.\n");
    
    char line[256];
    Transaction *transaction_history = NULL;
    *num_transactions = 0;

    // Loop through each line in the transaction file
    while (fgets(line, sizeof(line), file) != NULL) {
        Transaction transaction;

        // Trim newline character from the end of the line
        line[strcspn(line, "\n")] = 0;

        // Parse the line and fill the transaction structure
        if (sscanf(line, "%[^,],%d,%d,%d,%lf,%d", 
            transaction.transactionType,
            &transaction.fromId,
            &transaction.toId,
            &transaction.entryID,
            &transaction.money,
            &transaction.transactionID) == 6) {

            // Check if the transaction involves the given customer ID
            if (transaction.fromId == cust_id || transaction.toId == cust_id) {
                // Reallocate memory to store the new transaction
                transaction_history = realloc(transaction_history, (*num_transactions + 1) * sizeof(Transaction));
                if (transaction_history == NULL) {
                    printf("Error: Memory allocation failed.\n");
                    fclose(file);
                    *num_transactions = -1;
                    return NULL;  // Return NULL on memory allocation failure
                }

                // Store the transaction in the history array
                transaction_history[*num_transactions] = transaction;
                (*num_transactions)++;
            }
        } else {
            printf("Debug: Failed to parse transaction line: '%s'\n", line);
        }
    }

    fclose(file);
    return transaction_history;  // Return the array of matching transactions
}
