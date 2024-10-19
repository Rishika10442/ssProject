#include <stdio.h>
#include <string.h>
#include "admin_controller.h"
#include "../models/admin_model.h"
#include "../models/bank_model.h"
#include "../models/manager_model.h"
#include "../models/customer_model.h"
#include "../models/transaction_model.h"
#include "../models/loan_model.h"
#include "../models/feedback_model.h"


void handle_customer(int client_socket) {
    char buffer[1024] = {0};
    int valread;
    
    // Send a welcome message to the client
   const char *welcome_message = 
    "\n Welcome Customer. Enter one of the following commands:\n"
    "1. login\n"
    "2. logout\n"
    "3. status\n"
    "4. view account balance\n"
    "5. Deposit money\n"
    "6. Withraw money\n"
    "7. Transfer Funds\n"
    "8. Apply for a Loan\n"
    "9. change password\n"
    "10. Add Feedback\n"
    "11. View Transaction History(enter 11 for this)\n"
    "12. View your loan applications(enter 12)\n"
    "14. View feedbacks \n"
    "13. Exit \n";

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
                handle_customer_session(client_socket);  // Call login function
            }
            else if (strncmp(buffer, "logout", 6) == 0) {
                customer_logout(client_socket);  // Call login function
            }else if (strncmp(buffer, "status", 6) == 0) {
                check_customer_status(client_socket);  // Call login function
            }else if (strncmp(buffer, "exit", 4) == 0) {
            send(client_socket, "goodbye\n", strlen("goodbye\n"), 0);

                close(client_socket);

            // Print a message on the server side for debugging
            printf("Client disconnected after sending 'exit' command.\n");

            // Exit the thread by returning NULL, effectively ending the client handler
            pthread_exit(NULL);
        } 
        else if (strncmp(buffer, "view account balance", 20) == 0) {
                customer_balance(client_socket);  // Call login function
            }
            else if (strncmp(buffer, "change password", 15) == 0) {
                change_passwordCustomer(client_socket);  // Call login function
            }
            else if (strncmp(buffer, "deposit money", 13) == 0) {
                deposit(client_socket);  // Call login function
            }
             else if (strncmp(buffer, "withraw money", 13) == 0) {
                withraw(client_socket);  // Call login function
            }else if (strncmp(buffer, "transfer funds", 14) == 0) {
                transfer(client_socket);  // Call login function
            }else if (strncmp(buffer, "11", 2) == 0) {
                history(client_socket);  // Call login function
            }else if (strncmp(buffer, "apply for loan", 14) == 0) {
                application(client_socket);  // Call login function
            }else if (strncmp(buffer, "12", 2) == 0) {
                view_app(client_socket);  // Call login function
            }else if (strncmp(buffer, "add feedback", 12) == 0) {
                feedback_addition(client_socket);  // Call login function
            }else if (strncmp(buffer, "view feedbacks", 14) == 0) {
                feedback_viewCustomer(client_socket);  // Call login function
            }

            else {
            const char *invalid_command = "Invalid command.\n";
            send(client_socket, invalid_command, strlen(invalid_command), 0);
        }


    }
}


void handle_customer_session(int client_socket) {
    char username[50]="", password[50]="";
    int cust_id;

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
    cust_id = validate_customer_credentials(username, password);

    if (cust_id == -1) {
       send(client_socket, "Invalid credentials.\n", strlen("Invalid credentials.\n"), 0);
    } else if (cust_id == -2) {
send(client_socket, "Already logged in.Only one session per admin is allowed\n", strlen("Already logged in .Only one session per admin is allowed\n"), 0);       
 
    } else if(cust_id == -3){
        send(client_socket, "Your account is deactivated\n", strlen("Your account is deactivated\n"), 0);
    }else {
      
        
        send(client_socket, "Login successful.\n", strlen("Login successful.\n"), 0);

 
    }
}

void customer_logout(int client_socket) {
    char buffer[50];  // Buffer to read the ID
    int cust_id;

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
    cust_id = atoi(buffer);

    // Call the function to log out the admin
   int res =  logout_customer(cust_id);
	printf("%d",res);
fflush(stdout);
if(res==-1){
send(client_socket,"you were already logged out\n",strlen("you were already logged out\n"));
}
  else{  // Send confirmation message to the client
    send(client_socket, "You have been logged out.\n", strlen("You have been logged out.\n"), 0);
}
}

