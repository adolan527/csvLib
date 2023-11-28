//
// Created by Aweso on 11/24/2023.
//

#ifndef CSV_CSVUTILS_H
#include "csvUtils.h"
#endif

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
            size_t size = source->size.cCount * source->size.maxEntrySize;
            memcpy(tempRows + (i-emptyRowsReached) * (source->size.cCount * source->size.maxEntrySize), CSVREADREF(source,i,0), size);
        }
        else{emptyRowsReached++;}
    }
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

