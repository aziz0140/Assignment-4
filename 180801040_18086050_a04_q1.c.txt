#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#define MAX_FILE_NAME 100

/*
 * CP386 Assignment 4 Submission
 * Student Name: Raiyan Jugbhery
 * Student ID: 180686050
 * Student Email: jugb6050@mylaurier.ca
 * github login id: jugb6050
 * github repo url: https://github.com/aziz0140/Assignment-4
 * Student Task Split: Question 1 is done by Raiyan Jugbhery, Question 2 is done by Carson Aziz
 */

//mutex init var
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// args struct created so that variables can be passed into thread function
struct args {
	int num_res;
	int cust_num;
	int *avail;
	int *allo;
	int *req;
};
//Safetycheck method
bool SafetyCheck(int num_customers, int num_resources,
		int available[num_resources],
		int allocation[num_customers][num_resources],
		int requested[num_customers][num_resources],
		int safe_seq[num_customers]) {
	bool safe = true;
	int finish[num_customers];
	// 0 false
	// 1 true
	// 2 true, but already added
	int work[num_resources];
	int index = 0;
	bool done = true;

	//step 1 from textbook
	// Initialize work = available
	for (int i = 0; i < num_resources; i++) {
		work[i] = available[i];
	}

	// for i, if allocation[i] !=0, finish[i] = false. Otherwise finish[i] = true
	for (int i = 0; i < num_customers; i++) {
		finish[i] = 1;
		for (int j = 0; j < num_resources; j++) {
			if (allocation[i][j] != 0) {
				finish[i] = 0;
			}
		}
	}
	//step 2 from textbook
	for (int i = 0; i < num_customers; i++) {
		if (i == 0) {
			done = true;
		}

		if (finish[i] == 0) {
			bool work_avail = true;
			for (int j = 0; j < num_resources; j++) {
				if (requested[i][j] > work[j]) {
					work_avail = false;
				}
			}
			//step 3 from textbook
			if (work_avail) {
				for (int j = 0; j < num_resources; j++) {
					work[j] += allocation[i][j];
				}
				safe_seq[index] = i;
				index++;
				finish[i] = 2;
				done = false;
			}
		} else if (finish[i] == 1) {
			safe_seq[index] = i;
			index++;
			finish[i] = 2;
		}
		if (i == num_customers - 1 && !done) {
			i = -1;
		}
	}

	// Step 4 from textbook
	for (int i = 0; i < num_customers; i++) {
		if (finish[i] == 0) {
			safe = false;
		}
	}

	return safe;
}

void* CustomerRun(void *in) {
	pthread_mutex_lock(&mutex);

	//variable casting/getting arguments

	// int customer = ((struct args*) in)->cust_num;
	int num_resources = ((struct args*) in)->num_res;
	int *available = ((struct args*) in)->avail;
	int *allocated = ((struct args*) in)->allo;
	int *need = ((struct args*) in)->req;

	printf("    Thread has started\n");
	// do request stuff
	for (int i = 0; i < num_resources; i++) {
		allocated[i] += need[i];
		available[i] -= need[i];
		need[i] = 0;
	}
	//print statements
	printf("    Thread has finished\n");
	printf("    Thread is releasing resources\n");
	// do release stuff
	for (int i = 0; i < num_resources; i++) {
		available[i] += allocated[i];
		allocated[i] = 0;
	}
	printf("    New Available:");
	// print new available resources
	for (int i = 0; i < num_resources; i++) {
		printf(" %d", available[i]);
	}
	printf("\n");
	//releasing lock
	pthread_mutex_unlock(&mutex);
	//exiting thread
	pthread_exit(0);
}
//run function
void Run(int num_customers, int num_resources,
		int allocated[num_customers][num_resources],
		int need[num_customers][num_resources], int available[num_resources],
		pthread_t cust_threads[]) {
	// safe seq array init
	int safe_seq[num_customers];
	// set bool safe to output of SafetyCheck method
	bool safe = SafetyCheck(num_customers, num_resources, available, allocated,
			need, safe_seq);

	// find safe sequence
	if (!safe) {
		printf("No possible safe sequence\n");
	} else {
		printf("Safe Sequence is: ");
		for (int i = 0; i < num_customers; i++) {
			printf(" %d", safe_seq[i]);
		}
	}
	//print statement values
	printf("\n");
	for (int i = 0; i < num_customers; i++) {
		printf("--> Customer/Thread %d\n", safe_seq[i]);
		printf("    Allocated resources:");
		for (int r = 0; r < num_resources; r++) {
			printf(" %d", allocated[safe_seq[i]][r]);
		}
		printf("\n");
		printf("    Needed:");
		for (int r = 0; r < num_resources; r++) {
			printf(" %d", need[safe_seq[i]][r]);
		}
		printf("\n");
		printf("    Available:");
		for (int r = 0; r < num_resources; r++) {
			printf(" %d", available[r]);
		}
		printf("\n");
		// threading
		struct args *cust_run = (struct args*) malloc(sizeof(struct args));
		cust_run->num_res = num_resources;
		cust_run->cust_num = safe_seq[i];
		cust_run->avail = available;
		cust_run->allo = allocated[safe_seq[i]];
		cust_run->req = need[safe_seq[i]];

		// Creating the thread instances.
		pthread_create(&cust_threads[safe_seq[i]], NULL, CustomerRun,
				(void*) cust_run);
		pthread_join(cust_threads[safe_seq[i]], NULL);

	}
}

