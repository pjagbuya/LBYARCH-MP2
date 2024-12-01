// G11 Member: Paul Josef P. Agbuya  S12
// G11 Member: Ken Ivan T. Cheng

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <math.h>
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
	FILE* file = NULL; 
	float** matrix = NULL;
	int i, j;

	if (fopen_s(&file, filename, "r") != 0) {
		perror("Error opening file");
		return NULL;
	}

	if (fscanf_s(file, "%llu %llu", height, width) != 2) {
		fprintf(stderr, "Error: Unable to read matrix dimensions\n");
		fclose(file);
		return NULL;
	}

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

			for (int k = 0; k < i; k++) {
				free(matrix[k]);
			}
			free(matrix);

			fclose(file);
			return NULL;
		}
	}

	for (i = 0; i < *height; i++) {
		for (j = 0; j < *width; j++) {
			if (fscanf_s(file, "%f", &matrix[i][j]) != 1) {
				fprintf(stderr, "Error: Unable to read matrix value at [%d][%d]\n", i, j);

				for (int k = 0; k < *height; k++) {
					free(matrix[k]);
				}
				free(matrix);

				fclose(file);
				return NULL;
			}
		}
	}

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

	FILE* file = NULL;

	do {
		printf("Enter input filename: ");
		scanf_s("%255s", buffer, (unsigned int)sizeof(buffer)); // Read filename

		if (fopen_s(&file, buffer, "r") != 0) {
			printf("Error: File '%s' not found. Please try again.\n", buffer);
		}
		else {
			fclose(file); // Close the file as it's just being tested
			break;        // Exit the loop if the file is valid
		}
	} while (1); // Infinite loop until a valid file is found

	x = readMatrixFromFile(buffer, &height, &width);

	cvtX = (unsigned char**)malloc(height * sizeof(unsigned char*));

	for (int i = 0; i < width; i++) {
		cvtX[i] = (unsigned char*)malloc(width * sizeof(unsigned char));
	}

	printf("Original image:\n");
	display(x, width, height);

	time_taken = benchmark(height, width, x, cvtX, imgCvtGrayFloatToInt);
	time_taken_c = benchmark(height, width, x, cvtX, imgCvtGrayFloatToInt_c);

	printf("\nGrayscale image:\n");
	displayInt(cvtX, width, height);

	printf("\n");
	printf("===========================\n\n");
	printf("Time asm = %fms\n", time_taken);
	printf("Time c = %fms\n\n", time_taken_c);
	
	char ans;
	int valid_input;

	do {
		printf("Do you want to generate tests? (Y/N): ");
		valid_input = scanf_s(" %c", &ans, 1);

		while (getchar() != '\n');

		if (valid_input == 1) {
			ans = tolower(ans); 
			if (ans == 'y') {
				printf("Generating tests...\n");
				break;
			}
			else if (ans == 'n') {
				printf("Exiting...\n");
				return 0;
			}
			else {
				printf("Invalid input. Please enter 'Y' or 'N'.\n\n");
			}
		}
		else {
			printf("Invalid input. Please enter 'Y' or 'N'.\n\n");
		}
	} while (1);

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

			if (ctr == 0) {
				printf("Original image values: \n");
				display(x, width, height);
				printf("\n\nConverted Grayscale: \n");
				displayInt(cvtX, width, height);
			}

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