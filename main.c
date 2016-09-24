#include <unistd.h>
#include <sys/wait.h>

#include "soupGenerator.h"

void listNums(int **digits){
	
	FILE *file;
	//open file input stream
	file = fopen("num_soup", "r");
	char num;
	int i =0;
	while(fscanf(file,"%c", &num) != EOF) {
		(*digits)[i++] = (int) num;
	}
	
	fclose(file);
}

void runner(int digits[], int len) {	
	for(int i = 0; i < len; i++) {
		//printf("%d",digits[i]);
	}
}

void createTeam(char *teamName, int digits[], int numberOfDigits, int levels) {
	pid_t pid;
	
	if(levels != 0) {
		pid = fork();
		if(pid == 0) {
			runner(digits, numberOfDigits);
			printf("soy team %s\n", teamName);
			createTeam(teamName, digits, numberOfDigits, levels -1);
			exit(0);
		}
	}
}

int main(int argc, char *argv[]) {	
	if(argc == 2){
		int numberOfDigits = atoi(argv[1]);
		int digits[numberOfDigits];
		int *ptrDigit = digits;
		
		generateSoup(numberOfDigits);
		listNums(&ptrDigit);
		
	 	createTeam("A", digits, numberOfDigits, 2);
	 	createTeam("B", digits, numberOfDigits, 2);
	}
	else {
        printf("Invalid number of arguments!\n"
        "Call program with the number of digits\n");
    }
    
    int status;
    waitpid(-1,&status,WNOHANG);
    
}

