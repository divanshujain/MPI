#include <iomanip>
#include <math.h>
#include <iostream>
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

dbl fact(dbl f) {
    dbl iter = 1;
    for(dbl i=1 ; i<=f ; i++) {
        iter *= i;
    }
    return iter;
}

dbl compute(int world_rank, dbl x, int nd) {
    dbl ans=0;
    dbl ini = 2*(world_rank*nd)+1;
    dbl f = fact(ini);
    dbl num = pow(x, ini);
    dbl sign = pow(-1, world_rank*nd);
    ini++;
    for(int i=0 ; i<nd ; i++, ini++) {
        dbl term = sign * (num/f);
        sign *= -1;
        num *= (x*x);
        f *= ini;
        ans += term;
    }
//    cout << world_rank << ":" << ans << endl;
    return ans;
}

int main ( int argc, char *argv[] )
{
	ios::sync_with_stdio(0);

	MPI_Init(NULL, NULL);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	int n, nd;
    dbl x;

	if(world_rank == 0)
	{
		cin	>> x >> n;

        nd = ceil(dbl(n)/dbl(world_size));
	}

	MPI_Bcast(&nd, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
    MPI_Bcast(&x, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    dbl interm_ans = compute(world_rank, x, nd);

    if(world_rank == 0) {
        
        dbl ans = 0;
        ans += interm_ans;

        for(int i=1 ; i<world_size ; i++) {
            MPI_Recv(&interm_ans, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            ans += interm_ans;
        }

        cout << fixed << setprecision(2) << ans << endl;
        
    }
    else {
        MPI_Send(&interm_ans, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
		
    MPI_Finalize();

	return 0;
}
