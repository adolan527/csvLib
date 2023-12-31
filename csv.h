//
// Created by Aweso on 11/12/2023.
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#ifndef CSV_CSV_H
#define CSV_CSV_H



typedef struct{
    int rCount;
    int cCount;
    int maxEntrySize;
}Dimensions;

typedef struct{
    char colDelin;
    char rowDelin;
    int colHeader;
    int rowHeader;

}CSVSettings;

typedef struct{
    Dimensions size;
    CSVSettings settings;
    char *filename;
    char *rows;

}CSV;



extern const CSVSettings DEFAULT_SETTINGS;

//Macros for accessing CSV, function calls would be unecessary, these don't have side-effects, and are commonly used.
//Ref for pointer to CSV, LIT for CSV by value.

// "Returns" the value of the number that needs to be inputted into csv.rows[NUM] to access the data at _row,_col.
#define CSVINDEXREF(_csvPtr,_row,_col) (_csvPtr->size.maxEntrySize * (_row * _csvPtr->size.cCount + _col))
#define CSVINDEXLIT(_csv,_row,_col) (_csv.size.maxEntrySize * (_row * _csv.size.cCount + _col))

// "return" a pointer to the string at that index.
#define CSVREADREF(_csvPtr,_row,_col) (&_csvPtr->rows[_csvPtr->size.maxEntrySize * (_row * _csvPtr->size.cCount + _col)])
#define CSVREADLIT(_csv,_row,_col) (&_csv.rows[_csv.size.maxEntrySize * (_row * _csv.size.cCount + _col)])

//wrapper for strncpy_s. Having function calls in a macro is cringe, but strncpy_s is far superior to strcpy at least.
//Returns 0 on success, non zero on failure. If you are going to use this, you should at least check for 0.
#define CSVWRITEREF(_csvPtr,_row,_col,_srcPtr) strncpy_s(&_csvPtr->rows[CSVINDEXREF(_csvPtr,_row,_col)], _csvPtr->size.maxEntrySize, _srcPtr, _csvPtr->size.maxEntrySize)
#define CSVWRITELIT(_csv,_row,_col,_srcPtr) strncpy_s(&_csv.rows[CSVINDEXLIT(_csv,_row,_col)], _csv.size.maxEntrySize,_srcPtr, _csv.size.maxEntrySize)


CSV openCSV(FILE *source, CSVSettings settings);

void closeCSV(CSV *subject);

void printRow(CSV *source,int row, int displayedCharPerEntry, FILE *outputStream);

void printColumn(CSV *source,int col, int displayedCharPerEntry, FILE *outputStream);

void displayCSV(CSV *source, int displayedCharPerEntry, int colStart, int colEnd, FILE *outputStream);

int saveCSV(CSV *source, char *filename);

Dimensions getSize(FILE *source,CSVSettings settings);

void arrayToCSV(CSV *dest, char array[], int arraySize, size_t arrayElementSize, int rowIndex, int colIndex, char rc);

CSV makeBlankCSV(int rCount, int cCount, int maxEntrySize);

CSV DMakeBlankCSV(Dimensions source);

int indexOfHeaderRow(CSV *source, char *header);

int indexOfHeaderCol(CSV *source, char *header);

char * indexByHeader(CSV *source, char *rKey, char *cKey);

int indexToCoordinates(CSV *source, int index, int *rowDest, int *colDest);

void copyCSV(CSV *source, CSV *dest);

void resizeCSV(CSV *source,Dimensions newSize);

void changeMES(CSV *source,int newMES);

void removeRow(CSV *source, int row);

void removeColumn(CSV *source, int col);

CSV easyOpenCSV(char *filename);

void easyCloseCSV(CSV *subject);

void easySaveCSV(CSV *source);


#endif //CSV_CSV_H