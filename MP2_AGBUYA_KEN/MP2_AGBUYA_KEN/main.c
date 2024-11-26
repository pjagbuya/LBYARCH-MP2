// G11 Member: Paul Josef P. Agbuya  S12
// G11 Member: Ken Ivan T. Cheng
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
extern void imgCvtGrayFloatToInt();


void display(float* arr, unsigned long long int w, unsigned long long int h) {
	int i;
	int j;
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			printf("%.2f ",  arr[i*w + j]);
		}
		printf("\n");
			
	}

}

void displayInt(int* arr, unsigned long long int w, unsigned long long int h) {
	int i;
	int j;
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			if (j == w - 1) {
				printf("%d", arr[i * w + j]);
			}
			else {
				printf("%d, ", arr[i * w + j]);
			}
			

		}
		printf("\n");

	}

}

void randInputs(float* arr, unsigned long long int w, unsigned long long int h) {
	int i;
	int j;
	
	int x;
	float z;

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
			
		{

			x = ((int)rand() / (float)(RAND_MAX)) * 100; // Max amount from 0.00 to 1.00

			z = x*0.01;
			arr[i*w + j] = z;
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

	unsigned long long int ARRAY_SIZE = height*width;
	unsigned long long int ARRAY_BYTES = ARRAY_SIZE * sizeof(float);
	

	
	clock_t start, end; 
	double time_taken;
	
	float* x = malloc(ARRAY_BYTES);
	int* cvtX = malloc(ARRAY_BYTES);


	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			cvtX[i*width + j] = 0;


	
	randInputs(x, width, height);
	imgCvtGrayFloatToInt(height, width, x, cvtX);
	displayInt(cvtX, width, height);

	return 0;
}