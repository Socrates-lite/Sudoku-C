#include <windows.h>
#include <stdio.h>
#define NumOfThreads 20

// Struct that stores the data to be passed to threads
struct ParamData {
	int rowStart;
	int rowEnd;
	int colStart;
	int colEnd;

	int flagIndex;
};

boolean valid[NumOfThreads];


// Sudoku puzzle to be solved
char sudoku[16][16] = {
			{'A', 'E', 'F', '1', '9', '2', 'D', '8', 'B', 'G', '6', 'C', '5', '3', '7', '4'},
			{'3', '7', 'G', '2', '1', 'E', 'A', '6', 'F', '9', '5', '4', '8', 'D', 'C', 'B'},
			{'B', '9', '8', '5', '4', 'C', 'F', '7', 'D', '3', '1', 'E', '6', 'A', 'G', '2'},
			{'D', '4', '6', 'C', '3', 'B', '5', 'G', '8', 'A', '7', '2', 'F', 'E', '1', '9'},
			{'5', 'D', 'A', '8', 'B', 'G', 'E', '4', 'C', '6', '9', '1', '7', 'F', '2', '3'},
			{'2', '6', '9', '3', 'A', '7', 'C', '1', '5', 'F', '4', 'G', 'D', '8', 'B', 'E'},
			{'1', 'C', '7', 'E', '2', 'F', '9', '5', '3', 'D', 'B', '8', 'A', '6', '4', 'G'},
			{'4', 'G', 'B', 'F', '8', '6', '3', 'D', '7', 'E', '2', 'A', 'C', '9', '5', '1'},
			{'C', 'A', '1', '4', 'F', '5', '8', 'E', '2', 'B', 'G', '9', '3', '7', '6', 'D'},
			{'F', 'B', '5', 'G', 'D', '3', '7', '2', 'E', 'C', '8', '6', '4', '1', '9', 'A'},
			{'8', '3', 'E', '7', 'G', '1', '6', '9', 'A', '4', 'D', '5', 'B', '2', 'F', 'C'},
			{'9', '2', 'D', '6', 'C', '4', 'B', 'A', '1', '7', 'F', '3', 'E', 'G', '8', '5'},
			{'E', '1', '4', 'A', '5', 'D', '2', 'F', 'G', '8', 'C', '7', '9', 'B', '3', '6'},
			{'7', 'F', '2', 'D', '6', 'A', 'G', 'C', '9', '5', '3', 'B', '1', '4', 'E', '8'},
			{'6', '8', 'C', 'B', 'E', '9', '1', '3', '4', '2', 'A', 'F', 'G', '5', 'D', '7'},
			{'G', '5', '3', '9', '7', '8', '4', 'B', '6', '1', 'E', 'D', '2', 'C', 'A', 'F'} };


// Method to check if numbers 1-16 are duplicated in a row
DWORD WINAPI IsRowValid(LPVOID p)
{
	struct ParamData params = *(struct ParamData*)p;
	int rowStart = params.rowStart;
	int rowEnd   = params.rowEnd;
	int flagIndex = params.flagIndex;

	valid[flagIndex] = 0;		//assume not valid

	for (int row = rowStart; row <= rowEnd; row++) {  //check all these rows
		int val_seen[16] = { 0 };
		for (int col = 0; col < 16; col++) {		  //check every column in each row
			int val = sudoku[row][col];			  //puzzle is global in C program	
			if (!((val >= '1' && val <= '9') || (val >= 'A' && val <= 'G' ) && val_seen[val - 1] == 0)) {
				return 0;   // found an invalid digit
			}
			val_seen[val - 1] == 0;
		}
	}

	//NOTE: in C, 0 means false and not zero is true
	valid[flagIndex] = 1;	//no bad digit found
	return 0;
}

