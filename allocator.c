#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#define SIZE 20  // Maximum size of the array

// Structure to hold the data each thread will process
typedef struct {
    char *name;
    int start;
    int end;
} Process;

void create_process(Process *processes, char *n, int s, int e, int index) {
	processes[index].name = (char *)malloc(strlen(n) + 1);
	strcpy(processes[index].name, n); // Set name
	processes[index].start = s; // Set start
	processes[index].end = e; // Set end
}

// Function to check memory
void check_memory(Process *processes, int size) {
	// Compact all process in memory
	for (int i = 0; i < size; i++) {
		if (strcmp(processes[i].name, "Unused") == 0) {
			printf("Addresses [%d:%d] %s\n", processes[i].start, processes[i].end, processes[i].name);
		}
		else {
			printf("Addresses [%d:%d] Process %s\n", processes[i].start, processes[i].end, processes[i].name);
		}
	}
}

// Get input function that cleans trailing newline character
void get_input(char *string, size_t size) {
	fgets(string, size, stdin);
	string[strcspn(string, "\n\r")] = 0;
}

// Function to request the first empty hole for memory
int request_first(char *process, int mem_amount,  Process *processes, int size, int total_mem) {
	// Check array
	for(int i = 0; i < size; i++) {
		// Check if unused memory block with enough space between them
		if(strcmp(processes[i].name, "Unused") == 0 && processes[i].end - processes[i].start >= mem_amount) {
			// If all memory will be used, change name from Unused to process parameter
			if (processes[i].end - processes[i].start == mem_amount) {
				processes[i].name = (char *)malloc(strlen(process) + 1);
				strcpy(processes[i].name, process); // Set name
				// Return 0 because size of array has not changed
				return 0;
			}
			
			// Get int memory value to start new process at
			int start = processes[i].start;
			// Change start of unused memory block
			processes[i].start = processes[i].start + mem_amount;
			// Shift up all processes after new spot
			for(int j = size - 1; j >= i; j--) {
				// Moves current index 1 index farther
				processes[j + 1] = processes[j];
			}
			// Insert new process into the array between two processes
			create_process(processes, process, start, start + mem_amount, i);
			// Return 1 because size should be increased 
			return 1;
		}
		
	}
}

// Function to request the first empty hole for memory
int request_best(char *process, int mem_amount,  Process *processes, int size, int total_mem) {
	// int for extra space found so far
	int extra_space = -1;
	// int for index of the best range
	int best_index = -1;
	
	// Check array and return only if a perfect memory block is found
	for(int i = 0; i < size; i++) {
		// Check if unused memory block with enough space between them
		if(strcmp(processes[i].name, "Unused") == 0 && processes[i].end - processes[i].start >= mem_amount) {
			// If all memory will be used, change name from Unused to process parameter
			if (processes[i].end - processes[i].start == mem_amount) {
				// free(processes[i].name);
				processes[i].name = (char *)malloc(strlen(process) + 1);
				strcpy(processes[i].name, process); // Set name
				// Return 0 because size of array has not changed
				return 0;
			}
			// If no possible memory location has been found yet
			if (extra_space == -1) {
				// Set extra_space to current range and set best_index to the current index
				extra_space = processes[i].end - processes[i].start - mem_amount;
				best_index = i;
			}
			// If current unused memory is smaller (better fit)
			else if (extra_space > processes[i].end - processes[i].start - mem_amount) {
				// Set extra_space to current range and set best_index to the current index
				extra_space = processes[i].end - processes[i].start - mem_amount;
				best_index = i;
			}
		}
	}
	
	// Get int memory value to start new process at
	int start = processes[best_index].start;
	// Change start of unused memory block
	processes[best_index].start = processes[best_index].start + mem_amount;
	// Shift up all processes after new spot
	for(int j = size - 1; j >= best_index; j--) {
		// Moves current index 1 index farther
		processes[j + 1] = processes[j];
	}
	// Insert new process into the array between two processes
	create_process(processes, process, start, start + mem_amount, best_index);
	// Return 1 because size should be increased 
	return 1;
}

