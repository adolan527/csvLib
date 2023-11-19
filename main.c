#include <stdio.h>
#include "csv.h"



int main() {
    FILE *file = fopen("C:\\Users\\Aweso\\CLionProjects\\csvLib\\stuff.csv","r+");

    int max = 32;

    CSV chungus = openCSV(file, ',', '\n', max);

    printCSV(&chungus,0,',','\n');
    saveCSV(&chungus,file,0);
    printf("Row count: %d",chungus.size.rCount);
    closeCSV(&chungus);
    fclose(file);




    return 0;
}
