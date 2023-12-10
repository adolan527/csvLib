//
// Created by Aweso on 11/24/2023.
//

#ifndef CSV_CSVUTILS_H
#include "csvUtils.h"
#endif

int (*sortModes[4])(char*,char*) = {&numIncreasing, &numDecreasing, &alphaIncreasing, &alphaDecreasing};


int numIncreasing(char *op1, char *op2){
    char *buffer;
    long long num1 = strtoll(op1,&buffer,10);
    if(buffer==op1){
        return -1;
    }
    long long num2 = strtoll(op2,&buffer,10);
    if(buffer==op2){
        return -1;
    }
    if(num1>num2){
        return 1;
    }
    return 0;
}

int numDecreasing(char *op1, char *op2){
    char *buffer;
    long long num1 = strtoll(op1,&buffer,10);
    if(buffer==op1){
        return -1;
    }
    long long num2 = strtoll(op2,&buffer,10);
    if(buffer==op2){
        return -1;
    }
    if(num1<num2){
        return 1;
    }
    return 0;
}

int alphaIncreasing(char *op1, char *op2){
    if(strcmp(op1,op2)>0){
        return 1;
    }
    return 0;
}

int alphaDecreasing(char *op1, char *op2){
    if(strcmp(op1,op2)<0){
        return 1;
    }
    return 0;
}

int rmRowBefore(CSV *source,const char map[source->size.rCount]);

int rmRowInside(CSV *source,const char map[source->size.rCount]);

int rmRowAfter(CSV *source,const char map[source->size.rCount]);

int rmRowAll(CSV *source,const char map[source->size.rCount]);

int rmColAll(CSV *source,const char map[source->size.cCount]);

int rmColBefore(CSV *source,const char map[source->size.cCount]);

int rmColInside(CSV *source,const char map[source->size.cCount]);

int rmColAfter(CSV *source,const char map[source->size.cCount]);


void removeEmpty(CSV *source, rmSelector mode, char rc){
    if(rc=='r'){
        removeEmptyRows(source,mode);
        return;
    }
    if(rc=='c'){
        removeEmptyColumns(source,mode);
        return;
    }
    else{
        removeEmptyRows(source,mode);
        removeEmptyColumns(source,mode);
        return;
    }
}

void removeEmptyRows(CSV *source,rmSelector mode){
    char dataMap[source->size.rCount];
    mapRows(source, dataMap);

    switch(mode){
        case Before: {
            rmRowBefore(source, dataMap);
            return;
        }
        case Inside:{
            rmRowInside(source, dataMap);
            return;
        }
        case After:{
            rmRowAfter(source, dataMap);
            return;
        }
        case BeforeInside:{
            if(rmRowBefore(source, dataMap) == 0){
                mapRows(source, dataMap);
            }
            rmRowInside(source, dataMap);
            return;
        }
        case Outside:{
            if(rmRowBefore(source, dataMap) == 0){
                mapRows(source, dataMap);
            }
            rmRowAfter(source, dataMap);
            return;
        }
        case AfterInside:{
            if(rmRowAfter(source, dataMap) == 0){
                mapRows(source, dataMap);
            }
            return;
        }
        case All:{
            rmRowAll(source, dataMap);
            return;
        }
        default: perror("Invalid Remove Empty Rows Mode");

    }
}

void removeEmptyColumns(CSV *source, rmSelector mode){
    char dataMap[source->size.cCount];
    mapColumns(source, dataMap);

    switch(mode){
        case Before: {
            rmColBefore(source, dataMap);
            return;
        }
        case Inside:{
            rmColInside(source, dataMap);
            return;
        }
        case After:{
            rmColAfter(source, dataMap);
            return;
        }
        case BeforeInside:{
            if(rmColBefore(source, dataMap) == 0){
                mapColumns(source, dataMap);
            }
            rmColInside(source, dataMap);
            return;
        }
        case Outside:{
            if(rmColBefore(source, dataMap) == 0){
                mapColumns(source, dataMap);
            }
            rmColAfter(source, dataMap);
            return;
        }
        case AfterInside:{
            if(rmColAfter(source, dataMap) == 0){
                mapColumns(source, dataMap);
            }
            return;
        }
        case All:{
            rmColAll(source, dataMap);
            return;
        }
        default: perror("Invalid Remove Empty Columns Mode");

    }

}