void check_customer_status(int client_socket) {
    char buffer[50];  // Buffer to read the ID
    int cust_id;

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
    cust_id = atoi(buffer);
	int res = customer_status(cust_id);
	if(res==1){
	send(client_socket, "Session in progress.\n", strlen("Session in progress.\n"), 0);

		}
	if(res==-1){
	send(client_socket, "Sesssion Invalid.\n", strlen("Session Invalid.\n"), 0);

       }

}

void customer_balance(int client_socket) {
    char buffer[50];  // Buffer to read the ID
    int cust_id;

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
    cust_id = atoi(buffer);

    // Call the function to log out the admin
   int res =  check_balance(cust_id);
	printf("%d",res);
fflush(stdout);

char message[256]=""; // Buffer to hold the response message
    int length = snprintf(message, sizeof(message), "Your account balance is %d\n", res);

    // Send the balance to the client
    if (send(client_socket, message, length, 0) == -1) {
        perror("Error sending balance");
    }

}

void change_passwordCustomer(client_socket){
char buffer[50];  // Buffer to read the ID
    int cust_id;

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
    cust_id = atoi(buffer);

char current_password[50]="", password[50]="";

// Send request for password
    send(client_socket, "Enter current Password: ", strlen("Enter current Password: "), 0);
    read(client_socket, current_password, 50);

	current_password[strcspn(current_password, "\n")] = 0;
// Send request for password
    send(client_socket, "Enter new Password: ", strlen("Enter new Password: "), 0);
    read(client_socket, password, 50);

	password[strcspn(password, "\n")] = 0;

int res =  change_customer_password(cust_id,current_password,password);
if(res==1){
send(client_socket, "Password changed successfully.\n", strlen("Password changed successfully.\n"), 0);
}else if (res ==-1){
send(client_socket, "current password is wrong.\n", strlen("current password is wrong.\n"), 0);
}

else{
send(client_socket, "Customer ID is wrong.\n", strlen("Customer ID is wrong.\n"), 0);
}

}

void deposit(int client_socket){

    char buffer[256]="";  // Buffer to read the ID
    int cust_id;

    // Prompt for admin ID
    send(client_socket, "Enter your customer ID: ", strlen("Enter your customer ID: "), 0);

    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    // Convert the buffer to an integer
    cust_id = atoi(buffer);
    int res = customer_status(cust_id);


    if(res==1){
        send(client_socket, "Enter amount to be deposited: ", strlen("Enter amount to be deposited: "), 0);
    ssize_t balance_len = read(client_socket, buffer, sizeof(buffer) - 1);
    if (balance_len < 0) {
        perror("eror in getting money to deposit");
        return;
    }
    buffer[balance_len] = '\0';  // Null-terminate the string
    printf("Buffer content before conversion: %s\n", buffer);
    int money = atoi(buffer);  // Convert string to double
    printf("recieved money for deposit %d\n",money);
    //
    int rep = deposit_money(money,cust_id);
    if(rep==1){
        char success_message[100];
        snprintf(success_message, sizeof(success_message), "Money deposited to your account %d\n",money);
        send(client_socket, success_message, strlen(success_message), 0);
    }
    }
    else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }

    }

void withraw(int client_socket){

    char buffer[256]="";  // Buffer to read the ID
    int cust_id;

    // Prompt for admin ID
    send(client_socket, "Enter your customer ID: ", strlen("Enter your customer ID: "), 0);

    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    // Convert the buffer to an integer
    cust_id = atoi(buffer);
    int res = customer_status(cust_id);


    if(res==1){
        send(client_socket, "Enter amount to be withrawn: ", strlen("Enter amount to be withrrawn: "), 0);
    ssize_t balance_len = read(client_socket, buffer, sizeof(buffer) - 1);
    if (balance_len < 0) {
        perror("eror in getting money to withraw");
        return;
    }
    buffer[balance_len] = '\0';  // Null-terminate the string
    printf("Buffer content before conversion: %s\n", buffer);
    int money = atoi(buffer);  // Convert string to double
    printf("recieved money for withraw %d\n",money);
    //
    int rep = withraw_money(money,cust_id);
    if(rep==1){
        char success_message[100];
        snprintf(success_message, sizeof(success_message), "Money withrawn from  your account %d\n",money);
        send(client_socket, success_message, strlen(success_message), 0);
    }
    }
    else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }

    }

