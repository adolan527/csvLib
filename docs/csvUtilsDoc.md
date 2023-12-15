# Documentation for csv.c

## Overview

This document assumes you have at least glanced over the csv.c/csv.h/csvDoc.md files.
The data structures there are used in this document as well.
This document is meant to give the developer information to use the tools in csvUtils.c.
These functions including sorting, removing empty rows/columns, and copying large sections of the CSV.

## Sorting

#### SortMode

```c
typedef enum{
    IncNum,
    DecNum,
    IncAlpha,
    DecAlpha,
}SortMode;

extern int (*sortModes[4])(char*,char*);
```

This enum can be used as an index into the global array sortModes to select a sorting algorithm.
Used in the **sortRows** function.
I wish that this could be constant, but I could not figure out how to do so.

### Functions

#### numIncreasing, numDecreasing, alphaIncreasing, alphaDecreasing

```c
int numIncreasing(char *op1, char *op2);
//sorts in increasing numeric order using strtold. 
//Does not try to work around symbols such as $, %, etc.
int numDecreasing(char *op1, char *op2);
//decreasing numeric order
int alphaIncreasing(char *op1, char *op2);
//alphabetic order (according to strcmp)
int alphaDecreasing(char *op1, char *op2);
//reverse alphabetic order (according to strcmp)
```

These functions evaluate two strings. If the one on the left is TRUE, then return 0.
Else, return 1. Errors return -1.
These functions are pointed to by the sortModes array, and are used in the sortRows function.

#### sortRows

```c
void sortRows(CSV *source, int (*sortAlgo)(char*, char*), int sortedColumn);
```

This function sorts the rows of the **CSV** based on the column specified by sortedColumn.
The comparison algorithm used is specified by the function pointer **sortAlgo**.
If an entry returns an error, then the function will try and push the error entry to the bottom.
The developer could put any function in there, so long as it has the same return type and parameters. Its return value should be:
- 0 if the left operand should come first or the two are equal
- 1 if the right operand should come first
- -1 if there is an error or an invalid entry


Example with DecAlpha sort mode on the Job column:

```c
    FILE *file = fopen(filename2,"r");
    if(file == NULL){
        return 1;
    }
    CSV names = openCSV(file, DEFAULT_SETTINGS);
    fclose(file);
    
    names.settings.colHeader = 1;
    names.settings.rowHeader = 1;
    
    displayCSV(&names,12,stdout);
    
    int colIndex = indexOfHeaderCol(&names,"Job");
    if(colIndex == -1) {
        printf("Error: Header not found.\n");
        return 1;
    }

    sortRows(&names,sortModes[DecAlpha], colIndex);
    printf("\nSorted:\n");
    displayCSV(&names,12,stdout);
    
    closeCSV(&names);
```

Example Output:

```
        Name|         Age|         Job|        rank|      Salary|
         Joe|          21,     manager,     private,          99
         Bob|          34,      worker,     general,         100
       Jimmy|           4,       thief,       Elite,      Unpaid
     Derrick|           9,      intern,    corporal,          -1
"Charles, th|          80,        King,Grand Marsha,"1,000,000,0

Sorted:
        Name|         Age|         Job|        rank|      Salary|
         Bob|          34,      worker,     general,         100
       Jimmy|           4,       thief,       Elite,      Unpaid
         Joe|          21,     manager,     private,          99
     Derrick|           9,      intern,    corporal,          -1
"Charles, th|          80,        King,Grand Marsha,"1,000,000,0
```

Example with IncNum sort mode on the Salary column:

```c
    FILE *file = fopen(filename2,"r");
    if(file == NULL){
        return 1;
    }
    CSV names = openCSV(file, DEFAULT_SETTINGS);
    fclose(file);
    
    names.settings.colHeader = 1;
    names.settings.rowHeader = 1;
    
    displayCSV(&names,12,stdout);
    
    int colIndex = indexOfHeaderCol(&names,"Salary");
    if(colIndex == -1) {
        printf("Error: Header not found.\n");
        return 1;
    }

    sortRows(&names,sortModes[IncNum], colIndex);
    printf("\nSorted:\n");
    displayCSV(&names,12,stdout);
    
    closeCSV(&names);
```