int rmRowAll(CSV *source,const char map[source->size.rCount]){
    int emptyRowCount = 0;
    for(int i = 0;i<source->size.rCount;i++){
        if(map[i]==0)emptyRowCount++;
    }
    if(emptyRowCount == 0)return 1;
    source->size.rCount -= emptyRowCount;
    char *tempRows = (char*)calloc(source->size.rCount,sizeof(char[source->size.maxEntrySize]) * source->size.cCount);
    int emptyRowsReached = 0;
    for(int i = 0;i<source->size.rCount+emptyRowCount;i++){
        if(map[i]==1){
            size_t rowSize = source->size.cCount * source->size.maxEntrySize;
            memcpy(tempRows + (i-emptyRowsReached) * rowSize, CSVREADREF(source,i,0), rowSize);
        }
        else{emptyRowsReached++;}
    }
    free(source->rows);
    source->rows = tempRows;
    return 0;
}

int rmRowBefore(CSV *source,const char map[source->size.rCount]){
    int emptyRowCount = 0;
    for (int i = 0; i < source->size.rCount; i++) {
        if (map[i] == 1)break;
        emptyRowCount++;
    }
    if(emptyRowCount==0) return 1;
    source->size.rCount-=emptyRowCount;
    char *tempRows = (char*)calloc(source->size.rCount,sizeof(char[source->size.maxEntrySize]) * source->size.cCount);
    memcpy(tempRows, CSVREADREF(source,emptyRowCount,0), source->size.rCount * source->size.cCount * source->size.maxEntrySize);
    free(source->rows);
    source->rows = tempRows;
    return 0;

}

int rmRowInside(CSV *source,const char map[source->size.rCount]){
    int firstValueIndex = -1;
    int lastValueIndex = -1;
    int emptyRowCount = 0;
    for(int i = 0;i<source->size.rCount;i++){
        if(map[i] == 1){
            if(firstValueIndex == -1) firstValueIndex = i;
            lastValueIndex = i;
        }
    }
    for(int i = firstValueIndex + 1;i<  lastValueIndex;i++){
        if(map[i]==0)emptyRowCount++;
    }
    if(emptyRowCount == 0)return 1;
    source->size.rCount -= emptyRowCount;
    char *tempRows = (char*)calloc(source->size.rCount,sizeof(char[source->size.maxEntrySize]) * source->size.cCount);
    int emptyRowsReached = 0;
    for(int i = 0;i<source->size.rCount+emptyRowCount;i++){
        if(i >= firstValueIndex && i <= lastValueIndex){
            if(map[i]==1){
                size_t size = source->size.cCount * source->size.maxEntrySize;
                memcpy(tempRows + (i-emptyRowsReached) * (source->size.cCount * source->size.maxEntrySize), CSVREADREF(source,i,0), size);
            }
            else{emptyRowsReached++;}
        }
    }

    free(source->rows);
    source->rows = tempRows;

    return 0;
}

int rmRowAfter(CSV *source,const char map[source->size.rCount]){
    int emptyRowCount = 0;
    for (int i = source->size.rCount-1; i >=0 ; i--) {
        if (map[i] == 1)break;
        emptyRowCount++;
    }
    if(emptyRowCount==0) return 1;
    source->size.rCount-=emptyRowCount;
    char *tempRows = (char*)calloc(source->size.rCount,sizeof(char[source->size.maxEntrySize]) * source->size.cCount);
    memcpy(tempRows,CSVREADREF(source,0,0), source->size.rCount * source->size.cCount * source->size.maxEntrySize);
    free(source->rows);
    source->rows = tempRows;
    return 0;
}

