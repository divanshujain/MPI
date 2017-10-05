#include <bits/stdc++.h>
#include <mpi.h>
#include <fstream>

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

unsigned line_count;
int nd;
string line;

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
		ifstream myfile(argv[1]);
		ifstream stream(argv[1]);
		myfile.unsetf(ios_base::skipws);
		line_count = count(
				istream_iterator<char>(myfile),
				istream_iterator<char>(), 
				'\n');
		
		nd = ceil(dbl(line_count)/dbl(world_size-1));

		MPI_Bcast(&line_count, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

		MPI_Barrier(MPI_COMM_WORLD);
		
		int i=0, c=1;
		while(getline(stream, line))
		{
			if(i!=0 && i%nd==0)
				c++;
			int tsize = line.size();
			MPI_Send(&tsize, 1, MPI_INT, c, 0, MPI_COMM_WORLD);
			MPI_Send(line.c_str(), line.size(), MPI_CHAR, c, 0, MPI_COMM_WORLD);
			i++;
		}
		
		vector<string> ans;

		for(int i=1 ; i<world_size ; i++)
		{
			int s;
			MPI_Recv(&s, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			forn(j, s)
			{
				int len;
				MPI_Recv(&len, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				char* l = new char[len+1];
				MPI_Recv(l, len, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				string t(l, len);
				delete[] l;
				ans.pb(t);
			}
		}

		forn(i, sz(ans))
			cout	<< ans[i] << "\n" << endl;

		cout	<< "total=" << sz(ans) << endl;
	}
	else
	{
		MPI_Bcast(&line_count, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

		MPI_Barrier(MPI_COMM_WORLD);

		nd = ceil(dbl(line_count)/dbl(world_size-1));

		vector<string> sv;
		vector<string> ansv;

		forn(i, nd)
		{
			if(((world_rank-1)*nd)+i>=line_count)
				break;

			int len;
			MPI_Recv(&len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			char* l = new char[len+1];
			MPI_Recv(l, len, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			string t(l, len);
			delete[] l;
			sv.pb(t);
		}

		forn(i, sz(sv))
		{
			string tline = sv[i];
			string token(argv[2]);
			if(tline.find(token)!=string::npos)
				ansv.pb(tline);
		}

		int sans = sz(ansv);
		MPI_Send(&sans, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

/*		forn(i, sz(ansv))
			cout	<< ansv[i] << endl;
*/
		forn(i, sz(ansv))
		{
			int tsize = ansv[i].size();
			MPI_Send(&tsize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			MPI_Send(ansv[i].c_str(), ansv[i].size(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
		}

	}

	MPI_Barrier(MPI_COMM_WORLD);	
	MPI_Finalize();

	return 0;
}
