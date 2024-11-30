// G11 Member: Paul Josef P. Agbuya  S12
// G11 Member: Ken Ivan T. Cheng
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
extern void imgCvtGrayFloatToInt();


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
void scanInputs(float** arr) {
	int i;
	int j;
	float num;
	unsigned int w;
	unsigned int h;
	char trsh;
	FILE* fp;

	errno_t err = fopen_s(&fp, "input1.txt", "r"); // Add the file pointer argument


	if (err != 0) {
		printf("Error opening file\n", err);
		return 1;
	}
	fscanf_s(fp, "%d", &h);
	fscanf_s(fp, "%d", &w);

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			fscanf_s(fp, "%f", &arr[i][j]);

		}
	}

	fclose(fp);
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


int main() {
	unsigned long long int height;
	unsigned long long int width;
	FILE* fp;
	int i;
	int j;

	srand((unsigned int)time(NULL));
	scanf_s("%lld %lld", &height, &width);
	double sum;

	unsigned long long int ARRAY_SIZE = height * width;
	unsigned long long int ROW_BYTES = height * sizeof(float*);
	unsigned long long int COL_BYTES = width * sizeof(float);

	unsigned long long int INT_ROW_BYTES = height * sizeof(unsigned char*);
	unsigned long long int INT_COL_BYTES = width * sizeof(unsigned char);

	clock_t start, end;
	double time_taken;

	float** x = (float**)malloc(ROW_BYTES);
	unsigned char** cvtX = (unsigned  char**)malloc(INT_ROW_BYTES);

	for (i = 0; i < height; i++) {
		x[i] = (float*)malloc(COL_BYTES);
		cvtX[i] = (unsigned char*)malloc(INT_COL_BYTES);
	}

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			x[i][j] = 0.0;
			cvtX[i][j] = 0;
		}
	}

	sum = 0;
	randInputs(x, width, height);
	//scanInputs(x);


	for (i = 0; i < 30; i++) {
		start = clock();
		imgCvtGrayFloatToInt(height, width, x, cvtX);
		end = clock();
		time_taken = ((double)(end - start)*1000.0 / CLOCKS_PER_SEC);
		printf("Time in c, ms = %f \n", time_taken);
		sum += time_taken;
	}

	printf("Time average c, ms = %f \n", (sum/30.0));




	
	
	
	display(x, width, height);
	printf("Converted Grayscale: \n");
	displayInt(cvtX, width, height);

	return 0;
}