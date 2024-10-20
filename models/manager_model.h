#ifndef MANAGER_MODEL_H
#define MANAGER_MODEL_H




typedef struct {
    int id;                 
    char name[50];          
    char password[50];      
    char employee_name[50];   
    int applications_pending; 
    int status;              
    int emp_id;              
} Manager;


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
