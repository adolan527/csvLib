//
// Created by Aweso on 11/12/2023.
//
#ifndef CSV_CSV_H
#include "csv.h"
#endif

const CSVSettings DEFAULT_SETTINGS = {',','\n',0,0};

Dimensions getSize(FILE *source,const CSVSettings settings){
    Dimensions retVal;
    retVal.rCount = 1;
    retVal.cCount = 1;
    int quoteFlag = 0;
    int entrySizeCounter = 0;
    int largestESC = 0;

    int input = fgetc(source);
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

    retVal.maxEntrySize = largestESC + 2; //2 characters for padding.


    return retVal;
}


CSV openCSV(FILE *source, CSVSettings settings){
    CSV data;
    data.settings = settings;
    CSV *dataPtr = &data;
    data.size = getSize(source, settings);
    size_t rowSize = sizeof(char[data.size.maxEntrySize]) * data.size.cCount;
    data.rows = (char*)calloc(data.size.rCount,rowSize);
    if(data.rows == NULL){
        printf("Failed to malloc");
        return data;
    }
    //indexing
    //MES * (rowIndex * cCount + colIndex) + entryIndex
    char input = (char)fgetc(source);
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

        input = (char)fgetc(source);
    }
    return data;
}
/*
 * These functions are entirely obsolete with the use of macros, but are being left here for now.
 *
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
 */

void displayCSV(CSV *source, int displayedCharPerEntry, int colStart, int colEnd, FILE *outputStream){
    int endColumn = (colEnd ? colEnd : source->size.cCount);
    colStart -= source->settings.rowHeader;
    int entryCharacterCount = (displayedCharPerEntry ? displayedCharPerEntry : source->size.maxEntrySize);
    char buffer[entryCharacterCount+1];

    if(source->settings.rowHeader == 0){
        fprintf(outputStream,"  |");
    }
    else{
        if(source->settings.colHeader==1){
            strncpy(buffer, CSVREADREF(source,0,0),entryCharacterCount);
            buffer[entryCharacterCount] = 0;
            fprintf(outputStream,"%*s|",entryCharacterCount,buffer);
        }
        else{
            fprintf(outputStream,"%*c|",entryCharacterCount,' ');
        }
    }

    for(int header = source->settings.rowHeader + colStart;header<endColumn;header++){
        if(source->settings.colHeader== 0){
            fprintf(outputStream,"%*c|",entryCharacterCount,header+65);
        }
        else{
            strncpy(buffer, CSVREADREF(source,0,header),entryCharacterCount);
            buffer[entryCharacterCount] = 0;
            fprintf(outputStream,"%*s|",entryCharacterCount, buffer);
        }
    }

    fprintf(outputStream,"%c",source->settings.rowDelin);
    for(int row = source->settings.colHeader;row<source->size.rCount;row++){
        if(source->settings.rowHeader==0){
            fprintf(outputStream,"%2d|", row);
        }
        else{
            strncpy(buffer, CSVREADREF(source,row,0),entryCharacterCount);
            buffer[entryCharacterCount] = 0;
            fprintf(outputStream,"%*s|", entryCharacterCount,buffer);
        }
        for(int col = source->settings.rowHeader + colStart;col<endColumn;col++){


            strncpy(buffer,&source->rows[CSVINDEXREF(source, row, col)],entryCharacterCount);
            buffer[entryCharacterCount] = 0;
            fprintf(outputStream,"%*s",entryCharacterCount,buffer);

            if(endColumn - col != 1){
                fprintf(outputStream,"%c",source->settings.colDelin);
            }

        }
        fprintf(outputStream,"%c",source->settings.rowDelin);

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
                fprintf(destination, "%c",source->settings.colDelin);
            }
        }
        if(source->size.rCount - row != 1){
            fprintf(destination, "%c",source->settings.rowDelin);
        }
    }
    fclose(destination);
    return 0;
}

int indexOfHeaderRow(CSV *source, char *header){
    if(source->settings.rowHeader == 0){
        return -1;
    }
    int row = -1;
    for(int i = 0; i < source->size.rCount; i++){
        if(strcmp(header, CSVREADREF(source,i,0))==0){
            row = i;
        }
    }
    return row;
}

int indexOfHeaderCol(CSV *source, char *header){
    if(source->settings.colHeader == 0){
        return 0;
    }
    int col = 0;
    for(int i = 0; i < source->size.cCount; i++){
        if(strcmp(header, CSVREADREF(source,0,i))==0){
            col = i;
        }
    }
    return col;
}

char * indexByHeader(CSV *source, char *rKey, char *cKey){
    char *buffer;
    int row = -1;
    int col = -1;

    if(source->settings.rowHeader == 0){

        row = strtol(rKey,&buffer,10);
        if(buffer==rKey){
            return NULL;
        }
    }
    else{
        for(int i = 0;i < source->size.rCount; i++){
            if(strcmp(rKey, CSVREADREF(source,i,0))==0){
                row = i;
            }
        }
    }

    if(source->settings.colHeader == 0){
        col = strtol(cKey,&buffer,10);
        if(buffer==cKey){
            return NULL;
        }
    }
    else{
        for(int i = 0;i < source->size.cCount; i++){
            if(strcmp(cKey, CSVREADREF(source,0,i))==0){
                col = i;
            }
        }
    }
    if(row == -1 || col == -1) return NULL;

    return CSVREADREF(source,row,col);
}

