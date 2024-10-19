# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source files
SRCS = first.c controllers/admin_controller.c controllers/employee_controller.c controllers/customer_controller.c controllers/manager_controller.c models/admin_model.c models/customer_model.c models/bank_model.c models/manager_model.c models/transaction_model.c models/loan_model.c models/feedback_model.c

# Header files
HDRS = models/admin_model.h models/customer_model.h controllers/admin_controller.h controllers/employee_controller.h controllers/customer_controller.h controllers/manager_controller.h models/bank_model.h models/manager_model.h models/transaction_model.h models/loan_model.h models/feedback_model.h

# Output executable
TARGET = server

# Default rule to compile the program
all: $(TARGET)

# Linking object files to create the final executable
$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean rule to remove the executable and object files
clean:
	rm -f $(TARGET)

# Rule to run the server
run: $(TARGET)
	./$(TARGET)

