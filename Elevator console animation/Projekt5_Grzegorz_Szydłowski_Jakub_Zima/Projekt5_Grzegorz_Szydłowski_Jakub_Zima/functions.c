#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<windows.h>
#include<math.h>

void clear()
{
	printf("\033[2J\033[H");
}

void delay(double number_of_seconds)
{
	int milli_seconds = 1000 * number_of_seconds;

	clock_t start_time = clock();

	while (clock() < start_time + milli_seconds);
}

int generate_poisson(double lambda) {
	double L = exp(-lambda);
	int k = 0;
	double p = 1.0;

	do {
		k++;
		double u = ((double)rand() / (double)(RAND_MAX));
		p *= u;
	} while (p > L);

	return k - 1;
}

void reset_elevator(char elevator[Height][Width])
{
	for (int i = 0; i < Height; ++i)
	{
		elevator[i][8] = ' ';
		elevator[i][9] = ' ';
	}
}

void print_elevator(char elevator[Height][Width], int current_passengers, int elevator_capacity)
{
	printf("Elevator capacity: %d\n", elevator_capacity); //Current passenger count: %d\n  current_passengers,
	for (int i = 0; i < Height; ++i)
	{
		for (int j = 0; j < Width; ++j)
		{
			printf("%c", elevator[i][j]);
		}
		printf("\n");
	}
}

void elevator_animation(char elevator[Height][Width], int current_passengers, int elevator_capacity, int direction)
{
	reset_elevator(elevator);
	print_elevator(elevator, current_passengers, elevator_capacity);
	clear();
	delay(1.25);
	for (int i = 0; i < Height; ++i)
	{
		if (direction == 1) //going up
		{
			elevator[Height - i][8] = '/';
			elevator[Height - i][9] = '\\';
			print_elevator(elevator, current_passengers, elevator_capacity);
			delay(1.25);
			clear();
		}
		if (direction == -1)// going down
		{
			elevator[i][8] = '\\';
			elevator[i][9] = '/';
			print_elevator(elevator, current_passengers, elevator_capacity);
			delay(1.25);
			clear();
		}
		if (direction == 0)//static image
		{
			reset_elevator(elevator);
			print_elevator(elevator, current_passengers, elevator_capacity);
			delay(1.25);
			clear();
		}
	}
	reset_elevator(elevator);
	print_elevator(elevator, current_passengers, elevator_capacity);
}