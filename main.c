#include <stdio.h>
#ifndef CSV_CSV_H
#include "csv.h"
#endif
#include "csvUtils.h"




int main() {
    char filename[] = "C:\\Users\\Aweso\\Downloads\\Urban Water Testing (Lake Burgess) - Sheet1.csv";
    char filename2[] = "C:\\Users\\Aweso\\CLionProjects\\csvLib\\stuff.csv";
    FILE *file = fopen("C:\\Users\\Aweso\\CLionProjects\\csvLib\\names.csv","r");
    if(file == NULL){
        return 1;
    }


    CSV names = openCSV(file, ',', '\n', 0);
    fclose(file);
    removeEmptyRows(&names,Before + Inside + Outside);

    saveCSV(&names,"C:\\Users\\Aweso\\CLionProjects\\csvLib\\names.csv");

    closeCSV(&names);






    return 0;
}
