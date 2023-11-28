//
// Created by Aweso on 11/24/2023.
//

#include "csvUtils.h"



void removeEmptyRows(CSV *source,rmSelector mode){
    char isRowEmpty[source->size.rCount];
    memset(isRowEmpty,1,source->size.rCount);
    for(int row = 0;row<source->size.rCount;row++){
        for(int col = 0;col<source->size.cCount;col++){
            if(source->rows[CSVINDEXREF(source, row, col)] != 0 ){
                isRowEmpty[row]=0;
                break;
            }
        }
    }


    switch(mode){
        case Before: {
            int emptyRowCount = 0;
            for (int i = 0; i < source->size.rCount; i++) {
                if (isRowEmpty[i] == 0)break;
                emptyRowCount++;
            }
            if(emptyRowCount==0)break;
            source->size.rCount-=emptyRowCount;
            char *tempRows = (char*)calloc(source->size.rCount,sizeof(char[source->size.maxEntrySize]) * source->size.cCount);
            memcpy(tempRows, &source->rows[CSVINDEXREF(source, emptyRowCount, 0)], source->size.rCount * source->size.cCount * source->size.maxEntrySize);
            free(source->rows);
            source->rows = tempRows;
            break;
        }
        case 0:{
            break;
        }
        case After:{
            break;
        }
        default:
        case All:{
            break;
        }
    }
}