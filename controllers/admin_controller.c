// #include <stdio.h>
// #include <string.h>
// #include "admin_controller.h"
// #include "../models/admin_model.h"
// #include "../models/bank_model.h"
// #include "../models/manager_model.h"

// void handle_admin(int client_socket) {
//     char buffer[1024] = {0};
//     int valread;
//     //send(client_socket, "Welcome Admin , please login\n", strlen("Welcome Admin , please login\n"), 0);
//     int r = handle_admin_session(client_socket);
//     // Main loop for handling admin session
// while (r != 1) {
//     printf("Debug: wasn't logged in, retrying...\n");
//     fflush(stdout); // Ensure the debug message is printed immediately
    
//     r = handle_admin_session(client_socket);
//     printf("Debug: handle_admin_session returned %d\n", r);
//     fflush(stdout); // Ensure the return value is printed immediately
// }

//     // Send a welcome message to the client
//    const char *welcome_message = 
// "\n Welcome Admin. Enter one of the following commands:\n"
// "1. Exit\n"
// "2. logout\n"
// "3. status\n"
// "4. Add New Bank Employee\n"
// "5. Modify Customer/Employee Details(for this just write 5)\n"
// "6. Manage Roles\n"
// "7. Change Password\n";
//     //send(client_socket, welcome_message, strlen(welcome_message), 0);

//     while (1) {
// 	 send(client_socket, welcome_message, strlen(welcome_message), 0);

//         memset(buffer, 0, sizeof(buffer));

//         // Read client input
//         valread = read(client_socket, buffer, 1024);
//         if (valread <= 0) {
//             printf("Client disconnected.\n");
//             close(client_socket);
//             break;
//         }

//         // Determine which command the admin sent
//         if (strncmp(buffer, "login", 5) == 0) {
//             handle_admin_session(client_socket);  // Call login function
//         } 
//         else if (strncmp(buffer, "logout", 6) == 0) {
//             logout(client_socket);  // Call logout function
//         } 
//         else if (strncmp(buffer, "status", 6) == 0) {
//             check_admin_status(client_socket);  // Example function to check session status
//         } 
// 	else if (strncmp(buffer, "exit", 4) == 0) {
//             logout(client_socket); 
//             printf("returned after logout\n");
//             sleep(1);  
//             send(client_socket, "goodbye\n", strlen("goodbye\n"), 0);
//             sleep(1);  
// 		close(client_socket);

//     // Print a message on the server side for debugging
//     printf("Client disconnected after sending 'exit' command.\n");

//     // Exit the thread by returning NULL, effectively ending the client handler
//     pthread_exit(NULL);
//         } 


//  else if (strncmp(buffer, "change password", 15) == 0) {
//             change_password(client_socket);  // Call logout function
//         }
//  else if (strncmp(buffer, "add new bank employee", 21) == 0) {
//             bank_employee_addition(client_socket);  // Example function to check session status
//         } 
// else if (strncmp(buffer, "5", 1) == 0) {
//             bank_employee_modify(client_socket);  // Example function to check session status
//         } 
// else if (strncmp(buffer, "manage roles", 12) == 0) {
//             manager_addition(client_socket);  // Example function to check session status
//         } 
 
 

//        else {
//             const char *invalid_command = "Invalid command.\n";
//             send(client_socket, invalid_command, strlen(invalid_command), 0);
//         }
	
//     }
// }



// int handle_admin_session(int client_socket) {
//     printf("admin session entry\n");
//     fflush(stdout);
//     char username[50]="", password[50]="";
//     int admin_id;

//     send(client_socket, "Welcome Admin , please login\nEnter Username: ", strlen("Welcome Admin , please login\nEnter Username: "), 0);
//     read(client_socket, username, 50);
// 	 username[strcspn(username, "\n")] = 0;
    
//     // Print received username for debugging
//     printf("Received Username: %s\n", username);
//     // Send request for password
//     send(client_socket, "Enter Password: ", strlen("Enter Password: "), 0);
//     read(client_socket, password, 50);

