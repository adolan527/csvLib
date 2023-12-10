# csvLib
A library with functions for managing CSV files in C.

Ignore the Cmake stuff, it was easier just to commit the entire directory.

main.c and the txt files are just for "testing" functions, not essential to anything.

csv.h/csv.c - Declares Dimensions, CSVSettings and CSV structs. Most of the essential functions are here.

csvUtils.h/csvUtils.c - Rectangle copy, remove empty rows/columns, and sorting have been implemented here.

testing.c - Future implementation of functions for testing the functions in csv.c and csvUtils.c

Roadmap -
1. <s> Implement functions for removing empty rows/columns </s> Completed, but will require optimization during the testing phase.
2. <s> Implement "headers" that can be used for index - row 0 and column 0 have the option of being labels that are used to describe their datasets. </s>
3. <s> Add functionality for sorting data - sorting alphabetically and numerically </s>
4. Expand upon "easy" functions - Minimal user overhead to work with csv files.
5. Testing
6. <s> Documentation. </s>
