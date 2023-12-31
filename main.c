#include <stdio.h>
#ifndef CSV_CSV_H
#include "csv.h"
#endif

#include "csvUtils.h"

#include <stdlib.h>




int main() {
    char filename[] = "C:\\Users\\Aweso\\CLionProjects\\csvLib\\input\\frcData.csv";
    char filename2[] = "C:\\Users\\Aweso\\CLionProjects\\csvLib\\input\\names.csv";
    char filename3[] = "C:\\Users\\Aweso\\CLionProjects\\csvLib\\input\\stuff.csv";


    FILE *file = fopen(filename2,"r");
    printf("Pointer: %p, Base: %p, TMPFNAME: %p\n",file->_ptr,file->_base,file->_tmpfname);
    if(file == NULL){
        printf("Could not open %s",filename2);
        return 1;
    }
    CSV csv = openCSV(file, DEFAULT_SETTINGS);
    fclose(file);
    displayCSV(&csv,8,0,0,stdout);
    removeColumn(&csv,4);
    removeRow(&csv,1);
    printf("\n");
    displayCSV(&csv,8,0,0,stdout);
    closeCSV(&csv);
    return 0;

    file = fopen(filename,"r");
    if(file == NULL){
        printf("Could not open %s",filename);
        return 1;
    }

    CSV frc = openCSV(file, DEFAULT_SETTINGS);
    fclose(file);
    frc.settings.colHeader=1;
    frc.settings.rowHeader=1;

    displayCSV(&frc,5,0,0,stdout);

    int index=indexOfHeaderCol(&frc,"6 Did the robot show");
    if(index!=0){
        sortRows(&frc,sortModes[DecNum],index);
        printf("\n\n");
        displayCSV(&frc,0,index,index+1,stdout);
        printf("\n\n");
        displayCSV(&frc,0, indexOfHeaderCol(&frc,"Points direct and alliance at 1/3"),indexOfHeaderCol(&frc,"Points direct and alliance at 1/3") +1,stdout);
    }
    else{
        printf("could not find that header");
    }
    printf("\n\n");
    printRow(&frc,0,4,stdout);

    printRow(&frc,indexOfHeaderRow(&frc,"Team 17"),4,stdout);
    printf("\n\n");
    printColumn(&frc, indexOfHeaderCol(&frc,"Auto Points"),0,stdout);

    closeCSV(&frc);


    return 0;
}
