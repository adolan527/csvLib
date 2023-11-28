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

void removeEmptyRows(CSV *source,rmSelector mode);