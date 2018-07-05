#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

#define HOME_DIR "./labyrinth/"
#define SIZE 3000
#define WAY 200

typedef struct Info{
	char way[WAY];
	char name[50];
	int c;
} Info;

void findWays(const char* home_dir, Info* info, int* i){
	DIR *dir = opendir(home_dir);
	if(!dir)
		return;
	
	int flag = 0;
	char way[WAY];
	struct dirent *de = readdir(dir);
	while(de){
		if((de->d_type == DT_DIR) && (strcmp(de->d_name, ".") != 0) && (strcmp(de->d_name, "..") != 0)){
			sprintf(way, "%s%s/", home_dir, de->d_name);
			findWays(way, info, i);
		}
		if(de->d_type == DT_REG){
			strcpy(info[*i].way, home_dir);
			strcpy(info[*i].name, de->d_name);
			info[*i].c = 0;
			(*i)++;
		}
		de = readdir(dir);
	}
	closedir(dir);
}

void findFile(Info* info, int size, const char* filename, char** result, int* i){
	for(int j=0; j<size; j++){
		if( /*!strcmp(info[j].name, filename) && */ !info[j].c){
			char str[250];
			sprintf(str, "%s%s", info[j].way, info[j].name);
			strcpy(result[*i], str);
			info[j].c = 1;
			(*i)++;
			FILE* file = fopen(str, "r");
			if(file){
				char content[WAY];
				while(fgets(content, WAY, file)){
					if(strstr(content, "Minotaur")){
						content[0] = '\0';
						break;
					}
					if(strstr(content, "@include ")){
//						if (info[j].c) continue;
						content[strlen(content)-5] = '\0';
						char* cont = &content[strlen("@include ")];
//						info [j].c = 1;
						for(int k=0; k<size; k++)
							if(!strcmp(info[k].name, cont))
								if(info[k].c)
									continue;
						findFile(info, size, cont, result, i);
//						info [j].c = 1;
						content[0] = '\0';
					}
					if(strstr(content, "Deadlock")){
						(*i)--;
						result[*i][0] = '\0';
						content[0] = '\0';
						break;
					}
				}
				fclose(file);
			}
//			info [j].c = 1;
		}
	}
}

int main(){
	Info* info = (Info*)malloc(SIZE*sizeof(Info)); // 
	int i = 0;

	char** result = (char**)malloc(SIZE*sizeof(char*));
	for(int j=0; j<SIZE; j++)
		result[j] = (char*)malloc(WAY*sizeof(char));

	findWays(HOME_DIR, info, &i);
	i = 0;
	findFile(info, SIZE, "file", result, &i);

	FILE* file = fopen("result.txt", "w");
	if(file){
		for(int j=0; j<i; j++){
			if (result[j][0]){
				char* str = result[j] + strlen(HOME_DIR) - 2;
				str[0] = '.';
				printf("%s\n", str);
				fwrite(str, sizeof(char), strlen(str), file);
			}
		}
		fclose(file);
	}
	for(int j=0; j<SIZE; j++)
		free(result[j]);
	free(result);
	free(info);
	return 0;
}

/*	ЧАТ ДЛЯ ГОМИКОВ
	- что делает strcat?
	- Складывает строки?
	- ну и нахуй оно тут... бля, работал же терминал




*/