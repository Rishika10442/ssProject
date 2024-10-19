#ifndef LOAN_MODEL_H
#define LOAN_MODEL_H

#include <stdio.h>

// Define the Transaction struct
typedef struct {
    char desc[20]; // Type of the transaction (e.g., "DEPOSIT", "WITHDRAWAL", "TRANSFER")
    double amount;
    int custID;               // ID of the account from which money is transferred
    int manID;                 // ID of the account to which money is transferred
    int empID;             // Amount of money involved in the transaction
    int loanID;              // Entry ID for logging or tracking
    int loanStatus;        // Unique identifier for the transaction
} Loan;

int apply_loan(const char *subject, double money, int custID);
int log_loan(Loan* loan);
int get_last_loan_id();
int applications_history(int custID, Loan loans[], int max_loans);
int applications_historyManager(int manID, Loan loans[], int max_loans);
int assignLoan_ToEmployee(int loanID, int empID);
int applications_historyEmployee(int empID, Loan loans[], int max_loans);
void save_all_loans(Loan loans[], int num_loans);
int load_all_loans(Loan loans[], int max_loans);
int update_loan_status(int loanID, int sts, int emp_id);

#endif 