// Function to request the first empty hole for memory
int request_worst(char *process, int mem_amount,  Process *processes, int size, int total_mem) {
	// int for extra space found so far
	int extra_space = -1;
	// int for index of the best range
	int best_index = -1;
	
	// Check array and return only if a perfect memory block is found
	for(int i = 0; i < size; i++) {
		// Check if unused memory block with enough space between them
		if(strcmp(processes[i].name, "Unused") == 0 && processes[i].end - processes[i].start >= mem_amount) {
			// If no possible memory location has been found yet
			if (extra_space == -1) {
				// Set extra_space to current range and set best_index to the current index
				extra_space = processes[i].end - processes[i].start - mem_amount;
				best_index = i;
			}
			// If current unused memory is smaller (better fit)
			else if (extra_space < processes[i].end - processes[i].start - mem_amount) {
				// Set extra_space to current range and set best_index to the current index
				extra_space = processes[i].end - processes[i].start - mem_amount;
				best_index = i;
			}
		}
	}
	
	// Get int memory value to start new process at
	int start = processes[best_index].start;
	// Change start of unused memory block
	processes[best_index].start = processes[best_index].start + mem_amount;
	// Shift up all processes after new spot
	for(int j = size - 1; j >= best_index; j--) {
		// Moves current index 1 index farther
		processes[j + 1] = processes[j];
	}
	// Insert new process into the array between two processes
	create_process(processes, process, start, start + mem_amount, best_index);
	// Return 1 because size should be increased 
	return 1;
}

// Function to request memory
int request_memory(char *process, int mem_amount, char *fit, Process *processes, int size, int total_mem) {
	// Add process to memory with size and fit
	if(strcasecmp(fit, "F") == 0) {
		return request_first(process, mem_amount, processes, size, total_mem);
	}
	if(strcasecmp(fit, "B") == 0) {
		return request_best(process, mem_amount, processes, size, total_mem);
	}
	if(strcasecmp(fit, "W") == 0) {
		return request_worst(process, mem_amount, processes, size, total_mem);
	}
}

// Compact leading/trailing Unused memory blocks during program release
int compact_release(Process *processes, int i, int size) {
	// Return value for array size to be decreased by
	int ret = 0;
	// Checks if there is an element before the current element
	if (i - 1 >= 0) {
		// Checks if previous element was also unused
		if(strcmp(processes[i - 1].name, "Unused") == 0) {
			// Get start value for Unused memory block
			int start = processes[i - 1].start;
			// Copy previous Unused memory block to current index
			processes[i - 1] = processes[i];
			// Switch start of memory value
			processes[i - 1].start = start;
			// Shift up all processes after new spot
			for(int j = i ; j < size - 1; j++) {
				// Moves current index 1 index farther
				processes[j] = processes[j + 1];
			}
			// Return 1 because size should be decreased 
			ret++;;
			// Change i to -1 to allow for checking following element as well
			i--;
			size--;
		}
	}
	//check_memory(processes, size);
	// Checks if their is another element after the current element
	if(i + 1 < size) {
		// Checks if the next element is also unused
		if(strcmp(processes[i + 1].name, "Unused") == 0) {
			// printf("i: %d - ", i);
			//printf("size: %d\n", size);
			// Get end value for Unused memory block
			int end = processes[i + 1].end;
			// Copy Following Unused memory block to current index
			processes[i + 1] = processes[i];
			// Switch end of memory value
			processes[i + 1].end = end;
			// Shift up all processes after new spot
			for(int j = i; j < size - 1; j++) {
				// Moves current index 1 index farther
				processes[j] = processes[j + 1];
			}
			// Return 1 because size should be decreased 
			ret++;
		}
	} 
	return ret;
}

// Function to release memory
int release_memory(char *process, Process *processes, int size) {
	// Return value for array size to be decreased by
	int ret = 0;
	// Checks each array element for the process to be removed
	for (int i = 0; i < size; i++) {
		// Found element to be removed
		if(strcmp(process, processes[i].name) == 0) {
			// Change name of process to Unused
			processes[i].name = (char *)malloc(strlen(process) + 1);
			strcpy(processes[i].name, "Unused"); // Set name
			// Checks if Unused memory blocks need to be compacted
			ret += compact_release(processes, i, size);
			size -= ret;
			i = 0;
		} 
	}
	// Return changed size
	return ret;
}

