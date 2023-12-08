# csvLib
A library with functions for managing CSV files in C.

Ignore the Cmake stuff, it was easier just to commit the entire directory.

main.c and the txt files are just for "testing" functions, not essential to anything.

csv.h/csv.c - Declares Dimensions and CSV structs. Most of the essential functions are here.

csvUtils.h/csvUtils.c - Mostly empty, planned implementation of functions to perform common operations on csv's.

testing.c - Future implementation of functions for testing the functions in csv.c and csvUtils.c

Currently, documentation is a low priority, however, there are future plans for documentation. 
Roadmap -
1. <s> Implement functions for removing empty rows/columns </s> Completed, but will require optimization during the testing phase.
2. Implement "headers" that can be used for index - row 0 and column 0 have the option of being labels that are used to describe their datasets.
3. Add functionality for sorting data - sorting alphabetically and numerically
4. Expand upon "easy" functions - Minimal user overhead to work with csv files.
5. Testing
6. Documentation.
