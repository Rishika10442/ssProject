#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "transaction_model.h"
#include "customer_model.h"

#define TRANSACTION_DATA_FILE "/home/rishika-gupta/ssProject/transactions_data.txt"


void acquire_file_lock(FILE *file) {
    int fd = fileno(file);
    struct flock lock;
    lock.l_type = F_WRLCK;  
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0; 

   
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error acquiring file lock");
        exit(EXIT_FAILURE);
    }
}


void release_file_lock(FILE *file) {
    int fd = fileno(file);
    struct flock lock;
    lock.l_type = F_UNLCK;  
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0; 

    
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error releasing file lock");
    }
}


int get_last_transaction_id() {
    int last_id = 0;
    FILE *file = fopen(TRANSACTION_DATA_FILE, "r");
    
    if (file == NULL) {
        printf("Debug: Transaction data file not found. Starting with ID 0.\n");
        return 0;  
    } else {
        printf("Debug: Successfully opened transaction data file.\n");
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int transactionID;

        
        if (sscanf(line, "%*[^,],%*d,%*d,%*d,%*lf,%d", &transactionID) == 1) {
            printf("Debug: Read transaction ID: %d from line: '%s'\n", transactionID, line);
            if (transactionID > last_id) {
                last_id = transactionID; 
            }
        } else {
            printf("Debug: Failed to read transaction ID from line: '%s'\n", line);
        }
    }

    fclose(file);
    printf("Debug: Last transaction ID found: %d\n", last_id); 
    return last_id;  
}



void log_transaction(const Transaction *transaction) {
    FILE *file = fopen(TRANSACTION_DATA_FILE, "a");
    if (file == NULL) {
        perror("Error opening transaction data file");
        return;
    }

    
    acquire_file_lock(file);

    fprintf(file, "%s,%d,%d,%d,%.2f,%d\n",
            transaction->transactionType,
            transaction->fromId,
            transaction->toId,
            transaction->entryID,
            transaction->money,
            transaction->transactionID);

    
    release_file_lock(file);
    fclose(file);
}

int deposit_money(double amount, int customer_id) {
    
    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    
    Customer *customer = NULL;
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == customer_id) {
            customer = &customers[i];
            break;
        }
    }

   
    if (customer == NULL) {
        printf("Customer ID %d not found.\n", customer_id);
        return 0;
    }

    
    customer->balance += amount;

    
    Transaction new_transaction;

   
    strcpy(new_transaction.transactionType, "DEPOSIT");
    new_transaction.fromId = customer_id;      
    new_transaction.toId = customer_id;       
    new_transaction.money = amount;            
    new_transaction.entryID = customer_id;      
    new_transaction.transactionID = get_last_transaction_id() + 1; 

    
    log_transaction(&new_transaction);

   
    save_customers(customers, num_customers);  

    printf("Deposit successful: Customer ID: %d, Amount: %.2f, New Balance: %.2f\n", 
           customer_id, amount, customer->balance);
    return 1; 
}

int withraw_money(double amount, int customer_id) {
    printf("Withraw request: Customer ID: %d, Amount: %.2f, \n", customer_id, amount);
    fflush(stdout);
  

    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    
    Customer *customer = NULL;
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == customer_id) {
            customer = &customers[i];
            break;
        }
    }

    
    if (customer == NULL) {
        printf("Customer ID %d not found.\n", customer_id);
        return 0; 
    }

   
    customer->balance -= amount;

    
    Transaction new_transaction;
    
    strcpy(new_transaction.transactionType, "WITHRAW");
    new_transaction.fromId = customer_id;      
    new_transaction.toId = customer_id;        
    new_transaction.money = amount;            
    new_transaction.entryID = customer_id;     
    new_transaction.transactionID = get_last_transaction_id() + 1; 
  
    printf("Withraw request: Transaction ID %d, \n", new_transaction.transactionID);
    
    log_transaction(&new_transaction);

    
    save_customers(customers, num_customers);  

    printf("Withraw successful: Customer ID: %d, Amount: %.2f, New Balance: %.2f\n", 
           customer_id, amount, customer->balance);
    return 1; 
}


int transfer_funds(double amount, int customer_id1,int customer_id2) {
    printf("Transfer request: Customer ID: %d, Amount: %.2f, \n", customer_id1, amount);
    fflush(stdout);
    

    Customer customers[MAX_CUSTOMERS];
    int num_customers = load_all_customers(customers, MAX_CUSTOMERS);

    
    Customer *customer = NULL;
    Customer *customer2 = NULL;
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == customer_id1) {
            customer = &customers[i];
            break;
        }
    }

    
    if (customer == NULL) {
        printf("Customer ID %d not found.\n", customer_id1);
        return 0; 
    }

    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == customer_id2) {
            if(customers[i].activeAccount==0){return -2;}
            customer2 = &customers[i];
            break;
        }
    }
    
    if (customer2 == NULL) {
        printf("Customer 2 ID %d not found.\n", customer_id2);
        return 0; 
    }
    
    customer->balance -= amount;
    customer2->balance += amount;

    
    Transaction new_transaction;
   
    strcpy(new_transaction.transactionType, "TRANSFER");
    new_transaction.fromId = customer_id1;      
    new_transaction.toId = customer_id2;        
    new_transaction.money = amount;             
    new_transaction.entryID = customer_id1;      
    new_transaction.transactionID = get_last_transaction_id() + 1; 
  //  printf("Withraw request: Transaction ID %d, \n", ix);
    printf("Transfer request: Transaction ID %d, \n", new_transaction.transactionID);
    
    log_transaction(&new_transaction);

    
    save_customers(customers, num_customers);  

    printf("Transfer successful: Customer ID: %d, Amount: %.2f, New Balance: %.2f ,Customer 2 ID: %d, 2balance: %.2f,\n", 
           customer_id1, amount, customer->balance,customer_id2,customer2->balance);
    return 1;
}



Transaction* get_transaction_history(int cust_id, int *num_transactions) {
    FILE *file = fopen(TRANSACTION_DATA_FILE, "r");
    if (file == NULL) {
        printf("Debug: Transaction data file not found.\n");
        *num_transactions = -1;  
        return NULL;
    }

    printf("Debug: Successfully opened transaction data file.\n");
    
    char line[256];
    Transaction *transaction_history = NULL;
    *num_transactions = 0;

    
    while (fgets(line, sizeof(line), file) != NULL) {
        Transaction transaction;

        
        line[strcspn(line, "\n")] = 0;

        
        if (sscanf(line, "%[^,],%d,%d,%d,%lf,%d", 
            transaction.transactionType,
            &transaction.fromId,
            &transaction.toId,
            &transaction.entryID,
            &transaction.money,
            &transaction.transactionID) == 6) {

            
            if (transaction.fromId == cust_id || transaction.toId == cust_id) {
               
                transaction_history = realloc(transaction_history, (*num_transactions + 1) * sizeof(Transaction));
                if (transaction_history == NULL) {
                    printf("Error: Memory allocation failed.\n");
                    fclose(file);
                    *num_transactions = -1;
                    return NULL;  
                }

                
                transaction_history[*num_transactions] = transaction;
                (*num_transactions)++;
            }
        } else {
            printf("Debug: Failed to parse transaction line: '%s'\n", line);
        }
    }

    fclose(file);
    return transaction_history;  
}
