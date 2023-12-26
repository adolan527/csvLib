# csvLib
## A library with functions for managing CSV files in C.

### Goal
For many projects it is required that some convention of data storage is used. CSV (comma-separated values) files are an efficient, lightweight, and portable data format that can be used for data storage.
Parsing files in C can be tedious, error-prone, and can add unnecessary overhead.
This library solves all of these problems by creating a simple struct CSV that can represent the data in a CSV file with ease.
Beyond data storage, there are also applications for data analysis, without having to use high-level API's or languages such as Python.

### Structure
The main header file, ```csv.h``` contains all of the necessary structs and function declarations for CSV operations. 
```csv.c``` must be included in the project, as it contains the function definitions. Creating binaries for ```csv.c``` is a goal for the future.

The utilities header file ```csvUtils.h``` offers some common operations that one might wish to do on a CSV file/struct.
Similarly, ```csvUtils.c``` contains the function definitions.

To include this library in your project, simply download/clone csv.c and csv.h, and optionally csvUtils.c and csvUtils.h, into your projects and include the header files in your main file.

To learn more about the functions, see the [documentation](https://github.com/adolan527/csvLib/blob/b6cb55ec113783f2a1c3be334d47b27cd05df10e/docs/csvDoc.md).

Limitations - 
The current problem is the MES is used for every single entry regardless of its size. Making the MES variable would be useful, however that would require a lot of refactoring. 
Thus, making the headers have a separate MES would seem to be the best method for reducing memory usage. (For example, 2kb file malloc() 19kb of memory, 
as there were a lot of entries with a few characters but a few with a lot of characters.)
Other potential solutions include making the CSV into a 2D linked list of entries, or a linked list of rows where each row has its own MES.
When considering the scope of the project and what I plan on using this library for, this should be fine, however, should I come back to this and try to make it more scalable, 
transforming the CSV from an array to a list would be ideal.
