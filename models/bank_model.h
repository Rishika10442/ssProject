// bank_model.h

#ifndef BANK_MODEL_H
#define BANK_MODEL_H


typedef struct {
    int id;
    char name[50];
    char password[50];
    int assignedLoans;
    int rejectedLoans;
    int acceptedLoans;
    int status;
    int manager_id;
} Employee;

void add_employee(const char *name, const char *password);
void modify_employee(int employee_id,const char *new_name,const char *new_password);
void save_all_employees(Employee employees[], int num_employees);
int load_all_employees(Employee employees[],int max_employees);
int validate_employee_credentials(const char *username, const char *password);
void login_employee(int id);
int employee_status(int id);
int employee_logout(int id);
int change_employee_password(int employee_id, const char *current_password, const char *new_password);

int get_last_customer_id();
void add_customer(const char *name, const char *password, double balance);
void modify_customer(int id, const char *new_name, const char *new_password, double new_balance);



#endif