// 	password[strcspn(password, "\n")] = 0;

//     // Print received password for debugging
//     printf("Received Password: %s\n", password);

//     // Validate credentials
//     admin_id = validate_admin_credentials(username, password);

//     if (admin_id == -1) {
//        send(client_socket, "Invalid credentials.Press Enter to continue\n", strlen("Invalid credentials.Press Enter to continue\n"), 0);
//         return -1;
//     } else if (admin_id == -2) {
// send(client_socket, "Already logged in.Only one session per admin is allowed.Press Enter to continue\n", strlen("Already logged in .Only one session per admin is allowed.Press Enter to continue\n"), 0);       
//   // Wait for the client to press Enter to continue
//         // char enter_buffer[10];
//         // read(client_socket, enter_buffer, sizeof(enter_buffer));
 
//  return -1;
//     } else {
//         // Log in the admin
//         login_admin(admin_id);
//         send(client_socket, "Login successful.Press Enter to continue\n", strlen("Login successful.Press Enter to continue\n"), 0);
//         return 1;
 
//     }
// }
     

// void logout(int client_socket) {
//     char buffer[50];  // Buffer to read the ID
//     int admin_id;
    
//     // Prompt for admin ID
//     send(client_socket, "Enter ID: ", strlen("Enter ID: "), 0);
//     printf("hello\n");
//     // Read admin ID from the client
//     ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
//     if (bytes_read < 0) {
//         perror("Failed to read from client");
//         return; // Handle the error
//     }

//     // Null-terminate the string
//     buffer[bytes_read] = '\0';

//     // Convert the buffer to an integer
//     admin_id = atoi(buffer);
// int rep = admin_status(admin_id);
// if(rep==1){
//     // Call the function to log out the admin
//    int res =  logout_admin(admin_id);
// 	printf("%d",res);
// fflush(stdout);
// if(res==-1){
// send(client_socket,"you were already logged out.\n",strlen("you were already logged out.\n"));
// }
//   else{  // Send confirmation message to the client
//     send(client_socket, "You have been logged out.Press Enter to continue\n", strlen("You have been logged out.Press Enter to continue\n"), 0);
// }
// // }else{
// // send(client_socket,"you were already logged out.Press Enter to continue\n",strlen("you were already logged outPress Enter to continue\n"));

// }
// }

// void check_admin_status(int client_socket) {
//     char buffer[50];  // Buffer to read the ID
//     int admin_id;

//     // Prompt for admin ID
//     send(client_socket, "Enter ID: ", strlen("Enter ID: "), 0);
    
//     // Read admin ID from the client
//     ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
//     if (bytes_read < 0) {
//         perror("Failed to read from client");
//         return; // Handle the error
//     }

//     // Null-terminate the string
//     buffer[bytes_read] = '\0';

//     // Convert the buffer to an integer
//     admin_id = atoi(buffer);
// 	int res = admin_status(admin_id);
// 	if(res==1){
// 	send(client_socket, "Session in progress.\n", strlen("Session in progress.\n"), 0);

// 		}
// 	if(res==-1){
// 	send(client_socket, "Sesssion Invalid.\n", strlen("Session Invalid.\n"), 0);

//        }

// }

// void change_password(client_socket){
// char buffer[50];  // Buffer to read the ID
//     int admin_id;

// // Prompt for admin ID
//     send(client_socket, "Enter ID: ", strlen("Enter ID: "), 0);
    
//     // Read admin ID from the client
//     ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
//     if (bytes_read < 0) {
//         perror("Failed to read from client");
//         return; // Handle the error
//     }

//     // Null-terminate the string
//     buffer[bytes_read] = '\0';

//     // Convert the buffer to an integer
//     admin_id = atoi(buffer);

// char current_password[50], password[50];

// // Send request for password
//     send(client_socket, "Enter current Password: ", strlen("Enter current Password: "), 0);
//     read(client_socket, current_password, 50);

