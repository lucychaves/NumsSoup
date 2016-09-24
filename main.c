#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "soupGenerator.h"


int SCORE_A = 0;
int SCORE_B = 0;

void listNums(int **digits){
	
	FILE *file;
	//open file input stream
	file = fopen(FILE_NAME, "r");
	char num;
	int i =0;
	while(fscanf(file,"%c", &num) != EOF) {
		(*digits)[i++] = (int) num;
	}
	fclose(file);
}

int runner(int digits[], int len) {	
	int found = 1;

	for(int i = 0; i < len; i++) {
		//printf("%d",digits[i]);
	}

	return found;
}

void createTeam(char *teamName, int digits[], int numberOfDigits, int levels) {
	int fd[2], found = 0;
	pid_t childpid;
	char buffer[64];
	
	while(levels-- != 0) {
		pipe(fd);

		childpid = fork();
		if (childpid == 0) {

			found += runner(digits, numberOfDigits);
			
			//child closes input side of pipe
			close(fd[0]);
			//write number of pids found by runner
			sprintf(buffer, "%d", found);			
			write(fd[1], buffer, strlen(buffer) + 1);

			exit(0);
		} else { //parent processes
			wait(NULL);

			close(fd[1]); //close output side of pipe
			read(fd[0], buffer, sizeof(buffer));
			sscanf(buffer, "%d", &found);

			//child closes input side of pipe
			close(fd[0]);
			//write number of pids found by runner
			sprintf(buffer, "%d", found);			
			write(fd[1], buffer, strlen(buffer) + 1);
			
			if (*teamName == 'A')
				SCORE_A = found;
			else if (*teamName == 'B')
				SCORE_B = found;
		}
		
	}

}

void checkWinner() {
	printf("A found: %d\nB found: %d\n", SCORE_A, SCORE_B);

	if (SCORE_A == SCORE_B) {
		printf("Team A and B tied!\n");
	} else if (SCORE_A > SCORE_B) {
		printf("Team A Won!\n");
	} else {
		printf("Team B Won!\n");
	}
}


int main(int argc, char *argv[]) {	
	pid_t wpid;
	int status = 0;

	if(argc == 2){
		int numberOfDigits = atoi(argv[1]);
		int digits[numberOfDigits];
		int *ptrDigit = digits;
		
		generateSoup(numberOfDigits);
		listNums(&ptrDigit);

	 	createTeam("A", digits, numberOfDigits, 4);
	 	createTeam("B", digits, numberOfDigits, 3);
	}
	else {
        printf("Invalid number of arguments!\n"
        "Call program with the number of digits\n");
    }
    
    while ((wpid = wait(&status)) > 0);

	//all processes have finished
	checkWinner();
}

