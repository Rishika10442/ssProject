#ifndef ADMIN_MODEL_H
#define ADMIN_MODEL_H


#define MAX_ADMINS 5
#define ADMIN_DATA_FILE "admin_data.txt"


typedef struct {
    int id;                 
    char username[50];      
    char password[50];       
    int logged_in;           
}Admin;


void load_admins_from_file();


void save_admins_to_file();


int validate_admin_credentials(const char *username, const char *password);


void login_admin(int id);


int  logout_admin(int id);

int admin_status(int id);
int change_admin_password(int admin_id,const char *current_password,const char *password);

#endif // ADMIN_MODEL_H