// Function to compact memory
int compact_memory(Process *processes, int size) {
	// int for how much size has been reduced by
	int ret = 0;
	// int to hold the amount to shift start/end values of following processes
	int mem_shift = 0;
	// Loop through all elements in the array
	int i = 0;
	while (i < size) {
		// Found unused memory
		if(strcmp(processes[i].name, "Unused") == 0 && i < size - 1) {
			// Increases shift value based on total memory in current unused space
			mem_shift += processes[i].end - processes[i].start;
			// Shift up all processes after current index
			for(int j = i ; j < size - 1; j++) {
				// Moves current index 1 index farther
				processes[j] = processes[j + 1];
			}
			// Goes through current i again because the element has been updated
			size--;
			ret++;
		} 
		// If used memory
		else {
			processes[i].start -= mem_shift;
			processes[i].end -= mem_shift;
			i++;
		}
		// printf("%d", i);
	}
	// If last array element is not unused
	if (strcmp(processes[size - 1].name, "Unused") != 0 && mem_shift > 0) {
		// Insert new unused element into the array
		processes[size].name = (char *)malloc(strlen("Unused") + 1);
		strcpy(processes[size].name, "Unused"); // Set name
		processes[size].start = processes[size - 1].end; // Set start
		processes[size].end = processes[size - 1].end + mem_shift; // Set end
		ret--;
	}
	// Return number for size to be reduced by
	return ret;
}

void run(int total_mem) {
	
	char input[100];
	char *args[5];
	Process processes[SIZE];
	int size = 0;
	
	// Create Initial Unused Memory
	create_process(processes, "Unused", 0, total_mem, 0);
	size++;
	
	// Prompt user
	while(1) {
		
		// Print query for data
		printf("allocator> ");
		fflush(stdout);
		// Get input from user and remove newline character
		get_input(input, sizeof(input));
		
		// Displays input when using a file (comment out when doing user input)
		printf("%s\n", input);
		
		// Tokenize the input based on spaces
		char *token = strtok(input, " ");
		int index = 0;
		
		// Populate the args array with tokens
		while (token != NULL && index < 4) {
			args[index++] = token;  // Add the token to the args array
			token = strtok(NULL, " ");  // Get the next token
		}
		args[index] = NULL;  // Null-terminate the array of arguments
		
		
		// Handle quit command
		if(strcasecmp(args[0], "q") == 0 || strcasecmp(args[0], "x") == 0) {
			break;
		}
		
		// Request for a contiguous block of memory
        else if (strcasecmp(args[0], "RQ") == 0) {
			// Parse int
			int mem_amount = atoi(args[2]);
			// Input validation
			if (mem_amount > total_mem) {
				printf("Invalid amount of memory to be allocated.\n");
			}
			// Check for available space in the array
			else if (size < SIZE) {
				// Call request memory function and check whether size should be changed
				size += request_memory(args[1], mem_amount, args[3], processes, size, total_mem);
			}
        }
		
		// Release a contiguous block of memory
        else if (strcasecmp(args[0], "RL") == 0) {
			// Decrements array size based on amount of elements removed
			size -= release_memory(args[1], processes, size);
        }
		
		// Compact any unused blocks of memory
        else if (strcasecmp(args[0], "C") == 0) {
			size -= compact_memory(processes, size);
        }
		
		// Display current processes
        else if (strcasecmp(args[0], "STAT") == 0) {
			check_memory(processes, size);
        }
	}
	
	// Free strings
	for (int i = 0; i < size; i++) {
		free(processes[i].name);
	}
}

int main(int argc, char *argv[]) {
	
	int total_mem = 0;
	
	// Check if the user provided the correct number of arguments
    if (argc != 2) {
        printf("Invalid number of arguments\n");
        return 1;
    }
	
    // Total memory to use
    total_mem = atoi(argv[1]);
	
    run(total_mem);
	
    return 0;
}