void mapRows(CSV *source,char dest[source->size.rCount]){
    //1 means it has data
    //0 means empty
    memset(dest,0,source->size.rCount);
    for(int row = 0;row<source->size.rCount;row++){
        for(int col = 0;col<source->size.cCount;col++){
            if(source->rows[CSVINDEXREF(source, row, col)] != 0 ){
                dest[row]=1;
                break;
            }
        }
    }

}

int rmColAll(CSV *source,const char map[source->size.cCount]){
    int emptyColCount = 0;
    for(int i = 0;i<source->size.cCount;i++){
        if(map[i]==0)emptyColCount++;
    }
    if(emptyColCount == 0)return 1;

    source->size.cCount -= emptyColCount;

    char *tempCols = (char*)calloc(source->size.rCount,sizeof(char[source->size.maxEntrySize]) * source->size.cCount);
    int emptyColReached = 0;

    for(int col = 0; col < source->size.cCount + emptyColCount; col++){
        if(map[col] == 1){
            for(int row = 0; row < source->size.rCount; row++){
                memcpy(tempCols + source->size.maxEntrySize * ((row * source->size.cCount) + (col - emptyColReached)),
                       &(source->rows[source->size.maxEntrySize * (row * (source->size.cCount + emptyColCount) + col)]),
                       source->size.maxEntrySize);
            }
        }
        else{
            emptyColReached++;
        }
    }
    free(source->rows);
    source->rows = tempCols;
    return 0;

}

int rmColBefore(CSV *source,const char map[source->size.cCount]){
    int emptyColCount = 0;
    for(int i = 0;i<source->size.cCount;i++){
        if(map[i]!=0)break;
        emptyColCount++;
    }
    if(emptyColCount == 0)return 1;
    source->size.cCount -= emptyColCount;
    char *tempCols = (char*)calloc(source->size.rCount,sizeof(char[source->size.maxEntrySize]) * source->size.cCount);
    int emptyColReached = 0;

    for(int col = 0; col < source->size.cCount + emptyColCount; col++){
        if(map[col] == 1 || emptyColReached >= emptyColCount){
            for(int row = 0; row < source->size.rCount; row++){
                memcpy(tempCols + source->size.maxEntrySize * ((row * source->size.cCount) + (col - emptyColReached)),&(source->rows[source->size.maxEntrySize * (row * (source->size.cCount+emptyColCount) + col)]),source->size.maxEntrySize);
            }
        }
        else{
            emptyColReached++;
        }
    }
    free(source->rows);
    source->rows = tempCols;
    return 0;
}

int rmColInside(CSV *source,const char map[source->size.cCount]){
    int emptyColCount = 0;
    int firstValueIndex = -1;
    int lastValueIndex = -1;

    for(int i = 0;i<source->size.cCount;i++){
        if(map[i] == 1){
            if(firstValueIndex == -1) firstValueIndex = i;
            lastValueIndex = i;
        }
    }
    for(int i = firstValueIndex + 1;i<  lastValueIndex;i++){
        if(map[i]==0)emptyColCount++;
    }

    if(emptyColCount == 0) return 1;
    source->size.cCount-=emptyColCount;
    char *tempCols = (char*)calloc(source->size.rCount, sizeof(char[source->size.maxEntrySize]) * source->size.cCount);
    int emptyColReached = 0;

    for(int col = 0; col < source->size.cCount + emptyColCount; col++){
        if(col >= firstValueIndex && col <= lastValueIndex){
            if(map[col] == 1){
                for(int row = 0; row < source->size.rCount; row++){
                    memcpy(tempCols + source->size.maxEntrySize * ((row * source->size.cCount) + (col - emptyColReached)),
                           &(source->rows[source->size.maxEntrySize * (row * (source->size.cCount + emptyColCount) + col)]),
                           source->size.maxEntrySize);
                }
            }
            else{
                emptyColReached++;
            }
        }

    }

    free(source->rows);
    source->rows = tempCols;
    return 0;
}