DWORD WINAPI IsColValid(LPVOID p)
{
	struct ParamData params = *(struct ParamData*)p;
	int colStart = params.colStart;
	int colEnd = params.colEnd;
	int flagIndex = params.flagIndex;

	valid[flagIndex] = 0;		//assume not valid

	for (int col = colStart; col <= colEnd; col++) { 
		int val_seen[16] = { 0 };//check all these rows
		for (int row = 0; row < 16;row++) {		  //check every column in each row
			int val = sudoku[col][row];			  //puzzle is global in C program	
			if (!((val >= '1' && val <= '9') || (val >= 'A' && val <= 'G' && val_seen[val - 1] == 0))) {
				return 0;   // found an invalid digit
			}
			val_seen[val - 1] == 0;
		}
	}

	DWORD WINAPI IsRegValid(LPVOID p)
	{
		struct ParamData params = *(struct ParamData*)p;
		int rowStart = params.rowStart;
		int rowEnd = params.rowEnd;
		int colStart = params.colStart;
		int colEnd = params.colEnd;
		int i = 0;
		int j = 0;

		int flagIndex = params.flagIndex;

		valid[flagIndex] = 0;		//assume not valid

		for (int row = rowStart; row <= rowEnd; row++) {
			for (int col = 0; col < 16; col++) {
				if (row % 4 == 0 && col % 4 == 0) {
					int val_seen[16] = { 0 };

					for (int i = row; i <= row + 4; row++) {
						for (int j = col; j < col + 4; col++) {		  //check every column in each row
							int val = sudoku[row][col];			  //puzzle is global in C program	
							if (!((val >= '1' && val <= '9') || (val >= 'A' && val <= 'G' && val_seen[val - 1] == 0))) {
								return 0;   // found an invalid digit
							}
							val_seen[val - 1] == 0;
						}
					}
				}
			}
	}

	//NOTE: in C, 0 means false and not zero is true

	valid[flagIndex] = 1;	//no bad digit found
	return 0;
}

int main(int argc, char* argv[])
{
	/*	The number of threads should be 20
		2 threads to check that each column contains the values 1-9, A-G
		2 threads to check that each row contains the values 1-9. A-G
		16 threads to check that each of the 4x4 sub-grids contain the digits 1-9, A-G */

	DWORD ThreadId[NumOfThreads];
	HANDLE ThreadHandle[NumOfThreads];
	struct ParamData params[NumOfThreads];
	int i;

	// Create the threads for 16 4x4 sub-grids area, 2 threads for 16 columns and 2 threads for 16 rows.
	//first 16 threads for the sub-grids
	//TO-BE_COMPLETED
	ThreadHandle[0] = CreateThread(NULL, 0, IsRegValid, &params[0], 0, &ThreadId[0]);
	if (ThreadHandle[0] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 0);
		return -2;
	}

	ThreadHandle[1] = CreateThread(NULL, 0, IsRegValid, &params[1], 0, &ThreadId[1]);
	if (ThreadHandle[1] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 1);
		return -2;
	}

	ThreadHandle[2] = CreateThread(NULL, 0, IsRegValid, &params[2], 0, &ThreadId[2]);
	if (ThreadHandle[2] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 2);
		return -2;
	}

	ThreadHandle[3] = CreateThread(NULL, 0, IsRegValid, &params[3], 0, &ThreadId[3]);
	if (ThreadHandle[3] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 18);
		return -2;
	}	
	
	ThreadHandle[4] = CreateThread(NULL, 0, IsRegValid, &params[4], 0, &ThreadId[4]);
	if (ThreadHandle[4] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 4);
		return -2;
	}

	ThreadHandle[5] = CreateThread(NULL, 0, IsRegValid, &params[5], 0, &ThreadId[5]);
	if (ThreadHandle[5] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 5);
		return -2;
	}
	ThreadHandle[6] = CreateThread(NULL, 0, IsRegValid, &params[6], 0, &ThreadId[6]);
	if (ThreadHandle[6] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 6);
		return -2;
	}
	ThreadHandle[7] = CreateThread(NULL, 0, IsRegValid, &params[7], 0, &ThreadId[7]);
	if (ThreadHandle[7] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 7);
		return -2;
	}

	ThreadHandle[8] = CreateThread(NULL, 0, IsRegValid, &params[8], 0, &ThreadId[8]);
	if (ThreadHandle[8] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 8);
		return -2;
	}
	ThreadHandle[9] = CreateThread(NULL, 0, IsRegValid, &params[9], 0, &ThreadId[9]);
	if (ThreadHandle[9] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 9);
		return -2;
	}
	ThreadHandle[10] = CreateThread(NULL, 0, IsRegValid, &params[10], 0, &ThreadId[10]);
	if (ThreadHandle[10] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 10);
		return -2;
	}
	ThreadHandle[11] = CreateThread(NULL, 0, IsRegValid, &params[11], 0, &ThreadId[11]);
	if (ThreadHandle[11] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 11);
		return -2;
	}
	ThreadHandle[12] = CreateThread(NULL, 0, IsRegValid, &params[12], 0, &ThreadId[12]);
	if (ThreadHandle[12] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 12);
		return -2;
	}
	ThreadHandle[13] = CreateThread(NULL, 0, IsRegValid, &params[13], 0, &ThreadId[13]);
	if (ThreadHandle[13] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 13);
		return -2;
	}
	ThreadHandle[14] = CreateThread(NULL, 0, IsRegValid, &params[14], 0, &ThreadId[14]);
	if (ThreadHandle[14] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 14);
		return -2;
	}
	ThreadHandle[15] = CreateThread(NULL, 0, IsRegValid, &params[15], 0, &ThreadId[15]);
	if (ThreadHandle[0] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 15);
		return -2;

	}

	// two for the columns
	//TO-BE_COMPLETED
	ThreadHandle[15] = CreateThread(NULL, 0, IsRegValid, &params[15], 0, &ThreadId[15]);
	if (ThreadHandle[0] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 15);
		return -2;

	}

	ThreadHandle[15] = CreateThread(NULL, 0, IsRegValid, &params[15], 0, &ThreadId[15]);
	if (ThreadHandle[0] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 15);
		return -2;

	}
	//two for the rows
	//TO-BE_COMPLETED

	params[16].colStart = 0;
	params[16].colEnd = 7;
	params[16].flagIndex = 16;

	ThreadHandle[16] = CreateThread(NULL, 0, IsColValid, &params[16], 0, &ThreadId[16]); 
	if (ThreadHandle[16] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 16);
		return -2;
	}

	params[17].rowStart = 8;
	params[17].rowEnd = 15;
	params[17].flagIndex = 17;

	ThreadHandle[17] = CreateThread(NULL, 0, IsRowValid, &params[17], 0, &ThreadId[17]); 
	if (ThreadHandle[17] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 17);
		return -3;
	}


	//this example checks that all the numbers are valid (only values 1-9 and A-G)
	params[18].rowStart = 0;
	params[18].rowEnd = 7;
	params[18].flagIndex = 18;

	ThreadHandle[18] = CreateThread(NULL, 0, IsRowValid, &params[18], 0, &ThreadId[18]); /* returns the thread identifier */
	if (ThreadHandle[18] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 18);
		return -2;
	}

	params[19].rowStart = 8;
	params[19].rowEnd = 15;
	params[19].flagIndex = 19;

	ThreadHandle[19] = CreateThread(NULL, 0, IsRowValid, &params[19], 0, &ThreadId[19]); /* returns the thread identifier */
	if (ThreadHandle[19] == NULL) {
		fprintf(stderr, "Something went wrong creating thread " + 19);
		return -3;
	}

	/* now wait for the thread to finish */
	WaitForMultipleObjects(NumOfThreads, &ThreadHandle, TRUE, INFINITE);
	//WaitForSingleObject(ThreadHandle[18], INFINITE);
	//WaitForSingleObject(ThreadHandle[19], INFINITE);

	for (int i = 0; i < NumOfThreads; i++) {
		/* close the thread handle */
		CloseHandle(ThreadHandle[i]);
	}

	// If any of the entries in the valid array are 0, then the sudoku solution is invalid
	for (i = 18; i < NumOfThreads; i++) {
		if (valid[i] == 0) {
			printf("Puzzle has invalid digits (not all in the range 1-9 and A-G)\n");
			return EXIT_SUCCESS;
		}
	}
	printf("Puzzle has only valid digits.\n");
	return EXIT_SUCCESS;
}