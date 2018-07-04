#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "png_open.h"
int main(int argc, char **argv)
{
    if (argc != 6) { printf("Fail with arguments"); return 0;}
    char *strPath;
    strPath = (char *) malloc(sizeof(char)*255);
    strcpy(strPath, argv[1]);

    int x0 = atoi(argv[2]), y0 = atoi(argv[3]);
    int x1 = atoi(argv[4]), y1 = atoi(argv[5]);

    //Проверка параметров на корректность
    int checkParams = isSquare(x0, y0, x1, y1);
    if( ! checkParams ) {
        return 0;
    }


    int width = 0;
    int height = 0;
    //
    int isOpen = read_png_file(strPath, &width, &height);
    if( !isOpen ) {
        printf("Cann`t open png file! \n");
        return 0;
    } else {
        printf("Png file is opened! \n");
    }

    int isOut = isOtsideOfImage(width, height, x0, y0, x1, y1);
    if( !isOut ) {
        printf("Point(x1,y1) is outside of square! \n");
        return 0;
    }
    
    y0 = height - y0;
    y1 = height - y1;

    drawX(x0, y0, x1, y1);

    write_png_file("test.png");


    return 0;
}
