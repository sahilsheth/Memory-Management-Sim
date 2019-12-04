// Prompt user for the MEMORY SIZE and PAGE SIZE
// Ex : Memory size> 2000
//Page Size (1: 100, 2: 200, 3: 400)> 3
//Prompt user for name of a workload file( Has int N = # of processes in file)
//Each process has unique process id first line, Arrival time, Lifetime in Memory
// and Address space seperated by single blank line
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
  if(memAllocation == (*void)-1) {
    return NULL;
  }
  else{
    b->nxt = NULL;
    b->free = false;
    b->size = size;
    b->memAddress = memAddy + BLOCK_SIZE;
    return b;
  }
}