void arrayToCSV(CSV *dest, char array[], int arraySize, size_t arrayElementSize, int rowIndex, int colIndex, char rc){
    if(array == NULL){
        return;
    }
    size_t offset = (dest->size.maxEntrySize > arrayElementSize ? arrayElementSize : dest->size.maxEntrySize) - 1;
    if(rc == 'r'){//copies array to a row
        if(arraySize > dest->size.cCount){
            return;
        }
        for(int i = 0; i < arraySize && colIndex+i < dest->size.cCount; i++){
            array[i*arrayElementSize+offset] = 0;
            if(CSVWRITEREF(dest,rowIndex,(colIndex+i),&array[i*arrayElementSize])!=0){
                printf("\nError %d iteration.\n Copying %s to %s.\n",i,&array[i*arrayElementSize],(CSVREADREF(dest,rowIndex,(colIndex+i))));
            }

        }
    }
    else if (rc == 'c'){//copies array to a column
        if(arraySize > dest->size.rCount){
            return;
        }

        for(int i = 0; i < arraySize && rowIndex+i < dest->size.rCount; i++){
            array[i*arrayElementSize+offset] = 0;
            if(CSVWRITEREF(dest,(rowIndex+i),colIndex,&array[i*arrayElementSize])!=0){
                printf("\nError %d iteration.\n Copying %s to %s.\n",i,&array[i*arrayElementSize],(CSVREADREF(dest,(rowIndex+i),colIndex)));
            }
        }
    }
}

void copyCSV(CSV *source, CSV *dest){
    char buffer[source->size.maxEntrySize];
    for(int row = 0;row<dest->size.rCount && row < source->size.rCount;row++){
        for(int col = 0;col<dest->size.cCount && col < source->size.cCount;col++){
            if(CSVWRITEREF(dest,row,col,CSVREADREF(source,row,col))!=0){
                strncpy(buffer,CSVREADREF(source,row,col),source->size.maxEntrySize);
                buffer[dest->size.maxEntrySize-1] = 0;
                if(CSVWRITEREF(dest,row,col,buffer)!=0){
                    printf("Error copying %s to row %d column %d\n",buffer,row,col);
                    printf("Source string size: %llu Dest size: %d\n",strlen(buffer),dest->size.maxEntrySize);
                }
            }
        }
    }
}

void resizeCSV(CSV *source,Dimensions newSize){
    CSV temp;
    temp.size = newSize;
    size_t rowSize = sizeof(char[temp.size.maxEntrySize]) * temp.size.cCount;
    temp.rows = (char*)calloc(temp.size.rCount,rowSize);

    char buffer[source->size.maxEntrySize];
    for(int row = 0;row<temp.size.rCount && row < source->size.rCount;row++){
        for(int col = 0;col<temp.size.cCount && col < source->size.cCount;col++){
            if(CSVWRITELIT(temp,row,col,CSVREADREF(source,row,col))!=0){
                strncpy(buffer,CSVREADREF(source,row,col),source->size.maxEntrySize);
                buffer[temp.size.maxEntrySize-1] = 0;
                if(CSVWRITELIT(temp,row,col,buffer)!=0){
                    printf("Error copying %s to row %d column %d\n",buffer,row,col);
                    printf("Source string size: %llu Dest size: %d\n",strlen(buffer),temp.size.maxEntrySize);
                }
            }
        }
    }
    free(source->rows);
    *source = temp;

}

void changeMES(CSV *source,int newMES){
    Dimensions temp = source->size;
    temp.maxEntrySize = newMES;
    resizeCSV(source,temp);
}

CSV makeBlankCSV(int rCount, int cCount, int maxEntrySize){
    CSV new;
    new.settings = DEFAULT_SETTINGS;
    new.size.cCount = cCount;
    new.size.rCount = rCount;
    new.size.maxEntrySize = maxEntrySize;
    size_t rowSize = sizeof(char[new.size.maxEntrySize]) * new.size.cCount;
    new.rows = (char*)calloc(new.size.rCount,rowSize);
    return new;
}

CSV DMakeBlankCSV(Dimensions source){
    CSV new;
    new.settings = DEFAULT_SETTINGS;
    new.size = source;
    size_t rowSize = sizeof(char[new.size.maxEntrySize]) * new.size.cCount;
    new.rows = (char*)calloc(new.size.rCount,rowSize);
    return new;
}

int indexToCoordinates(CSV *source, int index, int *rowDest, int *colDest){

    int firstRowGuess = index/(source->size.maxEntrySize * source->size.cCount) - 1;

    int colGuess = (index/source->size.maxEntrySize) - firstRowGuess * source->size.cCount;
    while(colGuess>source->size.cCount){
        firstRowGuess++;
        colGuess = (index/source->size.maxEntrySize) - firstRowGuess * source->size.cCount;
    }

    if(firstRowGuess > source->size.rCount){
        printf("Error: Out of bounds input to indexToCoordinates\n");
        printf("R: %d C: %d |Rmax: %d Cmax: %d\n",firstRowGuess,colGuess,source->size.rCount,source->size.cCount);
        *rowDest = -1;
        *colDest = -1;
        return -1;
    }
    else{
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
    saveCSV(source,source->filename);
}
