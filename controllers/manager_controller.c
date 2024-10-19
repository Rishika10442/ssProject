#include <stdio.h>
#include <string.h>
#include "manager_controller.h"
#include "../models/admin_model.h"
#include "../models/bank_model.h"
#include "../models/manager_model.h"
#include "../models/customer_model.h"
#include "../models/transaction_model.h"
#include "../models/loan_model.h"
#include "../models/feedback_model.h"


void handle_manager(int client_socket) {
    char buffer[1024] = {0};
    int valread;
    
    // Send a welcome message to the client
   const char *welcome_message = 
    "\n Welcome Manager. Enter one of the following commands:\n"
    "1. login\n"
    "2. logout\n"
    "3. status\n"
    "4. Activate/Deactivate customer accounts(press 4)\n"
    "5. Assign loan applications to employees (press 5)\n"
    "6. review customer feedback( press 6)\n"
    "7. change password\n"
    "8. view assigned aplications(press 8)\n"
    "9. view all feedbacks(press 9)\n"
    "10. Exit \n";

    while (1) {
        send(client_socket, welcome_message, strlen(welcome_message), 0);

            memset(buffer, 0, sizeof(buffer));

            // Read client input
            valread = read(client_socket, buffer, 1024);
            if (valread <= 0) {
                printf("Client disconnected.\n");
                close(client_socket);
                break;
            }

            // Determine which command the admin sent
            if (strncmp(buffer, "login", 5) == 0) {
                handle_manager_session(client_socket);  // Call login function
            }
            else if (strncmp(buffer, "logout", 6) == 0) {
                manager_logout(client_socket);  // Call login function
            }else if (strncmp(buffer, "status", 6) == 0) {
                check_manager_status(client_socket);  // Call login function
            }else if (strncmp(buffer, "exit", 4) == 0) {
            send(client_socket, "goodbye\n", strlen("goodbye\n"), 0);

                close(client_socket);

            // Print a message on the server side for debugging
            printf("Client disconnected after sending 'exit' command.\n");

            // Exit the thread by returning NULL, effectively ending the client handler
            pthread_exit(NULL);
        }else if (strncmp(buffer, "change password", 15) == 0) {
                change_passwordManager(client_socket);  // Call login function
            }else if (strncmp(buffer, "4", 1) == 0) {
                change_customerActivation(client_socket);  // Call login function
            }
            else if (strncmp(buffer, "5", 1) == 0) {
                assign_applicationToEmployee(client_socket);  // Call login function
            }else if (strncmp(buffer, "8", 1) == 0) {
                view_appManager(client_socket);  // Call login function
            }else if (strncmp(buffer, "6", 1) == 0) {
                review_feedback(client_socket);  // Call login function
            }else if (strncmp(buffer, "9", 1) == 0) {
                feedback_historyManager(client_socket);  // Call login function
            }
        else {
            const char *invalid_command = "Invalid command.\n";
            send(client_socket, invalid_command, strlen(invalid_command), 0);
        }
    }


}


void handle_manager_session(int client_socket) {
    char username[50]="", password[50]="";
    int man_id;

    send(client_socket, "Enter Username: ", strlen("Enter Username: "), 0);
    read(client_socket, username, 50);
	 username[strcspn(username, "\n")] = 0;
    
    // Print received username for debugging
    printf("Received Username: %s\n", username);
    // Send request for password
    send(client_socket, "Enter Password: ", strlen("Enter Password: "), 0);
    read(client_socket, password, 50);

	password[strcspn(password, "\n")] = 0;

    // Print received password for debugging
    printf("Received Password: %s\n", password);

    // Validate credentials
    man_id = validate_manager_credentials(username, password);

    if (man_id == -1) {
       send(client_socket, "Invalid credentials.\n", strlen("Invalid credentials.\n"), 0);
    } else if (man_id == -2) {
send(client_socket, "Already logged in.Only one session per admin is allowed\n", strlen("Already logged in .Only one session per admin is allowed\n"), 0);       
 
    }else {
      
        
        send(client_socket, "Login successful.\n", strlen("Login successful.\n"), 0);

 
    }
}


void manager_logout(int client_socket) {
    char buffer[50];  // Buffer to read the ID
    int man_id;

    // Prompt for admin ID
    send(client_socket, "Enter ID: ", strlen("Enter ID: "), 0);
    
    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    // Convert the buffer to an integer
    man_id = atoi(buffer);

    // Call the function to log out the admin
   int res =  logout_manager(man_id);
	printf("%d",res);
fflush(stdout);
if(res==-1){
send(client_socket,"you were already logged out\n",strlen("you were already logged out\n"));
}
  else{  // Send confirmation message to the client
    send(client_socket, "You have been logged out.\n", strlen("You have been logged out.\n"), 0);
}
}