int rmColAfter(CSV *source,const char map[source->size.cCount]){
    int emptyColCount = 0;
    int lastValueIndex = 0;
    for (int i = source->size.cCount-1; i >=0 ; i--) {
        if (map[i] == 1){
            lastValueIndex = i;
            break;
        }
        emptyColCount++;
    }
    if(emptyColCount == 0) return 1;
    source->size.cCount-=emptyColCount;
    char *tempCols = (char*)calloc(source->size.cCount, sizeof(char[source->size.maxEntrySize]) * source->size.cCount);
    int emptyColReached = 0;

    for(int col = 0; col < source->size.cCount + emptyColCount; col++){
        if(map[col] == 1 || col < lastValueIndex){
            for(int row = 0; row < source->size.rCount; row++){
                memcpy(tempCols + source->size.maxEntrySize * ((row * source->size.cCount) + (col - emptyColReached)),
                       &(source->rows[source->size.maxEntrySize * (row * (source->size.cCount + emptyColCount) + col)]),
                       source->size.maxEntrySize);
            }
        }
        else{
            emptyColReached++;
        }
    }

    free(source->rows);
    source->rows = tempCols;
    return 0;
}

void mapColumns(CSV *source,char dest[source->size.cCount]){
    memset(dest,0,source->size.cCount);
    for(int col = 0;col<source->size.cCount;col++){
        for(int row = 0;row<source->size.rCount;row++){
            if(source->rows[CSVINDEXREF(source, row, col)] != 0 ){
                dest[col]=1;
                break;
            }
        }
    }

}

int rectangleCopy(CSV *source, CSV *dest,
                  int rSrc1,int cSrc1,
                  int rSrc2,int cSrc2,
                  int rDest,int cDest){
    /*source and dest can be the same array, however:
     * If dest is before source, it will work as intended even if they overlap
     * If dest and source don't overlap, then it works
     * If dest is after source and they overlap, then it will copy itself and data will be lost
     */
    if(rSrc1 >= source->size.rCount || cSrc1 >= source->size.cCount || rSrc2 >= source->size.rCount || cSrc2 >= source->size.cCount){
        printf("Error: Out of bounds source in rectangleCopy\n"); //src is an index. Therefore the max is rCount-1 or cCount -1.
        printf("rSrc1: %d cSrc1: %d rSrc2: %d cSrc2: %d rMax: %d cMax: %d\n",rSrc1,cSrc1,rSrc2,cSrc2,dest->size.rCount,dest->size.cCount);
        return -1;
    }
    if(rDest >= dest->size.rCount || cDest >= dest->size.cCount){
        printf("Error: Out of bounds destination in rectangleCopy\n"); //dest is an index. Therefore the max is rCount-1 or cCount -1.
        printf("rDest: %d cDest: %d rMax: %d cMax: %d\n",rDest,cDest,dest->size.rCount,dest->size.cCount);
        return -1;
    }
    char buffer[source->size.maxEntrySize];
    int height = rSrc2-rSrc1;
    int width = cSrc2-cSrc1;
    for(int row = 0;row<=height;row++){
        for(int col = 0;col<=width;col++){
            if(rDest+row >= dest->size.rCount || cDest+col >= dest->size.cCount){
                break;
            }
            if(CSVWRITEREF(dest,(rDest+row),(cDest+col),CSVREADREF(source,(row+rSrc1),(col+cSrc1)))!=0){
                strncpy(buffer,CSVREADREF(source,(row+rSrc1),(col+cSrc1)),source->size.maxEntrySize);
                buffer[dest->size.maxEntrySize-1] = 0;
                if(CSVWRITEREF(dest,(rDest+row),(cDest+col),buffer)!=0){
                    printf("Error copying %s to row %d column %d\n",buffer,(rDest+row),(cDest+col));
                    printf("Source string size: %llu Dest size: %d\n",strlen(buffer),dest->size.maxEntrySize);
                }
            }
        }
    }
    return 0;
}

