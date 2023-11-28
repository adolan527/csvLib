//
// Created by Aweso on 11/12/2023.
//

#include "csv.h"



Dimensions getSize(FILE *source, char colDelin, char rowDelin, int maxEntrySize){
    Dimensions retVal;
    retVal.rCount = 1;
    retVal.cCount = 1;
    int quoteFlag = 0;
    int entrySizeCounter = 0;
    int largestESC = 0;

    char input = fgetc(source);
    while(input != EOF){
        entrySizeCounter++;
        if (input == '\"') {
            quoteFlag = 1 - quoteFlag;
        }
        if(input == colDelin && retVal.rCount==1 && quoteFlag==0){
            retVal.cCount++;

        }
        if(input== rowDelin && quoteFlag==0){
            retVal.rCount++;
        }
        if(input== rowDelin ||input == colDelin){
            if(entrySizeCounter > largestESC){
                largestESC = entrySizeCounter;
            }
            entrySizeCounter = 0;
        }
        input = fgetc(source);
    }
    fseek(source,0,SEEK_SET);

    if(maxEntrySize<largestESC){ //if an invalid size is entered, it rounds up to the nearest power of 2
        for(int i = 0; i<11;i++){
            if(largestESC < pow(2,i)){
                retVal.maxEntrySize = (int)pow(2,i);
                break;
            }
        }
    }
    else{
        retVal.maxEntrySize = maxEntrySize;
    }

    return retVal;
}


CSV openCSV(FILE *source, char colDelin, char rowDelin, int MAX_ENTRY_SIZE){
    CSV data;
    CSV *dataPtr = &data;
    data.size = getSize(source,colDelin,rowDelin,MAX_ENTRY_SIZE);
    size_t rowSize = sizeof(char[data.size.maxEntrySize]) * data.size.cCount;
    data.rows = (char*)calloc(data.size.rCount,rowSize);
    if(data.rows == NULL){
        printf("Failed to malloc");
        return data;
    }
    //indexing
    //MES * (rowIndex * cCount + colIndex) + entryIndex
    char input = fgetc(source);
    int entryIndex = 0;
    int rowIndex = 0;
    int colIndex = 0;
    int quoteFlag = 0;
    while(input != EOF){
        if (input == '\"') {
            quoteFlag = 1 - quoteFlag;
        }

        if(entryIndex>data.size.maxEntrySize-1){
            if(colIndex == data.size.cCount-1){
                entryIndex = 0;
                colIndex = 0;
                rowIndex++;
            }
            else{
                entryIndex = 0;
                colIndex++;
            }
        }
        if(input == colDelin && quoteFlag == 0){
            entryIndex = 0;
            colIndex++;
        }
        else if(input == rowDelin && quoteFlag == 0){
            entryIndex = 0;
            colIndex = 0;
            rowIndex++;
        }
        else{
            data.rows[CSVINDEXREF(dataPtr, rowIndex, colIndex) + entryIndex] = input;
            entryIndex++;
        }

        input = fgetc(source);
    }
    return data;
};

void readCSV(CSV *source, char destination[source->size.maxEntrySize], int rowIndex, int colIndex, int characterIndex){
    if(characterIndex > source->size.maxEntrySize - 1){
        characterIndex=0;
    }
    strncpy(destination, &source->rows[CSVINDEXREF(source, rowIndex, colIndex) + characterIndex], source->size.maxEntrySize - characterIndex);
}

void editCSV(CSV *source, char info[source->size.maxEntrySize], int rowIndex, int colIndex, int characterIndex){
    if(characterIndex > source->size.maxEntrySize - 1){
        characterIndex=0;
    }
    strncpy(&source->rows[source->size.maxEntrySize*(rowIndex * source->size.cCount + colIndex) + characterIndex], info, source->size.maxEntrySize);
}

void displayCSV(CSV *source, int rowStartVal, char colDelin, char rowDelin,FILE *outputStream){
    fprintf(outputStream," |");
    for(int header = 0;header<source->size.cCount;header++){
        fprintf(outputStream,"%c|",header+65);
    }
    fprintf(outputStream,"%c",rowDelin);

    for(int row = 0;row<source->size.rCount;row++){
        fprintf(outputStream,"%d|", row + rowStartVal);
        for(int col = 0;col<source->size.cCount;col++){
            if(source->rows[CSVINDEXREF(source, row, col)] != '\0'){
                fprintf(outputStream,"%s",&source->rows[CSVINDEXREF(source, row, col)]);
            }
            if(source->size.cCount - col != 1){
                fprintf(outputStream,"%c",colDelin);
            }
        }
        fprintf(outputStream,"%c",rowDelin);
    }

}

int saveCSV(CSV *source, char *filename){
    FILE *destination = fopen(filename,"w");

    if(destination == NULL){
        perror("ERROR: Cannot open file.");
        return 1;
    }

    for(int row = 0;row<source->size.rCount;row++){
        for(int col = 0;col<source->size.cCount;col++){
            fprintf(destination, "%s", &source->rows[CSVINDEXREF(source, row, col)]);
            if(source->size.cCount - col != 1){
                fprintf(destination, ",");
            }
        }
        if(source->size.rCount - row != 1){
            fprintf(destination, "\n");
        }
    }
    fclose(destination);
    return 0;
}


void arrayToCSV(CSV *dest, char array[], int arraySize, size_t arrayElementSize, int rowIndex, int colIndex, char rc){
    if(array == NULL){
        return;
    }
    if(rc == 'r'){//copies array to a row
        if(arraySize > dest->size.cCount){
            return;
        }
        for(int i = 0; i < arraySize; i++){
            editCSV(dest,&array[i*arrayElementSize],rowIndex,colIndex+i,0);
        }
    }
    else if (rc == 'c'){//copies array to a column
        if(arraySize > dest->size.rCount){
            return;
        }

        for(int i = 0; i < arraySize; i++){
            editCSV(dest,&array[i*arrayElementSize],rowIndex+i,colIndex,0);
        }
    }
}

CSV makeBlankCSV(int rCount, int cCount, int maxEntrySize){
    CSV new;
    new.size.cCount = cCount;
    new.size.rCount = rCount;
    new.size.maxEntrySize = maxEntrySize;
    size_t rowSize = sizeof(char[new.size.maxEntrySize]) * new.size.cCount;
    new.rows = (char*)calloc(new.size.rCount,rowSize);
    return new;
}

CSV DMakeBlankCSV(Dimensions *source){
    CSV new;
    new.size = *source;
    size_t rowSize = sizeof(char[new.size.maxEntrySize]) * new.size.cCount;
    new.rows = (char*)calloc(new.size.rCount,rowSize);
    return new;
}



void closeCSV(CSV *source){
    free(source->rows);
}

CSV easyOpenCSV(char *filename){
    FILE *easyFile = fopen(filename,"r+");
    CSV easyCSV = openCSV(easyFile, ',', '\n', 0);
    easyCSV.filename = filename;
    fclose(easyFile);
    return easyCSV;
}

void easyCloseCSV(CSV *source){
    free(source->rows);
}

void easySaveCSV(CSV *source){
    saveCSV(source,source->filename);
}
