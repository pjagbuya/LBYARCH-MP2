// G11 Member: Paul Josef P. Agbuya  S12
// G11 Member: Ken Ivan T. Cheng

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
extern void imgCvtGrayFloatToInt();

void imgCvtGrayFloatToInt_c(unsigned long long int height, unsigned long long int width, float** x, unsigned char** cvtX) {
	for (unsigned long long int i = 0; i < height; i++) {
		for (unsigned long long int j = 0; j < width; j++) {
			// Scale value to range [0, 255]
			float scaled_value = x[i][j] * 255.0f;

			// Implement ties to even rounding
			int lower = (int)floor(scaled_value); // Lower integer
			int upper = lower + 1;               // Upper integer
			float diff = scaled_value - lower;

			if (diff > 0.5f) {
				cvtX[i][j] = (unsigned char)upper; // Round up
			}
			else if (diff < 0.5f) {
				cvtX[i][j] = (unsigned char)lower; // Round down
			}
			else {
				// Tied case: Choose the even value
				cvtX[i][j] = (unsigned char)((lower % 2 == 0) ? lower : upper);
			}
		}
	}
}

double benchmark(unsigned long long int height, unsigned long long int width, float** x, unsigned char** cvtX,
	void(*function)(unsigned long long int, unsigned long long int, float**, unsigned char**)) {
	double total_time = 0;

	LARGE_INTEGER frequency, start, end;
	for (int i = 0; i < 30; i++) {
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);
		function(height, width, x, cvtX);
		QueryPerformanceCounter(&end);
		total_time += (double)(end.QuadPart - start.QuadPart) * 1e3 / (double)frequency.QuadPart;
	}
	return total_time;
}

void display(float** arr, unsigned long long int w, unsigned long long int h) {
	int i;
	int j;
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			printf("%.2f ", arr[i][j]);
		}
		printf("\n");

	}

}

void displayInt(unsigned char** arr, unsigned long long int w, unsigned long long int h) {
	int i;
	int j;
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			if (j == w - 1) {
				printf("%d", arr[i][j]);
			}
			else {
				printf("%d, ", arr[i][j]);
			}
		}
		printf("\n");
	}
}

void randInputs(float** arr, unsigned long long int w, unsigned long long int h) {
	int i;
	int j;

	int x;
	float z;

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			x = ((int)rand() / (float)(RAND_MAX)) * 100.0f;
			z = x * 0.01f;								// Max amount from 0.000000 to 1.000000		
			z = ((int)(z * 100 + .5) / 100.0);			// Truncate the result
			arr[i][j] = z;
		}
	}
}

float** readMatrixFromFile(const char* filename, unsigned long long int* height, unsigned long long int* width) {
	FILE* file = NULL; // Initialize file pointer to NULL
	float** matrix = NULL;
	int i, j;

	// Open the file
	if (fopen_s(&file, filename, "r") != 0) {
		perror("Error opening file");
		return NULL;
	}

	// Read matrix dimensions
	if (fscanf_s(file, "%llu %llu", height, width) != 2) {
		fprintf(stderr, "Error: Unable to read matrix dimensions\n");
		fclose(file);
		return NULL;
	}

	// Allocate memory for the matrix
	matrix = (float**)malloc((*height) * sizeof(float*));
	if (matrix == NULL) {
		fprintf(stderr, "Error: Memory allocation failed\n");
		fclose(file);
		return NULL;
	}

	for (i = 0; i < *height; i++) {
		matrix[i] = (float*)malloc((*width) * sizeof(float));
		if (matrix[i] == NULL) {
			fprintf(stderr, "Error: Memory allocation failed\n");

			// Free previously allocated rows
			for (int k = 0; k < i; k++) {
				free(matrix[k]);
			}
			free(matrix);

			fclose(file);
			return NULL;
		}
	}

	// Read matrix values
	for (i = 0; i < *height; i++) {
		for (j = 0; j < *width; j++) {
			if (fscanf_s(file, "%f", &matrix[i][j]) != 1) {
				fprintf(stderr, "Error: Unable to read matrix value at [%d][%d]\n", i, j);

				// Free allocated memory
				for (int k = 0; k < *height; k++) {
					free(matrix[k]);
				}
				free(matrix);

				fclose(file);
				return NULL;
			}
		}
	}

	// Close the file
	fclose(file);
	return matrix;
}

int main() {
	int tests[] = { 10, 100, 1000 };
	int testSize = sizeof(tests) / sizeof(tests[0]);
	int run_times = 30;

	int i;
	int j;
	char buffer[100];
	double time_taken, time_taken_c;
	unsigned long long int height = 0, width = 0;
	float** x = NULL;
	unsigned char** cvtX = NULL;

	srand((unsigned int)time(NULL));

	printf("Enter input filename: ");
	scanf_s("%s", buffer, sizeof(buffer));  // Read filename input from the user

	// Call the function to read the matrix from the file
	x = readMatrixFromFile(buffer, &height, &width);

	printf("Original image:\n");
	display(x, width, height);

	
	char ans;
	do {
		printf("Do you want to generate tests? (Y/N)\n");
		scanf_s("%c", &ans);

		switch (ans) {
		case 'Y':
		case 'y':
			break;
		case 'N':
		case 'n':
			return 0;
		default:
			printf("Invalid input.\n\n");
		}

	} while (ans != 'Y' && ans != 'y');

	for (int iteration = 0; iteration < testSize; iteration++) {
		double sum_time = 0, sum_time_c = 0;
		unsigned long long int ROW_BYTES = tests[iteration] * sizeof(float*);
		unsigned long long int COL_BYTES = tests[iteration] * sizeof(float);

		unsigned long long int INT_ROW_BYTES = tests[iteration] * sizeof(unsigned char*);
		unsigned long long int INT_COL_BYTES = tests[iteration] * sizeof(unsigned char);

		float** x = (float**)malloc(ROW_BYTES);
		unsigned char** cvtX = (unsigned  char**)malloc(INT_ROW_BYTES);

		for (i = 0; i < tests[iteration]; i++) {
			x[i] = (float*)malloc(COL_BYTES);
			cvtX[i] = (unsigned char*)malloc(INT_COL_BYTES);
		}

		printf("Running tests for matrix size %d x %d:\n\n", tests[iteration], tests[iteration]);
		for (int ctr = 0; ctr < run_times; ctr++) {
			randInputs(x, tests[iteration], tests[iteration]);
			time_taken = benchmark(tests[iteration], tests[iteration], x, cvtX, imgCvtGrayFloatToInt);
			sum_time += time_taken;

			time_taken_c = benchmark(tests[iteration], tests[iteration], x, cvtX, imgCvtGrayFloatToInt_c);
			sum_time_c += time_taken_c;

			/*if (ctr == 0) {
				printf("Original image values: \n");
				display(x, width, height);
				printf("\n\nConverted Grayscale: \n");
				displayInt(cvtX, width, height);
			}*/

			// Display the time taken for the first 3 iterations
			if (ctr < 3) {
				printf("\tRun %d: Time taken for asm: %fms      |      Time taken in c: %fms\n", ctr + 1, time_taken, time_taken_c);
			}
		}
		printf("\n");
		printf("Time average asm = %fms\n", (sum_time) / run_times);
		printf("Time average c = %fms\n", (sum_time_c) / run_times);
		printf("===========================\n\n");

		free(x);
		free(cvtX);
	}
	return 0;
}