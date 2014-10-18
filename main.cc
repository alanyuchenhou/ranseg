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

//output, and parameter "out" should be changed to int*; 
//the line commented out was for parallel sum testing it should be replaced by matrix mult!
//output should be possibly changed to another thing! I'm not sure yet!
int parallelPrefix(int out, int rank, int process_count)
{
	int in;
	int mate=rank;
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
		cout<<"rank="<<rank<<" i="<<i<<" mate="<<mate<<'\n';
		MPI_Status status;
		MPI_Sendrecv(&out,4,MPI_INT,mate,0,&in,4,MPI_INT,mate,0,MPI_COMM_WORLD,&status);
		//out+=in;
	}
	return out;
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
