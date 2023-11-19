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
            data.rows[data.size.maxEntrySize*(rowIndex * data.size.cCount + colIndex) + entryIndex] = input;
            entryIndex++;
        }

        input = fgetc(source);
    }
    return data;
};

void readCSV(CSV *subject, char destination[subject->size.maxEntrySize], int rowIndex, int colIndex, int characterIndex){
    if(characterIndex > subject->size.maxEntrySize - 1){
        characterIndex=0;
    }
    strncpy(destination, &subject->rows[subject->size.maxEntrySize*(rowIndex * subject->size.cCount + colIndex) + characterIndex], subject->size.maxEntrySize);
}

void editCSV(CSV *subject, char info[subject->size.maxEntrySize], int rowIndex, int colIndex, int characterIndex){
    if(characterIndex > subject->size.maxEntrySize - 1){
        characterIndex=0;
    }
    strncpy(&subject->rows[subject->size.maxEntrySize*(rowIndex * subject->size.cCount + colIndex) + characterIndex], info, subject->size.maxEntrySize);
}

void printCSV(CSV *source,int rowStartVal,char colDelin, char rowDelin){
    int index = 0;

    printf(" |");
    for(int header = 0;header<source->size.cCount;header++){
        printf("%c|",header+65);
    }
    printf("%c",rowDelin);

    for(int row = 0;row<source->size.rCount;row++){
        printf("%d|", row + rowStartVal);
        for(int col = 0;col<source->size.cCount;col++){
            index = source->size.maxEntrySize*(row * source->size.cCount + col);
            if(source->rows[index] != '\0'){
                printf("%s",&source->rows[index]);
            }
            if(source->size.cCount - col != 1){
                printf("%c",colDelin);
            }
        }
        printf("%c",rowDelin);
    }

}

void saveCSV(CSV *source, FILE *dest, int indexStart){
    rewind(dest);
    int buffer = fgetc(dest);
    while(buffer != EOF){
        fputc(0,dest);
        buffer = fgetc(dest);
    }
    rewind(dest);
    for(int row = 0;row<source->size.rCount;row++){
        for(int col = 0;col<source->size.cCount;col++){
            fprintf(dest,"%s",&source->rows[source->size.maxEntrySize*(row * source->size.cCount + col)]);
            if(source->size.cCount - col != 1){
                fprintf(dest,",");
            }
        }
        if(source->size.rCount - row != 1){
            fprintf(dest,"\n");
        }
    }
}


void arrayToCSV(CSV *dest,void *array, int size, size_t arrayElementSize, int rowIndex, int colIndex,char rc){
    if(rc == 'r'){
        if(size > dest->size.cCount){
            return;
        }
        for(int i = 0;i<size;i++){
            editCSV(dest,&array[i],rowIndex,colIndex+i,0);
        }
    }
    else if (rc == 'c'){
        if(size > dest->size.rCount){
            return;
        }

        for(int i = 0;i<size;i++){
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
    new.size.cCount = source->cCount;
    new.size.rCount = source->rCount;
    new.size.maxEntrySize = source->maxEntrySize;
    size_t rowSize = sizeof(char[new.size.maxEntrySize]) * new.size.cCount;
    new.rows = (char*)calloc(new.size.rCount,rowSize);
    return new;
}

void closeCSV(CSV *subject){
    free(subject->rows);
}

CSV easyOpenCSV(char *filename){
    FILE *easyFile = fopen(filename,"r+");
    CSV easyCSV = openCSV(easyFile, ',', '\n', 0);
    return easyCSV;
}

void easyCloseCSV(CSV *subject){
    free(subject->rows);
    fclose(subject->fileSource);
}

void easySaveCSV(CSV *source){
    saveCSV(source,source->fileSource,0);
}
