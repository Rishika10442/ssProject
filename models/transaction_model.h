#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdio.h>


typedef struct {
    char transactionType[20]; 
    int fromId;               
    int toId;                 
    double money;             
    int entryID;              
    int transactionID;        
} Transaction;

int get_last_transaction_id();
void log_transaction(const Transaction *transaction);
int deposit_money(double amount, int customer_id);
void acquire_file_lock(FILE *file);
void release_file_lock(FILE *file);
int withraw_money(double amount, int customer_id);
int transfer_funds(double amount, int customer_id1,int customer_id2);
Transaction* get_transaction_history(int cust_id, int *num_transactions);


#endif // TRANSACTION_H