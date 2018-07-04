#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int findFile(char* filename, char* dirname, int* size, char** path);
int readFile (char* filename, int* size, char** path);

void Elem(char* path, char* filename)
{
	path[0] = '.';
	path[1] = '/';
	strcat (path, filename);
	strcat (path, "\n");
	path[strlen(path)]='\0';
}

int findFile(char* filename, char* dirname, int* size, char** path) 
{
 
    //char* buff = (char*)malloc(255 * sizeof(char));
    char buff[255] = {0};
    DIR* dir = opendir(dirname);

    if (dir)
    {
        struct dirent* de = readdir(dir);
        while (de)
        {
            if (de->d_type == DT_DIR &&
		strcmp(de->d_name, ".") != 0 && 
		strcmp(de->d_name, "..") != 0)
            {
                strcat(buff, dirname);
                strcat(buff, "/");
                strcat(buff, de->d_name);

                if (findFile(filename, buff, size, path) == 1)
                { 
                    closedir(dir);
		    return 1;
                }
	    buff[strlen(buff)] = '\0';
	    }


            if (DT_REG == de->d_type &&
                strcmp(de->d_name, filename) == 0) 
            {
                strcat(buff, dirname);
                strcat(buff, "/");
                strcat(buff, de->d_name);

	        if (readFile(buff, size, path) == 1)
		{
		    closedir(dir);
                    return 1;
		}
		buff[strlen(buff)] = '\0';
            }

	//    free(path);
            de = readdir(dir);

        }
        closedir(dir);
        return 0;

    }

}

int readFile (char* filename, int* size, char** path) 
{
    FILE* fp = fopen(filename, "r");

    if (!fp) return 0;

    char buff[255];
    char buff_1[255];

    while (fgets(buff, 255, fp))
 //   while (!feof(fp)) 
    {
 //       fscanf(fp, "%s", buff);
 //       if(feof(fp)) break;

        if(!strcmp(buff, "Minotaur\n")) 
	{
            path[*size] = (char*)malloc((strlen(filename) + 1) * sizeof(char));
	    Elem(path[*size], filename);
	    *size = *size+1;

   	    fclose(fp);
	    return 1;
        }

        else if(!strcmp(buff, "Deadlock\n")) 
	{
            fclose(fp);
	    return 0;
        }

        else 
	{
	    char* p = strtok(buff, " ");

 	    while (p != NULL)
    	    {
		strcpy(buff_1, p);
       		p = strtok(NULL, " ");
   	    }
		
	    buff_1[strlen(buff_1)-1] = '\0';

	    if(findFile(buff_1, "labyrinth", size, path))
	    {
		path[*size] = (char*)malloc((strlen(filename) + 1) * sizeof(char));
	        Elem(path[*size], filename);
		*size = *size + 1;
		fclose(fp);
		return 1;
	    }
	}


    }
    fclose(fp);
    return 0;
}

int findMinotaur(char** path, int size)
{
    FILE* fp = fopen("result.txt", "a");

    if(!fp) return 0;

    for(int i=size-1; i>=0; i--)
    {
	fwrite(path[i], sizeof(char), strlen(path[i]), fp);
    }
    fclose(fp);
}

int main() 
{

    char** path = (char**)malloc(3000 * sizeof(char*));
    int size = 0;

    findFile("file.txt", "labyrinth", &size, path);
    findMinotaur(path, size);

    for(int i = 0; i < size; i++) 
    {
        free(path[i]);
    }
    free(path);
    return 0;

}
