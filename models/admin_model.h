#ifndef ADMIN_MODEL_H
#define ADMIN_MODEL_H

// Define maximum number of admins
#define MAX_ADMINS 5
#define ADMIN_DATA_FILE "admin_data.txt"

// Struct to store admin details
typedef struct {
    int id;                 // Admin ID
    char username[50];       // Admin username
    char password[50];       // Admin password
    int logged_in;           // 0 = not logged in, 1 = logged in
}Admin;

// Function declarations

// Loads admin data from the file into memory
void load_admins_from_file();

// Saves current admin data (including session status) to the file
void save_admins_to_file();

// Validates admin credentials, returns admin ID if valid, -1 if invalid, -2 if already logged in
int validate_admin_credentials(const char *username, const char *password);

// Logs in the admin (sets the logged_in flag to 1 and updates the file)
void login_admin(int id);

// Logs out the admin (sets the logged_in flag to 0 and updates the file)
int  logout_admin(int id);

int admin_status(int id);
int change_admin_password(int admin_id,const char *current_password,const char *password);

#endif // ADMIN_MODEL_H
