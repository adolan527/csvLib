# Documentation for csv.c

## Overview

This library is intended to make management of .csv files easy and powerful within C for the developer.
This document will provide enough information to use the csv.c functions and data structures.
It does not go into the source code of the functions, although that is available in csv.c, you could ideally treat these functions as black boxes.

### Data Structures

#### Dimensions
```c
typedef struct{ 
    int rCount;
    int cCount;
    int maxEntrySize;}
Dimensions; 
```
Provides a convenient way to store the dimensions of a CSV file. **rCount** is the number of rows, **cCount** is the number of columns, and **maxEntrySize** is the maximum number of characters in any entry of the CSV.
It is rarely used outside of a **CSV** struct.

#### CSVSettings
```c
typedef struct{
    char colDelin;
    char rowDelin;
    int colHeader;
    int rowHeader;
}CSVSettings;
```
Similar to **Dimensions**, **CSVSettings** is primarily used as a part of the **CSV** struct.
**colDelin** is the character that separates columns (","), **rowDelin** is the character that separates rows ("\n").
**colHeader** and **rowHeader** indicate the presence of a header row and column respectively. 0 indicates no header, 1 indicates a header.
The default settings are declared in csv.h and defined in csv.c as:
```c
const CSVSettings DEFAULT_SETTINGS = {',','\n',0,0};
```
If you need to change the default settings, make a new CSVSettings, as DEFAULT_SETTINGS is a global constant.

#### CSV
```c
typedef struct{
    Dimensions size;
    CSVSettings settings;
    char *filename;
    char *rows;
}CSV;
```
The main data structure for this library. The **size** and **settings** fields are explained above. **filename** points to a string that contains the name of the file the CSV was made from, entirely optional to use.
**rows** is a pointer to a string that contains the data of the CSV. The data is stored in a 1D array, with each entry separated by **rowDelin** and **colDelin**. The data is stored in row-major order, meaning that the first row is stored first, then the second row, and so on. The first entry of each row is stored first, then the second entry, and so on. The size of the array is **size.rCount * size.cCount * size.maxEntrySize**. All entries are strings.


### Macros
#### CSVINDEXREF and CSVINDEXLIT
```c
#define CSVINDEXREF(_csvPtr,_row,_col) (_csvPtr->size.maxEntrySize * (_row * _csvPtr->size.cCount + _col))
#define CSVINDEXLIT(_csv,_row,_col) (_csv.size.maxEntrySize * (_row * _csv.size.cCount + _col))
```
"Returns" the index of the entry at the given row and column. The index is the index of the first character of the entry. **_csvPtr** is a pointer to a **CSV** struct. **_row** and **_col** are the row and column of the entry respectively.

Example:
```c
CSV example;
example.rows[CSVINDEXLIT(example,1,2)] = 'a';
CSV *examplePtr = &example;
examplePtr->rows[CSVINDEXREF(examplePtr,1,2)] = 'b';
```
#### CSVREADREF and CSVREADLIT
```c
#define CSVREADREF(_csvPtr,_row,_col) (&_csvPtr->rows[_csvPtr->size.maxEntrySize * (_row * _csvPtr->size.cCount + _col)])
#define CSVREADLIT(_csv,_row,_col) (&_csv.rows[_csv.size.maxEntrySize * (_row * _csv.size.cCount + _col)])
```
Similar to the CSVINDEX macros, except it returns a pointer to the entry instead of the index of the entry. **_csvPtr** is a pointer to a **CSV** struct. **_row** and **_col** are the row and column of the entry respectively.

Example:
```c
CSV example;
printf("%s",CSVREADLIT(example,1,2))
CSV *examplePtr = &example;
printf("%s",CSVREADREF(examplePtr,1,2))
```
#### CSVWRITEREF and CSVWRITELIT
```c
#define CSVWRITEREF(_csvPtr,_row,_col,_srcPtr) strncpy_s(&_csvPtr->rows[CSVINDEXREF(_csvPtr,_row,_col)], _csvPtr->size.maxEntrySize, _srcPtr, _csvPtr->size.maxEntrySize)
#define CSVWRITELIT(_csv,_row,_col,_srcPtr) strncpy_s(&_csv.rows[CSVINDEXLIT(_csv,_row,_col)], _csv.size.maxEntrySize,_srcPtr, _csv.size.maxEntrySize)
```
Similar to the CSVINDEX macros, except it writes the string at the given pointer to the entry instead of returning the index of the entry. **_csvPtr** is a pointer to a **CSV** struct. **_row** and **_col** are the row and column of the entry respectively. **_srcPtr** is a pointer to the string to be written to the entry.
As this is essentially a wrapper for strncpy_s, it is recommended to check the return value of this macro. If it is non-zero, the write failed.

