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


    CSV names = openCSV(file, DEFAULT_SETTINGS);
    fclose(file);

    CSVSettings custom = DEFAULT_SETTINGS;
    custom.maxEntrySize = 8;
    custom.colDelin = '|';


    Dimensions size = {5,10,21};
    CSV new = DMakeBlankCSV(size);

    rectangleCopy(&names,&new,1,1,3,4,1,1);

    displayCSV(&names,0,custom, stdout);
    printf("-------------------\n");

    displayCSV(&new,0,custom, stdout);
    printf("-------------------\n");

    rectangleCopy(&new,&new,1,1,3,4,2,6);
    displayCSV(&new,0,custom, stdout);
    printf("-------------------\n");

    rectangleSwap(&new,&new,1,1,3,3,1,7);
    displayCSV(&new,0,custom, stdout);
    printf("-------------------\n");

    displayCSV(&names,0,custom, stdout);
    printf("-------------------\n");

    rectangleSwap(&names,&names,1,0,1,4,2,0);
    displayCSV(&names,0,custom, stdout);
    printf("-------------------\n");
/*
    for(int i = 0;i<9;i++){
        size.rCount++;
        size.cCount--;
        size.maxEntrySize -= 2;
        resizeCSV(&new,size);
        displayCSV(&new,0,custom, stdout);
        printf("-------------------\n");

    }
*/


    //saveCSV(&names,filename2,custom);

    closeCSV(&names);

    closeCSV(&new);
    scanf("%c",filename);

    return 0;
}
