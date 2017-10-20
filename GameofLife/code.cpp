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

char* tochar(string str)
{
	char* writable = new char[str.size() + 1];
	std::copy(str.begin(), str.end(), writable);
	writable[str.size()] = '\0';
	return writable;
}

int cntN(vector<string> s, int i, int j)
{
	int c=0;
	int x[] = {-1, 0, 1};
	int y[] = {-1, 0, 1};

	for(int a=0 ; a<3 ; a++)
		for(int b=0 ; b<3 ; b++)
		{
			int x1 = x[a];
			int y1 = y[b];
			if(i+x1<s.size() && i+x1>-1 && j+y1<s.size() && j+y1>-1 && s[i+x1][j+y1]=='1')
			{
			//	cout	<< i+x1 << " " << j+y1 << " " << s[i+x1][j+y1] << endl;
				c++;
			}
		}

	if(s[i][j]=='1')
		c--;

	return c;
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
		
		vector<string> mat;
		
		while(getline(stream, line))
		{
			mat.pb(line);
		}
	
		cout	<< "T0" << endl;
		forn(i, line_count)
		{
			forn(j, line_count)
				if(mat[i][j]=='1')
					cout	<< "◼ ";
				else
					cout	<< "◻ ";

			cout << endl;
		}	

		cout << endl;

		int ticks = stoi(argv[2]);

		forn(i, ticks)
		{	
			forn(j, line_count)
			{	
				string line = mat[j];
				MPI_Bcast(tochar(line), line.size(), MPI_CHAR, 0, MPI_COMM_WORLD);
				MPI_Barrier(MPI_COMM_WORLD);
			}

			vector<string> ans;

			int c = 1;
			for(int i=0 ; i<line_count ; i++)
			{
					char* l = new char[line_count+1];
					if(i!=0 && i%nd==0)
						c++;
					MPI_Recv(l, line_count, MPI_CHAR, c, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					string t(l, line_count);
					delete[] l;
					ans.pb(t);
			}
			cout	<< "T" << i+1 << endl;

			forn(i, sz(ans)) 
			{
				forn(j, line_count)
					if(ans[i][j]=='1')
						cout	<< "◼ ";
					else
						cout	<< "◻ ";

				cout << endl;
			}
			cout << endl;

			mat.swap(ans);
		}
	}
	else
	{
		MPI_Bcast(&line_count, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

		MPI_Barrier(MPI_COMM_WORLD);

		nd = ceil(dbl(line_count)/dbl(world_size-1));

		int ticks = stoi(argv[2]);
		forn(q, ticks)
		{
			vector<string> sv;
			forn(i, line_count)
			{
				char* l = new char[line_count+1];
				MPI_Bcast(l, line_count, MPI_CHAR, 0, MPI_COMM_WORLD);
				MPI_Barrier(MPI_COMM_WORLD);
				string t(l, line_count);
				delete[] l;
				sv.pb(t);
			}

			vector<string> ansv(sv);

			forn(i, nd)
			{
				if(((world_rank-1)*nd)+i>=line_count)
					break;
				int x = ((world_rank-1)*nd)+i;
				forn(j, line_count)
				{
					int count = cntN(sv, x, j);
					if(count<2 || count>3)
						ansv[x][j] = '0';
					else if(count==3)
						ansv[x][j] = '1';
			//		if(x==4 && j==1)	
				//		cout	<< q << " " << x << " " << j << " " << count << " " << ansv[x][j] << endl;
				}
		//		cout	<< world_rank << " " << x << " " << ansv[x] << endl;
			}

			forn(i, nd)
			{
				if(((world_rank-1)*nd)+i>=line_count)
					break;

				int x = ((world_rank-1)*nd)+i;
				MPI_Send(ansv[x].c_str(), ansv[x].size(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);	
	MPI_Finalize();

	return 0;
}
