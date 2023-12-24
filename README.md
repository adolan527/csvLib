# csvLib
A library with functions for managing CSV files in C.

Ignore the Cmake stuff, it was easier just to commit the entire directory.

main.c and the txt files are just for "testing" functions, not essential to anything.

csv.h/csv.c - Declares Dimensions, CSVSettings and CSV structs. Most of the essential functions are here.

csvUtils.h/csvUtils.c - Rectangle copy, remove empty rows/columns, and sorting have been implemented here.

Limitations - 
The current problem is the MES is used for every single entry regardless of its size. Making the MES variable would be useful, however that would require a lot of refactoring. 
Thus, making the headers have a separate MES would seem to be the best method for reducing memory usage. (For example, 2kb file malloc() 19kb of memory, 
as there were a lot of entries with a few characters but a few with a lot of characters.)
Other potential solutions include making the CSV into a 2D linked list of entries, or a linked list of rows where each row has its own MES.
When considering the scope of the project and what I plan on using this library for, this should be fine, however, should I come back to this and try to make it more scalable, 
transforming the CSV from an array to a list would be ideal.