// 	password[strcspn(current_password, "\n")] = 0;
// // Send request for password
//     send(client_socket, "Enter new Password: ", strlen("Enter new Password: "), 0);
//     read(client_socket, password, 50);

// 	password[strcspn(password, "\n")] = 0;

// int res =  change_admin_password(admin_id,current_password,password);
// if(res==1){
// send(client_socket, "Password changed successfully.\n", strlen("Password changed successfully.\n"), 0);
// }else if (res ==-1){
// send(client_socket, "current password is wrong.\n", strlen("current password is wrong.\n"), 0);
// }

// else{
// send(client_socket, "Admin ID is wrong.\n", strlen("Admin ID is wrong.\n"), 0);
// }

// }


// void bank_employee_addition(int client_socket){

//     char buffer[50]="";  // Buffer to read the ID
//     int admin_id;

//     // Prompt for admin ID
//     send(client_socket, "Enter your admin ID: ", strlen("Enter your admin ID: "), 0);

//     // Read admin ID from the client
//     ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
//     if (bytes_read < 0) {
//         perror("Failed to read from client");
//         return; // Handle the error
//     }

//     // Null-terminate the string
//     buffer[bytes_read] = '\0';

//     // Convert the buffer to an integer
//     admin_id = atoi(buffer);
//     int res = admin_status(admin_id);


//     if(res==1){


//     char username1[50]="", password1[50]="";
//     send(client_socket, "Enter Username: ", strlen("Enter Username: "), 0);
//     read(client_socket, username1, 50);
//         username1[strcspn(username1, "\n")] = 0;

//     // Print received username for debugging
//     printf("Received Username: %s\n", username1);
//     // Send request for password
//     send(client_socket, "Enter Password: ", strlen("Enter Password: "), 0);
//     read(client_socket, password1, 50);

//     password1[strcspn(password1, "\n")] = 0;

//     // Print received password for debugging
//     printf("Received Password: %s\n", password1);
//     add_employee(username1,password1);

//     send(client_socket, "User has been added ", strlen("User has been added"), 0);
//     }
//     else{
//     send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

//     }

//     }

// void bank_employee_modify(int client_socket){

//     char buffer[50];  
//     int admin_id;

//     // Prompt for admin ID
//     send(client_socket, "Enter your admin ID: ", strlen("Enter your admin ID: "), 0);

//     // Read admin ID from the client
//     ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
//     if (bytes_read < 0) {
//         perror("Failed to read from client");
//         return; // Handle the error
//     }

//     // Null-terminate the string
//     buffer[bytes_read] = '\0';

//     // Convert the buffer to an integer
//     admin_id = atoi(buffer);
//     int res = admin_status(admin_id);


//     if(res==1){
//         char buff[1024] = {0};
//         memset(buffer, 0, sizeof(buffer));
        
        
//     send(client_socket, "Modify details of customer or employee", strlen("Modify details of customer or employee"), 0);
//     int valread = read(client_socket, buffer, 1024);
//     if (valread <= 0) {
//             printf("Client disconnected.\n");
//             close(client_socket);
            
//         }
//     if (strncmp(buffer, "employee", 5) == 0) {
    
//         int bank_id;

//         // Prompt for admin ID
//         send(client_socket, "Enter ID of employee :  ", strlen("Enter ID of employee :  "), 0);

//         // Read admin ID from the client
//         ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
//         if (bytes_read < 0) {
//             perror("Failed to read from client");
//             return; // Handle the error
//         }

//         // Null-terminate the string
//         buffer[bytes_read] = '\0';

//         // Convert the buffer to an integer
//         bank_id = atoi(buffer);




//         char new_username1[50]="", new_password1[50]="";
//         send(client_socket, "Enter Username: ", strlen("Enter Username: "), 0);
//         read(client_socket, new_username1, 50);
//         new_username1[strcspn(new_username1, "\n")] = 0;

//         // Print received username for debugging
//         printf("Received Username: %s\n", new_username1);
//         // Send request for password
//         send(client_socket, "Enter Password: ", strlen("Enter Password: "), 0);
//         read(client_socket, new_password1, 50);

