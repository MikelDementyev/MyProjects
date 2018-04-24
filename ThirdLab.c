#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

typedef struct ListOfMessage {
	char message[100];
	long long int number;
} ListOfMessage;

int compare(const void * x1, const void * x2)   
{
  return ( (*(ListOfMessage*)x1).number - (*(ListOfMessage*)x2).number );             
}

int isValid(char *filename){
	char *regexp = ".+\\.txt$";
	regex_t regexComp;

	if(regcomp(&regexComp, regexp, REG_EXTENDED)){
		return 0;
	}
	
	return regexec(&regexComp, filename, 0, NULL, 0) == 0;
}

void printMessage(char* filename, ListOfMessage*** List, int** Count){
	int i;
	char string[100];
	**(Count) += 1;
	**List = realloc(**List, sizeof(ListOfMessage)* (**Count));
	FILE *f = fopen(filename, "r");
	if(!f)
		return;
	fgets((**List)[**(Count) - 2].message, 100, f);
	for(i = 0; ;i++){
		string[i] = (**List)[**(Count) - 2].message[i];
		if((**List)[**(Count) - 2].message[i+1] == ' ')
			break;
	}
	(**List)[**(Count) - 2].number = atoll(string);
	
	fclose(f);
}

void listDir(char *startDir, ListOfMessage** List, int* Count){
	char nextDir[200]={0};
	strcpy(nextDir, startDir);
	DIR *dir = opendir(startDir);
	if(!dir)
		return;
	struct dirent *de = readdir(dir);
	while(de){
		if(de->d_type == DT_DIR && strcmp(de->d_name, ".") != 0 &&
								   strcmp(de->d_name, "..") != 0){
			int len = strlen(nextDir);
			strcat(nextDir, "/");
			strcat(nextDir,de->d_name);
			listDir(nextDir, List, Count);
			nextDir[len] = '\0';
		}
		if(de->d_type == DT_REG && isValid(de->d_name)){
            int len = strlen(nextDir);
            strcat(nextDir, "/");
            strcat(nextDir,de->d_name);
			printMessage(nextDir, &List, &Count);
            nextDir[len] = '\0';
		}
		de = readdir(dir);
	}
	closedir(dir);

}

int main(int argc, char **argv, char **env){
	int Count = 1, i;
	ListOfMessage* List; 
	List = (ListOfMessage*)malloc(sizeof(ListOfMessage));
if(argc != 2){
	printf("use ./prog <path>\n");
	return 0;
}

listDir("Root", &List, &Count);
qsort(List, Count - 2, sizeof(ListOfMessage), compare);
FILE* file = fopen("result.txt", "w");
	for (i = 0; i < Count - 1; i++){
		fprintf(file, "%s", List[i].message);
		printf("%s", List[i].message);
	} 

return 0;
}
