#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "soupGenerator.h"


int SCORE_A = 0;
int SCORE_B = 0;

void listNums(char **digits){
	
	FILE *file;
	//open file input stream
	file = fopen(FILE_NAME, "r");
	char num;
	int i =0;
	while(fscanf(file,"%c", &num) != EOF) {
		(*digits)[i++] = num;
	}
	fclose(file);
}

int runner(char *team, char digits[], int len, int dir) {	
	int found = 0, lenPid, i, numChar;
	char strPid[64];

	sprintf(strPid, "%d",getpid());
	lenPid = strlen(strPid);

	for(i = dir ? 0 : len, numChar=0; 
		dir ? i < len : i > 0; 
		dir ? i++ : i--) {
		if(numChar == lenPid) {
			printf("Team: %s \t Pid: %d \t PPid: %d \t Type: %s \t Found @: %d\n",
				team,getpid(),getppid(), dir ? "Right" : "Left", i);
			found++;
			numChar=0;
		} else if (digits[i] == strPid[numChar]) {
			numChar++;
		} else {
			numChar = 0;
		}
	}

	return found;
}

void pipeRead(int fd[], int *found, char buffer[]) {
	close(fd[1]); //close output side of pipe
	read(fd[0], buffer, sizeof(*buffer) *64);
	sscanf(buffer, "%d", found);
}

void pipeWrite(int fd[], int *found, char buffer[]) {
	//child closes input side of pipe
	close(fd[0]);
	//write number of pids found by runner
	sprintf(buffer, "%d", *found);			
	write(fd[1], buffer, strlen(buffer) + 1);
}

void createTeam(char *teamName, char digits[], int numberOfDigits, int levels) {
	int fd[2], found = 0;
	pid_t childpid;
	char buffer[64];

	while(levels-- != 0) {
		pipe(fd);

		childpid = fork();
		if (childpid == 0) {

			pipeRead(fd,&found,buffer);

			//right runner
			found += runner(teamName, digits, numberOfDigits, 1);
			//left runner
			found += runner(teamName, digits, numberOfDigits, 0);

			pipeWrite(fd,&found,buffer);

		} else { //parent processes
			pipeWrite(fd,&found,buffer);
			wait(NULL);
			exit(0);
		}
	}

	if (*teamName == 'A')
			SCORE_A = found;
	else if (*teamName == 'B')
			SCORE_B = found;

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

	if(argc == 3){
		int numberOfDigits = atoi(argv[1]);
		int levels = atoi(argv[2]);
		char digits[numberOfDigits];
		char *ptrDigit = digits;

		generateSoup(numberOfDigits);
		listNums(&ptrDigit);

	 	createTeam("A", digits, numberOfDigits, levels);
	 	createTeam("B", digits, numberOfDigits, levels);
	}
	else {
        printf("Invalid number of arguments!\n"
        "Call program with the number of digits\n");
    }
    
    while ((wpid = wait(&status)) > 0);

	//all processes have finished
	checkWinner();
}