//         new_password1[strcspn(new_password1, "\n")] = 0;

//         // Print received password for debugging
//         printf("Received Password: %s\n", new_password1);
//         modify_employee(bank_id,new_username1,new_password1);

//         send(client_socket, "User has been modified ", strlen("User has been modified"), 0);
//     }else{

//          int cust_id;

//     // Prompt for admin ID
//     send(client_socket, "Enter customer ID: ", strlen("Enter customer ID: "), 0);

//     // Read admin ID from the client
//     ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
//     if (bytes_read < 0) {
//         perror("Failed to read from client");
//         return; // Handle the error
//     }

//     // Null-terminate the string
//     buffer[bytes_read] = '\0';

//     // Convert the buffer to an integer
//     cust_id = atoi(buffer);



//     send(client_socket, "Enter customer name: ", strlen("Enter customer name: "), 0);
//     ssize_t name_len = read(client_socket, buffer, sizeof(buffer) - 1);
//     if (name_len < 0) {
//         perror("Error reading customer name");
//         return;
//     }
//     buffer[name_len] = '\0';  // Null-terminate the string
//     char customer_name[50]="";
//     strncpy(customer_name, buffer, sizeof(customer_name) - 1);

//     send(client_socket, "Enter customer password: ", strlen("Enter customer password: "), 0);
//     ssize_t pass_len = read(client_socket, buffer, sizeof(buffer) - 1);
//     if (pass_len < 0) {
//         perror("Error reading customer password");
//         return;
//     }
//     buffer[pass_len] = '\0';  // Null-terminate the string
//     char customer_password[50]="";
//     strncpy(customer_password, buffer, sizeof(customer_password) - 1);

//     send(client_socket, "Enter new balance: ", strlen("Enter new balance: "), 0);
//     ssize_t balance_len = read(client_socket, buffer, sizeof(buffer) - 1);
//     if (balance_len < 0) {
//         perror("Error reading customer balance");
//         return;
//     }
//     buffer[balance_len] = '\0';  // Null-terminate the string
//     printf("Buffer content before conversion: %s\n", buffer);
//     int customer_balance = atoi(buffer);  // Convert string to double
//     printf("recieved balance %d\n",customer_balance);

//     // Call the model function to add the customer
//     modify_customer(cust_id,customer_name, customer_password, customer_balance);

//     // Notify the client that the customer has been added
//     char success_message[100];
//     snprintf(success_message, sizeof(success_message), "Customer '%s' added successfully with  balance of %d\n", customer_name, customer_balance);
//     send(client_socket, success_message, strlen(success_message), 0);





//     }
//     }
//     else{
//     send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

//     }

// }

// void manager_addition(int client_socket){

//     char buffer[50]="";  // Buffer to read the ID
//     int admin_id;

//     // Prompt for admin ID
//     send(client_socket, "Enter your admin ID: ", strlen("Enter your admin ID: "), 0);

//     // Read admin ID from the client
//     ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
//     if (bytes_read < 0) {
//         perror("Failed to read from client");
//         return; // Handle the error
//     }

//     // Null-terminate the string
//     buffer[bytes_read] = '\0';

//     // Convert the buffer to an integer
//     admin_id = atoi(buffer);
//     int res = admin_status(admin_id);


//     if(res==1){

//         int bank_id;

//     // Prompt for admin ID
//     send(client_socket, "Enter bank employee ID: ", strlen("Enter bank employee ID: "), 0);

//     // Read admin ID from the client
//     ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
//     if (bytes_read < 0) {
//         perror("Failed to read from client");
//         return; // Handle the error
//     }

//     // Null-terminate the string
//     buffer[bytes_read] = '\0';

//     // Convert the buffer to an integer
//     bank_id = atoi(buffer);
    
//     char username1[50]="", password1[50]="";
//     send(client_socket, "Enter Manager Username: ", strlen("Enter Manager Username: "), 0);
//     read(client_socket, username1, 50);
//         username1[strcspn(username1, "\n")] = 0;

