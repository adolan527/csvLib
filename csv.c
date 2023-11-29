//
// Created by Aweso on 11/12/2023.
//
#ifndef CSV_CSV_H
#include "csv.h"
#endif

const CSVSettings DEFAULT_SETTINGS = {',','\n',8};

Dimensions getSize(FILE *source,const CSVSettings settings){
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
        if(input == settings.colDelin && retVal.rCount==1 && quoteFlag==0){
            retVal.cCount++;

        }
        if(input == settings.rowDelin && quoteFlag==0){
            retVal.rCount++;
        }
        if((input == settings.rowDelin ||input == settings.colDelin) && quoteFlag == 0){
            if(entrySizeCounter > largestESC){
                largestESC = entrySizeCounter;
            }
            entrySizeCounter = 0;
        }
        input = fgetc(source);
    }
    fseek(source,0,SEEK_SET);

    if(settings.maxEntrySize<largestESC){
        /*
        //if an invalid size is entered, it rounds up to the nearest power of 2
        for(int i = 0; i<11;i++){
            if(largestESC < pow(2,i)){
                retVal.maxEntrySize = (int)pow(2,i);
                break;
            }
        }
         */ //Removed because that seemed weird
        retVal.maxEntrySize = largestESC + 2; //2 characters for padding.
    }
    else{
        retVal.maxEntrySize = settings.maxEntrySize;
    }

    return retVal;
}


CSV openCSV(FILE *source, CSVSettings settings){
    CSV data;
    CSV *dataPtr = &data;
    data.size = getSize(source,settings);
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
        if(input == settings.colDelin && quoteFlag == 0){
            entryIndex = 0;
            colIndex++;
        }
        else if(input == settings.rowDelin && quoteFlag == 0){
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

void displayCSV(CSV *source, int rowStartVal, const CSVSettings settings,FILE *outputStream){
    int entryCharacterCount = (settings.maxEntrySize ? settings.maxEntrySize : source->size.maxEntrySize);
    fprintf(outputStream," |");
    for(int header = 0;header<source->size.cCount;header++){
        fprintf(outputStream,"%*c|",entryCharacterCount,header+65);
    }
    fprintf(outputStream,"%c",settings.rowDelin);

    for(int row = 0;row<source->size.rCount;row++){
        fprintf(outputStream,"%d|", row + rowStartVal);
        for(int col = 0;col<source->size.cCount;col++){
            fprintf(outputStream,"%*s",entryCharacterCount,&source->rows[CSVINDEXREF(source, row, col)]);

            if(source->size.cCount - col != 1){
                fprintf(outputStream,"%c",settings.colDelin);
            }
        }
        fprintf(outputStream,"%c",settings.rowDelin);
    }

}

int saveCSV(CSV *source, char *filename,const CSVSettings settings){
    FILE *destination = fopen(filename,"w");

    if(destination == NULL){
        perror("ERROR: Cannot open file.");
        return 1;
    }

    for(int row = 0;row<source->size.rCount;row++){
        for(int col = 0;col<source->size.cCount;col++){
            fprintf(destination, "%s", &source->rows[CSVINDEXREF(source, row, col)]);
            if(source->size.cCount - col != 1){
                fprintf(destination, "%c",settings.colDelin);
            }
        }
        if(source->size.rCount - row != 1){
            fprintf(destination, "%c",settings.rowDelin);
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
            array[i*arrayElementSize+dest->size.maxEntrySize-1] = 0;
            if(CSVWRITEREF(dest,rowIndex,(colIndex+i),&array[i*arrayElementSize])!=0){
                printf("\nError %d iteration.\n Copying %s to %s.\n",i,&array[i*arrayElementSize],(CSVREADREF(dest,rowIndex,colIndex)));
            }

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

void copyCSV(CSV *source, CSV *dest){
    char buffer[source->size.maxEntrySize];
    for(int row = 0;row<dest->size.rCount && row < source->size.rCount;row++){
        for(int col = 0;col<dest->size.cCount && col < source->size.cCount;col++){
            if(CSVWRITEREF(dest,row,col,CSVREADREF(source,row,col))!=0){
                strncpy(buffer,CSVREADREF(source,row,col),source->size.maxEntrySize);
                buffer[source->size.maxEntrySize-1] = 0;
                if(CSVWRITEREF(dest,row,col,buffer)!=0){
                    printf("Error copying %s to row %d column %d\n",buffer,row,col);
                    printf("Source string size: %d Dest size: %d\n",strlen(buffer),dest->size.maxEntrySize);
                }
            }
        }
    }
}

void rectangleCopy(CSV *source, CSV *dest,int r);

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

int indexToCoordinates(CSV *source, int index, int *rowDest, int *colDest){

    int firstRowGuess = index/(source->size.maxEntrySize * source->size.cCount) - 1;
    printf("F: %d\n",firstRowGuess);
    int colGuess = (index/source->size.maxEntrySize) - firstRowGuess * source->size.cCount;
    while(colGuess>source->size.cCount){
        firstRowGuess++;
        colGuess = (index/source->size.maxEntrySize) - firstRowGuess * source->size.cCount;
    }

    if(firstRowGuess > source->size.rCount){
        printf("Out of bounds\n");
        *rowDest = -1;
        *colDest = -1;
        return -1;
    }
    else{
        printf("R: %d C: %d\n",firstRowGuess,colGuess);
        *rowDest = firstRowGuess;
        *colDest = colGuess;
        return 0;
    }

}



void closeCSV(CSV *source){
    free(source->rows);
}

CSV easyOpenCSV(char *filename){
    FILE *easyFile = fopen(filename,"r+");
    CSV easyCSV = openCSV(easyFile, DEFAULT_SETTINGS);
    easyCSV.filename = filename;
    fclose(easyFile);
    return easyCSV;
}

void easyCloseCSV(CSV *source){
    free(source->rows);
}

void easySaveCSV(CSV *source){
    saveCSV(source,source->filename,DEFAULT_SETTINGS);
}