int rectangleSwap(CSV *source, CSV *dest,
                  int rSrc1,int cSrc1,
                  int rSrc2,int cSrc2,
                  int rDest,int cDest){

    if(rSrc1 >= source->size.rCount || cSrc1 >= source->size.cCount || rSrc2 >= source->size.rCount || cSrc2 >= source->size.cCount){
        printf("Error: Out of bounds source in rectangleSwap\n"); //src is an index. Therefore the max is rCount-1 or cCount -1.
        printf("rSrc1: %d cSrc1: %d rSrc2: %d cSrc2: %d rMax: %d cMax: %d\n",rSrc1,cSrc1,rSrc2,cSrc2,dest->size.rCount,dest->size.cCount);
        return -1;
    }
    if(rDest >= dest->size.rCount || cDest >= dest->size.cCount){
        printf("Error: Out of bounds destination in rectangleSwap\n"); //dest is an index. Therefore the max is rCount-1 or cCount -1.
        printf("rDest: %d cDest: %d rMax: %d cMax: %d\n",rDest,cDest,dest->size.rCount,dest->size.cCount);
        return -1;
    }
    char buffer[(source->size.maxEntrySize > dest->size.maxEntrySize ? source->size.maxEntrySize : dest->size.maxEntrySize)];
    int height = rSrc2-rSrc1;
    int width = cSrc2-cSrc1;

    if(rDest+height >= dest->size.rCount || cDest+width >= dest->size.cCount){
        printf("Error: Out of bounds destination in rectangleSwap\n"); //dest is an index. Therefore the max is rCount-1 or cCount -1.
        printf("rDest: %d cDest: %d rMax: %d cMax: %d\n",rDest,cDest,dest->size.rCount,dest->size.cCount);
        return -1;
    }

    for(int row = 0;row<=height;row++){
        for(int col = 0;col<=width;col++){
            if(rDest+row >= dest->size.rCount || cDest+col >= dest->size.cCount){
                break;
            }
            strncpy_s(buffer,dest->size.maxEntrySize,CSVREADREF(dest,(row+rDest),(col+cDest)),dest->size.maxEntrySize);
            //puts destination info into buffer
            if(CSVWRITEREF(dest,(rDest+row),(cDest+col),CSVREADREF(source,(row+rSrc1),(col+cSrc1)))!=0){ //copies source into dest
                char truncateBuffer[source->size.maxEntrySize];
                strncpy(truncateBuffer,CSVREADREF(source,(row+rSrc1),(col+cSrc1)),source->size.maxEntrySize);
                truncateBuffer[dest->size.maxEntrySize-1] = 0;
                if(CSVWRITEREF(dest,(rDest+row),(cDest+col),truncateBuffer)!=0){
                    printf("Error copying %s to row %d column %d\n",truncateBuffer,(rDest+row),(cDest+col));
                    printf("Source string size: %llu Dest size: %d\n",strlen(truncateBuffer),dest->size.maxEntrySize);
                }
            }
            if(CSVWRITEREF(source,(rSrc1+row),(cSrc1+col),buffer)!=0){ //copies source into source
                char truncateBuffer[dest->size.maxEntrySize];
                strncpy(truncateBuffer,CSVREADREF(dest,(row+rDest),(col+cDest)),dest->size.maxEntrySize);
                truncateBuffer[source->size.maxEntrySize-1] = 0;
                if(CSVWRITEREF(source,(rSrc1+row),(cSrc1+col),truncateBuffer)!=0){
                    printf("Error copying %s to row %d column %d\n",truncateBuffer,(rSrc1+row),(cSrc1+col));
                    printf("Source string size: %llu Dest size: %d\n",strlen(truncateBuffer),source->size.maxEntrySize);
                }
            }

        }
    }
    return 0;
}