Example:
```c
CSV example;
CSVWRITELIT(example,1,2,"Hello World!");
CSV *examplePtr = &example;
CSVWRITEREF(examplePtr,1,2,"Hello World!");
```

### Functions


#### openCSV
```c
CSV openCSV(FILE *source, CSVSettings settings);
```

Returns a **CSV** struct containing the data from the given file. **source** is a pointer to the file to be read. **settings** is a **CSVSettings** struct that contains the settings for the CSV.
The colDelin and rowDelin in the **CSVSettings** struct will be used in parsing the file.

Example:
```c
FILE *exampleFile = fopen("example.csv","r");
CSV example = openCSV(exampleFile,DEFAULT_SETTINGS);
```

#### closeCSV
```c
void closeCSV(CSV *subject);
```
Frees the memory allocated for the **CSV** struct. **subject** is a pointer to the **CSV** struct to be freed.

Example:
```c
CSV example;
closeCSV(&example);
```

#### displayCSV
```c
void displayCSV(CSV *source, int displayedCharPerEntry, int colStart, int colEnd, FILE *outputStream);
```
Displays the CSV to the given output stream. 
**source** is a pointer to the **CSV** struct to be displayed. 
**displayedCharPerEntry** is the number of characters to display per entry. If it is 0, then it will default to **source->size.maxEntrySize**. If a string is longer than the **displayedCharPerEntry**, it will be truncated.
**colStart** and **colEnd** define the range of columns to be displayed. If **colEnd** is 0 then it will display to the end of the **CSV**.
If **CSV.settings.colHeader** or **.rowHeader** are 1, those rows/columns will be displayed above/beside the data. If those values are 0, then indices are displayed instead.

Example Code:
```c
FILE *file = fopen(filename,"r");
if(file == NULL){
    return 1;
}
CSV names = openCSV(file, DEFAULT_SETTINGS);
fclose(file);
names.settings.colHeader = 1;
names.settings.rowHeader = 1;
displayCSV(&names,12,0,0,stdout);
```
Example Output:
```
        Name|         Age|         Job|        rank|      Salary|
         Joe|          21,     manager,     private,          99
         Bob|          34,      worker,     general,         100
       Jimmy|           4,       thief,       Elite,      Unpaid
     Derrick|           9,      intern,    corporal,          -1
"Charles, th|          80,        King,Grand Marsha,"1,000,000,0
```

#### printRow and printColumn

```c
void printRow(CSV *source,int row, int displayedCharPerEntry, FILE *outputStream);

void printColumn(CSV *source,int col, int displayedCharPerEntry, FILE *outputStream);
```

Displays the given row/column to the given output stream.
**source** is a pointer to the **CSV** struct to be displayed. **row** and **col** are the row and column to be displayed respectively.
**displayedCharPerEntry** is the number of characters to display per entry. If it is 0, then it will default to the largest entry present. If it is -1, then all entries will be displayed without padding. 
If a string is longer than the **displayedCharPerEntry** (excluding -1 and 0), it will be truncated.
Ignores headers.

Example:
```c
FILE *file = fopen(filename2,"r");
    if(file == NULL){
        return 1;
    }
    CSV names = openCSV(file, DEFAULT_SETTINGS);
    fclose(file);
    printRow(&names,0,0,stdout);
    printRow(&names,1,-1,stdout);
    printColumn(&names,0,4,stdout);
    closeCSV(&names);
```
Example Output:
```
  Name,   Age,   Job,  rank,Salary
Joe,21,manager,private,99
Name
 Joe
 Bob
Jimm
Derr
"Cha
```


#### saveCSV
```c
int saveCSV(CSV *source, char *filename);
```
Saves the CSV to the given file. **source** is a pointer to the **CSV** struct to be saved. **filename** is the name of the file to be saved to. If the file already exists, it will be overwritten.
The colDelin and rowDelin of the CSV will be used to separate the entries. Trailing whitespace will be removed from each entry.

