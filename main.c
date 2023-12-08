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
    FILE *file = fopen(filename3,"r");

    if(file == NULL){
        return 1;
    }

    
    CSV names = openCSV(file, DEFAULT_SETTINGS);
    fclose(file);

    displayCSV(&names,0,DEFAULT_SETTINGS,stdout);
    printf("\n\n");
    removeEmpty(&names,All,0);
    displayCSV(&names,0,DEFAULT_SETTINGS,stdout);


    //saveCSV(&names,filename2,custom);

    closeCSV(&names);

    return 0;
}
