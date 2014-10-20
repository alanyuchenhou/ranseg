#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <math.h>
#include <ctime>
#include <sys/time.h>
using std::cout;

long getTime (struct timeval time_begin, struct timeval time_end) {
  return (time_end.tv_sec*1000000 + time_end.tv_usec) -
    (time_begin.tv_sec*1000000 + time_begin.tv_usec);
}

int main(int argc, char **argv)
{
	struct timeval start, end;
	gettimeofday(&start,NULL);
	 int input_size = atoi(argv[1]);
	 int a0 = atoi(argv[2]);
	 int b0 = atoi(argv[3]);
	 int p0 = atoi(argv[4]);
	 int x0 = atoi(argv[5]);
	 int* seq;
	 seq=new int[input_size];
	 seq[0]=x0;
	 gettimeofday(&end,NULL);
	 cout<<"Time="<<getTime(start,end)<<" usec"<<'\n';
	/*
	 for (int i=1;i<input_size;i++)
	 {
		 seq[i]=(seq[i-1]*a0+b0)%p0;
		 cout<<" "<<seq[i];
	 }
	 */
	 return (0);
}