Example:
```c
FILE *file = fopen(filename,"r");
if(file == NULL){
    return 1;
}
CSV names = openCSV(file, DEFAULT_SETTINGS);
fclose(file);
if(CSVWRITELIT(names,0,2,"Dave") != 0){
    closeCSV(&names);
    return 1;
}
saveCSV(&names,filename);
closeCSV(&names);
```

#### getSize
```c
Dimensions getSize(FILE *source,CSVSettings settings);
```
Returns the size of the CSV in the given file. **source** is a pointer to the file to be read. **settings** is a **CSVSettings** struct that contains the settings for the CSV.
Only used internally for openCSV.

#### makeBlankCSV and DMakeBlankCSV
```c
CSV makeBlankCSV(int rCount, int cCount, int maxEntrySize);
CSV DMakeBlankCSV(Dimensions source);
```
Returns a blank **CSV** struct. **rCount** is the number of rows, **cCount** is the number of columns, and **maxEntrySize** is the maximum number of characters in any entry of the CSV.
DMakeBlankCSV is the same as makeBlankCSV, except it takes a **Dimensions** struct instead of the individual values.

Example:
```c
CSV example = makeBlankCSV(5,5,10);
Dimensions exampleSize = {5,5,10};
CSV example2 = DMakeBlankCSV(exampleSize);
saveCSV(&example,"example.csv");
closeCSV(&example);
closeCSV(&example2);
```

#### arrayToCSV
```c
void arrayToCSV(CSV *dest, char array[], int arraySize, size_t arrayElementSize, int rowIndex, int colIndex, char rc);
```
Copies **array** into the CSV pointed to by **dest**.
**arraySize** is the number of elements in the array. Each element is assumed to be a string that is **arrayElementSize** characters long.
**rowIndex** and **colIndex** are the row and column to start copying the array into. **rc** specifies if the data should be copied into the row or column. 'r' for row, 'c' for column.

Example:
```c
CSV example = makeBlankCSV(5,5,10);
char array[5][10] = {"Hello","World","How","Are","You"};
arrayToCSV(&example,array,5,10,0,0,'r');
arrayToCSV(&example,*array,5,10,1,3,'c');
displayCSV(&example,0,0,0,stdout);
closeCSV(&example);
```
Example Output:
```
  |             A|             B|             C|             D|             E|
 0|         Hello,         World,           How,           Are,           You
 1|              ,              ,              ,         Hello,              
 2|              ,              ,              ,         World,              
 3|              ,              ,              ,           How,              
 4|              ,              ,              ,           Are,                        


```

#### indexOfHeaderRow and indexOfHeaderCol
```c
int indexOfHeaderRow(CSV *source, char *header);
int indexOfHeaderCol(CSV *source, char *header);
```
Returns the index of the row/column with the given header. **source** is a pointer to the **CSV** struct to be searched. **header** is the header to search for.
If the header is not found or the CSV's settings have 0 for the header, -1 is returned.
Don't use these functions as indices for the macros, as functions inside of macros makes debugging very difficult and could cause unexpected behavior.

Example:
```c
int rowIndex = indexOfHeaderRow(&names,"Jimmy");
int colIndex = indexOfHeaderCol(&names,"Job");
if(rowIndex == -1 || colIndex == -1){
        printf("Error: Could not find Jimmy or Job");
        return 1;
    }
printf("\n Jimmy's Job: %s\n", CSVREADLIT(names,rowIndex,colIndex));
//Output: Jimmy's Job: thief
```

#### indexByHeader
```c
char * indexByHeader(CSV *source, char *rKey, char *cKey);
```
Returns a pointer to the entry at the row with the header **rKey** and the column with the header **cKey**. **source** is a pointer to the **CSV** struct to be searched. **rKey** and **cKey** are the headers to search for.
If the headers are not found, then NULL is returned. If the CSV settings indicate no header for the row/column, the key is attempted to be converted to an integer and used as an index. If the conversion fails NULL is returned.

Example:
```c
printf("Joe's job is: %s",indexByHeader(&example,"Joe","Job"));
//Output: Joe's job is: manager
```

