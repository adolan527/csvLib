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

### Limitations
An issue that could prevent scaling in the future would be the structure of a CSV file. 
Having every entry in the CSV taking up the same amount of space means that a file where every entry is one character except for one entry which is 100 characters means the size and efficiency would be drastically reduced. 
Potential solutions would be: 
 - Making each entry a pointer to a dynamically allocated string. This would require more referencing, a lot of refactoring, and more init/de-init.
 - Making each row have a different Max Entry Size. This would require less refactoring, and would probably minimize the aforementioned issue, but it is not guaranteed nor as effective as the previous option.
 - Making the CSV more like an array of strings, where each entry terminates with a null character that signifies the next entry starting. Easy implementation but would greatly reduce performance and add complexity.
 - Making the CSV into a linked list. Requires complete refactoring and some worsening of performance but much more efficient memory usage.
   - Each entry is its own node and points to the entries right and down of it. 2D (probably singly) linked list.
   - Each row (could also be column, it is arbitrary) is a node in a doubly linked list. Each node contains a singly linked list of its entries.
   - Each entry is its own node and points to the next entry in a 1D singly linked list.
  
