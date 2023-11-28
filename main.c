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
    char buf[36] = "qwerqwerqwerqwerqwerqwerqwerqwerasdf";
    printf("%s\n",CSVREADLIT(names,1,2));

    printf("%d\n",names.size.maxEntrySize);

    displayCSV(&names,0,',','\n',stdout);
    if(CSVWRITELIT(names,0,1,buf) != 0){printf("error");}
    displayCSV(&names,0,',','\n',stdout);



    //saveCSV(&names,"C:\\Users\\Aweso\\CLionProjects\\csvLib\\names.csv");

    closeCSV(&names);






    return 0;
}
