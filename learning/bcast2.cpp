#include <bits/stdc++.h>
#include <mpi.h>

#define forn(i, n) for(int i = 0 ; (i) < (n) ; ++i)
#define pb push_back
#define mp make_pair
#define sz(a) ((int)(a).size())

using namespace std;

typedef long long ll;
typedef double dbl;
typedef vector<int> vi;
typedef vector<long long> vll;
typedef pair <int, int> pi;

ll m = 1e9+7;
dbl eps = 1e-6;

bool* mstSet;
int num, nd, v, u, e, *anum, numkey, *anumkey;
int** graph;
int* parent;
int* key;

int minkey(int Prank, int key[], bool mstSet[])
{

	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < nd; v++)
		if (mstSet[((Prank-1)*nd)+v] == false && key[v] < min)
			min = key[v], min_index = v;

	return min_index;
}

int minkey1(int narr[], int narrkey[], bool mstSet[])
{
	int min = INT_MAX, min_index, s = (sizeof(narr)/sizeof(*narr));

	forn(i, s)
	{
		if(narrkey[i] < min)
			min = narrkey[i], min_index = narr[i];
	}

	return min_index;
}


int main ( int argc, char *argv[] )
{
	ios::sync_with_stdio(0);
	MPI_Init(NULL, NULL);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


	if(world_rank == 0)
	{
		cin >> v >> e;


		graph = new int*[v];
		forn(i, v)
			graph[i] = new int[v];

		forn(i, e)
		{

			int v1, v2, w;
			cin >> v1 >> v2 >> w;
			v1--;
			v2--;
			graph[v1][v2] = w;
			graph[v2][v1] = w;
		}
		nd = ceil(dbl(v)/dbl(world_size-1));
	
		forn(i, v)
		{
			forn(j, v)	
				cout	<< graph[i][j] << " ";
			cout << endl;
		}

		MPI_Barrier(MPI_COMM_WORLD);	
		
		MPI_Bcast(&v, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		MPI_Barrier(MPI_COMM_WORLD);	
		
		forn(i, v)
		{
			MPI_Bcast(graph[i], v, MPI_INT, 0, MPI_COMM_WORLD);
		}

	//	MPI_Bcast(&nd, 1, MPI_INT, 0, MPI_COMM_WORLD);

		//MPI_Barrier(MPI_COMM_WORLD);	
		cout	<< "Process " << world_rank << " done" << endl;

	}
	else
	{
		MPI_Barrier(MPI_COMM_WORLD);	
		MPI_Bcast(&v, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);	
		graph = new int*[v];
		forn(i, v)
			graph[i] = new int[v];
		forn(i, v)
		{
			MPI_Bcast(graph[i], v, MPI_INT, 0, MPI_COMM_WORLD);
		}
//		MPI_Bcast(&nd, 1, MPI_INT, 0, MPI_COMM_WORLD);
//		MPI_Barrier(MPI_COMM_WORLD);
/*		parent = new int[nd];
		key = new int[nd];
		if(world_rank==1)
		{
			key[0] = 0;
			parent[0] = -1;
		}
		cout	<< nd << endl;*/

		cout	<< "Process " << world_rank << " done" << endl;

		forn(i, v)
		{
			forn(j, v)	
				cout	<< graph[i][j] << " ";
			cout << endl;
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);	
	MPI_Finalize();

	return 0;
}