int main(int argc, char *argv[]) {
//variable init below -----------------------
	FILE *input_file;
	int customer_count = 0; // reads number of lines in a file to determine number of customers
	char filename[MAX_FILE_NAME] = "sample4_in.txt"; //hard coded file name
	char line[256];
	int num_resources = argc - 1;
	int available[num_resources];

//checking sample4_in for number of customers---------------------
	input_file = fopen(filename, "r"); // read only

//error checking for if file exists or not
	if (input_file == NULL) {
		printf("Error! Could not open file\n");
		exit(-1);
	}

	//line reading
	while (fgets(line, sizeof(line), input_file)) {
		//printf("%s \n", line);
		customer_count += 1;
	}

	fclose(input_file);
	printf("Number of Customers: %d \n", customer_count);

//checking user input for available resources below --------------

	for (int i = 1; i < argc; i++) {
		available[i - 1] = atoi(argv[i]);
	}

	printf("Currently Available Resources: ");

	for (int i = 0; i < argc - 1; i++) {
		printf("%d ", available[i]);
	}
	printf("\n");

//printing Maximum resources from file------------------------
	printf("Maximum resources from file: \n");
	input_file = fopen(filename, "r"); // read only

//error checking for if file exists or not
	if (input_file == NULL) {
		printf("Error! Could not open file\n");
		exit(-1);
	}

	int maximum[customer_count][num_resources];
	int need[customer_count][num_resources];
	int row = 0;
	while (fgets(line, sizeof(line), input_file)) {
		for (int i = 0; i < (num_resources * 2) - 1; i += 2) {
			maximum[row][i / 2] = line[i] - '0';
			need[row][i / 2] = line[i] - '0';
		}
		row++;
	}

	fclose(input_file);
	for (int cust = 0; cust < customer_count; cust++) {
		for (int res = 0; res < num_resources; res++) {
			printf("%d ", maximum[cust][res]);
		}
		printf("\n");
	}
	int allocated[customer_count][num_resources];

	for (int d = 0; d < customer_count; d++) {
		for (int j = 0; j < num_resources; j++) {
			allocated[d][j] = 0;
		}
	}

	char command[6];
	int customer;
	int resources[num_resources];

// Wait for user input, checks each different command input as given by assignment instructions
	while (true) {
		printf("Enter Command: ");
		// gets command portion
		scanf("%s", command);
		if (strcmp(command, "Exit") == 0) {
			return 0;
		} else if (strcmp(command, "Status") == 0) {
			printf("Available Resources: \n");
			for (int res = 0; res < num_resources; res++) {
				printf("%d ", available[res]);
			}
			printf("\n");
			printf("Maximum Resources: \n");
			for (int cust = 0; cust < customer_count; cust++) {
				for (int res = 0; res < num_resources; res++) {
					printf("%d ", maximum[cust][res]);
				}
				printf("\n");
			}
			printf("Allocated Resources: \n");
			for (int cust = 0; cust < customer_count; cust++) {
				for (int res = 0; res < num_resources; res++) {
					printf("%d ", allocated[cust][res]);
				}
				printf("\n");
			}
			printf("Need Resources: \n");
			for (int cust = 0; cust < customer_count; cust++) {
				for (int res = 0; res < num_resources; res++) {
					printf("%d ", need[cust][res]);
				}
				printf("\n");
			}
		} else if (strcmp(command, "Run") == 0) {
			pthread_t cust_threads[customer_count];
			Run(customer_count, num_resources, allocated, need, available,
					cust_threads);
		} else if (strcmp(command, "RQ") == 0) {
			// Read in customer number
			scanf(" %d", &customer);
			// check if valid customer
			if (customer < 0 || customer >= customer_count) {
				printf("Please enter a valid customer number");
			} else {
				// Read in the number of resources to request
				for (int i = 0; i < num_resources; i++) {
					scanf(" %d", &resources[i]);
				}

				// int requested[customer_count][num_resources];
				// for (int i = 0; i < customer_count; i++) {
				// 	for (int j = 0; j < num_resources; j++) {
				// 		if (i == customer) {
				// 			requested[i][j] = resources[i];
				// 		} else {
				// 			requested[i][j] = 0;
				// 		}
				// 	}
				// }
				//bool safe = SafetyCheck(customer_count, num_resources,
				//available, allocated, requested
				//);
				bool safe = true;
				for (int i = 0; i < num_resources; i++) {
					if (resources[i] > available[i]
							|| resources[i] > maximum[customer][i]) {
						safe = false;
					}
				}
				if (safe) {
					// allocate
					for (int i = 0; i < num_resources; i++) {
						allocated[customer][i] += resources[i];
						available[i] -= resources[i];
						need[customer][i] -= resources[i];
					}
					printf("State is safe, and request is satisfied\n");
				} else {
					// print error statement
					printf("State not safe, and request is not satisfied\n");
				}
			}
		} else if (strcmp(command, "RL") == 0) {
			// Read in customer number
			scanf(" %d", &customer);
			// check if valid customer
			if (customer < 0 || customer >= customer_count) {
				printf("Please enter a valid customer number");
			} else {
				// Read in the number of resources to release
				for (int i = 0; i < num_resources; i++) {
					scanf(" %d", &resources[i]);
				}
				// Check to see if the customer has enough resources to release
				bool safe = true;
				for (int i = 0; i < num_resources; i++) {
					if (resources[i] > allocated[customer][i]) {
						safe = false;
					}
				}
				if (safe) {
					// release the resources
					for (int i = 0; i < num_resources; i++) {
						allocated[customer][i] -= resources[i];
						available[i] += resources[i];
					}
					printf("The resources have been released successfully\n");
				} else {
					// print error statement
					printf(
							"Not enough allocated resources for customer, enter values <= currently allocated resources for the customer\n");
				}
			}
		} else {
			printf("Invalid input, use one of RQ, RL, Status, Run, Exit \n");
		}

	}

}

