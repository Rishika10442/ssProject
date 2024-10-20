#ifndef LOAN_MODEL_H
#define LOAN_MODEL_H

#include <stdio.h>


typedef struct {
    char desc[20]; 
    double amount;
    int custID;               
    int manID;                
    int empID;             
    int loanID;            
    int loanStatus;        
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