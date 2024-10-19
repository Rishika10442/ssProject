#ifndef FEEDBACK_MODEL_H
#define FEEDBACK_MODEL_H

#include <stdio.h>

#define MAX_FEEDBACKS 100  // You can adjust this limit as per your needs

// Define the Feedback struct
typedef struct {
    char desc[100];       // Description of the feedback
    char review[255];     // Review content provided by the customer
    int custID;           // Customer ID who gave the feedback
    int manID;            // Manager ID to whom the feedback is directed
    int feedbackID;       // Unique feedback ID for tracking
} Feedback;


int enter_feedback(const char *subject, int custID);
int log_feedback(Feedback* feedback);
int get_last_feedback_id();
int feedback_history(int custID, Feedback feedbacks[], int max_feedbacks);
int load_all_feedbacks(Feedback feedbacks[], int max_feedbacks);
int save_all_feedbacks(Feedback feedbacks[], int num_feedbacks);
int enter_feedbackReview(int feedbackID, const char *review, int manID);
int feedback_history_by_manager(int manID, Feedback feedbacks[], int max_feedbacks);


#endif 