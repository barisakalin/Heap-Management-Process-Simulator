#include <algorithm>
#include <iostream>
#include <list>
#include <pthread.h>
#include <unistd.h>

using namespace std;

struct node{
	int id;
	int size;
	int index;
};

class HeapManager{

public:
	
	int initHeap(int size){
	
		init_size = size;
		node first_node;
		first_node.id = -1;
		first_node.size = size;
		first_node.index = 0;
		heap_list.push_front(first_node);

		mutex_for_print = PTHREAD_MUTEX_INITIALIZER;
		mutex_for_allocations = PTHREAD_MUTEX_INITIALIZER;

		cout << "[" << first_node.id << "] " << "[" << 
		first_node.size << "] " << "[" << first_node.index << "]" << endl;
		return 1; 
	}

	int myMalloc(int ID, int size){

		pthread_mutex_lock(&mutex_for_allocations);
		
		list <node> :: iterator iterator;
		for(iterator = heap_list.begin(); iterator != heap_list.end(); ++iterator){
			if(iterator->id == -1 && iterator->size >= size){
				node alloc_node;
				alloc_node.id = ID;
				alloc_node.size = size;
				alloc_node.index = iterator->index;
				heap_list.insert (iterator,alloc_node);
				iterator->size -= size;
				iterator->index = alloc_node.index + alloc_node.size;
				cout << "allocated for thread " << ID << endl;
				print();
				pthread_mutex_unlock(&mutex_for_allocations);
				return alloc_node.index;
			}
		}
		cout << "Can not allocate, requested size " << size
		<< " for thread " << ID << " is bigger than remaining size" << endl;
		print();
		pthread_mutex_unlock(&mutex_for_allocations);
		return -1;
	}

	int myFree(int ID, int index){
		
		pthread_mutex_lock(&mutex_for_allocations);

		if(heap_list.size() == 1){
			pthread_mutex_unlock(&mutex_for_allocations);
			return -1;
		}
		list <node> :: iterator iterator;
		for(iterator = heap_list.begin(); iterator != heap_list.end(); ++iterator){
			if(iterator->id == ID && iterator->index == index){
			
				if(iterator->index == 0){
					list <node> :: iterator temp_next = ++iterator;
					iterator--;
					if(temp_next->size + temp_next->index == init_size){
						iterator->id = -1;
						iterator->size = init_size;
						heap_list.erase(temp_next);
					}
					else{
						if(temp_next->id == -1){
							iterator->id = -1;
							iterator->size += temp_next->size;
							heap_list.erase(temp_next);
						}
						else{
							iterator->id = -1;
						}
					}
				cout << "freed for thread " << ID << endl;
				print();
				pthread_mutex_unlock(&mutex_for_allocations);
				return 1;
			}
			else if(iterator->size + iterator->index == init_size){
				list <node> :: iterator temp_prev = --iterator;
				iterator++;
				if(temp_prev->id == -1){
					iterator->id = -1;
					iterator->size += temp_prev->size;
					iterator->index = temp_prev->index;
					heap_list.erase(temp_prev);
				}
				else{
					iterator->id = -1;
				}
				cout << "freed for thread " << ID << endl;
				print();
				pthread_mutex_unlock(&mutex_for_allocations);
				return 1;
			}
			else{
				list <node> :: iterator temp_next = ++iterator;
				iterator--;
				list <node> :: iterator temp_prev = --iterator;
				iterator++;

				if(temp_next->id == -1 && temp_prev->id == -1){
					iterator->id = -1;
					iterator->size = iterator->size + temp_next->size + temp_prev->size;
					iterator->index = temp_prev->index;
					heap_list.erase(temp_next);
					heap_list.erase(temp_prev);
				}
				else{
					if(temp_next->id == -1){
						iterator->id = -1;
						iterator->size += temp_next->size;
						heap_list.erase(temp_next);
					}
					else if(temp_prev->id == -1){
						iterator->id = -1;
						iterator->size += temp_prev->size;
						iterator->index = temp_prev->index;
						heap_list.erase(temp_prev);
					}
					else{
						iterator->id = -1;
					}
				}
				cout << "freed for thread " << ID << endl;
				print();
				pthread_mutex_unlock(&mutex_for_allocations);
				return 1;
			}
		}
	}
	
	pthread_mutex_unlock(&mutex_for_allocations);
    return -1; 
}
	void print(){
		
		pthread_mutex_lock(&mutex_for_print);
		list <node> :: iterator iterator;
		for(iterator = heap_list.begin(); iterator != heap_list.end(); ++iterator){
			if(iterator->size + iterator->index == init_size){
				cout << "[" << iterator->id << "] " << "[" << 
				iterator->size << "] " << "[" << iterator->index << "]";
			}
			else{
				cout << "[" << iterator->id << "] " << "[" << 
				iterator->size << "] " << "[" << iterator->index << "]" << "---";
			}
		}
		cout << endl;
		pthread_mutex_unlock(&mutex_for_print);
}

private:
    list<node> heap_list;
	int init_size;
	pthread_mutex_t mutex_for_allocations; 
	pthread_mutex_t mutex_for_print;
};









