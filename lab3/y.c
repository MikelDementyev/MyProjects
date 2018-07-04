#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include<sys/types.h>

#define DEADLOCK 0

typedef struct ways{
        char **arr;
        int count;
}Ways;

int checkFile(char *startDir, char *filename, Ways *ways){
        char nextDir[200]={0};
        strcpy(nextDir, startDir);
        DIR *dir = opendir(nextDir);
        if(!dir)
                return 0;
        struct dirent *de=readdir(dir);
        while(de){
		int len = strlen(nextDir);
		sprintf(nextDir,"%s/%s",nextDir,de->d_name);
		if(de->d_type==DT_DIR && strcmp(de->d_name,".")  && strcmp(de->d_name,"..")){
                        if(checkFile(nextDir, filename,ways)){
                                closedir(dir);
                                return 1;
                          }
                }
		if(de->d_type == DT_REG && !strcmp(filename, de->d_name)){
				 FILE *f=fopen(nextDir, "r");
				 char include[50];
				 while(fgets(include,50,f)){
					if(include[strlen(include)-1]=='\n')
                        			include[strlen(include)-1]='\0';
				  	if(!strcmp(include,"Deadlock")){
                        			fclose(f);
						return DEADLOCK;
					}
					if(strstr(include,"Minotaur")){
						 strcpy(ways->arr[ways->count],nextDir);
						 ways->count++;
                       				 fclose(f);
						 return 1;
                			}
					if(checkFile("./labyrinth", strstr(include,"file"),ways)){
						 strcpy(ways->arr[ways->count],nextDir);
                                                 ways->count++;
                                                 fclose(f);
						 closedir(dir);
                                                 return 1;
					}
			 	  }
		}
		nextDir[len] = '\0';
		de=readdir(dir);
	}
	closedir(dir);
	return 0;
}
void destroy_struct(Ways *ways){
        int i;
        for(i=0;i<(ways->count); i++)
 	        free(ways->arr[i]);
        free(ways->arr);
}
int main(){
        Ways ways;
        ways.count=0;
        ways.arr=malloc(20* sizeof(char *));
	for(int i=0;i<20;i++)
		ways.arr[i] = malloc(50* sizeof(char));

        checkFile("./labyrinth", "file.txt",&ways);

	FILE *f=fopen("./result.txt","w+");
        for(int i=ways.count-1;i>=0;i--)
                fprintf(f,"%s\n",ways.arr[i]);
        fclose(f);

	destroy_struct(&ways);
	return 0;
}

