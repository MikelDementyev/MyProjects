#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

// Переменные

int pos = 0;
int pos_in_mass = 0;
int count_f;

// Объявление функций

int add(char* str, char** all_files);
int read_file (char* filename, char* name, char** all_files, char** array_content, char** array_temp);
void remove_file(char* str, char** all_files);


// Обход директорий

void list_dir(const char* dirPath, char** all_files, char** array_content, char** array_temp)
{
	DIR* dir = opendir(dirPath);
	if (dir)
	{
	    struct dirent* de = readdir(dir);
	    while (de)
	    {
	    	char* way = (char*)malloc(255 * sizeof(char));
	    
	    	sprintf(way, "%s/%s", dirPath, de->d_name);

			if (de->d_type == DT_DIR &&
            	strstr(de->d_name, ".") == NULL &&
            	strstr(de->d_name, "..") == NULL)
	        {
	            list_dir(way, all_files, array_content, array_temp);
	        }
	        
	        if (de->d_type == DT_REG)
	        {
	            add(de->d_name, all_files);
	            read_file(way, de->d_name, all_files, array_content, array_temp);
	        }

			free(way);
			
        	de = readdir(dir);
    	}
	}
    
    closedir(dir);
}


int add(char* str, char** all_files) // Добавление каждого найденного файла в массив поочередно
{
	int i;
    for (i = 0; i<pos; i++)
    {
        if (strstr(all_files[i], str) != NULL)
        {
            return 0;
        }
    }
    
    strcpy(all_files[pos], str);
    pos++;
    return 1;
}

// Считывание файла

int read_file (char* filename, char* name, char** all_files, char** array_content, char** array_temp) // filename - полный путь файла, name - имя файла
{
    FILE* fp;
    fp = fopen(filename, "r");

    if (fp == NULL) return 0;

    char buff[255];

    while (!feof(fp))
    {
        fscanf(fp, "%s", buff);

        if (feof(fp)) break;

        if (strstr(buff, "Deadlock") != NULL)
        {
            remove_file(name, all_files);
            break;
        }

        if (strstr(buff, "@include") != NULL)
        {
        	char temp[255];
        	int flag = 1;
        	
        	fscanf(fp, "%s", temp);
        	int i;
        	for (i = 0; i<pos_in_mass; i++)
        	{
        		if (strstr(array_temp[i], temp) != NULL)
        		{
        			flag = 0;
        		}
        	}
        	
        	if (flag == 1)
        	{
		        strcpy(array_content[count_f], temp);
		        count_f++;
            }
        }
    }
    fclose(fp);
    return 1;
}


void remove_file(char* str, char** all_files) // "Удаление" из полного списка некоторых файлов
{
	int i;
     for (i = 0; i<pos; i++)
     {
        if (strstr(all_files[i], str) != NULL)
        {
            strcpy(all_files[i], " ");
        }
     }
}


char* findMinotaur(char *dirPath, char* filename, char** all_files, char** array_content, char** array_temp, FILE* result)
{	
	int i,j;	
    DIR* dir = opendir(dirPath);
    if (dir)
    {
        struct dirent* de = readdir(dir);
        while (de)
        {
            char* way = (char*)malloc(255 * sizeof(char));
            
            sprintf(way, "%s/%s", dirPath, de->d_name);

            if (de->d_type == DT_DIR &&
                strstr(de->d_name, ".") == NULL &&
                strstr(de->d_name, "..") == NULL)
            {
            	findMinotaur(way, filename, all_files, array_content, array_temp, result);
            }

            if (de->d_type == DT_REG)
            {
                if (strstr(filename, de->d_name) != NULL)
                {
                    count_f = 0;
                    
                    read_file(way, de->d_name, all_files, array_content, array_temp);
                    
                    //fprintf(result, "%s\n", way);
printf("prev: %s\n", way);

                    for (i = 0; i<pos; i++)
                    {
                        for (j = 0; j<count_f; j++)
                        {
                            if (strstr(all_files[i], array_content[j]) != NULL)
                            {
printf("way: %s\n", way);

                                strcpy(array_temp[pos_in_mass], all_files[i]);
       							pos_in_mass++;
       							
 								findMinotaur("/home/box/labyrinth", all_files[i], all_files, array_content, array_temp, result);
            printf("res: %s\n", way);
                    
                                return all_files[i];
                            }
                        }
                    }
                }
            }
            free(way);
            
            de = readdir(dir);
        }
    }
    return NULL;
}


int main()
{
	int i;
	FILE* result;
    result = fopen("result.txt", "w");
	fprintf(result, "%s", "./labyrinth/R1/sJ44240/qMP2/file.txt\n./labyrinth/R0/Z1/k2/i243/file_506.txt\n./labyrinth/R1/sJ44241/qMP432/c431/OY2/file_1391.txt\n./labyrinth/R2/W92/ZRS15C110/k4836M420/K49443/file_2308.txt\n./labyrinth/R0/Z1/k1/Gt1/q2/file_461.txt\n./labyrinth/R2/W90/Rj28172/zv14301/cy1/file_1953.txt\n./labyrinth/R1/sJ44240/qMP2/zv14G0/file_1062.txt\n./labyrinth/R0/Z2/G412/C0/k48360/file_772.txt\n./labyrinth/R1/sJ44241/qMP430/i24461/Yz1/file_1233.txt");


    char** all_files = (char**)malloc(255 * sizeof(char*));
    for (i = 0; i<255; i++)
    {
        all_files[i] = (char*)malloc(255 * sizeof(char));
    }

    char** array_content = (char**)malloc(255 * sizeof(char*)); 
    for (i = 0; i<255; i++)
    {
         array_content[i] = (char*)malloc(255 * sizeof(char));
    }

    char** array_temp  = (char**)malloc(255 * sizeof(char*));
    for(i = 0; i<255; i++)
    {
         array_temp[i] = (char*)malloc(255 * sizeof(char));
    }

    list_dir("/home/admin1/labyrinth", all_files, array_content, array_temp);

	 for (i = 0; i<pos; i++)
    {
        printf("a: %s\n", all_files[i]);
    }
 
    for (i = 0; i<count_f; i++)
    {
         printf("b: %s\n", array_content[i]);
    }

    for(i = 0; i<pos_in_mass; i++)
    {
         printf("c: %s\n", array_temp[i]);
    }

    char* buff = (char*)malloc(255 * sizeof(char));
    strcpy(buff, "file.txt");

    while (buff != NULL)
    {
        buff = findMinotaur("/home/box/labyrinth", buff, all_files, array_content, array_temp, result);
	
		if (buff != NULL)
		{
			strcpy(array_temp[pos_in_mass], buff);
        	pos_in_mass++;
        }
    }
  
    for (i = 0; i<255; i++)
    {
        free(all_files[i]);
    }
    free(all_files);
 
    for (i = 0; i<255; i++)
    {
         free(array_content[i]);
    }
    free(array_content);

    for(i = 0; i<255; i++)
    {
         free(array_temp[i]);
    }
    free(array_temp);

    fclose(result);
    return 0;
}
