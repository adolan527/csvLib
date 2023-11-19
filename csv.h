//
// Created by Aweso on 11/12/2023.
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef CSV_CSV_H
#define CSV_CSV_H

#endif //CSV_CSV_H

typedef struct{
    int rCount;
    int cCount;
    int maxEntrySize;
}Dimensions;

typedef struct{
    Dimensions size;
    char *rows;
    FILE *fileSource;
}CSV;


CSV easyOpenCSV(char *filename);

void easyCloseCSV(CSV *subject);

void closeCSV(CSV *subject);

Dimensions getSize(FILE *source, char colDelin, char rowDelin, int maxEntrySize);

void readCSV(CSV *subject, char destination[subject->size.maxEntrySize], int rowIndex, int colIndex, int characterIndex);

CSV openCSV(FILE *source, char colDelin, char rowDelin, int MAX_ENTRY_SIZE);

void printCSV(CSV *source,int rowStartVal,char colDelin, char rowDelin);

void editCSV(CSV *subject, char info[subject->size.maxEntrySize], int rowIndex, int colIndex, int characterIndex);

void saveCSV(CSV *source, FILE *dest, int indexStart);

void arrayToCSV(CSV *dest,void *array, int size, size_t arrayElementSize, int rowIndex, int colIndex,char rc);

CSV makeBlankCSV(int rCount, int cCount, int maxEntrySize);

CSV DMakeBlankCSV(Dimensions *source);
