#include <stdio.h>
#include <string.h>
#include "admin_controller.h"
#include "../models/admin_model.h"
#include "../models/bank_model.h"
#include "../models/manager_model.h"
#include "../models/loan_model.h"
#include "../models/transaction_model.h"

void handle_employee(int client_socket) {
    char buffer[1024] = {0};
    int valread;
    
    
   const char *welcome_message = 
"\n Welcome Employee. Enter one of the following commands:\n"
"1. login\n"
"2. logout\n"
"3. status\n"
"4. Add New Customer\n"
"5. Modify Customer Details(for this just write 5)\n"
"6. Process Loan Application Approve/Reject (press 6)\n"
"7. View Assigned Loan Applications(press 7)\n"
"8. View Customer Transactions(press 8)\n"
"9. Change Password\n"
"10. Exit \n";

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
            handle_employee_session(client_socket);  
        } else if (strncmp(buffer, "logout", 6) == 0) {
            logout_employee(client_socket);  
        } 
        else if (strncmp(buffer, "status", 6) == 0) {
            check_employee_status(client_socket);  
        }else if (strncmp(buffer, "add new customer", 16) == 0) {
            new_customer(client_socket);  
        }
        else if (strncmp(buffer, "5", 1) == 0) {
            change_customer(client_socket);  
        }else if (strncmp(buffer, "6", 1) == 0) {
            process_application(client_socket);  
        }
        else if (strncmp(buffer, "exit", 4) == 0) {
            send(client_socket, "goodbye\n", strlen("goodbye\n"), 0);

		close(client_socket);

    
    printf("Client disconnected after sending 'exit' command.\n");

    
    pthread_exit(NULL);
        }
        else if (strncmp(buffer, "7", 1) == 0) {
            view_appEmployee(client_socket);  
        }else if (strncmp(buffer, "8", 1) == 0) {
            customer_transactions(client_socket);  
        }else if (strncmp(buffer, "change password", 15) == 0) {
                change_passwordEmployee(client_socket);  
            }

        else {
            const char *invalid_command = "Invalid command.\n";
            send(client_socket, invalid_command, strlen(invalid_command), 0);
        }
 }
}

void handle_employee_session(int client_socket) {
    char username[50]="", password[50]="";
    int bank_id;

    send(client_socket, "Enter Username: ", strlen("Enter Username: "), 0);
    read(client_socket, username, 50);
	 username[strcspn(username, "\n")] = 0;
    
  
    printf("Received Username: %s\n", username);
 
    send(client_socket, "Enter Password: ", strlen("Enter Password: "), 0);
    read(client_socket, password, 50);

	password[strcspn(password, "\n")] = 0;

    
    printf("Received Password: %s\n", password);

   
    bank_id = validate_employee_credentials(username, password);

    if (bank_id == -1) {
       send(client_socket, "Invalid credentials.\n", strlen("Invalid credentials.\n"), 0);
    } else if (bank_id == -2) {
send(client_socket, "Already logged in.Only one session per admin is allowed.Enter ok to continue\n", strlen("Already logged in .Only one session per admin is allowed.Enter ok to continue\n"), 0);       
 
    } else {
      
        
        send(client_socket, "Login successful.\n", strlen("Login successful.\n"), 0);

 
    }
}

void logout_employee(int client_socket) {
    char buffer[50];  
    int bank_id;

    
    send(client_socket, "Enter ID: ", strlen("Enter ID: "), 0);
    
  
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; 
    }

    
    buffer[bytes_read] = '\0';

   
    bank_id = atoi(buffer);

    
   int res =  employee_logout(bank_id);
	printf("%d",res);
fflush(stdout);
if(res==-1){
send(client_socket,"you were already logged out\n",strlen("you were already logged out\n"));
}
  else{  
    send(client_socket, "You have been logged out.\n", strlen("You have been logged out.\n"), 0);
}
}

