/*
 * 
 * LC-2K Cache Simulator
 * Instructions are found in the project spec.
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_CACHE_SIZE 256
#define MAX_BLOCK_SIZE 256

// **Note** this is a preprocessor macro. This is not the same as a function.
// Powers of 2 have exactly one 1 and the rest 0's, and 0 isn't a power of 2.
#define is_power_of_2(val) (val && !(val & (val - 1)))


/*
 * Accesses 1 word of memory.
 * addr is a 16-bit LC2K word address.
 * write_flag is 0 for reads and 1 for writes.
 * write_data is a word, and is only valid if write_flag is 1.
 * If write flag is 1, mem_access does: state.mem[addr] = write_data.
 * The return of mem_access is state.mem[addr].
 */
extern int mem_access(int addr, int write_flag, int write_data);

/*
 * Returns the number of times mem_access has been called.
 */
extern int get_num_mem_accesses(void);

//Use this when calling printAction. Do not modify the enumerated type below.
enum actionType
{
    cacheToProcessor,
    processorToCache,
    memoryToCache,
    cacheToMemory,
    cacheToNowhere
};

/* You may add or remove variables from these structs */
typedef struct blockStruct
{
    int data[MAX_BLOCK_SIZE];
    int dirty;
    int lruLabel;
    int tag;
    int start;//modified
    int valid;
} blockStruct;

typedef struct cacheStruct
{
    blockStruct blocks[MAX_CACHE_SIZE][MAX_CACHE_SIZE];//modified
    int blockSize;
    int numSets;
    int blocksPerSet;
} cacheStruct;

/* Global Cache variable */
cacheStruct cache;
int pc = 0;
void printAction(int, int, enum actionType);
void printCache(void);

/*
 * Set up the cache with given command line parameters. This is
 * called once in main(). You must implement this function.
 */
void cache_init(int blockSize, int numSets, int blocksPerSet)
{
    if (blockSize <= 0 || numSets <= 0 || blocksPerSet <= 0) {
        printf("error: input parameters must be positive numbers\n");
        exit(1);
    }
    if (blocksPerSet * numSets > MAX_CACHE_SIZE) {
        printf("error: cache must be no larger than %d blocks\n", MAX_CACHE_SIZE);
        exit(1);
    }
    if (blockSize > MAX_BLOCK_SIZE) {
        printf("error: blocks must be no larger than %d words\n", MAX_BLOCK_SIZE);
        exit(1);
    }
    if (!is_power_of_2(blockSize)) {
        printf("warning: blockSize %d is not a power of 2\n", blockSize);
    }
    if (!is_power_of_2(numSets)) {
        printf("warning: numSets %d is not a power of 2\n", numSets);
    }
    printf("Simulating a cache with %d total lines; each line has %d words\n",
        numSets * blocksPerSet, blockSize);
    printf("Each set in the cache contains %d lines; there are %d sets\n",
        blocksPerSet, numSets);

    // Your code here
    cache.blockSize = blockSize;
    cache.blocksPerSet = blocksPerSet;
    cache.numSets = numSets;
    for (int i = 0; i < numSets; ++i){
        for (int j = 0; j < blocksPerSet; ++j){
            cache.blocks[i][j].tag = -1;
            cache.blocks[i][j].valid = 0;
            cache.blocks[i][j].dirty = 0;
        }
    }
    return;
}

/*
 * Access the cache. This is the main part of the project,
 * and should call printAction as is appropriate.
 * It should only call mem_access when absolutely necessary.
 * addr is a 16-bit LC2K word address.
 * write_flag is 0 for reads (fetch/lw) and 1 for writes (sw).
 * write_data is a word, and is only valid if write_flag is 1.
 * The return of mem_access is undefined if write_flag is 1.
 * Thus the return of cache_access is undefined if write_flag is 1.
 */