void check_manager_status(int client_socket) {
    char buffer[50];  // Buffer to read the ID
    int man_id;

    // Prompt for admin ID
    send(client_socket, "Enter ID: ", strlen("Enter ID: "), 0);
    
    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    // Convert the buffer to an integer
    man_id = atoi(buffer);
	int res = manager_status(man_id);
	if(res==1){
	send(client_socket, "Session in progress.\n", strlen("Session in progress.\n"), 0);

		}
	if(res==-1){
	send(client_socket, "Sesssion Invalid.\n", strlen("Session Invalid.\n"), 0);

       }

}

void change_passwordManager(client_socket){
char buffer[50];  // Buffer to read the ID
    int man_id;

// Prompt for admin ID
    send(client_socket, "Enter ID: ", strlen("Enter ID: "), 0);
    
    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    // Convert the buffer to an integer
    man_id = atoi(buffer);

char current_password[50]="", password[50]="";

// Send request for password
    send(client_socket, "Enter current Password: ", strlen("Enter current Password: "), 0);
    read(client_socket, current_password, 50);

	password[strcspn(current_password, "\n")] = 0;
// Send request for password
    send(client_socket, "Enter new Password: ", strlen("Enter new Password: "), 0);
    read(client_socket, password, 50);

	password[strcspn(password, "\n")] = 0;

int res =  change_manager_password(man_id,current_password,password);
if(res==1){
send(client_socket, "Password changed successfully.\n", strlen("Password changed successfully.\n"), 0);
}else if (res ==-1){
send(client_socket, "current password is wrong.\n", strlen("current password is wrong.\n"), 0);
}

else{
send(client_socket, "Manager ID is wrong.\n", strlen("Manager ID is wrong.\n"), 0);
}

}

void change_customerActivation(client_socket){

    char buffer[256]="";  // Buffer to read the ID
    int man_id;

    // Prompt for admin ID
    send(client_socket, "Enter your manager ID: ", strlen("Enter your manager ID: "), 0);

    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    // Convert the buffer to an integer
    man_id = atoi(buffer);
    int res = manager_status(man_id);


    if(res==1){


        send(client_socket, "Enter customer ID: ", strlen("Enter customer ID: "), 0);
    ssize_t balance_len = read(client_socket, buffer, sizeof(buffer) - 1);
    if (balance_len < 0) {
        perror("eror in getting custID in actiavtion");
        return;
    }
    buffer[balance_len] = '\0';  // Null-terminate the string
    printf("Buffer content before conversion: %s\n", buffer);
    int custID = atoi(buffer);  // Convert string to double
    printf("CustID %d\n",custID);

     send(client_socket, "Enter 1 for Activate,0 for Deactivate: ", strlen("Enter 1 for Activate,2 for Deactivate: "), 0);
    ssize_t balance_len2 = read(client_socket, buffer, sizeof(buffer) - 1);
    if (balance_len2 < 0) {
        perror("eror in getting custID in actiavtion");
        return;
    }
    buffer[balance_len2] = '\0';  // Null-terminate the string
    printf("Buffer content before conversion: %s\n", buffer);
    int act = atoi(buffer);  // Convert string to double
    printf("Act %d\n",act);

    //
    int rep = activation_change(act,custID);
    if(rep==1){
        char success_message[100];
        snprintf(success_message, sizeof(success_message), "Succesfully changed activation\n");
        send(client_socket, success_message, strlen(success_message), 0);
    }
    }
    else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }

}

void assign_applicationToEmployee(client_socket){
 char buffer[256]="";  // Buffer to read the ID
    int man_id;

    // Prompt for admin ID
    send(client_socket, "Enter your manager ID: ", strlen("Enter your manager ID: "), 0);

    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    // Convert the buffer to an integer
    man_id = atoi(buffer);
    int res = manager_status(man_id);


    if(res==1){
    
            int loan_id;

            // Prompt for admin ID
            send(client_socket, "Enter loan ID: ", strlen("Enter loan ID: "), 0);

            // Read admin ID from the client
            ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
            if (bytes_read < 0) {
                perror("Failed to read from client");
                return; // Handle the error
            }

            // Null-terminate the string
            buffer[bytes_read] = '\0';

            // Convert the buffer to an integer
            loan_id = atoi(buffer);
            
            int emp_id;

            // Prompt for admin ID
            send(client_socket, "Enter employee ID: ", strlen("Enter employee ID: "), 0);

            // Read admin ID from the client
            ssize_t bytes_read2 = read(client_socket, buffer, sizeof(buffer) - 1);
            if (bytes_read2 < 0) {
                perror("Failed to read from client");
                return; // Handle the error
            }

            // Null-terminate the string
            buffer[bytes_read2] = '\0';

            // Convert the buffer to an integer
            emp_id = atoi(buffer);
        int rep = assignLoan_ToEmployee(loan_id,emp_id);
        if(res==0){
        send(client_socket, "Assigned successfully.\n", strlen("Password changed successfully.\n"), 0);
        }else if (res ==-1){
        send(client_socket, "something wrong in assigning.\n", strlen("current password is wrong.\n"), 0);
        }
            
    }
    else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }


}