void transfer(int client_socket){

    char buffer[256]="";  // Buffer to read the ID
    int cust_id;

    // Prompt for admin ID
    send(client_socket, "Enter your customer ID: ", strlen("Enter your customer ID: "), 0);

    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    // Convert the buffer to an integer
    cust_id = atoi(buffer);
    int res = customer_status(cust_id);


    if(res==1){
        int cust_id2;

        // Prompt for admin ID
        send(client_socket, "Enter customer ID whom you want to send money: ", strlen("Enter customer ID whom you want to send money: "), 0);

        // Read admin ID from the client
        ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            perror("Failed to read from client");
            return; // Handle the error
        }

        // Null-terminate the string
        buffer[bytes_read] = '\0';

        // Convert the buffer to an integer
        cust_id2 = atoi(buffer);





        send(client_socket, "Enter amount: ", strlen("Enter amount: "), 0);
    ssize_t balance_len = read(client_socket, buffer, sizeof(buffer) - 1);
    if (balance_len < 0) {
        perror("eror in getting money to withraw");
        return;
    }
    buffer[balance_len] = '\0';  // Null-terminate the string
    printf("Buffer content before conversion: %s\n", buffer);
    int money = atoi(buffer);  // Convert string to double
    printf("rmoney to send %d\n",money);
    //
    int rep = transfer_funds(money,cust_id,cust_id2);
    if(rep==1){
        char success_message[100];
        snprintf(success_message, sizeof(success_message), "Money sent successfully %d\n",money);
        send(client_socket, success_message, strlen(success_message), 0);
    }
    if(rep==-2){
        char success_message[100];
        snprintf(success_message, sizeof(success_message), "customer 2 is deactivated\n");
        send(client_socket, success_message, strlen(success_message), 0);
    }
    }
    else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }

}

void history(int client_socket){

    char buffer[256]="";  // Buffer to read the ID
    int cust_id;

    // Prompt for admin ID
    send(client_socket, "Enter your customer ID: ", strlen("Enter your customer ID: "), 0);

    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    // Convert the buffer to an integer
    cust_id = atoi(buffer);
    int res = customer_status(cust_id);


    if(res==1){
       int num_transactions = 0;
        Transaction* transactions = get_transaction_history(cust_id, &num_transactions);

        if (num_transactions > 0) {
            char transaction_buffer[1024] = "";  // To hold the formatted transactions

            // Loop through all the retrieved transactions and format them
            for (int i = 0; i < num_transactions; i++) {
                char temp[256];
                snprintf(temp, sizeof(temp), "TransactionID: %d, Type: %s, From: %d, To: %d, Amount: %.2f\n",
                         transactions[i].transactionID,
                         transactions[i].transactionType,
                         transactions[i].fromId,
                         transactions[i].toId,
                         transactions[i].money);

                // Concatenate the transaction details to the buffer
                strcat(transaction_buffer, temp);
            }

            // Send the transaction history to the client
            send(client_socket, transaction_buffer, strlen(transaction_buffer), 0);

        } else if (num_transactions == 0) {
            send(client_socket, "No transactions found.\n", strlen("No transactions found.\n"), 0);
        } else {
            send(client_socket, "Error retrieving transactions.\n", strlen("Error retrieving transactions.\n"), 0);
        }

        // Free the dynamically allocated memory for transactions
        if (transactions != NULL) {
            free(transactions);
        }

    
    }
    else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }

}


