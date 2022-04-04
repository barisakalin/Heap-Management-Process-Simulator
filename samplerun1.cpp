//////////////////////////////////////////////////////
// include Memory Manager and related libraries///////
#include <algorithm>
#include <iostream>
#include <list>
//#include <pthread.h>
//#include <unistd.h>
#include "allocator.cpp"

//////////////////////////////////////////////////////
HeapManager m; // Creating global instance of Memory Manager
/*
void * run(void * id)  // Thread function
{
  int * tid = (int * ) id;

  //  N = 1, Size = 100, one thread is allocating 40 Bytes of memory
  // Test area
  int rv = m.myMalloc( * tid, 40); // Allocate 40 Bytes
  
	
 }*/

int main() {

  int N = 1;
  int size = 100;

  //pthread_t ids[N];
  //int intids[N];
  //pthread_t cntr = 0;
  /*
  for (int i = 0; i < N; i++) {
    intids[i] = i;
    ids[i] = cntr++;
  }*/

  cout << "Memory initialized" << endl;
  m.initHeap(size);
  m.myMalloc(0,14);
  m.myMalloc(1,28);
  m.myMalloc(4,24);
  m.myMalloc(2,17);
  m.myMalloc(3,10);
  m.myMalloc(2,3);
  m.myMalloc(3,11);
  m.myFree(3,83);
  m.myMalloc(0,4);
  m.myFree(2,66);
  m.myFree(2,93);
  m.myMalloc(1,1);
  m.myMalloc(4,13);
  m.myFree(0,0);
  m.myFree(0,83);
  m.myFree(1,14);
  m.myFree(1,66);
  m.myFree(4,42);
  m.myFree(4,67);

  /*
  for (int i = 0; i < N; i++) {
    pthread_create( & ids[i], NULL, run, (void * ) & intids[i]);
  }

  for (int i = 0; i < N; i++) {
    pthread_join(ids[i], NULL);
  }
  */
  cout << "Execution is done" << endl;
  m.print();

  return 0;
}
