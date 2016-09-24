#include "soupGenerator.h"

void generateSoup(int numDig){
	FILE *file;
	int randNumber;
	
	//open file output stream
	file = fopen(FILE_NAME, "w");
	
	//set random seed based on time
    srand(time(NULL));

	while(numDig > 0){
		randNumber = rand() % 10;
        fprintf(file, "%d", randNumber);
		numDig--;
	}
	
	fclose(file);
}
