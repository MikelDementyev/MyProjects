#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

char* findFile(char* filename, char* dirname, FILE** file) {
 
    DIR* dir = opendir(dirname);
    char* path = NULL;

    struct dirent* cur = readdir(dir);
    while( cur )
    {
        if( DT_DIR == cur->d_type && 
            strcmp(".", cur->d_name) && 
            strcmp("..", cur->d_name) ) 
        {
            char* dpath = (char*)malloc(255 * sizeof(char));
            strcat(dpath, dirname);
            strcat(dpath, "/");
            strcat(dpath, cur->d_name);
            if( (path = findFile(filename, dpath, file)) )
            {
                free(dpath);
                break;
            }
            free(dpath);
        }

        if( DT_REG == cur->d_type )
            if( !strcmp(cur->d_name, filename) ) 
            {
                char* fpath = (char*)malloc(255 * sizeof(char));
                strcat(fpath, dirname);
                strcat(fpath, "/");
                strcat(fpath, filename);
                *file = fopen(fpath, "r");
                closedir(dir);
                return fpath;
            }

        cur = readdir(dir);

    }

    closedir(dir);
    
    return path;

}

char* readFile(FILE* file) {
    
    char* buff = (char*)malloc(255 * sizeof(char));

    int flag;
    while( EOF != (flag = fscanf(file, "@include %s ", buff)) && flag ) 
    {    
        return buff;    
    }
    
    if( EOF != fscanf(file, "%s", buff) ) 
    {
        return buff;
    }

    return NULL;

}

int findMinotaur(char* filename, char* dirname, char** a, int* size) {
    
    FILE* current_file;
    char* buff;
    char* path;

    path = findFile(filename, dirname, &current_file);
    while( (buff = readFile(current_file)) ) 
    {
        if( !strcmp("Deadlock", buff) ) 
        {
            free(buff);
            free(path);
            return 0;
        }
        if( !strcmp("Minotaur", buff) || 
            1 == findMinotaur(buff, dirname, a, size)) 
        {
            free(buff);
            a[*size] = path;
            (*size)++;
            return 1;
        }
    }

    free(path);
    fclose(current_file);

    return 0;

}

int main() {

    char** a = (char**)malloc(3000 * sizeof(char*));
    int size = 0;
    FILE* fp = fopen("./result.txt", "w");
    char filename[] = "file.txt";
    char dirname[] = "./labyrinth";

    findMinotaur(filename, dirname, a, &size);

    for(int i = size-1; i > -1; i--) {
        fprintf(fp, "%s\n", a[i]);
        free(a[i]);
    }
    free(a);

    fclose(fp);

}
