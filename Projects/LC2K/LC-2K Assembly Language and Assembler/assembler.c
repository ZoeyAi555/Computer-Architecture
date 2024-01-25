/**
 * 
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
static inline int isNumber(char *);

struct pair{
    int value;
    char key[MAXLINELENGTH];
};


int
main(int argc, char **argv)
{
    
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    // if (argc != 3) {
    //     printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
    //         argv[0]);
    //     exit(1);
    // }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }
    struct pair labels[MAXLINELENGTH];
    int labels_num = 0;
    int address = 0;
    char null[1000] = "";
    char noop[1000] = "noop";
    char add[1000] = "add";
    char nor[1000] = "nor";
    char lw[1000] = "lw";
    char sw[1000] = "sw";
    char beq[1000] = "beq";
    char jalr[1000] = "jalr";
    char halt [1000] = "halt";
    char fill [1000] = ".fill";
    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */
    
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        if(strcmp (label, null) != 0){
            for(int i = 0;i<labels_num;i++){
                if(strcmp (label, labels[i].key) == 0){
                    exit(1);
                }
            }
            strcpy(labels[labels_num].key ,label);
            labels[labels_num].value = address;
            labels_num++;
        }
        address++;
    }

     /* reached end of file */

   
    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);
    address = 0;
    
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
    if(strcmp (opcode, noop) != 0 && 
    strcmp (opcode, add) != 0 && 
    strcmp (opcode, nor) != 0 &&
    strcmp (opcode, lw) != 0 &&
    strcmp (opcode, sw) != 0 &&
    strcmp (opcode, jalr) != 0 &&
    strcmp (opcode, beq) != 0 &&
    strcmp (opcode, halt) != 0 &&
    strcmp (opcode, fill) != 0){
        exit(1);
    }
    
    int mc = 0;
        if(strcmp (opcode, noop) == 0){
            int opcode1 = 0b111;
            mc = opcode1 << 22;
            
        }
        else{
            switch (opcode[0]){
            case 'a':{
                printf("1\n");
            int opcode1 = 0b000;
            if(!isNumber(arg0)||!isNumber(arg1)||!isNumber(arg2)){
                exit(1);
            }
            
            int regA=atoi(arg0);
            if (regA > 7 || regA < 0){
                exit(1);
            }
            int regB=atoi(arg1);
            if (regB > 7 || regB < 0){
                exit(1);
            }
            int destReg=atoi(arg2);
            if (destReg > 7 || destReg < 0){
                exit(1);
            }
            mc = (opcode1 << 22) | (regA << 19) | (regB << 16) | destReg;
            break;
            }
            case 'n':{
                printf("2\n");
            int opcode1 = 0b001;
            if(!isNumber(arg0)||!isNumber(arg1)||!isNumber(arg2)){
                exit(1);
            }
            int regA=atoi(arg0);
            if (regA > 7 || regA < 0){
                exit(1);
            }
            int regB=atoi(arg1);
            if (regB > 7 || regB < 0){
                exit(1);
            }
            int destReg=atoi(arg2);
            if (destReg > 7 || destReg < 0){
                exit(1);
            }
            mc = (opcode1 << 22) | (regA << 19) | (regB << 16) | destReg;
            break;
            }
            case 'l':{
                printf("3\n");
            int opcode1 = 0b010;
            
            if(!isNumber(arg0)||!isNumber(arg1)){
                exit(1);
            }
            int regA=atoi(arg0);
            if (regA > 7 || regA < 0){
                exit(1);
            }
            int regB=atoi(arg1);
            if (regB > 7 || regB < 0){
                exit(1);
            }
            int offsetField=0;
            bool find = false;
            if(isNumber(arg2)){
                offsetField = atoi(arg2);
                if(offsetField>32767 || offsetField<-32768){
                    exit(1);
                }
            }
            else{
                for(int i = 0;i<labels_num;i++){
                if(strcmp(arg2,labels[i].key)==0){
                    offsetField = labels[i].value;
                    if(offsetField>32767 || offsetField<-32768){
                        exit(1);
                    }
                    find = true;
                    break;
                }
            }
            if(!find){
                    exit(1);
                }
            }
            offsetField = offsetField & 0b00000000000000001111111111111111;
            if(offsetField<0){
                offsetField = ~offsetField + 1;
            }
            mc = (opcode1 << 22) | (regA << 19) | (regB << 16) | offsetField;
            break;
            }
            case 's':{
                printf("4\n");
            int opcode1 = 0b011;
            if(!isNumber(arg0)||!isNumber(arg1)){
                exit(1);
            }
            int regA=atoi(arg0);
            if (regA > 7 || regA < 0){
                exit(1);
            }
            int regB=atoi(arg1);
            if (regB > 7 || regB < 0){
                exit(1);
            }
            int offsetField=0;
            bool find = false;
            if(isNumber(arg2)){
                offsetField = atoi(arg2);
                if(offsetField>32767 || offsetField<-32768){
                    exit(1);
                }
            }
            else{
                for(int i = 0;i<labels_num;i++){
                 if(strcmp(arg2,labels[i].key)==0){
                    offsetField = labels[i].value;
                    if(offsetField>32767 || offsetField<-32768){
                    exit(1);
                }
                    find = true;
                    break;
                 }
                }
                if(!find){
                    exit(1);
                }
            }
            offsetField = offsetField & 0b00000000000000001111111111111111;
             if(offsetField<0){
                offsetField = ~offsetField + 1;
            }
            mc = (opcode1 << 22) | (regA << 19) | (regB << 16) | offsetField;
            break;
            }
            case 'b':{
            printf("beq\n");
            int opcode1 = 0b100;
            if(!isNumber(arg0)||!isNumber(arg1)){
                exit(1);
            }
            int regA=atoi(arg0);
            if (regA > 7 || regA < 0){
                exit(1);
            }
            int regB=atoi(arg1);
            if (regB > 7 || regB < 0){
                exit(1);
            }
            int offsetField=0;
            bool find = false;
            if(isNumber(arg2)){
                offsetField = atoi(arg2);
                if(offsetField>32767 || offsetField<-32768){
                    exit(1);
                }
            }
            else{
                for(int i = 0;i<labels_num;i++){
                if(strcmp (arg2, labels[i].key) == 0){
                    offsetField = labels[i].value-address-1;
                    if(offsetField>32767 || offsetField<-32768){
                    exit(1);
                }
                    find = true;
                    break;
                }
            }
            if(!find){
                    exit(1);
                }
            }
            offsetField = offsetField & 0b00000000000000001111111111111111;
            if(offsetField < 0){
                offsetField = ~offsetField + 1;
            }
            
            mc = (opcode1 << 22) | (regA << 19) | (regB << 16) | offsetField;
            break;
            }
            case 'j':{
                printf("5\n");
            int opcode1 = 0b101;
            if(!isNumber(arg0)||!isNumber(arg1)){
                exit(1);
            }
            int regA=atoi(arg0);
            if (regA > 7 || regA < 0){
                exit(1);
            }
            int regB=atoi(arg1);
            if (regB > 7 || regB < 0){
                exit(1);
            }
            mc = (opcode1 << 22) | (regA << 19) | (regB << 16);
            break;
            }
            case 'h':{
                printf("6\n");
            int opcode1 = 0b110;
            mc = opcode1 << 22;
            break;
            }
            case '.':{
                printf("7\n");
            if(!isNumber(arg0)){
                bool find = false;
                for(int i = 0;i<labels_num;i++){
                    if(strcmp (arg0, labels[i].key) == 0){
                    mc = labels[i].value;
                    find = true;
                    break;
                    }
                }
                if(!find){
                    exit(1);
                }
            }
            else{
                mc = atoi(arg0);
            }
            if(mc >2147483647||mc<-2147483648){
                exit(1);
            }
                break;
            }      
        }
        }
        address++;
        fprintf(outFilePtr,"%d",mc);
        fprintf(outFilePtr,"\n");

    }





    /* after doing a readAndParse, you may want to do the following to test the
        opcode */
    if (!strcmp(opcode, "add")) {
        /* do whatever you need to do for opcode "add" */
    }
    return(0);
}

/*
* NOTE: The code defined below is not to be modifed as it is implimented correctly.
*/

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return(0);
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
	printf("error: line too long\n");
	exit(1);
    }

    // Treat a blank line as end of file.
    // Arguably, we could just ignore and continue, but that could
    // get messy in terms of label line numbers etc.
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for(size_t line_idx = 0; line_idx < strlen(line); ++line_idx) {
        int line_char_is_whitespace = 0;
        for(int whitespace_idx = 0; whitespace_idx < 4; ++ whitespace_idx) {
            if(line[line_idx] == whitespace[whitespace_idx]) {
                ++line_char_is_whitespace;
                break;
            }
        }
        if(!line_char_is_whitespace) {
            ++nonempty_line;
            break;
        }
    }
    if(nonempty_line == 0) {
        return 0;
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);

    return(1);
}



static inline int
isNumber(char *string)
{
    int num;
    char c;
    return((sscanf(string, "%d%c",&num, &c)) == 1);
}
