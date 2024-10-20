#ifndef CUSTOMER_MODEL_H
#define CUSTOMER_MODEL_H

#define MAX_LOANS 10
#define MAX_FEEDBACKS 10
#define MAX_TRANSACTIONS 100
#define MAX_CUSTOMERS 100

typedef struct {
    int id;
    char name[50];
    char password[50];
    double balance;
    int status;  
    int activeAccount;  
} Customer;

int validate_customer_credentials(const char *username, const char *password);

int load_all_customers(Customer *customers, int max_customers);
void login_customer(int id);
int save_customers(Customer *customers, int num_customers);
int logout_customer(int id);
int customer_status(int id);
double check_balance(int customer_id);
int change_customer_password(int customer_id, const char *current_password, const char *new_password);

#endif // CUSTOMER_MODEL_H
