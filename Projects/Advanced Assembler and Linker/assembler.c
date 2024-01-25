/**
 * 
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
static inline int isNumber(char *);
struct pair{
    int value;
    char key[MAXLINELENGTH];
};

struct labelin{
    int address;
    char opcode[MAXLINELENGTH];
    char key[MAXLINELENGTH];
};

struct symbolin{
    int address;
    char label[MAXLINELENGTH];
    char section;
};

int
main(int argc, char **argv)
{
    
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

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
    struct labelin relocation[MAXLINELENGTH];
    struct symbolin symbol[MAXLINELENGTH];
    int data[MAXLINELENGTH];
    int labels_num = 0;
    int address = 0;

    int data_size =0;
    int text_size =0;
    int symbol_size =0;
    int relocation_size=0;

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
        if(strcmp (label, null) != 0){
            for(int i = 0;i<labels_num;i++){
                if(strcmp (label, labels[i].key) == 0){
                    exit(1);
                }
            }
            strcpy(labels[labels_num].key ,label);
            labels[labels_num].value = address;
            labels_num++;
            if(isupper(label[0])){
                bool findit = false;
                for(int i = 0; i<symbol_size;i++){
                    if(strcmp (arg2,symbol[i].label ) == 0){
                        findit = true;
                        break;
                    }
                }
                if(!findit){
                    strcpy(symbol[symbol_size].label,label);
                    symbol[symbol_size].address = address;
                    symbol_size++;
                }
                 
            }
        }
        address++;
    }
    rewind(inFilePtr);
    address = 0;
    
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        int mc = 0;
        if(strcmp (opcode, noop) == 0){
            int opcode1 = 0b111;
            text_size++;
            mc = opcode1 << 22;
            
        }
        else{
            
            switch (opcode[0]){
            case 'a':{
                text_size++;
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
                text_size++;
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
                text_size++;
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
                bool global = false;
                if(isNumber(arg2)){
                    offsetField = atoi(arg2);
                    if(offsetField>32767 || offsetField<-32768){
                        exit(1);
                    }
                }
                else{
                    if(isupper(arg2[0])){
                        global = true;
                    }
                    for(int i = 0;i<labels_num;i++){
                        if(strcmp(arg2,labels[i].key)==0){
                            offsetField = labels[i].value;
                            if(offsetField>32767 || offsetField<-32768){
                                exit(1);
                            }
                            find = true;
                            strcpy(relocation[relocation_size].opcode ,opcode);
                            strcpy(relocation[relocation_size].key ,arg2);
                            relocation[relocation_size].address = address;
                            relocation_size++;
                            if(global){
                                bool findit = false;
                                for(int i = 0; i<symbol_size;i++){
                                    if(strcmp (arg2,symbol[i].label ) == 0){
                                        findit = true;
                                        break;
                                    }
                                }
                                if(!findit){
                                    strcpy(symbol[symbol_size].label,arg2);
                                    symbol[symbol_size].address = labels[i].value;
                                    symbol_size++;  
                                }
                                }
                            break;
                        }
                    }
                    if(!find&&!global){
                            exit(1);
                        }
                    if(!find && global){
                        bool findit = false;
                        for(int i = 0; i<symbol_size;i++){
                            if(strcmp (arg2,symbol[i].label) == 0){
                                findit = true;
                                break;
                            }
                        }
                        if(!findit){
                            strcpy(symbol[symbol_size].label,arg2);
                            symbol[symbol_size].section='U';
                            symbol[symbol_size].address = 0;
                            symbol_size++;
                        }
                        strcpy(relocation[relocation_size].opcode ,opcode);
                        strcpy(relocation[relocation_size].key ,arg2);
                        relocation[relocation_size].address = address;
                        relocation_size++;
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
                text_size++;
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
            bool global = false;
            if(isNumber(arg2)){
                offsetField = atoi(arg2);
                if(offsetField>32767 || offsetField<-32768){
                    exit(1);
                }
            }
            else{
                if(isupper(arg2[0])){
                    global = true;
                }
                for(int i = 0;i<labels_num;i++){
                 if(strcmp(arg2,labels[i].key)==0){
                    offsetField = labels[i].value;
                    if(offsetField>32767 || offsetField<-32768){
                    exit(1);
                }
                    find = true;
                    strcpy(relocation[relocation_size].opcode ,opcode);
                    strcpy(relocation[relocation_size].key ,arg2);
                    relocation[relocation_size].address = address;
                    relocation_size++;
                    if(global){
                        bool findit = false;
                        for(int i = 0; i<symbol_size;i++){
                            if(strcmp (arg2,symbol[i].label ) == 0){
                                findit = true;
                                break;
                            }
                        }
                        if(!findit){
                        strcpy(symbol[symbol_size].label,arg2);
                        symbol[symbol_size].address = labels[i].value;
                        symbol_size++; 
                        }
  
                    }
                    break;
                    }
                }
                if(!find&&!global){
                    exit(1);
                }
                if(!find && global){
                    bool findit = false;
                    for(int i = 0; i<symbol_size;i++){
                        if(strcmp (arg2,symbol[i].label ) == 0){
                            findit = true;
                            break;
                        }
                    }
                if(!findit){
                    strcpy(symbol[symbol_size].label,arg2);
                    symbol[symbol_size].section='U';
                    symbol[symbol_size].address = 0;
                    symbol_size++;
                }
                    strcpy(relocation[relocation_size].opcode ,opcode);
                    strcpy(relocation[relocation_size].key ,arg2);
                    relocation[relocation_size].address = address;
                    relocation_size++;
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
            text_size++;
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
            bool global = false;
            if(isNumber(arg2)){
                offsetField = atoi(arg2);
                if(offsetField>32767 || offsetField<-32768){
                    exit(1);
                }
            }
            else{
                if(isupper(arg2[0])){
                    global = true;
                }
                for(int i = 0;i<labels_num;i++){
                if(strcmp (arg2, labels[i].key) == 0){
                    offsetField = labels[i].value-address-1;
                    if(offsetField>32767 || offsetField<-32768){
                    exit(1);
                    }
                    find = true;
                    if(global){
                        bool findit = false;
                        for(int i = 0; i<symbol_size;i++){
                            if(strcmp (arg2,symbol[i].label ) == 0){
                                findit = true;
                                break;
                            }
                        }
                        if(!findit){
                        strcpy(symbol[symbol_size].label,arg2);
                        symbol[symbol_size].address = labels[i].value;
                        symbol_size++;
                        }

                    }
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
            text_size++;   
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
            text_size++;    
            int opcode1 = 0b110;
            mc = opcode1 << 22;
            break;
            }
            case '.':{
            if(!isNumber(arg0)){
                bool find = false;
                bool global = false;
                if(isupper(arg0[0])){
                    global  = true;
                }
                for(int i = 0;i<labels_num;i++){
                    if(strcmp (arg0, labels[i].key) == 0){
                    mc = labels[i].value;
                    find = true;
                    strcpy(relocation[relocation_size].opcode ,opcode);
                    strcpy(relocation[relocation_size].key ,arg0);
                    relocation[relocation_size].address = address;
                    relocation_size++;
                    data[data_size] = mc;
                    data_size++;

                    if(global){
                        bool findit = false;
                        for(int i = 0; i<symbol_size;i++){
                            if(strcmp (arg0,symbol[i].label) == 0){
                                findit = true;
                                break;
                            }
                        }
                    if(!findit){
                        strcpy(symbol[symbol_size].label,arg0);
                        symbol[symbol_size].address = labels[i].value;
                        symbol_size++;
                    }                        
                    }
                    break;
                    }
                }
                if(!find&&!global){
                    exit(1);
                }
                if(!find && global){
                    data[data_size] = 0;
                    data_size++;
                    bool findit = false;
                    for(int i = 0; i<symbol_size;i++){
                        if(strcmp (arg0,symbol[i].label) == 0){
                            findit = true;
                            break;
                        }
                    }
                    if(!findit){
                        strcpy(symbol[symbol_size].label,arg0);
                        symbol[symbol_size].section='U';
                        symbol[symbol_size].address = 0;
                        symbol_size++;
                    }
                    strcpy(relocation[relocation_size].opcode ,opcode);
                    strcpy(relocation[relocation_size].key ,arg0);
                    relocation[relocation_size].address = address;
                    relocation_size++;
                }
            }
            else{
                mc = atoi(arg0);
                data[data_size] = mc;
                data_size++;
            }
            if(mc >2147483647||mc<-2147483648){
                exit(1);
            }
                break;
            }      
        }
        }
        address++;
    

    }
//HEADER
        fprintf(outFilePtr,"%d %d %d %d\n",text_size,data_size,symbol_size,relocation_size);


//TEXT
rewind(inFilePtr);
    address = 0;
    
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
    int mc = 0;
        if(strcmp (opcode, noop) == 0){
            int opcode1 = 0b111;
            mc = opcode1 << 22;
            
        }
        else{
            switch (opcode[0]){
            case 'a':{
                
            int opcode1 = 0b000;
            int regA=atoi(arg0);
            int regB=atoi(arg1);
            int destReg=atoi(arg2);
            mc = (opcode1 << 22) | (regA << 19) | (regB << 16) | destReg;
            break;
            }
            case 'n':{
                printf("2\n");
            int opcode1 = 0b001;
            int regA=atoi(arg0);
            int regB=atoi(arg1);
            int destReg=atoi(arg2);
            mc = (opcode1 << 22) | (regA << 19) | (regB << 16) | destReg;
            break;
            }
            case 'l':{
                
            int opcode1 = 0b010;
            int regA=atoi(arg0);
            int regB=atoi(arg1);
            int offsetField=0;
            if(isNumber(arg2)){
                offsetField = atoi(arg2);
            }
            else{
                for(int i = 0;i<labels_num;i++){
                    if(strcmp(arg2,labels[i].key)==0){
                        offsetField = labels[i].value;
                        break;
                    }
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
                
            int opcode1 = 0b011;
            int regA=atoi(arg0);
            int regB=atoi(arg1);
            int offsetField=0;
            if(isNumber(arg2)){
                offsetField = atoi(arg2);
            }
            else{
                for(int i = 0;i<labels_num;i++){
                    if(strcmp(arg2,labels[i].key)==0){
                        offsetField = labels[i].value;
                        break;
                    }
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
            
            int opcode1 = 0b100;
            int regA=atoi(arg0);
            int regB=atoi(arg1);
            int offsetField=0;
            if(isNumber(arg2)){
                offsetField = atoi(arg2);
            }
            else{
                for(int i = 0;i<labels_num;i++){
                if(strcmp (arg2, labels[i].key) == 0){
                    offsetField = labels[i].value-address-1;
                    break;
                }
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
                
            int opcode1 = 0b101;
            int regA=atoi(arg0);
            int regB=atoi(arg1);
            mc = (opcode1 << 22) | (regA << 19) | (regB << 16);
            break;
            }
            case 'h':{
                
            int opcode1 = 0b110;
            mc = opcode1 << 22;
            break;
            }
            case '.':{
            if(!isNumber(arg0)){
                for(int i = 0;i<labels_num;i++){
                    if(strcmp (arg0, labels[i].key) == 0){
                    mc = labels[i].value;
                    break;
                    }
                }
            }
            else{
                mc = atoi(arg0);
            }
                break;
            }      
        }
        }
        address++;
        if(strcmp (opcode, fill) != 0){
        fprintf(outFilePtr,"%d",mc);
        fprintf(outFilePtr,"\n");
        }
    }

//DATA
for(int i = 0;i<data_size;i++){
    fprintf(outFilePtr,"%d\n",data[i]);
}

//SYMBOL
for(int i =0;i<symbol_size;i++){
    
    int offset = 0;
    if(symbol[i].section!='U'){
        if(symbol[i].address>=text_size){
            symbol[i].section='D';
            offset = symbol[i].address - text_size;
        }
        else{
            symbol[i].section='T';
            offset = symbol[i].address;
        }
    }
    fprintf(outFilePtr,"%s %c %d\n",symbol[i].label,symbol[i].section,offset);
}

//RELOCATION
for(int i =0;i<relocation_size;i++){
    int offset = 0;
    if(relocation[i].address>=text_size){
            offset = relocation[i].address - text_size;
        }
        else{
            offset = relocation[i].address;
        }
    fprintf(outFilePtr,"%d %s %s\n",offset,relocation[i].opcode,relocation[i].key);
}


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