Example Output:
Note that the non-numeric entries are sorted to the bottom. This is true in both numeric sorts.

```
        Name|         Age|         Job|        rank|      Salary|
         Joe|          21,     manager,     private,          99
         Bob|          34,      worker,     general,         100
       Jimmy|           4,       thief,       Elite,      Unpaid
     Derrick|           9,      intern,    corporal,          -1
"Charles, th|          80,        King,Grand Marsha,"1,000,000,0

Sorted:
        Name|         Age|         Job|        rank|      Salary|
         Joe|          21,     manager,     private,          99
     Derrick|           9,      intern,    corporal,          -1
         Bob|          34,      worker,     general,         100
       Jimmy|           4,       thief,       Elite,      Unpaid
"Charles, th|          80,        King,Grand Marsha,"1,000,000,0
```
## Removing Empty Rows/Columns

#### rmSelector

```c
typedef enum{
    Before = 2,
    Inside = 3,
    After = 4,
    All = 9, // Equal to B + I + A
    BeforeInside = 5, //Equal to before + inside
    BeforeAfter = 6, //Equal to before + after
    AfterInside = 7 //Equal to After + inside
}rmSelector;
```

This enum is used in the **removeEmpty** functions to select which parts of the **CSV** are removed.
Notably, you can add or append the section names. A very niche and potentially useless consideration, but interesting nonetheless.

### Functions

#### removeEmpty, removeEmptyRows, removeEmptyColumns

```c
void removeEmpty(CSV *source, rmSelector mode, char rc);

void removeEmptyRows(CSV *source,rmSelector mode);

void removeEmptyColumns(CSV *source, rmSelector mode);
```

These functions remove empty rows or columns from the **CSV**.
The **mode** parameter selects which rows/columns are removed.
Before, Inside, and After remove the rows/columns before, inside, and after section of the CSV with data in it.
**removeEmpty** uses the **rc** parameter to select whether to remove rows or columns. 'r' for rows, 'c' for columns, and anything else for both.
There is a small chance that some bugs still exist in the **removeEmptyColumns** sub-functions, but I will test that further in the future.
The time complexity of these functions is not great, but these should not be run too often.


Example with ALL mode on rows and columns:
```c
    FILE *file = fopen(filename3,"r");
    if(file == NULL){
        return 1;
    }
    CSV letters = openCSV(file, DEFAULT_SETTINGS);
    fclose(file);
    
    printf("Rows: %d, Columns: %d\n",letters.size.rCount,letters.size.cCount);
    displayCSV(&letters,0,stdout);
    printf("\n");
    removeEmpty(&letters,All,0);
    printf("Rows: %d, Columns: %d\n",letters.size.rCount,letters.size.cCount);
    displayCSV(&letters,0,stdout);

    closeCSV(&letters);
```

Example Output:
```
Rows: 7, Columns: 7
  |   A|   B|   C|   D|   E|   F|   G|
 0|    ,    ,    ,    ,    ,    ,
 1|    ,   a,    ,   b,    ,   c,
 2|    ,   d,    ,   e,    ,   f,
 3|    ,    ,    ,    ,    ,    ,
 4|    ,   g,    ,   h,    ,   i,
 5|    ,   j,    ,   k,    ,   l,
 6|    ,    ,    ,    ,    ,    ,

Rows: 4, Columns: 3
  |   A|   B|   C|
 0|   a,   b,   c
 1|   d,   e,   f
 2|   g,   h,   i
 3|   j,   k,   l
```
Example with Inside mode on columns and after mode on rows:
```c
FILE *file = fopen(filename3,"r");
    if(file == NULL){
        return 1;
    }
    CSV letters = openCSV(file, DEFAULT_SETTINGS);
    fclose(file);

    printf("Rows: %d, Columns: %d\n",letters.size.rCount,letters.size.cCount);
    displayCSV(&letters,0,stdout);
    printf("\n");
    removeEmptyColumns(&letters,Inside);
    removeEmptyRows(&letters,After);
    printf("Rows: %d, Columns: %d\n",letters.size.rCount,letters.size.cCount);
    displayCSV(&letters,0,stdout);

    closeCSV(&letters);
```