void application(int client_socket){

    char buffer[256]="";  // Buffer to read the ID
    int cust_id;

    // Prompt for admin ID
    send(client_socket, "Enter your customer ID: ", strlen("Enter your customer ID: "), 0);

    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    // Convert the buffer to an integer
    cust_id = atoi(buffer);
    int res = customer_status(cust_id);


    if(res==1){
            char subject[50]="";
        send(client_socket, "Enter subject: ", strlen("Enter subject: "), 0);
        read(client_socket, subject, 50);
        subject[strcspn(subject, "\n")] = 0;

        // Print received username for debugging
        printf("Received subject: %s\n", subject);
        // Send request for password






        send(client_socket, "Enter amount required: ", strlen("Enter amount required: "), 0);
    ssize_t balance_len = read(client_socket, buffer, sizeof(buffer) - 1);
    if (balance_len < 0) {
        perror("eror in getting money to loan");
        return;
    }
    buffer[balance_len] = '\0';  // Null-terminate the string
    printf("Buffer content before conversion: %s\n", buffer);
    int money = atoi(buffer);  // Convert string to double
    printf("recieved money for loan %d\n",money);
    //
    int rep = apply_loan(subject,money,cust_id);
    if(rep==1){
        char success_message[100];
        snprintf(success_message, sizeof(success_message), "Loan Application Successful \n");
        send(client_socket, success_message, strlen(success_message), 0);
    }
    }
    else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }

}


void view_app(int client_socket){

    char buffer[256]="";  // Buffer to read the ID
    int cust_id;

    // Prompt for admin ID
    send(client_socket, "Enter your customer ID: ", strlen("Enter your customer ID: "), 0);

    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    // Convert the buffer to an integer
    cust_id = atoi(buffer);
    int res = customer_status(cust_id);


    if(res==1){
        Loan loans[100];
        int num_loans = applications_history(cust_id, loans, 100);

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
            snprintf(loan_info, sizeof(loan_info), 
                 "Loan ID: %d, Desc: %s, Amount: %.2f, Status: %d, Manager ID: %d, Employee ID: %d\n", 
                 loans[i].loanID, loans[i].desc, loans[i].amount, loans[i].loanStatus, 
                 loans[i].manID, loans[i].empID);
            strcat(buffer, loan_info);
        }

        // Send the response to the client
        send(client_socket, buffer, strlen(buffer), 0);
        
    }
    else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }

}

void feedback_addition(client_socket){
    char buffer[256]="";  // Buffer to read the ID
    int cust_id;

    // Prompt for admin ID
    send(client_socket, "Enter your customer ID: ", strlen("Enter your customer ID: "), 0);

    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    // Convert the buffer to an integer
    cust_id = atoi(buffer);
    int res = customer_status(cust_id);


     if(res==1){
            char subject[50]="";
        send(client_socket, "Enter feedback: ", strlen("Enter feedback: "), 0);
        read(client_socket, subject, 50);
        subject[strcspn(subject, "\n")] = 0;

        // Print received username for debugging
        printf("Received feedbak: %s\n", subject);
        // Send request for password


    int rep = enter_feedback(subject,cust_id);
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


void feedback_viewCustomer(int client_socket){

    char buffer2[256]="";  // Buffer to read the ID
    int cust_id;

    // Prompt for admin ID
    send(client_socket, "Enter your customer ID: ", strlen("Enter your customer ID: "), 0);

    // Read admin ID from the client
    ssize_t bytes_read = read(client_socket, buffer2, sizeof(buffer2) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; // Handle the error
    }

    // Null-terminate the string
    buffer2[bytes_read] = '\0';

    // Convert the buffer to an integer
    cust_id = atoi(buffer2);
    int res = customer_status(cust_id);


    if(res==1){
        Feedback feedbacks[100];
    char buffer[1024]="";
    int feedback_count;

    // Get the feedback history for the customer
    feedback_count = feedback_history(cust_id, feedbacks, MAX_FEEDBACKS);
    
    if (feedback_count < 0) {
        // Send an error message to the client
        snprintf(buffer, sizeof(buffer), "Error: Unable to retrieve feedback history.\n");
        send(client_socket, buffer, strlen(buffer), 0);
        return;
    }

    if (feedback_count == 0) {
        // Send a message indicating no feedbacks found
        snprintf(buffer, sizeof(buffer), "No feedback found for customer ID %d.\n", cust_id);
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