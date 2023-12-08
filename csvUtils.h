//
// Created by Aweso on 11/24/2023.
//
#ifndef CSV_CSV_H
#include "csv.h"
#endif

#ifndef CSV_CSVUTILS_H
#define CSV_CSVUTILS_H
#endif //CSV_CSVUTILS_H

typedef enum{
    Before = 2,
    Inside = 3,
    After = 4,
    All = 9, // Equal to B + I + A
    BeforeInside = 5, //Equal to before + inside
    Outside = 6, //Equal to before + after
    AfterInside = 7 //Equal to After + inside


}rmSelector;

void removeEmpty(CSV *source, rmSelector mode, char rc);

void mapRows(CSV *source,char dest[source->size.rCount]);

void removeEmptyRows(CSV *source,rmSelector mode);

void removeEmptyColumns(CSV *source, rmSelector mode);

void mapColumns(CSV *source,char dest[source->size.cCount]);

int rectangleCopy(CSV *source, CSV *dest,
                  int rSrc1,int cSrc1,
                  int rSrc2,int cSrc2,
                  int rDest,int cDest);

int rectangleCopy_s(CSV *restrict source, CSV *restrict dest,
                    int rSrc1,int cSrc1,
                    int rSrc2,int cSrc2,
                    int rDest,int cDest);

int rectangleSwap(CSV *source, CSV *dest,
                  int rSrc1,int cSrc1,
                  int rSrc2,int cSrc2,
                  int rDest,int cDest);