Example Output:
```
Rows: 7, Columns: 7
  |   A|   B|   C|   D|   E|   F|   G|
 0|    ,    ,    ,    ,    ,    ,
 1|    ,   a,    ,   b,    ,   c,
 2|    ,   d,    ,   e,    ,   f,
 3|    ,    ,    ,    ,    ,    ,
 4|    ,   g,    ,   h,    ,   i,
 5|    ,   j,    ,   k,    ,   l,
 6|    ,    ,    ,    ,    ,    ,

Rows: 6, Columns: 5
  |   A|   B|   C|   D|   E|
 0|    ,    ,    ,    ,
 1|    ,   a,   b,   c,
 2|    ,   d,   e,   f,
 3|    ,    ,    ,    ,
 4|    ,   g,   h,   i,
 5|    ,   j,   k,   l,
 ```

#### MapRows, MapColumns

```c
void mapRows(CSV *source,char dest[source->size.rCount]);
void mapColumns(CSV *source,char dest[source->size.cCount]);
```

These functions fill the dest array with 1s and 0s, where 1s represent rows/columns with data in them.
These functions are used in the **removeEmpty** functions, but could be useful in other applications.

Example:
```c
    FILE *file = fopen(filename3,"r");
    if(file == NULL){
        return 1;
    }
    CSV letters = openCSV(file, DEFAULT_SETTINGS);
    fclose(file);

    char map[letters.size.cCount];
    mapColumns(&letters,map);
    printf("Columns map before:\n");
    for(int i = 0;i<letters.size.cCount;i++){
        printf("%c,",map[i]+48);
    }
    printf("\n");

    displayCSV(&letters,0,stdout);
    printf("\n");
    removeEmptyColumns(&letters,Inside);

    mapColumns(&letters,map);
    printf("Columns map after:\n");
    for(int i = 0;i<letters.size.cCount;i++){
        printf("%c,",map[i]+48);
    }
    printf("\n");

    displayCSV(&letters,0,stdout);

    closeCSV(&letters);
```

Example Output:
```
Columns map before:
0,1,0,1,0,1,0,
  |   A|   B|   C|   D|   E|   F|   G|
 0|    ,    ,    ,    ,    ,    ,
 1|    ,   a,    ,   b,    ,   c,
 2|    ,   d,    ,   e,    ,   f,
 3|    ,    ,    ,    ,    ,    ,
 4|    ,   g,    ,   h,    ,   i,
 5|    ,   j,    ,   k,    ,   l,
 6|    ,    ,    ,    ,    ,    ,

Columns map after:
0,1,1,1,0,
  |   A|   B|   C|   D|   E|
 0|    ,    ,    ,    ,
 1|    ,   a,   b,   c,
 2|    ,   d,   e,   f,
 3|    ,    ,    ,    ,
 4|    ,   g,   h,   i,
 5|    ,   j,   k,   l,
 6|    ,    ,    ,    ,
 ```

## Rectangle Copying

```c
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
```

Copies data defined by a rectangle from the source **CSV** to the destination **CSV**.
The source rectangle is defined by the two points (rSrc1,cSrc1) and (rSrc2,cSrc2).
The destination rectangle is defined by the point (rDest,cDest) and the size of the source rectangle.
Return 0 on success, -1 on failure.

#### rectangleCopy
Fails if:
- Source coordinates are out of bounds of the CSV
- Destination coordinates are out of bounds of the CSV
  Source and dest can be the same **CSV**, however:
- If dest is before source, it will work as intended even if they overlap
- If dest and source don't overlap at all, then it works
- If dest is after source and they overlap, then it will copy itself and data will be lost
Will truncate the rectangle if it exceeds the bounds of the destination.
Will truncate entries.

