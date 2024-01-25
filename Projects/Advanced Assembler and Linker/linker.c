/**
 * Project 2
 * LC-2K Linker
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>

#define MAXSIZE 300
#define MAXLINELENGTH 1000
#define MAXFILES 6

typedef struct FileData FileData;
typedef struct SymbolTableEntry SymbolTableEntry;
typedef struct RelocationTableEntry RelocationTableEntry;
typedef struct CombinedFiles CombinedFiles;

struct pair{
    int value;
    char key[MAXLINELENGTH];
};

struct SymbolTableEntry {
	char label[7];
	char location;
	int offset;
};

struct RelocationTableEntry {
	int offset;
	char inst[7];
	char label[7];
	int file;
};

struct FileData {
	int textSize;
	int dataSize;
	int symbolTableSize;
	int relocationTableSize;
	int textStartingLine; // in final executable
	int dataStartingLine; // in final executable
	int text[MAXSIZE];
	int data[MAXSIZE];
	SymbolTableEntry symbolTable[MAXSIZE];
	RelocationTableEntry relocTable[MAXSIZE];
};

struct CombinedFiles {
	int text[MAXSIZE];
	int data[MAXSIZE];
	SymbolTableEntry     symTable[MAXSIZE];
	RelocationTableEntry relocTable[MAXSIZE];
	int textSize;
	int dataSize;
	int symTableSize;
	int relocTableSize;
};

int findGlobal(FileData files[MAXFILES],int fileSize,char*label,int allSum);
static inline int convertNum(int32_t);

int main(int argc, char *argv[])
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr; 
	int i, j;

	if (argc <= 2) {
		printf("error: usage: %s <obj file> ... <output-exe-file>\n",
				argv[0]);
		exit(1);
	}

	outFileString = argv[argc - 1];

	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	FileData files[MAXFILES];

  // read in all files and combine into a "master" file
	for (i = 0; i < argc - 2; i++) {
		inFileString = argv[i+1];

		inFilePtr = fopen(inFileString, "r");
		printf("opening %s\n", inFileString);

		if (inFilePtr == NULL) {
			printf("error in opening %s\n", inFileString);
			exit(1);
		}

		char line[MAXLINELENGTH];
		int sizeText, sizeData, sizeSymbol, sizeReloc;

		// parse first line of file
		fgets(line, MAXSIZE, inFilePtr);
		sscanf(line, "%d %d %d %d",
				&sizeText, &sizeData, &sizeSymbol, &sizeReloc);

		files[i].textSize = sizeText;
		files[i].dataSize = sizeData;
		files[i].symbolTableSize = sizeSymbol;
		files[i].relocationTableSize = sizeReloc;

		// read in text section
		int instr;
		for (j = 0; j < sizeText; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			instr = atoi(line);
			files[i].text[j] = instr;
		}

		// read in data section
		int data;
		for (j = 0; j < sizeData; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			data = atoi(line);
			files[i].data[j] = data;
		}

		// read in the symbol table
		char label[7];
		char type;
		int addr;
		for (j = 0; j < sizeSymbol; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			sscanf(line, "%s %c %d",
					label, &type, &addr);
			files[i].symbolTable[j].offset = addr;
			strcpy(files[i].symbolTable[j].label, label);
			files[i].symbolTable[j].location = type;
		}

		// read in relocation table
		char opcode[7];
		for (j = 0; j < sizeReloc; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			sscanf(line, "%d %s %s",
					&addr, opcode, label);
			files[i].relocTable[j].offset = addr;
			strcpy(files[i].relocTable[j].inst, opcode);
			strcpy(files[i].relocTable[j].label, label);
			files[i].relocTable[j].file	= i;
		}
		fclose(inFilePtr);
	} // end reading files

	int fileSize = argc -2;
	int labels_num=0;
	struct pair labels[MAXLINELENGTH];
	//assign text and data start
	int textSum = 0;
	for(int i = 0;i < fileSize;i++){
		files[i].textStartingLine = textSum;
		textSum+=files[i].textSize;
	}
	int allSum = textSum;
	for(int i = 0;i < fileSize;i++){
		files[i].dataStartingLine = allSum;
		allSum += files[i].dataSize;
	}
	//check duplicate global labels
	for(int i = 0;i < fileSize;i++){
		for(int j = 0;j<files[i].symbolTableSize;j++){
			if(files[i].symbolTable[j].location!='U'){
                if(strcmp(files[i].symbolTable[j].label, "Stack")== 0 && files[i].symbolTable[j].location != 'U'){
                    printf("define Stack\n");
                    exit(1);
                }
				for(int k = 0;k<labels_num;k++){
					if(strcmp (files[i].symbolTable[j].label, labels[k].key) == 0){
						printf("duplicate labels");
						exit(1);
					}
				}
				strcpy(labels[labels_num].key ,files[i].symbolTable[j].label);
        		labels_num++;
			}
		}
	}

	//fix relocation labels 
	for(int i = 0;i < fileSize;i++){
		for(int j = 0; j<files[i].relocationTableSize;j++){
		//if it is used in data
		if ((!strcmp(files[i].relocTable[j].inst , ".fill"))){
			//global
			if(isupper(files[i].relocTable[j].label[0])){
				int location = findGlobal(files,fileSize,files[i].relocTable[j].label,allSum);
				files[i].data[files[i].relocTable[j].offset] = location;
			}
			//local
			else{
				int origin = files[i].data[files[i].relocTable[j].offset];//label original defined address
				//deifined in data
				if(origin>=files[i].textSize){
					//add all text lines
					int allOtherLineSize=textSum-files[i].textSize;
					//add all data lines come before this line
					for(int  k = 0; k<i;k++){
						allOtherLineSize+=files[k].dataSize;
					}
                    allOtherLineSize+=origin;
					files[i].data[files[i].relocTable[j].offset] = allOtherLineSize;
				}
				//defined in text
				else{
					int allOtherLineSize=0;
					//add all text lines come before this line
					for(int  k = 0; k < i;k++){
						allOtherLineSize+=files[k].textSize;
					}
					allOtherLineSize+=origin;
					files[i].data[files[i].relocTable[j].offset] = allOtherLineSize;
				}
				
			}
		}
		//if it is used in text
		if ((!strcmp(files[i].relocTable[j].inst , "lw"))||(!strcmp(files[i].relocTable[j].inst , "sw"))){
			//global
			if(isupper(files[i].relocTable[j].label[0])){
				int16_t offset = (files[i].text[files[i].relocTable[j].offset] & (0b0000000001111111111111111));
                if(offset<0){
                    offset = convertNum(offset);
                }
				int location = findGlobal(files,fileSize,files[i].relocTable[j].label,allSum)-offset;
				files[i].text[files[i].relocTable[j].offset]+=location;
			}
			//local
			else{
                //label .fill 5 or label .fill label0
                //find label orginal defined
                //offset
                int16_t offset = (files[i].text[files[i].relocTable[j].offset] & (0b0000000001111111111111111));
                if(offset<0){
                    offset = convertNum(offset);
                }
                //deifined in data
                if(offset>=files[i].textSize){
                    //add all text lines
                    int allOtherLineSize=textSum-files[i].textSize;
                    //add all data lines come before this line
                    for(int  k = 0; k<i;k++){
                        allOtherLineSize+=files[k].dataSize;
                    }
                    files[i].text[files[i].relocTable[j].offset]+= allOtherLineSize;
                }
                //defined in text
                else{
                    int allOtherLineSize=0;
                    //add all text lines come before this line
                    for(int  k = 0; k < i;k++){
                        allOtherLineSize+=files[k].textSize;
                    }
                    files[i].text[files[i].relocTable[j].offset]+= allOtherLineSize;
                    printf("%d",files[i].text[files[i].relocTable[j].offset]);
                }
            }
        }
    }
}

    
	//print final output
	for(int i = 0;i < fileSize;i++){
		for ( int j = 0; j<files[i].textSize;j++){
			fprintf(outFilePtr, "%d\n", files[i].text[j]);
		}
	}
	for(int i = 0;i < fileSize;i++){
		for ( int j = 0; j<files[i].dataSize;j++){
			fprintf(outFilePtr, "%d\n", files[i].data[j]);
		}
	}
} // main

int findGlobal(FileData files[MAXFILES],int fileSize,char*label,int allSum){
	int location = -1;
	for (int i = 0; i < fileSize; i++){
		for (int j = 0; j < files[i].symbolTableSize; j++){
			if (strcmp(files[i].symbolTable[j].label , label) == 0){
				if(files[i].symbolTable[j].location == 'T'){
					location = files[i].symbolTable[j].offset+files[i].textStartingLine;
				}
				else if(files[i].symbolTable[j].location == 'D'){
					location = files[i].symbolTable[j].offset+files[i].dataStartingLine;
				}
				else if(files[i].symbolTable[j].location == 'U'&&strcmp(label, "Stack")== 0){
					location = allSum;//Stack
				}
                
			}
		}
	}
	if(location == -1 && strcmp(label, "Stack")!= 0){
		printf("Global label undefined.\n");
		exit(1);
	}
	if(location == -1 && strcmp(label, "Stack")== 0){
		location = allSum;//Stack
	}
	return location;
}

static inline int convertNum(int num)
{
    return num - ( (num & (1<<15)) ? 1<<16 : 0 );
}
