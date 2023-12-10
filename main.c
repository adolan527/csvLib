#include <stdio.h>
#ifndef CSV_CSV_H
#include "csv.h"
#endif

#include "csvUtils.h"

#include <stdlib.h>




int main() {
    char filename[] = "C:\\Users\\Aweso\\Downloads\\Project 3 Peer Review - Sheet1.csv";
    char filename2[] = "C:\\Users\\Aweso\\CLionProjects\\csvLib\\names.csv";
    char filename3[] = "C:\\Users\\Aweso\\CLionProjects\\csvLib\\stuff.csv";
    FILE *file = fopen(filename2,"r");

    if(file == NULL){
        return 1;
    }

    
    CSV names = openCSV(file, DEFAULT_SETTINGS,0);
    fclose(file);

    names.settings.colHeader = 1;
    names.settings.rowHeader = 1;

    displayCSV(&names,0,12,stdout);

    sortRows(&names,sortModes[DecAlpha], indexOfHeaderCol(&names,"Job"));
    printf("\n");
    displayCSV(&names,0,12,stdout);


    //saveCSV(&names,filename2,custom);

    closeCSV(&names);

    return 0;
}
