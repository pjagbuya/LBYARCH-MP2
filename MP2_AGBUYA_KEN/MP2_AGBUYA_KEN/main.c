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

void randInputs(float** arr, unsigned long long int w, unsigned long long int h) {
	int i;
	int j;

	int x;
	float z;

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)

		{

			x = ((int)rand() / (float)(RAND_MAX)) * 100.0f; // Max amount from 0.00 to 1.00

			z = x * 0.01f;
			arr[i][j] = z;
		}
	}


}


int main() {
	unsigned long long int height;
	unsigned long long int width;
	int i;
	int j;

	srand((unsigned int)time(NULL));
	scanf_s("%lld %lld", &height, &width);

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
		
			



	
	randInputs(x, width, height);
	imgCvtGrayFloatToInt(height, width, x, cvtX);
	display(x, width, height);
	printf("Converted Grayscale: \n");
	displayInt(cvtX, width, height);

	return 0;
}