int cache_access(int addr, int write_flag, int write_data)
{
    /* The next line is a placeholder to connect the simulator to
    memory with no cache. You will remove this line and implement
    a cache which interfaces between the simulator and memory. */
    int tag = addr / cache.numSets / cache.blockSize;
    int setIndex = (addr / cache.blockSize) % cache.numSets;
    int blockOffset = addr % cache.blockSize;
    int start = addr - blockOffset;
    //printf("addr %d ",addr);
    //printf("start %d",start);
    //printf("\n");
    for(int i =0;i<cache.numSets;i++){
        for(int j =0;j<cache.blocksPerSet;j++){
            for(int z =0;z<cache.blockSize;z++){
                //printf("ssssssss%d\n",cache.blocks[i][j].data[z]);
            }
        }
    }

    for(int i =0;i<cache.blocksPerSet;i++){
        //found
        if(cache.blocks[setIndex][i].tag == tag){
            cache.blocks[setIndex][i].lruLabel = pc;
            pc++;
            if(write_flag == 0){ // lw
                printAction(addr, 1, cacheToProcessor);
                //printf("%d",cache.blocks[setIndex][i].data[blockOffset]);
                return cache.blocks[setIndex][i].data[blockOffset];
            }
            else{ //sw
                cache.blocks[setIndex][i].dirty =1;
                cache.blocks[setIndex][i].data[blockOffset] = write_data;
                printAction(addr, 1, processorToCache);
                return 0;
            }
            
        }
    }

    //not found
    //found  empty cache
    for(int i =0;i<cache.blocksPerSet;i++){
        if(cache.blocks[setIndex][i].valid == 0){
            cache.blocks[setIndex][i].tag = tag;
            cache.blocks[setIndex][i].valid = 1;
            cache.blocks[setIndex][i].dirty = 0;
            cache.blocks[setIndex][i].start = start;
            cache.blocks[setIndex][i].lruLabel = pc;
            pc++;

            for(int j = 0; j < cache.blockSize; ++j){
                cache.blocks[setIndex][i].data[j] = mem_access(start+j,0,-1);
            }

            printAction(start, cache.blockSize, memoryToCache);
           if(write_flag == 0){ // lw
                printAction(addr, 1, cacheToProcessor);
                //printf("%d",cache.blocks[setIndex][i].data[blockOffset]);
                return cache.blocks[setIndex][i].data[blockOffset];
            }
            else{ //sw
                cache.blocks[setIndex][i].dirty =1;
                cache.blocks[setIndex][i].data[blockOffset] = write_data;
                printAction(addr, 1, processorToCache);
                return 0;
            }
        }
    }

    //didn't found  empty cache
    // find lru
    int lru = cache.blocks[setIndex][0].lruLabel;
    int evict = 0;
    for(int i =0;i<cache.blocksPerSet;i++){
        if(cache.blocks[setIndex][i].lruLabel < lru){
            lru = cache.blocks[setIndex][i].lruLabel;
            evict = i;
        }
    }
    //evict
        //not dirty 
        //evicting LRU
    if(cache.blocks[setIndex][evict].dirty == 0){
        printAction(cache.blocks[setIndex][evict].start, cache.blockSize, cacheToNowhere); 
    }

        //dirty
    else if(cache.blocks[setIndex][evict].dirty == 1) {
        //write back
        for(int i = 0; i < cache.blockSize; i++){
            //printf("nnnnnn%d    ",start+i);
            //printf("nnnnnn%d    ",cache.blocks[setIndex][evict].data[i]);
            mem_access(cache.blocks[setIndex][evict].start+i,1,cache.blocks[setIndex][evict].data[i]);
        }
        printAction(cache.blocks[setIndex][evict].start, cache.blockSize, cacheToMemory);

    }

    //update new block
    cache.blocks[setIndex][evict].tag = tag;
    cache.blocks[setIndex][evict].valid = 1;
    cache.blocks[setIndex][evict].dirty = 0;
    cache.blocks[setIndex][evict].start = start;
    cache.blocks[setIndex][evict].lruLabel = pc;
    pc++;

    for(int j = 0; j < cache.blockSize; j++){
        cache.blocks[setIndex][evict].data[j] = mem_access(start+j,0,-1);
        //printf("%d    ",start+j);
        //printf("%d    ",mem_access(start+j,0,-1));
        //printf("data %d\n",cache.blocks[setIndex][evict].data[j]);
    }

    printAction(cache.blocks[setIndex][evict].start, cache.blockSize, memoryToCache);
    
    if(write_flag == 0){ // lw
        printAction(addr, 1, cacheToProcessor);
        //printf("33333%d",cache.blocks[setIndex][evict].data[blockOffset]);
        return cache.blocks[setIndex][evict].data[blockOffset];
    }
    else{ //sw
        cache.blocks[setIndex][evict].dirty =1;
        cache.blocks[setIndex][evict].data[blockOffset] = write_data;
        printAction(addr, 1, processorToCache);
        return 0;
    }


    
}


/*
 * print end of run statistics like in the spec. **This is not required**,
 * but is very helpful in debugging.
 * This should be called once a halt is reached.
 * DO NOT delete this function, or else it won't compile.
 * DO NOT print $$$ in this function
 */
void printStats(void)
{
    return;
}

/*
 * Log the specifics of each cache action.
 *
 *DO NOT modify the content below.
 * address is the starting word address of the range of data being transferred.
 * size is the size of the range of data being transferred.
 * type specifies the source and destination of the data being transferred.
 *  -    cacheToProcessor: reading data from the cache to the processor
 *  -    processorToCache: writing data from the processor to the cache
 *  -    memoryToCache: reading data from the memory to the cache
 *  -    cacheToMemory: evicting cache data and writing it to the memory
 *  -    cacheToNowhere: evicting cache data and throwing it away
 */
void printAction(int address, int size, enum actionType type)
{
    printf("$$$ transferring word [%d-%d] ", address, address + size - 1);

    if (type == cacheToProcessor) {
        printf("from the cache to the processor\n");
    }
    else if (type == processorToCache) {
        printf("from the processor to the cache\n");
    }
    else if (type == memoryToCache) {
        printf("from the memory to the cache\n");
    }
    else if (type == cacheToMemory) {
        printf("from the cache to the memory\n");
    }
    else if (type == cacheToNowhere) {
        printf("from the cache to nowhere\n");
    }
    else {
        printf("Error: unrecognized action\n");
        exit(1);
    }

}

/*
 * Prints the cache based on the configurations of the struct
 * This is for debugging only and is not graded, so you may
 * modify it, but that is not recommended.
 */
void printCache(void)
{
    printf("\ncache:\n");
    for (int set = 0; set < cache.numSets; ++set) {
        printf("\tset %i:\n", set);
        for (int block = 0; block < cache.blocksPerSet; ++block) {
            printf("\t\t[ %i ]: {", block);
            for (int index = 0; index < cache.blockSize; ++index) {
                printf(" %i", cache.blocks[set][block - (set-1)*cache.blocksPerSet].data[index]);//modified
            }
            printf(" }\n");
        }
    }
    printf("end cache\n");
}
