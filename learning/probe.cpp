#include <mpi.h>
#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char** argv) {
	MPI_Init(NULL, NULL);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	int number_amount;
	
	if (world_rank == 0) 
	{
		int size, i;
		cin	>> size;
	
		vector<int> numbers;
		
		for(i=0; i<size ; i++)
		{
			cin	>> t;
			numbers.push_back(t);
		}

		number_amount = size;

		MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
		printf("0 sent %d numbers to 1\n", number_amount);
	} 
	else 
	{
		MPI_Status status;
		MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &number_amount);

		int* number_buf = (int*)malloc(sizeof(int) * number_amount);
		
		MPI_Recv(number_buf, number_amount, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		printf("1 dynamically received %d numbers from 0.\n", number_amount);

		int i;
		for(i=0 ; i<number_amount ; i++)
			cout	<< number_buf[i] << endl;

		free(number_buf);
	}

	MPI_Finalize();
}
