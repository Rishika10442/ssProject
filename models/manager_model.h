#ifndef MANAGER_MODEL_H
#define MANAGER_MODEL_H



// Struct to represent Manager data
typedef struct {
    int id;                  // Manager ID
    char name[50];           // Manager's own name
    char password[50];       // Manager's password
    char employee_name[50];   // Employee's name (from employee table)
    int applications_pending; // Number of pending applications
    int status;              // Status, 0 by default
    int emp_id;              // Employee ID linked to the manager
} Manager;

// Function declarations
void add_manager(const char *name,const char *password, int emp_id);
void save_all_managers(Manager managers[], int num_managers);
int load_all_managers(Manager managers[], int max_managers);
int get_max_manager_id();
void login_manager(int manager_id);
int validate_manager_credentials(const char *name, const char *password);
int manager_status(int id);
int logout_manager(int id);
int change_manager_password(int manager_id, const char *current_password, const char *new_password);
int activation_change(int act, int cust_id);


#endif