//     // Print received username for debugging
//     printf("Received Username: %s\n", username1);
//     // Send request for password
//     send(client_socket, "Enter Manager Password: ", strlen("Enter Manager Password: "), 0);
//     read(client_socket, password1, 50);

//     password1[strcspn(password1, "\n")] = 0;

//     // Print received password for debugging
//     printf("Received Password: %s\n", password1);
//     add_manager(username1,password1,bank_id);

//     send(client_socket, "User has been added ", strlen("User has been added"), 0);
//     }
//     else{
//     send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

//     }

//     }


#include <stdio.h>
#include <string.h>
#include "admin_controller.h"
#include "../models/admin_model.h"
#include "../models/bank_model.h"
#include "../models/manager_model.h"

void handle_admin(int client_socket) {
    char buffer[1024] = {0};
    int valread;
    
   const char *welcome_message = 
"\n Welcome Admin. Enter one of the following commands:\n"
"1. login\n"
"2. logout\n"
"3. status\n"
"4. Add New Bank Employee\n"
"5. Modify Customer/Employee Details(for this just write 5)\n"
"6. Manage Roles\n"
"7. Change Password\n"
"8. Exit\n";
    //send(client_socket, welcome_message, strlen(welcome_message), 0);

    while (1) {
	 send(client_socket, welcome_message, strlen(welcome_message), 0);

        memset(buffer, 0, sizeof(buffer));

       
        valread = read(client_socket, buffer, 1024);
        if (valread <= 0) {
            printf("Client disconnected.\n");
            close(client_socket);
            break;
        }

        if (strncmp(buffer, "login", 5) == 0) {
            handle_admin_session(client_socket);  
            printf("returned from admin session\n");
            fflush(stdout);
        } 
        else if (strncmp(buffer, "logout", 6) == 0) {
            logout(client_socket);  
        } 
        else if (strncmp(buffer, "status", 6) == 0) {
            check_admin_status(client_socket);  
        } 
	else if (strncmp(buffer, "exit", 4) == 0) {
            send(client_socket, "goodbye\n", strlen("goodbye\n"), 0);

		close(client_socket);

    printf("Client disconnected after sending 'exit' command.\n");

    pthread_exit(NULL);
        } 


 else if (strncmp(buffer, "change password", 15) == 0) {
            change_password(client_socket);  
        }
 else if (strncmp(buffer, "add new bank employee", 21) == 0) {
            bank_employee_addition(client_socket);  
        } 
else if (strncmp(buffer, "5", 1) == 0) {
            bank_employee_modify(client_socket);  
        } 
else if (strncmp(buffer, "manage roles", 12) == 0) {
            manager_addition(client_socket);  
        } 
 
 

       else {
            const char *invalid_command = "Invalid command.\n";
            send(client_socket, invalid_command, strlen(invalid_command), 0);
        }
	
    }
}



void handle_admin_session(int client_socket) {
    char username[50]="", password[50]="";
    int admin_id;

    send(client_socket, "Enter Username: ", strlen("Enter Username: "), 0);
    read(client_socket, username, 50);
	 username[strcspn(username, "\n")] = 0;
    
    printf("Received Username: %s\n", username);
    send(client_socket, "Enter Password: ", strlen("Enter Password: "), 0);
    read(client_socket, password, 50);

	password[strcspn(password, "\n")] = 0;

    printf("Received Password: %s\n", password);

    admin_id = validate_admin_credentials(username, password);

    if (admin_id == -1) {
       send(client_socket, "Invalid credentials.\n", strlen("Invalid credentials.\n"), 0);
    } else if (admin_id == -2) {
send(client_socket, "Already logged in.Only one session per admin is allowed.Enter ok to continue\n", strlen("Already logged in .Only one session per admin is allowed.Enter ok to continue\n"), 0);       
 
    } else {
        login_admin(admin_id);
        send(client_socket, "Login successful.\n", strlen("Login successful.\n"), 0);

 
    }
}
     

