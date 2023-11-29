#include <stdio.h>
#ifndef CSV_CSV_H
#include "csv.h"
#endif

#include "csvUtils.h"




int main() {
    char filename[] = "C:\\Users\\Aweso\\Downloads\\Project 3 Peer Review - Sheet1.csv";
    char filename2[] = "C:\\Users\\Aweso\\CLionProjects\\csvLib\\names.csv";
    FILE *file = fopen(filename2,"r");

    if(file == NULL){
        return 1;
    }
    CSVSettings custom = DEFAULT_SETTINGS;
    custom.maxEntrySize = 0;

    CSV names = openCSV(file, custom);
    fclose(file);


    CSV new = makeBlankCSV(5,10,21);
    copyCSV(&names,&new);

    displayCSV(&names,0,custom, stdout);
    printf("-------------------\n");
    displayCSV(&new,0,custom, stdout);

    saveCSV(&names,filename2,custom);

    closeCSV(&names);




    return 0;
}
