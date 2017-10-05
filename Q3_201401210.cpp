#include <bits/stdc++.h>
#include <mpi.h>
#include <unistd.h>

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
int sum, *ssum;

int minkey(int Prank, int* key, bool* mstSet)
{

	int min = INT_MAX, min_index = 0;

	for (int v = 0; v < nd; v++)
		if (mstSet[((Prank-1)*nd)+v] == false && key[v] < min)
			min = key[v], min_index = v;

	return min_index;
}

int minkey1(int* narr, int* narrkey, bool* mstSet)
{
	int min = INT_MAX, min_index, s = (sizeof(narr)/sizeof(*narr))+1;
	
	/*forn(i, s)
		cout	<< "Array : " << narr[i] << " " << narrkey[i] << endl;
	cout << endl;
*/
	for(int i=1 ; i<s ; i++)
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

	anum = new int[world_size];
	anumkey = new int[world_size];
	ssum = new int[world_size];

	if(world_rank == 0)
	{
		cin >> v >> e;
		
		mstSet = new bool[v];
		graph = new int*[v];
		forn(i, v)
			graph[i] = new int[v], mstSet[i] = false;

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
		
		MPI_Barrier(MPI_COMM_WORLD);	

		MPI_Bcast(&v, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		forn(i, v)
			MPI_Bcast(graph[i], v, MPI_INT, 0, MPI_COMM_WORLD);
		
		MPI_Bcast(&nd, 1, MPI_INT, 0, MPI_COMM_WORLD);

		MPI_Barrier(MPI_COMM_WORLD);	
		forn(count, v-1)
		{

			MPI_Barrier(MPI_COMM_WORLD);	
			MPI_Gather(&num, 1, MPI_INT, anum, 1, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Gather(&numkey, 1, MPI_INT, anumkey, 1, MPI_INT, 0, MPI_COMM_WORLD);

			u = minkey1(anum, anumkey, mstSet);

			mstSet[u] = true;

			MPI_Bcast(&u, 1, MPI_INT, 0, MPI_COMM_WORLD);

		}
		
		sum = 0;
		MPI_Gather(&sum, 1, MPI_INT , ssum, 1, MPI_INT, 0, MPI_COMM_WORLD);

		for(int i=1 ; i<world_size ; i++)
			sum += ssum[i];

		cout	<< sum << endl;
	}
	else
	{
		MPI_Barrier(MPI_COMM_WORLD);	
		
		MPI_Bcast(&v, 1, MPI_INT, 0, MPI_COMM_WORLD);
		mstSet = new bool[v];
		graph = new int*[v];
		forn(i, v)
			graph[i] = new int[v], mstSet[i] = false;

		forn(i, v)
			MPI_Bcast(graph[i], v, MPI_INT, 0, MPI_COMM_WORLD);
		
		MPI_Bcast(&nd, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		MPI_Barrier(MPI_COMM_WORLD);
		parent = new int[nd];
		key = new int[nd];
		forn(i, nd)
			key[i] = INT_MAX, parent[i] = -1;
		
		if(world_rank==1)
		{
			key[0] = 0;
		}

		forn(count, v-1)
		{
			num = minkey(world_rank, key, mstSet);
			numkey = key[num];
			num = ((world_rank-1)*nd)+num;

			MPI_Barrier(MPI_COMM_WORLD);	
			MPI_Gather(&num, 1, MPI_INT, anum, 1, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Gather(&numkey, 1, MPI_INT, anumkey, 1, MPI_INT, 0, MPI_COMM_WORLD);

			MPI_Bcast(&u, 1, MPI_INT, 0, MPI_COMM_WORLD);

			mstSet[u] = true;

			forn(i, nd)
			{
				int V = ((world_rank-1)*nd) + i;
				if(V>=v)
					continue;

				if(graph[u][V] && mstSet[V] == false && graph[u][V] < key[i])
					parent[i] = u, key[i] = graph[u][V];
			}

		}
		
		sum = 0;

		forn(i, nd)
		{
			if(((world_rank-1)*nd)+i>=v)
				continue;
		//	cout	<< parent[i]+1 << " " << ((world_rank-1)*nd)+i+1 << " " << graph[((world_rank-1)*nd)+i][parent[i]] << endl; 
			sum += graph[((world_rank-1)*nd)+i][parent[i]];
		}

	//	cout	<< sum << endl;
		MPI_Gather(&sum, 1, MPI_INT , ssum, 1, MPI_INT, 0, MPI_COMM_WORLD);
	}

	MPI_Barrier(MPI_COMM_WORLD);	
	MPI_Finalize();

	return 0;
}
