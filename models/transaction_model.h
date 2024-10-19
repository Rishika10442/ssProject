#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdio.h>

// Define the Transaction struct
typedef struct {
    char transactionType[20]; // Type of the transaction (e.g., "DEPOSIT", "WITHDRAWAL", "TRANSFER")
    int fromId;               // ID of the account from which money is transferred
    int toId;                 // ID of the account to which money is transferred
    double money;             // Amount of money involved in the transaction
    int entryID;              // Entry ID for logging or tracking
    int transactionID;        // Unique identifier for the transaction
} Transaction;

// Function declarations (if needed)
// Example: void log_transaction(const Transaction *transaction);

// Function declarations
int get_last_transaction_id();
void log_transaction(const Transaction *transaction);
int deposit_money(double amount, int customer_id);
void acquire_file_lock(FILE *file);
void release_file_lock(FILE *file);
int withraw_money(double amount, int customer_id);
int transfer_funds(double amount, int customer_id1,int customer_id2);
Transaction* get_transaction_history(int cust_id, int *num_transactions);


#endif // TRANSACTION_H