#### rectangleCopy_s
Fails if:
- Source coordinates are out of bounds of the CSV
- Destination coordinates are out of bounds of the CSV
- Source and dest are the same **CSV**
- Dest + height or Dest + width of rectangle exceeds bounds of the CSV

Source and dest cannot be the same **CSV**. This is a safer version of rectangleCopy.
Will not truncate the rectangle if it exceeds the bounds of the destination, will fail instead.
Will truncate entries.

#### rectangleSwap
Fails if:
- Source coordinates are out of bounds of the CSV
- Destination coordinates are out of bounds of the CSV
- Dest + height or Dest + width of rectangle exceeds bounds of the CSV
Source and dest can be the same **CSV**, however:
- If dest is before source, it will work as intended even if they overlap
- If dest and source don't overlap at all, then it works
- If dest is after source and they overlap, then it will copy itself and data will be lost
Will not truncate the rectangle if it exceeds the bounds of the destination, will fail instead.
Will truncate entries.




Example rectangleCopy overlap error:
```c
    FILE *file = fopen(filename3,"r");
    if(file == NULL){
        return 1;
    }
    CSV letters = openCSV(file, DEFAULT_SETTINGS);
    fclose(file);

    displayCSV(&letters, 0, stdout);
    printf("\n");
    rectangleCopy(&letters, &letters, 1, 1, 5, 3, 2, 2);
    displayCSV(&letters, 0, stdout);

    closeCSV(&letters);


```
Example Output:
```
  |     A|     B|     C|     D|     E|     F|     G|
 0|     1,     2,     3,     4,     5,     6,     7
 1|     a,     b,     c,     d,     e,     f,     g
 2|    11,    22,    33,    44,    55,    66,    77
 3|    aa,    bb,    cc,    dd,    ee,    ff,    gg
 4|   101,   202,   303,   404,   505,   606,   707
 5|   a0a,   b0b,   c0c,   d0d,   e0e,   f0f,   g0g

  |     A|     B|     C|     D|     E|     F|     G|
 0|     1,     2,     3,     4,     5,     6,     7
 1|     a,     b,     c,     d,     e,     f,     g
 2|    11,    22,     b,     c,     d,    66,    77
 3|    aa,    bb,    22,     b,     c,    ff,    gg
 4|   101,   202,    bb,    22,     b,   606,   707
 5|   a0a,   b0b,   202,    bb,    22,   f0f,   g0g
```
Example rectangleCopy, dest truncated:
```c
    //all other code is the same
    rectangleCopy(&letters, &letters, 0, 0, 5, 1, 1, 3);
```
Example output:
```
  |     A|     B|     C|     D|     E|     F|     G|
 0|     1,     2,     3,     4,     5,     6,     7
 1|     a,     b,     c,     d,     e,     f,     g
 2|    11,    22,    33,    44,    55,    66,    77
 3|    aa,    bb,    cc,    dd,    ee,    ff,    gg
 4|   101,   202,   303,   404,   505,   606,   707
 5|   a0a,   b0b,   c0c,   d0d,   e0e,   f0f,   g0g

  |     A|     B|     C|     D|     E|     F|     G|
 0|     1,     2,     3,     4,     5,     6,     7
 1|     a,     b,     c,     1,     2,     f,     g
 2|    11,    22,    33,     a,     b,    66,    77
 3|    aa,    bb,    cc,    11,    22,    ff,    gg
 4|   101,   202,   303,    aa,    bb,   606,   707
 5|   a0a,   b0b,   c0c,   101,   202,   f0f,   g0g
```