void logout(int client_socket) {
    char buffer[50];  
    int admin_id;

    send(client_socket, "Enter ID: ", strlen("Enter ID: "), 0);
    
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return;
    }

    buffer[bytes_read] = '\0';

    admin_id = atoi(buffer);

   
   int res =  logout_admin(admin_id);
	printf("%d",res);
fflush(stdout);
if(res==-1){
send(client_socket,"you were already logged out\n",strlen("you were already logged out\n"));
}
  else{  
    send(client_socket, "You have been logged out.\n", strlen("You have been logged out.\n"), 0);
}
}

void check_admin_status(int client_socket) {
    char buffer[50];  
    int admin_id;

    send(client_socket, "Enter ID: ", strlen("Enter ID: "), 0);
    
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; 
    }

    buffer[bytes_read] = '\0';

    admin_id = atoi(buffer);
	int res = admin_status(admin_id);
	if(res==1){
	send(client_socket, "Session in progress.\n", strlen("Session in progress.\n"), 0);

		}
	if(res==-1){
	send(client_socket, "Sesssion Invalid.\n", strlen("Session Invalid.\n"), 0);

       }

}

void change_password(client_socket){
char buffer[50];  
    int admin_id;

    send(client_socket, "Enter ID: ", strlen("Enter ID: "), 0);
    
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; 
    }

    buffer[bytes_read] = '\0';

    admin_id = atoi(buffer);

char current_password[50], password[50];

    send(client_socket, "Enter current Password: ", strlen("Enter current Password: "), 0);
    read(client_socket, current_password, 50);

	password[strcspn(current_password, "\n")] = 0;
    send(client_socket, "Enter new Password: ", strlen("Enter new Password: "), 0);
    read(client_socket, password, 50);

	password[strcspn(password, "\n")] = 0;

int res =  change_admin_password(admin_id,current_password,password);
if(res==1){
send(client_socket, "Password changed successfully.\n", strlen("Password changed successfully.\n"), 0);
}else if (res ==-1){
send(client_socket, "current password is wrong.\n", strlen("current password is wrong.\n"), 0);
}

else{
send(client_socket, "Admin ID is wrong.\n", strlen("Admin ID is wrong.\n"), 0);
}

}


void bank_employee_addition(int client_socket){

    char buffer[50]="";  
    int admin_id;

    send(client_socket, "Enter your admin ID: ", strlen("Enter your admin ID: "), 0);

    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; 
    }

    buffer[bytes_read] = '\0';

    admin_id = atoi(buffer);
    int res = admin_status(admin_id);


    if(res==1){


    char username1[50]="", password1[50]="";
    send(client_socket, "Enter Username: ", strlen("Enter Username: "), 0);
    read(client_socket, username1, 50);
        username1[strcspn(username1, "\n")] = 0;

    printf("Received Username: %s\n", username1);
    send(client_socket, "Enter Password: ", strlen("Enter Password: "), 0);
    read(client_socket, password1, 50);

    password1[strcspn(password1, "\n")] = 0;

    printf("Received Password: %s\n", password1);
    add_employee(username1,password1);

    send(client_socket, "User has been added ", strlen("User has been added"), 0);
    }
    else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }

    }

