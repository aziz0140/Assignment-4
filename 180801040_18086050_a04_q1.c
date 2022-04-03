#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <string.h>
#include <stdbool.h>
#define MAX_FILE_NAME 100

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

	// Wait for user input
	for (;;) {
		printf("Enter Command: ");

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

		} else if (strcmp(command, "RQ") == 0) {
			scanf(" %d", &customer);
			for (int i = 0; i < num_resources; i++) {
				scanf(" %d", &resources[i]);
			}
			bool safe = true;
			for (int i = 0; i < num_resources; i++) {
				if (resources[i] > available[i]) {
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
		} else if (strcmp(command, "RL") == 0) {
			scanf(" %d", &customer);
			for (int i = 0; i < num_resources; i++) {
				scanf(" %d", &resources[i]);
			}
			bool safe = true;
			for (int i = 0; i < num_resources; i++) {
				if (resources[i] > allocated[customer][i]) {
					safe = false;
				}
			}
			if (safe) {
				// release
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

		} else {
			printf("Invalid input, use one of RQ, RL, Status, Run, Exit \n");
		}

	}

}
