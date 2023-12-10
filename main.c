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
    CSV letters = openCSV(file, DEFAULT_SETTINGS);
    fclose(file);

    CSV blank = makeBlankCSV(5,5,10);

    printf("Letters:\n");
    displayCSV(&letters, 0, stdout);
    printf("\n");

    rectangleCopy_s(&letters, &blank, 2, 2, 5, 5, 1, 1);

    printf("Letters:\n");
    displayCSV(&letters, 0, stdout);
    printf("Blank:\n");
    displayCSV(&blank, 0, stdout);

    rectangleSwap(&letters, &blank,0,0,4,1,0,0);

    printf("Letters:\n");
    displayCSV(&letters, 0, stdout);
    printf("Blank:\n");
    displayCSV(&blank, 0, stdout);

    closeCSV(&letters);
    closeCSV(&blank);


    return 0;
}
