// Will Higgins
// 11/8/2017
// lab4 malloc

// Include file goes here
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>

void *first = NULL;
#define NODE_SIZE sizeof(struct Node)
#define array_size 50000
#define short_array 250

int longword(int x){
	x = x-1;
	x = x/8;
	x = x*8;
	x = x+8;
	return x;
}

struct Node{
	size_t size;
	struct Node* next;
	int free;		//1 free 0 not
	int filler;		//makes NODE_SIZE divisible by 8
};

struct Node *find_free(struct Node **last, size_t size){
	struct Node *check = first;
	while (check && !(check->free == 1 && check->size >= size)){
		*last = check;
		check = check->next;
	}
	return check;
}

struct Node *new_mem(struct Node* last, size_t size){
	struct Node *newmem;
	newmem = sbrk(0);
	void *call = sbrk(size + NODE_SIZE);
	if (call == (void*) -1) {
		return NULL;
	}

	if(last) {
		last->next = newmem;
	}
	newmem->size = size;
	newmem->free = 0;
	newmem->next = NULL;
	return newmem;
}


struct Node *node_ptr(void *ptr) {
  return (struct Node*)ptr - 1;
}

void* malloc(size_t size){
	struct Node *mem;
	size = longword(size);
	if (first == NULL) {
		mem = new_mem(NULL, size);
		if (mem == NULL){
			return NULL;
		}
		first = mem;
	} else {
		struct Node *check = first;
		mem = find_free(&check, size);
		if (mem != NULL){
			if ((mem->size) >= (size+NODE_SIZE + 4)){
			}
			mem->free = 0;
		} else {
			mem = new_mem(check, size);
			if (mem == NULL){
                        	return NULL;
                	}
		}
	}
	return(mem+1);
}

void free(void *ptr){
	if (ptr == NULL){
		return;
	}
	struct Node* mem_ptr = node_ptr(ptr);
	mem_ptr->free = 1;
}

void *calloc(size_t memnum, size_t memsize){
	size_t size = memnum * memsize;
	size = longword(size);
	void *newptr = malloc(size);
	memset(newptr, 0, size);
	return newptr;
}

void *realloc(void *ptr, size_t size){
	struct Node* node_pointer = node_ptr(ptr);
	size = longword(size);
	if (!ptr) {
		return malloc(size);
	}
	if (node_pointer->size >= size) {
		return ptr;
	}
	else {
		void* new_ptr;
		new_ptr = malloc(size);
		if (new_ptr == NULL){
			return NULL;
		}
		memcpy(new_ptr, ptr, node_pointer->size);
		free(ptr);
		return new_ptr;
	}
}
/*
int main(int argc, char* argv[]){

        int *array = (int*) mymalloc(sizeof(int) * array_size);
        int *array2 = (int*) mymalloc(sizeof(int) * array_size);
        int i;

        printf("array 1 pointer: %p\n", array);
        printf("array 2 pointer: %p\n", array2);

        for (i=0; i<array_size; i++){
                array[i] = i;
        }
	for (i=0; i<array_size; i++){
                array2[i] = i;
        }

	myfree(array);
        myfree(array2);

        array = (int*) mymalloc(sizeof(int) * short_array);
        array2 = (int*) mymalloc(sizeof(int) * short_array);

        printf("array 1 pointer: %p\n", array);
        printf("array 2 pointer: %p\n", array2);

        for (i=0; i<short_array; i++){
                array[i] = i;
        }

	myfree(array);
        myfree(array2);

	array = (int*) mymalloc(sizeof(int) * short_array);
        array2 = (int*) mymalloc(sizeof(int) * short_array);

	printf("testing calloc\n");
        printf("array 1 pointer: %p\n", array);
        printf("array 2 pointer: %p\n", array2);

        for (i=0; i<short_array; i++){
                array[i] = i;
        }

	mycalloc(10, 100);

	int *array3 = (int*) mymalloc(sizeof(int) * short_array);

        printf("array 3 pointer: %p\n", array);

        return 0;
}
*/
/*
int main(int argc, char* argv[]){
	int x = 31;
	x = longword(x);
	printf("%d\n", x);
	return 0;
}
*/