void view_appManager(int client_socket){

    char buffer[256]="";  // Buffer to read the ID
    int man_id;

    // Prompt for admin ID
    send(client_socket, "Enter your manager ID: ", strlen("Enter your manager ID: "), 0);

    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    // Convert the buffer to an integer
    man_id = atoi(buffer);
    int res = manager_status(man_id);


    if(res==1){
        Loan loans[100];
        int num_loans = applications_historyManager(man_id, loans, 100);

        if (num_loans < 0) {
            send(client_socket, "Error: Could not retrieve loan history\n", 
                strlen("Error: Could not retrieve loan history\n"), 0);
            return;
        }

        if (num_loans == 0) {
            send(client_socket, "No loan applications found\n", 
                strlen("No loan applications found\n"), 0);
            return;
        }

        // Prepare the response to the client
        char buffer[1024];  // Adjust buffer size as needed
        memset(buffer, 0, sizeof(buffer));
        strcat(buffer, "Loan Applications History:\n");

        for (int i = 0; i < num_loans; i++) {
            char loan_info[256];
            // Example of how to format the loan information
snprintf(loan_info, sizeof(loan_info), 
         "Loan ID: %d, Desc: %s, Amount: %.2f, Status: %d, Manager ID: %d, Employee ID: %d, Customer ID: %d\n", 
         loans[i].loanID, loans[i].desc, loans[i].amount, 
         loans[i].loanStatus, loans[i].manID, 
         loans[i].empID, loans[i].custID);
            strcat(buffer, loan_info);
        }

        // Send the response to the client
        send(client_socket, buffer, strlen(buffer), 0);
        
    }
    else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }

}

void review_feedback(client_socket){
     char buffer[256]="";  // Buffer to read the ID
    int man_id;

    // Prompt for admin ID
    send(client_socket, "Enter your manager ID: ", strlen("Enter your manager ID: "), 0);

    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    // Convert the buffer to an integer
    man_id = atoi(buffer);
    printf("mamager ID recieved %d\n",man_id);

    int res = manager_status(man_id);


     if(res==1){

        char buffer[256]="";  // Buffer to read the ID
    int feedID;

    // Prompt for admin ID
    send(client_socket, "Enter feedback ID: ", strlen("Enter feedback ID: "), 0);

    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    // Convert the buffer to an integer
    feedID = atoi(buffer);
    




            char subject[50]="";
        send(client_socket, "Enter feedback review: ", strlen("Enter feedback review: "), 0);
        read(client_socket, subject, 50);
        subject[strcspn(subject, "\n")] = 0;

        // Print received username for debugging
        printf("Received feedbak review: %s\n", subject);
        // Send request for password


    int rep = enter_feedbackReview(feedID,subject,man_id);
    if(rep==1){
        char success_message[100];
        snprintf(success_message, sizeof(success_message), "feedback added Successfully \n");
        send(client_socket, success_message, strlen(success_message), 0);
    }
    }
    else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }

}

void feedback_historyManager(int client_socket){

    char buffer2[256]="";  // Buffer to read the ID
    int man_id;

    // Prompt for admin ID
    send(client_socket, "Enter your manager ID: ", strlen("Enter your manager ID: "), 0);

    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer2, sizeof(buffer2) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer2[bytes_read] = '\0';

    // Convert the buffer to an integer
    man_id = atoi(buffer2);
    int res = manager_status(man_id);


    if(res==1){
        Feedback feedbacks[100];
    char buffer[1024]="";
    int feedback_count;

    // Get the feedback history for the customer
    feedback_count = feedback_history_by_manager(man_id, feedbacks, MAX_FEEDBACKS);
    
    if (feedback_count < 0) {
        // Send an error message to the client
        snprintf(buffer, sizeof(buffer), "Error: Unable to retrieve feedback history.\n");
        send(client_socket, buffer, strlen(buffer), 0);
        return;
    }

    if (feedback_count == 0) {
        // Send a message indicating no feedbacks found
        snprintf(buffer, sizeof(buffer), "No feedback found for manager ID %d.\n", man_id);
        send(client_socket, buffer, strlen(buffer), 0);
        return;
    }

    // Send the feedback data to the client
    for (int i = 0; i < feedback_count; i++) {
        char temp[256]="";
        snprintf(temp, sizeof(temp),
                 "Feedback ID: %d\nDescription: %s\nReview: %s\nManager ID: %d\n\n",
                 feedbacks[i].feedbackID, feedbacks[i].desc, feedbacks[i].review,
                 feedbacks[i].manID);
        strcat(buffer, temp);
    }


    send(client_socket, buffer, strlen(buffer), 0);
    
    }
    else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }

}