#### indexToCoordinates
```c
int indexToCoordinates(CSV *source, int index, int *rowDest, int *colDest);
```
Converts the given index to the row and column of the entry. **source** is a pointer to the **CSV** struct to be searched. **index** is the index of the entry to be converted. **rowDest** and **colDest** are pointers to the integers to store the row and column in.
Very niche use, but doing it manually is a pain.

Example:
```c
int row,col;
indexToCoordinates(&names,CSVINDEXLIT(example,1,2),&row,&col);
indexToCoordinates(&names,indexByHeader(&names,"Jimmy","Job"),&row,&col);
```

#### copyCSV
```c
void copyCSV(CSV *source, CSV *dest);
```
Copies the data from **source** to **dest**. **source** is a pointer to the **CSV** struct to be copied. **dest** is a pointer to the **CSV** struct to be copied to.
Truncates entries if necessary. 
If the destination CSV is smaller than the source CSV, the data will be truncated. If the destination CSV is larger than the source CSV, the extra entries will be set to empty strings.
If the destination CSV is larger than the source CSV, then data will be lost.

Example:
```c
CSV blank = makeBlankCSV(8,3,5);
CSV names = openCSV(file, DEFAULT_SETTINGS);
fclose(file);
copyCSV(&names, &blank);
displayCSV(&blank,5,0,0,stdout);
```
Example Output:
```
  |    A|    B|    C|
 0| Name,  Age,  Job
 1|  Joe,   21, mana
 2|  Bob,   34, work
 3| Jimm,    4, thie
 4| Derr,    9, inte
 5| "Cha,   80, King
 6|     ,     ,
 7|     ,     ,
 ```

#### resizeCSV
```c
void resizeCSV(CSV *source,Dimensions newSize);
```

Resizes the CSV to the given dimensions. **source** is a pointer to the **CSV** struct to be resized. **newSize** is a **Dimensions** struct containing the new dimensions of the CSV.
Functions similar to if you made a new blank CSV with **newSize** dimensions, copied source into it, and then replaced source with the new CSV.
Data is truncated, settings are not touched, dimensions are updated.

Example:
```c
CSV names = openCSV(file, DEFAULT_SETTINGS);
fclose(file);
resizeCSV(&names,(Dimensions){.rCount = 12, .cCount = 3, .maxEntrySize = 20});
displayCSV(&names,0,0,0,stdout);

```
Example Output:
```
  |                   A|                   B|                   C|
 0|                Name,                 Age,                 Job
 1|                 Joe,                  21,             manager
 2|                 Bob,                  34,              worker
 3|               Jimmy,                   4,               thief
 4|             Derrick,                   9,              intern
 5| "Charles, the Great,                  80,                King
 6|                    ,                    ,
 7|                    ,                    ,
 8|                    ,                    ,
 9|                    ,                    ,
10|                    ,                    ,
11|                    ,                    ,
```

#### changeMES
```c
void changeMES(CSV *source,int newMES);
```
Changes the maximum entry size of the CSV. **source** is a pointer to the **CSV** struct to be resized. **newMES** is the new maximum entry size.
Wrapper for **resizeCSV** due to the frequency of changing the maximum entry size. All other attributes are unchanged.

### Easy Functions
These functions are not fully implemented yet, but the goal is to reduce the amount of overhead for the programmer with these functions.
In the future, there will be "easy" ways to all of the actions one might wish to do with a CSV.

#### easyOpenCSV
```c
CSV easyOpenCSV(char *filename);
```
Returns a **CSV** struct containing the data from the given file. **filename** is the name of the file to be read.
Wrapper for fopen, openCSV, and fclose.

Example:
```c
CSV example = easyOpenCSV("example.csv");
```

#### easyCloseCSV
```c
void easyCloseCSV(CSV *subject);
```
Frees the memory allocated for the **CSV** struct. **subject** is a pointer to the **CSV** struct to be freed.

Example:
```c
CSV example = easyOpenCSV("example.csv");
easyCloseCSV(&example);
```

#### easySaveCSV
```c
void easySaveCSV(CSV *source);
```
Saves the CSV to the given file. **source** is a pointer to the **CSV** struct to be saved. The filename is taken from **source->filename**.

Example:
```c
CSV example = easyOpenCSV("example.csv");
easySaveCSV(&example);
easyCloseCSV(&example);
```