void check_employee_status(int client_socket) {
    char buffer[50];  
    int bank_id;

    
    send(client_socket, "Enter ID: ", strlen("Enter ID: "), 0);
    
    
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; 
    }

    
    buffer[bytes_read] = '\0';

    
    bank_id = atoi(buffer);
	int res = employee_status(bank_id);
	if(res==1){
	send(client_socket, "Session in progress.\n", strlen("Session in progress.\n"), 0);

		}
	if(res==-1){
	send(client_socket, "Sesssion Invalid.\n", strlen("Session Invalid.\n"), 0);

       }

}

void new_customer(int client_socket){
    char buffer[256]=""; 
    int bank_id;

   
    send(client_socket, "Enter your employee ID: ", strlen("Enter your employee ID: "), 0);

    
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; 
    }

   
    buffer[bytes_read] = '\0';

   
    bank_id = atoi(buffer);
    int res = employee_status(bank_id);
    if (res != 1) {
        const char *error_message = "You must be logged in as an employee to add a new customer.\n";
        send(client_socket, error_message, strlen(error_message), 0);
        return;
    }

    if(res==1){
        
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

    send(client_socket, "Enter initial balance: ", strlen("Enter initial balance: "), 0);
    ssize_t balance_len = read(client_socket, buffer, sizeof(buffer) - 1);
    if (balance_len < 0) {
        perror("Error reading customer balance");
        return;
    }
    buffer[balance_len] = '\0';  
    printf("Buffer content before conversion: %s\n", buffer);
    int customer_balance = atoi(buffer);  
    printf("recieved balance %d\n",customer_balance);
    
    add_customer(customer_name, customer_password, customer_balance);

    char success_message[100];
    snprintf(success_message, sizeof(success_message), "Customer '%s' added successfully with an initial balance of %d\n", customer_name, customer_balance);
    send(client_socket, success_message, strlen(success_message), 0);

    }
}

void change_customer(int client_socket){
    char buffer[256]="";  
    int bank_id;

  
    send(client_socket, "Enter your employee ID: ", strlen("Enter your employee ID: "), 0);

   
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; 
    }

   
    buffer[bytes_read] = '\0';

   
    bank_id = atoi(buffer);
    int res = employee_status(bank_id);
    if (res != 1) {
        const char *error_message = "You must be logged in as an employee to modify customer.\n";
        send(client_socket, error_message, strlen(error_message), 0);
        return;
    }

    if(res==1){
        

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

void view_appEmployee(int client_socket){

    char buffer[256]="";  
    int emp_id;

   
    send(client_socket, "Enter your employee ID: ", strlen("Enter your employee ID: "), 0);

    
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; 
    }

   
    buffer[bytes_read] = '\0';

    
    emp_id = atoi(buffer);
    int res = employee_status(emp_id);


    if(res==1){
        Loan loans[100];
        int num_loans = applications_historyEmployee(emp_id, loans, 100);

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

        
        char buffer[1024]; 
        memset(buffer, 0, sizeof(buffer));
        strcat(buffer, "Loan Applications History:\n");

        for (int i = 0; i < num_loans; i++) {
            char loan_info[256];
            
snprintf(loan_info, sizeof(loan_info), 
         "Loan ID: %d, Desc: %s, Amount: %.2f, Status: %d, Manager ID: %d, Employee ID: %d, Customer ID: %d\n", 
         loans[i].loanID, loans[i].desc, loans[i].amount, 
         loans[i].loanStatus, loans[i].manID, 
         loans[i].empID, loans[i].custID);
            strcat(buffer, loan_info);
        }

        
        send(client_socket, buffer, strlen(buffer), 0);
        
    }
    else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }

}

