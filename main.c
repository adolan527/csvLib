#include <stdio.h>
#ifndef CSV_CSV_H
#include "csv.h"
#endif

#include "csvUtils.h"

#include <stdlib.h>




int main() {
    char filename[] = "C:\\Users\\Aweso\\CLionProjects\\csvLib\\frcData.csv";
    char filename2[] = "C:\\Users\\Aweso\\CLionProjects\\csvLib\\names.csv";
    char filename3[] = "C:\\Users\\Aweso\\CLionProjects\\csvLib\\stuff.csv";


    FILE *file = fopen(filename,"r");
    if(file == NULL){
        return 1;
    }
    CSV frc = openCSV(file, DEFAULT_SETTINGS);
    fclose(file);
    frc.settings.colHeader=1;
    frc.settings.rowHeader=1;

    displayCSV(&frc,5,0,0,stdout);
    int index=indexOfHeaderCol(&frc,"Defense rank");
    if(index!=0){
        sortRows(&frc,sortModes[DecNum],index);
        printf("\n\n");
        displayCSV(&frc,16,index,0,stdout);
    }
    else{
        printf("could not find that header");
    }



    closeCSV(&frc);


    return 0;
}