int rectangleCopy_s(CSV *restrict source, CSV *restrict dest,
                    int rSrc1,int cSrc1,
                    int rSrc2,int cSrc2,
                    int rDest,int cDest){
    /*
     * Rectangle copy but with restrict.
     * Unsure how much this helps the compiler out due to the abstractions being dealt with here,
     * but it is generally considered unsafe to rectangleCopy with source and dest being the same, hence this function.
     */

    if(source == dest){
        printf("Error: source and destination are the same\n");
        return -1;
    }
    if(rSrc1 >= source->size.rCount || cSrc1 >= source->size.cCount || rSrc2 >= source->size.rCount || cSrc2 >= source->size.cCount){
        printf("Error: Out of bounds source in rectangleCopy\n"); //src is an index. Therefore the max is rCount-1 or cCount -1.
        printf("rSrc1: %d cSrc1: %d rSrc2: %d cSrc2: %d rMax: %d cMax: %d\n",rSrc1,cSrc1,rSrc2,cSrc2,dest->size.rCount,dest->size.cCount);
        return -1;
    }
    if(rDest >= dest->size.rCount || cDest >= dest->size.cCount){
        printf("Error: Out of bounds destination in rectangleCopy\n"); //dest is an index. Therefore the max is rCount-1 or cCount -1.
        printf("rDest: %d cDest: %d rMax: %d cMax: %d\n",rDest,cDest,dest->size.rCount,dest->size.cCount);
        return -1;
    }

    char buffer[source->size.maxEntrySize];
    int height = rSrc2-rSrc1;
    int width = cSrc2-cSrc1;

    if(rDest+height >= dest->size.rCount || cDest+width >= dest->size.cCount){
        printf("Error: Out of bounds destination in rectangleCopy\n"); //dest is an index. Therefore the max is rCount-1 or cCount -1.
        printf("rDest: %d cDest: %d rMax: %d cMax: %d\n",rDest,cDest,dest->size.rCount,dest->size.cCount);
        return -1;
    }

    for(int row = 0;row<=height;row++){
        for(int col = 0;col<=width;col++){
            if(rDest+row >= dest->size.rCount || cDest+col >= dest->size.cCount){
                break;
            }
            if(CSVWRITEREF(dest,(rDest+row),(cDest+col),CSVREADREF(source,(row+rSrc1),(col+cSrc1)))!=0){
                strncpy(buffer,CSVREADREF(source,(row+rSrc1),(col+cSrc1)),source->size.maxEntrySize);
                buffer[dest->size.maxEntrySize-1] = 0;
                if(CSVWRITEREF(dest,(rDest+row),(cDest+col),buffer)!=0){
                    printf("Error copying %s to row %d column %d\n",buffer,(rDest+row),(cDest+col));
                    printf("Source string size: %llu Dest size: %d\n",strlen(buffer),dest->size.maxEntrySize);
                }
            }
        }
    }
    return 0;
}

void sortRows(CSV *source, int (*sortAlgo)(char*, char*), int sortedColumn){
    size_t rowSize = source->size.maxEntrySize * source->size.cCount;
    char rowBuffer[rowSize];



    for(int iterator = source->settings.colHeader;iterator<source->size.rCount;iterator++){
        char base[source->size.maxEntrySize];
        strncpy(base,CSVREADREF(source,iterator,sortedColumn),source->size.maxEntrySize);

        for(int row = iterator;row<source->size.rCount;row++){
            int sortResult = sortAlgo(base,CSVREADREF(source,row,sortedColumn));
            if(sortResult==1){
                memcpy(rowBuffer, CSVREADREF(source,row,0),rowSize);
                memcpy(CSVREADREF(source,row,0),CSVREADREF(source,iterator,0),rowSize);
                memcpy(CSVREADREF(source,iterator,0),rowBuffer,rowSize);
                strncpy(base,CSVREADREF(source,iterator,sortedColumn),source->size.maxEntrySize);
            }
            else if(sortResult == -1){ //makes invalid entries go to the bottom
                for(int backwards = source->size.rCount-1; backwards > row; backwards--){
                    if(sortAlgo(base,CSVREADREF(source,backwards,sortedColumn)) != -1){
                        memcpy(rowBuffer, CSVREADREF(source,row,0),rowSize);
                        memcpy(CSVREADREF(source,row,0),CSVREADREF(source,backwards,0),rowSize);
                        memcpy(CSVREADREF(source,backwards,0),rowBuffer,rowSize);
                    }
                }
            }

        }

    }


}