void process_application(client_socket){

     char buffer[256]="";  
    int emp_id;

   
    send(client_socket, "Enter your employee ID: ", strlen("Enter your employee ID: "), 0);

    
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; 
    }

   
    buffer[bytes_read] = '\0';

   
    emp_id = atoi(buffer);
    int res = employee_status(emp_id);


    if(res==1){
        int loan_id;

           
            send(client_socket, "Enter loan ID: ", strlen("Enter loan ID: "), 0);

            
            ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
            if (bytes_read < 0) {
                perror("Failed to read from client");
                return; 
            }

          
            buffer[bytes_read] = '\0';

           
            loan_id = atoi(buffer);
            
        int sts;

         
            send(client_socket, "Enter 1 for accept, 0 for reject: ", strlen("Enter 1 for accept, 0 for reject: "), 0);

            ssize_t bytes_read2 = read(client_socket, buffer, sizeof(buffer) - 1);
            if (bytes_read2 < 0) {
                perror("Failed to read from client");
                return; 
            }

            
            buffer[bytes_read2] = '\0';

            
            sts = atoi(buffer);

        int rep = update_loan_status(loan_id,sts,emp_id);    

        if(rep==1){
        char success_message[100];
        snprintf(success_message, sizeof(success_message), "Succesfully processed application\n");
        send(client_socket, success_message, strlen(success_message), 0);
        }else if(rep==-2){
            char success_message[100];
        snprintf(success_message, sizeof(success_message), "This loan application is not assigned to you\n");
        send(client_socket, success_message, strlen(success_message), 0);
        }
    
    }else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }
}

void customer_transactions(client_socket){

     char buffer[256]="";  
    int emp_id;

    
    send(client_socket, "Enter your employee ID: ", strlen("Enter your employee ID: "), 0);

    
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; 
    }

    
    buffer[bytes_read] = '\0';

   
    emp_id = atoi(buffer);
    int res = employee_status(emp_id);


    if(res==1){
        int cust_id;

   
    send(client_socket, "Enter your customer ID: ", strlen("Enter your customer ID: "), 0);

   
    ssize_t bytes_read2 = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read2 < 0) {
        perror("Failed to read from client");
        return; 
    }

   
    buffer[bytes_read2] = '\0';

   
    cust_id = atoi(buffer);


    int num_transactions = 0;
        Transaction* transactions = get_transaction_history(cust_id, &num_transactions);

        if (num_transactions > 0) {
            char transaction_buffer[1024] = ""; 

            
            for (int i = 0; i < num_transactions; i++) {
                char temp[256];
                snprintf(temp, sizeof(temp), "TransactionID: %d, Type: %s, From: %d, To: %d, Amount: %.2f\n",
                         transactions[i].transactionID,
                         transactions[i].transactionType,
                         transactions[i].fromId,
                         transactions[i].toId,
                         transactions[i].money);

               
                strcat(transaction_buffer, temp);
            }

            
            send(client_socket, transaction_buffer, strlen(transaction_buffer), 0);

        } else if (num_transactions == 0) {
            send(client_socket, "No transactions found.\n", strlen("No transactions found.\n"), 0);
        } else {
            send(client_socket, "Error retrieving transactions.\n", strlen("Error retrieving transactions.\n"), 0);
        }

        
        if (transactions != NULL) {
            free(transactions);
        }

    
    }else{
    send(client_socket, "You are not logged in", strlen("You are not logged in"), 0);

    }
}

void change_passwordEmployee(client_socket){
char buffer[50];  
    int emp_id;


    send(client_socket, "Enter ID: ", strlen("Enter ID: "), 0);
    
   
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from client");
        return; 
    }

    
    buffer[bytes_read] = '\0';

   
    emp_id = atoi(buffer);

char current_password[50]="", password[50]="";

    send(client_socket, "Enter current Password: ", strlen("Enter current Password: "), 0);
    read(client_socket, current_password, 50);

	current_password[strcspn(current_password, "\n")] = 0;
    send(client_socket, "Enter new Password: ", strlen("Enter new Password: "), 0);
    read(client_socket, password, 50);

	password[strcspn(password, "\n")] = 0;

int res =  change_employee_password(emp_id,current_password,password);
if(res==1){
send(client_socket, "Password changed successfully.\n", strlen("Password changed successfully.\n"), 0);
}else if (res ==-1){
send(client_socket, "current password is wrong.\n", strlen("current password is wrong.\n"), 0);
}

else{
send(client_socket, "Customer ID is wrong.\n", strlen("Customer ID is wrong.\n"), 0);
}

}