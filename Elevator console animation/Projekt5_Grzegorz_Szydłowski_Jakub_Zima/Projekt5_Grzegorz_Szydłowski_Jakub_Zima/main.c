/*Projekt ma na celu optymalizacjê przejazdu widny po piêtrach.
Program ma umo¿liwiaæ podanie pojemnoœci windy oraz liczbê piêter
(co najmniej 5). Liczba osób oczekuj¹cych
na windê na danym piêtrze powinna
generowana zgodnie z rozk³adem Poissona. Program powinien
pokazywaæ pseudo-animacjê przejazdu windy (tak¿e liczbê osób w windzie).*/

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>

#define Height 5
#define Width 10

char elevator[Height][Width] =
{
	{"___|___   "},
	{"|  o  |   "},
	{"| \\|/ |   "},
	{"| / \\ |   "},
	{"|_____|   "}
};

void clear()
{
	printf("\033[2J\033[H");
}

void delay(double number_of_seconds)
{
	double milli_seconds = 1000 * number_of_seconds;

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
			elevator[Height - i - 1][8] = '/';
			elevator[Height - i - 1][9] = '\\';
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

int main()
{
	srand((unsigned int)time(NULL));
	int current_passengers = 0;
	//int destination_floor;
	int elevator_capacity;
	int floors;
	int current_floor = 0;
	int direction = 0;
	printf("Enter how many floors do you want the building to have (at least  5).\n");
	if (scanf_s("%d", &floors) != 1 || floors < 5)
	{
		printf("Input error!\nTry again!\n");
		return 1;
	};
	printf("Enter the elevator capacity.\n");
	if (scanf_s("%d", &elevator_capacity) != 1 || elevator_capacity <= 0)
	{
		printf("Input error!\nTry again!\n");
		return 1;
	}
	int* waiting_passengers = (int*)malloc((floors + 1) * sizeof(int));
	if (waiting_passengers == NULL)
	{
		printf("Memory allocation error!\n");
		return 1;
	}
	for (int i = 1; i < floors + 1; i++)
	{
		waiting_passengers[i] = generate_poisson(2.5);
		printf("Floor %d - people waiting [%d] \n", i, waiting_passengers[i]);
	}
	printf("\n");
	printf("The elevator starts on the ground floor.\n");
	delay(5);
	elevator_animation(elevator, current_passengers, elevator_capacity, direction);
	while (1)
	{
		clear();
		//people enter at the current floor
		if (waiting_passengers[current_floor] > 0)
		{
			direction = -1;
			int can_enter = elevator_capacity - current_passengers;
			int enter = (waiting_passengers[current_floor] > can_enter) ? can_enter : waiting_passengers[current_floor];
			waiting_passengers[current_floor] -= enter;
			current_passengers += enter;
			elevator_animation(elevator, current_passengers, elevator_capacity, direction);
			delay(2);
			printf("The elevator arrives at the floor no. %d.\n", current_floor);
			printf("%d people enter.\n", enter);
			printf("Current passenger count: %d\n\n", current_passengers); //Elevator capacity: % , elevator_capacity
			delay(4);
		}
		//ckecking if all the passengers arrived to the ground floor
		int all_delivered = 1;
		for (int i = 1; i < floors + 1; i++)
		{
			if (waiting_passengers[i] > 0)
			{
				all_delivered = 0;
				break;
			}
		}
		//passengers getting off on the ground floor
		if (current_floor == 0 && current_passengers > 0)
		{
			direction = -1;
			elevator_animation(elevator, current_passengers, elevator_capacity, direction);
			delay(2);
			printf("The elevator arrives at the floor no. %d.\n", current_floor);
			printf("%d people get off.\n", current_passengers);
			current_passengers = 0;
			delay(4);
		}
		if (all_delivered && current_floor == 0)
		{
			printf("The simulation has ended\n");
			break;
		}
		//the elevator going to the ground floor
		if (current_floor > 0 && current_passengers == elevator_capacity)
		{
			direction = -1;
			current_floor--;
			elevator_animation(elevator, current_passengers, elevator_capacity, direction);
			delay(2);
			printf("The elevator is heading to the floor no. %d\n", current_floor);
			delay(4);
		}
		//checking all the floors above ground
		for (int i = 1; i < floors + 1; i++)
		{
			if (waiting_passengers[i] > 0 && current_passengers < elevator_capacity)
			{

				int can_enter = elevator_capacity - current_passengers;
				int enter = (waiting_passengers[i] > can_enter) ? can_enter : waiting_passengers[i];
				waiting_passengers[i] -= enter;
				current_passengers += enter;
				direction = 1;
				elevator_animation(elevator, current_passengers, elevator_capacity, direction);
				delay(2);
				printf("Floor no. %d. \n%d people enter. \n", current_floor + i, enter);
				printf("Current passenger count: %d\n", current_passengers); //Elevator capacity: %d\n , elevator_capacity
				delay(4);
			}
		}
		delay(1);
	}
	free(waiting_passengers);
	return 0;
}