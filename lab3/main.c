#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

FILE *fTest;

char** a;
char** b;
int countF = 0;

int pos = 0, count = 0;

int readfile (char* filename, char* name);


int add(char* str)
{
    for(int i = 0; i<pos; i++)
    {
        if(strstr(a[i], str) != NULL)
        {
            return 0;
        }
    }
    strcpy(a[pos], str);
    pos++;
    return 1;
}


char* findMinotaur(char *dirPath, char* filename)
{
    DIR* dir = opendir(dirPath);
    if (dir)
    {
        struct dirent* de = readdir(dir);
        while (de)
        {
            char* dirPath2 =(char*) malloc(255 * sizeof(char));
            sprintf(dirPath2, "%s/%s", dirPath, de->d_name);

            if (de->d_type == DT_DIR &&
                strstr(de->d_name, ".") == NULL &&
                strstr(de->d_name, "..") == NULL)
            {
                    findMinotaur (dirPath2, filename);
            }

            if (de->d_type == DT_REG)
            {
                if(strstr(filename, de->d_name) != NULL)
                {
                    countF = 0;
                    readfile(dirPath2, de->d_name);
                  //  fprintf(fTest, "%s\n", dirPath2);

                    for(int i = 0; i<pos; i++)
                    {
                        for(int j = 0; j<countF; j++)
                        {
                            if(strstr(a[i], b[j]) != NULL)
                            {
                         //       printf("Path: %s\n", a[i]);
                                findMinotaur ("./labyrinth", a[i]);
                                return a[i];
                            }
                        }
                    }
                }
            }
            de = readdir(dir);
        }
    }
    return NULL;
}


void eraise(char* str)
{
    //printf("eraise %s: \n", str);

     for(int i = 0; i<pos; i++)
     {

       // printf("%s\n", a[i]);
        if(strstr(a[i], str) != NULL)
        {
           // printf("%s remove: ", a[i]);
            strcpy(a[i], " ");
        }
     }
}


int readfile (char* filename, char* name)
{
    FILE* fp;
    fp = fopen(filename, "r");

    if (fp == NULL) return 0;

    char buff[255];

    while (!feof(fp))
    {
        fscanf(fp, "%s", buff);

        if(feof(fp)) break;

        if(strstr(buff, "Deadlock") != NULL)
        {
            eraise(name);
            break;
        }

        if(strstr(buff, "Minotaur") != NULL)
        {
          //  printf("%s Minotaur\n", filename);
        }

        if(strstr (buff, "@include") != NULL)
        {
            fscanf(fp, "%s", b[countF]);
            countF++;
        }
    }
    fclose(fp);
    return 1;
}

void show()
{
   // printf("\n");
    for(int i = 0; i<pos; i++)
    {
        if(strstr(a[i], " ") == NULL)
        {
           // printf("%s\n", a[i]);
        }
    }
    /*
    printf("%d\n", pos);
    for (int i = 0; i<countF; i++)
    {
        printf("codershimoe: %s\n", b[i]);
    }
    */
}


void list_dir(const char* dirPath)
{
    DIR* dir = opendir(dirPath);
    if (dir)
    {
        struct dirent* de = readdir(dir);
        while (de)
        {
          //  printf("%s/%s\n", dirPath, de->d_name);

            char* dirPath2 =(char*) malloc(255 * sizeof(char));
            sprintf(dirPath2, "%s/%s", dirPath, de->d_name);

            if (de->d_type == DT_DIR &&
                strstr(de->d_name, ".") == NULL &&
                strstr(de->d_name, "..") == NULL)
            {
                list_dir (dirPath2);
            }

            if (de->d_type == DT_REG)
            {
                add(de->d_name);
            //    printf("add: %s\n", dirPath2);
                readfile(dirPath2, de->d_name);
            }

            de = readdir(dir);
        }
    }
    closedir(dir);
}

int main()
{
    fTest = fopen("./result.txt", "w");

    a = (char**)malloc(255 * sizeof(char*));
    for(int i = 0; i < 255; i++)
    {
        a[i] = (char*)malloc(255 * sizeof(char));
    }

    b = (char**)malloc(255 * sizeof(char*));
    for(int i = 0; i < 255; i++)
    {
         b[i] = (char*)malloc(255 * sizeof(char));
    }

    list_dir("./labyrinth");
    show();

    char * buff = (char*) malloc(255 * sizeof(char));
    strcpy(buff, "file.txt");

    while( buff != NULL )
    {
        buff = findMinotaur("./labyrinth", buff);
        //printf("res: %s\n", buff);
    }

    fclose(fTest);
    return 0;
}
