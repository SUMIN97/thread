#define HAVE_STRUCT_TIMESPEC
#define NUMBER_OF_THREADS 11
#define PUZZLE_SIZE 9


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h >

#include <unistd.h>


void *column_worker(void *param);
void *row_worker(void *param);
void *subfield_worker(void *param);
void printMatrix();

using namespace std;

/*example puzzle*/
int puzzle[PUZZLE_SIZE + 1][PUZZLE_SIZE + 1] = {
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, 5, 3, 4, 6, 7, 8, 9, 1, 2},
	{-1, 6, 7, 2, 1, 9, 5, 3, 4, 8},
	{-1, 1, 9, 8, 3, 4, 2, 5, 6, 7},
	{-1, 8, 5, 9, 7, 6, 1, 4, 2, 3},
	{-1, 4, 2, 6, 8, 5, 3, 7, 9, 1},
	{-1, 7, 1, 3, 9, 2, 4, 8, 5, 6},
	{-1, 9, 6, 1, 5, 3, 7, 2, 8, 4},
	{-1, 2, 8, 7, 4, 1, 9, 6, 3, 5},
	{-1, 3, 4, 5, 2, 8, 6, 1, 7, 9}
};

int status_map[NUMBER_OF_THREADS] = { 0 };

/*data structure for passong data to threads */
typedef struct
{
	int thread_numbers; //thread 의 id
	int x;
	int y;
}parameters;

int main(int argc, char** argv)
{
    const char* str = argv[1];
    int result = 0;


    for(int row = 0; row < 9; row++)
    {
        for(int i = 0; i < 9; i++)
        {
            result = str[row * 9 +i]-'0';
            printf("%d\n",result);
            puzzle[row+1][i+1] = result;
        }    
    }

    
	int i, j, count, check;
	int rv = 1;
	count = check = 0;
	pthread_t workers[NUMBER_OF_THREADS];//thread declaration
	parameters *data = (parameters *)malloc(sizeof(parameters));
	data->thread_numbers = count;
	data->x = 1;
	data->y = 1;

	printMatrix();


	//create row check thread
	pthread_create(&workers[data->thread_numbers], 0, row_worker, data);

	count++;
	data = (parameters *)malloc(sizeof(parameters));
	data->thread_numbers = count;
	data->x = 1;
	data->y = 1;
	//create column check thread
	pthread_create(&workers[data->thread_numbers], 0, column_worker, data);

	count++;
	/*create the threads that check each subfield */
	for (i = 1; i < PUZZLE_SIZE; i = i + (PUZZLE_SIZE / 3))
	{
		for (j = 1; j < PUZZLE_SIZE; j = j + (PUZZLE_SIZE / 3))
		{
			data = (parameters *)malloc(sizeof(parameters));
			data->thread_numbers = count;
			printf("count : %d\n", count);
			data->x = i;
			data->y = j;
			pthread_create(&workers[data->thread_numbers], 0, subfield_worker, data);
			++count;
		}
	}

	for (i = 0; i < count; i++)
		pthread_join(workers[i], NULL); //waiting until finished all threads
	/*check the status map*/
	for (i = 0; i < NUMBER_OF_THREADS; i++)
	{
		if (status_map[i] == 0)
			rv = 0;
	}
	if (rv == 1)
	{
    	string out_line = "1";
    	ofstream out("test.txt");
		out << out_line << endl;
    	out.close();
	    printf(" ==> Result : Sudoku puzzle is valid\n");
	    
	}
	    
		//
	else
	{
    	string out_line = "0";
    	ofstream out("test.txt");
		out << out_line << endl;
    	out.close();
    	printf("%d\n",getpid());
	    printf(" ==> Result : Sudoku puzzle is invalid\n");
	    
	}
    return 0;
}

void *row_worker(void *params)
{
	int i, j, k, status;
	int map[PUZZLE_SIZE + 1] = { 0, };

	parameters *p = (parameters *)params;

	for (i = 1; i < PUZZLE_SIZE +1; i++)
	{
		for (j = 1; j < PUZZLE_SIZE + 1; j++)
		{
			map[puzzle[i][j]] = 1;
			printf("Thread[%d] => Row_Check Row : %d, Col : %d\n", p->thread_numbers, i, j);
		}

		status = 1;
		//map check
		for (k = 1; k < PUZZLE_SIZE + 1; k++)
		{
			if (map[k] == 0)
			{
				status = 0;
				break;
			}
		}

		if (status == 0)
			break;
		else
		{
			/* reinitialize the map*/
			for (i = 0; i < PUZZLE_SIZE; i++)
			{
				map[i] = 0;
				//initialize for row check
			}
		}
	}

	if (status)
		status_map[p->thread_numbers] = 1;
	pthread_exit(0);
}

void *column_worker(void *params)
{
	int i, j, k, status;
	int map[PUZZLE_SIZE + 1] = { 0, };

	parameters *p = (parameters *)params;

	//Col check
	for (i = 1; i < PUZZLE_SIZE + 1; i++)
	{
		for (j = 1; j < PUZZLE_SIZE + 1; j++)
		{
			map[puzzle[j][i]] = 1;
			printf("Thread[%d] => Cow_Check Row : %d, Col : %d\n", p->thread_numbers, j, i);
		}

		status = 1;
		for (k = 1; k < PUZZLE_SIZE + 1; k++)
		{
			if (map[k] == 0)
			{
				if (status == 0)
					break;
				else
				{
					for (i = 0; i < PUZZLE_SIZE + 1; i++)
					{
						map[i] = 0;
					}
				}
			}
		}
	}

	if (status)
		status_map[p->thread_numbers] = 1;
	pthread_exit(0);
}

void *subfield_worker(void *params)
{
	int i, j, k, status;
	int map[PUZZLE_SIZE + 1] = { 0, };

	parameters* p = (parameters *)params;

	for (i = p->x; i < p->x + (PUZZLE_SIZE / 3); i++)
	{
		for (j = p->y; j < p->y + (PUZZLE_SIZE / 3); j++)
		{
			map[puzzle[j][i]] = 1;
			printf("Thread[%d] => Cow: %d, rol: %d\n", p->thread_numbers, j, i);
		}
	}

	status = 1;
	for (k = 1; k < PUZZLE_SIZE + 1; k++)
	{
		if (map[k] == 0)
		{
			status = 0;
			break;
		}
	}
	if (status)
		status_map[p->thread_numbers] = 1;
	pthread_exit(0);

}

void printMatrix()
{
    int i,j;
    
    printf("Hello Sudoku Puzzle !\n");
    for(i = 1; i <= PUZZLE_SIZE; i++)
    {
        for (j = 1; j <= PUZZLE_SIZE; j++)
        {
            printf(" [%d] ", puzzle[i][j]);
        }
        printf("\n");
    }
}
