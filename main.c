#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "soupGenerator.h"

#define MAX_BUF 1024

int readFromChild(char team) {
	int fd =0, found;
    char myfifo[6] = "/tmp/";
	myfifo[5] = team;
    char buf[MAX_BUF];

    /* open, read, and display the message from the FIFO */
    
	fd = open(myfifo, O_RDONLY);
	if(fd) {
		read(fd, buf, MAX_BUF);
    	close(fd);
		fprintf(strFound, "%s", buf);
		scanf(buf, "%d", &found);
		return atoi(buf);
	}
	return 0;
}

void writeFromChild(char* team, int found) {
	int fd;
    char myfifo[6] = "/tmp/";
	myfifo[5] = *team;
	char strFound[256];

	//read from past pipe
	found += readFromChild(team[0]);

	//convert int to string to write
	sprintf(strFound, "%d", found);

	printf("%d\n",found);

    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);

    /* write "Hi" to the FIFO */
    fd = open(myfifo, O_WRONLY);
    write(fd, strFound, sizeof(strFound));
    close(fd);
}

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

int runner(int digits[], int len) {	
	int found = 0;

	for(int i = 0; i < len; i++) {
		//printf("%d",digits[i]);
	}

	return found;
}

void createTeam(char *teamName, int digits[], int numberOfDigits, int levels, int child) {
	pid_t pid;
	
	if(levels != 0) {

		pid = fork();
		if(pid == 0) {
			runner(digits, numberOfDigits);

			createTeam(teamName, digits, numberOfDigits, levels -1, 1);
			
			//write number of pids found
			writeFromChild(teamName, 1);

			exit(0);
		}
		
	}

}

void checkWinner() {
	int scoreA, scoreB;
	scoreA = readFromChild('A');
	scoreB = readFromChild('B');

	printf("A: %d\nB: %d\n", scoreA, scoreB);
}


int main(int argc, char *argv[]) {	
	if(argc == 2){
		int numberOfDigits = atoi(argv[1]);
		int digits[numberOfDigits];
		int *ptrDigit = digits;
		
		generateSoup(numberOfDigits);
		listNums(&ptrDigit);
		
		//cleanup temp pipe files from previous runs
		remove("/tmp/A");
		remove("/tmp/B");

	 	createTeam("A", digits, numberOfDigits, 3,0);
	 	createTeam("B", digits, numberOfDigits, 4,0);
	}
	else {
        printf("Invalid number of arguments!\n"
        "Call program with the number of digits\n");
    }
    
    int status;
    waitpid(1,&status,WNOHANG);

	//all processes have finished
	checkWinner();
}

