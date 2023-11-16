#include <stdio.h>
#include "csv.h"



int main() {
    FILE *file = fopen("C:\\Users\\Aweso\\CLionProjects\\CSV\\stuff.csv","r+");

    int max = 32;

    CSV chungus = fileToCSV(file, ',', '\n', max);

    printCSV(&chungus,0,',','\n');

    closeCSV(&chungus);
    fclose(file);




    return 0;
}
