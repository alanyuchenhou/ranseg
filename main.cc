#define ARGUMENT_COUNT 6

#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <mpi.h>
#include <sys/time.h>

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;

void matrixMultMod(int* M1,int* M2,int modP)
{
	int temp[4];
	//cout<<"multing:"<<M1[0]<<" "<<M1[1]<<" "<<M1[2]<<" "<<M1[3]<<" with "<<M2[0]<<" "<<M2[1]<<M2[2]<<" "<<M2[3]<<'\n';
	temp[0]=(M1[0]*M2[0]+M1[1]*M2[2])%modP;
	temp[1]=(M1[0]*M2[1]+M1[1]*M2[3])%modP;
	temp[2]=(M1[2]*M2[0]+M1[3]*M2[2])%modP;
	temp[3]=(M1[2]*M2[1]+M1[3]*M2[3])%modP;
	M1[0]=temp[0];
	M1[1]=temp[1];
	M1[2]=temp[2];
	M1[3]=temp[3];
	//cout<<"result is:"<<M1[0]<<" "<<M1[1]<<" "<<M1[2]<<" "<<M1[3]<<'\n';
}

int* parallelPrefix(int* out, int rank, int process_count,int p0)
{
	int local[4]={1, 0, 0, 1};
	int in[4];
	bool done=false;
	int mate=rank;
	int stage=0; //this counts on which stage are we in, so we know which ranks will receive their required matrix
	for (int i=1; i<process_count;i=i*2)
	{
		if(rank%(2*i) < (i))
		{
			mate=rank+i;
		}
		else
		{
			mate=rank-i;
		}
		MPI_Status status;
		MPI_Sendrecv(&out[0],4,MPI_INT,mate,0,&in[0],4,MPI_INT,mate,0,MPI_COMM_WORLD,&status);

		//update the local matrix
		if(!done && mate<rank)
		{
			matrixMultMod(&local[0],in,p0);
		}

		//in this case it already has what it needs
		if(ceil(log2(rank))==stage)
			{
				done=true;
			}
		stage++;
		//in any case update the global matrix:
		matrixMultMod(&out[0],in,p0);
	}
	return local;
}

long getTime (struct timeval time_begin, struct timeval time_end) {
  return (time_end.tv_sec*1000000 + time_end.tv_usec) -
    (time_begin.tv_sec*1000000 + time_begin.tv_usec);
}

int main(int argc, char **argv) {
  assert(argc == ARGUMENT_COUNT);
  int input_size = atoi(argv[1]);
  int a0 = atoi(argv[2]);
  int b0 = atoi(argv[3]);
  int p0 = atoi(argv[4]);
  int x0 = atoi(argv[5]);
  assert(input_size > 0);
  assert(a0 > 0);
  assert(b0 > 0);
  assert(p0 > 0);
  assert(x0 > 0);
  int rank;
  int process_count;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &process_count);
  
  //matrix initializaion and parallel prefix call
  int M[4]={a0, 0, b0, 1};
  //int M[4]={1, 2, -2, -1};
  int* ans=new int[4];
  ans=parallelPrefix(M,rank,process_count,p0);
  cout<<"\n"<<rank<<" says:"<<ans[0]<<" "<<ans[1]<<" "<<ans[2]<<" "<<ans[3]<<'\n';

  struct timeval time_begin;
  if(rank == 0)
    gettimeofday(&time_begin, NULL);
  struct timeval time_end;
  long runtime = 0;
  if(rank == 0) {
    gettimeofday(&time_end, NULL);
    runtime += getTime(time_begin, time_end);
    printf("%d %d %ld \n", process_count, input_size, runtime);
  }
  MPI_Finalize();
  return 0;
}