Example rectangleCopy_s and rectangleSwap:
```c
    FILE *file = fopen(filename3,"r");
    if(file == NULL){
        return 1;
    }
    CSV letters = openCSV(file, DEFAULT_SETTINGS);
    fclose(file);

    CSV blank = makeBlankCSV(5,5,10);
    
    printf("Letters:\n");
    displayCSV(&letters, 0, stdout);
    printf("\n");

    rectangleCopy_s(&letters, &blank, 2, 2, 5, 5, 1, 1);

    printf("Letters:\n");
    displayCSV(&letters, 0, stdout);
    printf("Blank:\n");
    displayCSV(&blank, 0, stdout);

    rectangleSwap(&letters, &blank,0,0,4,1,0,0);

    printf("Letters:\n");
    displayCSV(&letters, 0, stdout);
    printf("Blank:\n");
    displayCSV(&blank, 0, stdout);

    closeCSV(&letters);
    closeCSV(&blank);
```
Example Output:
```
Letters: Pre copy
  |     A|     B|     C|     D|     E|     F|     G|
 0|     1,     2,     3,     4,     5,     6,     7
 1|     a,     b,     c,     d,     e,     f,     g
 2|    11,    22,    33,    44,    55,    66,    77
 3|    aa,    bb,    cc,    dd,    ee,    ff,    gg
 4|   101,   202,   303,   404,   505,   606,   707
 5|   a0a,   b0b,   c0c,   d0d,   e0e,   f0f,   g0g

Letters: Post copy, pre swap
  |     A|     B|     C|     D|     E|     F|     G|
 0|     1,     2,     3,     4,     5,     6,     7
 1|     a,     b,     c,     d,     e,     f,     g
 2|    11,    22,    33,    44,    55,    66,    77
 3|    aa,    bb,    cc,    dd,    ee,    ff,    gg
 4|   101,   202,   303,   404,   505,   606,   707
 5|   a0a,   b0b,   c0c,   d0d,   e0e,   f0f,   g0g
 
Blank: Post copy, pre swap
  |         A|         B|         C|         D|         E|
 0|          ,          ,          ,          ,
 1|          ,        33,        44,        55,        66
 2|          ,        cc,        dd,        ee,        ff
 3|          ,       303,       404,       505,       606
 4|          ,       c0c,       d0d,       e0e,       f0f
 
Letters: Post copy, post swap
  |     A|     B|     C|     D|     E|     F|     G|
 0|      ,      ,     3,     4,     5,     6,     7
 1|      ,    33,     c,     d,     e,     f,     g
 2|      ,    cc,    33,    44,    55,    66,    77
 3|      ,   303,    cc,    dd,    ee,    ff,    gg
 4|      ,   c0c,   303,   404,   505,   606,   707
 5|   a0a,   b0b,   c0c,   d0d,   e0e,   f0f,   g0g
 
Blank: Post copy, post swap
  |         A|         B|         C|         D|         E|
 0|         1,         2,          ,          ,
 1|         a,         b,        44,        55,        66
 2|        11,        22,        dd,        ee,        ff
 3|        aa,        bb,       404,       505,       606
 4|       101,       202,       d0d,       e0e,       f0f
```

##Other

#### removeRow and removeColumn
```c
void removeRow(CSV *source, int row);
void removeColumn(CSV *source, int col);
```
These functions remove a row or column from the **CSV**.
The row or column is specified by the **row** or **col** parameter.

Example:
```c
FILE *file = fopen(filename2,"r");
    if(file == NULL){
        return 1;
    }
    CSV csv = openCSV(file, DEFAULT_SETTINGS);
    fclose(file);
    displayCSV(&csv,8,0,0,stdout);
    deleteColumn(&csv,2);
    deleteRow(&csv,1);
    displayCSV(&csv,8,0,0,stdout);
    closeCSV(&csv);
```

Example Output:
```c
  |       A|       B|       C|       D|       E|
 0|    Name,     Age,     Job,    rank,  Salary
 1|     Joe,      21, manager, private,      99
 2|     Bob,      34,  worker, general,     100
 3|   Jimmy,       4,   thief,   Elite,  Unpaid
 4| Derrick,       9,  intern,corporal,      -1
 5|"Charles,      80,    King,Grand Ma,"1,000,0
  |       A|       B|       C|       D|
 0|    Name,     Age,    rank,  Salary
 1|     Joe,      21, private,      99
 2|     Bob,      34, general,     100
 3|   Jimmy,       4,   Elite,  Unpaid
 4| Derrick,       9,corporal,      -1
```