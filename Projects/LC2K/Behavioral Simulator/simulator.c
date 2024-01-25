/*
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 *
 * Make sure to NOT modify printState or any of the associated functions
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//DO NOT CHANGE THE FOLLOWING DEFINITIONS 

// Machine Definitions
#define NUMMEMORY 65536 /* maximum number of words in memory (maximum number of lines in a given file)*/
#define NUMREGS 8 /*total number of machine registers [0,7]*/

// File Definitions
#define MAXLINELENGTH 1000 /* MAXLINELENGTH is the max number of characters we read */

typedef struct 
stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);

static inline int convertNum(int32_t);

int 
main(int argc, char **argv)
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s , please ensure you are providing the correct path", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; ++state.numMemory) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address  %d\n. Please ensure you are providing a machine code file.", state.numMemory);
            perror("sscanf");
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    
    for(int i = 0;i<=7;i++){
        state.reg[i]=0;
    }
    state.pc = 0;
    int instruction = 0;
    printState(&state);
    while(state.pc<state.numMemory){
        int opcode = (state.mem[state.pc] & (0b1110000000000000000000000)) >>22;
        printf("%d\n",opcode);
        switch (opcode){
        case 0b000:{
            //add
            instruction++;
            int regAnum = (state.mem[state.pc] & (0b0001110000000000000000000)) >> 19;
            int regBnum = (state.mem[state.pc] & (0b0000001110000000000000000)) >> 16;
            int destRegnum = (state.mem[state.pc] & (0b0000000000000000000000111));
            state.reg[destRegnum]=state.reg[regAnum]+state.reg[regBnum];
            state.pc = state.pc + 1;
            printState(&state);
            break;
        }
        case 0b001:{
            //nor
            instruction++;
            int regAnum = (state.mem[state.pc] & (0b0001110000000000000000000)) >> 19;
            int regBnum = (state.mem[state.pc] & (0b0000001110000000000000000)) >> 16;
            int destRegnum = (state.mem[state.pc] & (0b0000000000000000000000111));
            state.reg[destRegnum] = ~(state.reg[regAnum] | state.reg[regBnum]);
            state.pc = state.pc + 1;
            printState(&state);
            break;
        }
        case 0b010:{
            //lw
            instruction++;
            int regA = state.reg[((state.mem[state.pc] & (0b0001110000000000000000000)) >> 19)];
            int regBnum = (state.mem[state.pc] & (0b0000001110000000000000000)) >> 16;
            int16_t offset = (state.mem[state.pc] & (0b0000000001111111111111111));
            if(offset<0){
                offset = convertNum(offset);
            }
            state.reg[regBnum] = state.mem[regA+offset];
            state.pc = state.pc + 1;
            printState(&state);
            break;
        }
        case 0b011:{
            //sw
            instruction++;
            int regA = state.reg[((state.mem[state.pc] & (0b0001110000000000000000000)) >> 19)];
            int regB = state.reg[((state.mem[state.pc] & (0b0000001110000000000000000)) >> 16)];
            int16_t offset = (state.mem[state.pc] & (0b0000000001111111111111111));
            if(offset<0){
                offset = convertNum(offset);
            }
            state.mem[regA+offset] = regB;
            state.pc = state.pc + 1;
            printState(&state);
            break;
        }
        case 0b100:{
            //beq
            instruction++;
            int regA = state.reg[((state.mem[state.pc] & (0b0001110000000000000000000)) >> 19)];
            int regB = state.reg[((state.mem[state.pc] & (0b0000001110000000000000000)) >> 16)];
            int16_t offset = (state.mem[state.pc] & (0b0000000001111111111111111));
            if(offset<0){
                offset = convertNum(offset);
            }
            if(regA == regB){
                state.pc = state.pc + 1 + offset;
                printState(&state);
            }
            else{
                state.pc = state.pc + 1;
                printState(&state);
            }
            break;
        }
        case 0b101:{
            //jalr
            instruction++;
            int regA = state.reg[((state.mem[state.pc] & (0b0001110000000000000000000)) >> 19)];
            int regBnum = (state.mem[state.pc] & (0b0000001110000000000000000)) >> 16;
            int regB = state.reg[regBnum];
            if(regA == regB) {
                state.pc = state.pc + 1;
            }
            else{
                state.reg[regBnum] = state.pc + 1;
                state.pc = regA;
            }
            printState(&state);
            break;
        }
        case 0b110:{
            //halt
            instruction++;
            state.pc = state.pc + 1;
            printf("machine halted\ntotal of %d instructions executed\nfinal state of machine:\n",instruction);
            printState(&state);
            return(0);
        }
        case 0b111:{
            //noop
            instruction++;
            state.pc = state.pc + 1;
            printState(&state);
            break;
        }
        }
        
        
        
    }
    

    return(0);
}

/*
* DO NOT MODIFY ANY OF THE CODE BELOW. 
*/

void 
printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) 
              printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) 
              printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    printf("end state\n");
}

// convert a 16-bit number into a 32-bit Linux integer
static inline int convertNum(int num) 
{
    return num - ( (num & (1<<15)) ? 1<<16 : 0 );
}

/*
* Write any helper functions that you wish down here. 
*/