void bank_employee_modify(int client_socket){

    char buffer[50];  
    int admin_id;

    
    send(client_socket, "Enter your admin ID: ", strlen("Enter your admin ID: "), 0);

   
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; 
    }

    
    buffer[bytes_read] = '\0';

   
    admin_id = atoi(buffer);
    int res = admin_status(admin_id);


    if(res==1){
        char buff[1024] = {0};
        memset(buffer, 0, sizeof(buffer));
        
        
    send(client_socket, "Modify details of customer or employee", strlen("Modify details of customer or employee"), 0);
    int valread = read(client_socket, buffer, 1024);
    if (valread <= 0) {
            printf("Client disconnected.\n");
            close(client_socket);
            
        }
    if (strncmp(buffer, "employee", 5) == 0) {
    
        int bank_id;

      
        send(client_socket, "Enter ID of employee :  ", strlen("Enter ID of employee :  "), 0);

       
        ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            perror("Failed to read from client");
            return; 
        }

        
        buffer[bytes_read] = '\0';

        
        bank_id = atoi(buffer);




        char new_username1[50]="", new_password1[50]="";
        send(client_socket, "Enter Username: ", strlen("Enter Username: "), 0);
        read(client_socket, new_username1, 50);
        new_username1[strcspn(new_username1, "\n")] = 0;

      
        printf("Received Username: %s\n", new_username1);
       
        send(client_socket, "Enter Password: ", strlen("Enter Password: "), 0);
        read(client_socket, new_password1, 50);

        new_password1[strcspn(new_password1, "\n")] = 0;

       
        printf("Received Password: %s\n", new_password1);
        modify_employee(bank_id,new_username1,new_password1);

        send(client_socket, "User has been modified ", strlen("User has been modified"), 0);
    }else{

         int cust_id;

    
    send(client_socket, "Enter customer ID: ", strlen("Enter customer ID: "), 0);

  
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; 
    }

   
    buffer[bytes_read] = '\0';

    
    cust_id = atoi(buffer);



    send(client_socket, "Enter customer name: ", strlen("Enter customer name: "), 0);
    ssize_t name_len = read(client_socket, buffer, sizeof(buffer) - 1);
    if (name_len < 0) {
        perror("Error reading customer name");
        return;
    }
    buffer[name_len] = '\0';  
    char customer_name[50]="";
    strncpy(customer_name, buffer, sizeof(customer_name) - 1);

    send(client_socket, "Enter customer password: ", strlen("Enter customer password: "), 0);
    ssize_t pass_len = read(client_socket, buffer, sizeof(buffer) - 1);
    if (pass_len < 0) {
        perror("Error reading customer password");
        return;
    }
    buffer[pass_len] = '\0';  
    char customer_password[50]="";
    strncpy(customer_password, buffer, sizeof(customer_password) - 1);

    send(client_socket, "Enter new balance: ", strlen("Enter new balance: "), 0);
    ssize_t balance_len = read(client_socket, buffer, sizeof(buffer) - 1);
    if (balance_len < 0) {
        perror("Error reading customer balance");
        return;
    }
    buffer[balance_len] = '\0';  
    printf("Buffer content before conversion: %s\n", buffer);
    int customer_balance = atoi(buffer);  
    printf("recieved balance %d\n",customer_balance);

    modify_customer(cust_id,customer_name, customer_password, customer_balance);

    char success_message[100];
    snprintf(success_message, sizeof(success_message), "Customer '%s' added successfully with  balance of %d\n", customer_name, customer_balance);
    send(client_socket, success_message, strlen(success_message), 0);





    }
    }
    else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }

}

void manager_addition(int client_socket){

    char buffer[50]="";  
    int admin_id;

    send(client_socket, "Enter your admin ID: ", strlen("Enter your admin ID: "), 0);

    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; 
    }

    buffer[bytes_read] = '\0';

    admin_id = atoi(buffer);
    int res = admin_status(admin_id);


    if(res==1){

        int bank_id;

    send(client_socket, "Enter bank employee ID: ", strlen("Enter bank employee ID: "), 0);

    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; 
    }

    buffer[bytes_read] = '\0';

    bank_id = atoi(buffer);
    
    char username1[50]="", password1[50]="";
    send(client_socket, "Enter Manager Username: ", strlen("Enter Manager Username: "), 0);
    read(client_socket, username1, 50);
        username1[strcspn(username1, "\n")] = 0;

    printf("Received Username: %s\n", username1);
    send(client_socket, "Enter Manager Password: ", strlen("Enter Manager Password: "), 0);
    read(client_socket, password1, 50);

    password1[strcspn(password1, "\n")] = 0;

    printf("Received Password: %s\n", password1);
    add_manager(username1,password1,bank_id);

    send(client_socket, "User has been added ", strlen("User has been added"), 0);
    }
    else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }

    }
