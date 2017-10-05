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

class Compare
{
	public:
		int operator() (const pi &n1, const pi &n2)
		{
			return n1.first > n2.first;
		}
};

int* merge(int* arr, int s, int nd)
{
	int* ret = new int[s];
	priority_queue <pi, vector< pi >, Compare > pq;
	int nb = s/nd, x=0;
	
	forn(i, nb)
	{
		pq.push(mp(arr[i*nd], i*nd));
	}

	while(!pq.empty())
	{
		pi t = pq.top();
		pq.pop();
		ret[x++] = t.first;
		if(t.second%nd!=(nd-1))
		{
			pq.push(mp(arr[t.second+1],t.second+1));
		}
	}

	return ret;	
}

int main ( int argc, char *argv[] )
{
	ios::sync_with_stdio(0);

	MPI_Init(NULL, NULL);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	int *nums;
	int nd, s, r;

	if(world_rank == 0)
	{
		cin	>> s;

		r = (world_size - (s%world_size))%world_size;

		nums = new int[s+r];
		
		forn(i, s)
			cin	>> nums[i];

		for(int i=s ;  i<s+r ; i++)
			nums[i] = INT_MAX;
		
		nd = (s+r)/world_size;
		
	}

		MPI_Bcast(&nd, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		MPI_Barrier(MPI_COMM_WORLD);
		
		int *suba = new int[nd];

		MPI_Scatter(nums, nd, MPI_INT, suba, nd, MPI_INT, 0, MPI_COMM_WORLD);

		sort(suba, suba+nd);
		
	
		int *snums;	
		if(world_rank == 0)
		{
			snums = new int[s+r];
		}

		MPI_Gather(suba, nd, MPI_INT, snums, nd, MPI_INT, 0, MPI_COMM_WORLD);
		
		if(world_rank == 0)
		{

	/*		cout	<< "gathered final array" << endl;
			forn(i, s)
				cout	<< snums[i] << endl;*/

			snums = merge(snums, s+r, nd);

	//		cout	<< "merged final array" << endl;

			forn(i, s)
				cout	<< snums[i] << " ";

			cout << endl;

		}

		cout	<< "Process " << world_rank << " :" << endl;
		
		forn(i, nd)
		{
			cout	<< suba[i] << endl;
		}
		
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Finalize();

	return 0;
}
