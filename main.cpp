#include <stdio.h>
#include <stdbool.h>

__intptr_t currBreak; //Global variable that tracks processes
// Used allocate more memory by incrementing break value
void *sbrk(__intptr_t inc){ //From Stackoverflow.com
  __intptr_t oldBreak = currBreak;
  currBreak += inc;
  return (void*) oldBreak;
}

typedef struct mem_block{
  struct mem_block *nxt; //Points to next memory
  bool free; // Checks if memory block is free
  size_t size; //Allocated size in memory
  void *memAddress; //Starting point in memory allocation
}_MEMBLOCK;

#define BLOCK_SIZE sizeof(_MEMBLOCK) //Structure size_t

//This function allocates memory
_MEMBLOCK *memBlockAllocation(size_t size){
  _MEMBLOCK *b = (_MEMBLOCK*)sbrk(0); //Stores break point to begin mem allocation
  void *memAddy = (void*)sbrk(0); //Stores memory address to object
  void *memAllocation = (void*)sbrk(BLOCK_SIZE + size); //Changes location with respect to Block size

  // Checks if memory allocation pointer can increment
  if(memAllocation == (void*) -1){
    return NULL;
  }
  else{
    b->nxt = NULL;
    b->free = false;
    b->size = size;
    b->memAddress = memAddy + BLOCK_SIZE; //ERROR HERE, MEANT TO INCREMENT BLOCK
    return b;
  }
}

// Allocates next memory block
void nextAllocationMemBlock(size_t size, _MEMBLOCK **head){
  _MEMBLOCK *current = *head;
  void *memAllocation = NULL;
  void *memAddy = (void*)sbrk(0);

  if (current == NULL){
    *head =  memBlockAllocation(size);
  }
  else{
    while(current->nxt != NULL){
      current = current->nxt;
    }
    _MEMBLOCK *newblock = sbrk(0);

    memAllocation = (void*)sbrk(BLOCK_SIZE + size);
    if(memAllocation == (void*) -1){
      //Overwrites data
    }
    else{
      newblock->nxt = NULL;
      newblock->free = false;
      newblock->size = size;
      newblock->memAddress = memAddy + BLOCK_SIZE; //ERROR HERE, MEANT TO INCREMENT BLOCK
      current->nxt = newblock;
    }
  }
}

void freeMemoryBlock(_MEMBLOCK **head){

  if(*head == NULL){
    //Memory is not free
  }
  else{//Overwrites contents
    (*head)->free = true;
  }
}

//Print function HERE

int main(int argc, char const *argv[]) {
  _MEMBLOCK *test = NULL;
  nextAllocationMemBlock(10,&test);
    nextAllocationMemBlock(35,&test);
    //print
    freeMemoryBlock(&(test->nxt));

  return 0;
}
