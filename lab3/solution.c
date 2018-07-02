#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>


typedef struct _file_struct file_struct;

struct _file_struct
{
    char *file_name; 
    char *path; 
    char *file_name_link;  
    int type; 
    file_struct *next; 
    file_struct *prev; 
};

file_struct* head=NULL;

void read_file(char *path, char *name);
file_struct* makeElem(char *path, char *name, char *link_name, int type);
void showList();
void addEl(file_struct *el);

void list_dir(const char *dirPath)
{
    DIR *dir = opendir(dirPath);    
    if (dir) { 
        struct dirent *de = readdir(dir); 
        while (de) {             

            char *dirPathNext = (char*) malloc( strlen(dirPath) + strlen(de->d_name) + 2 );

            sprintf(dirPathNext, "%s/%s", dirPath, de->d_name );

            if( de->d_type == DT_DIR && strstr(de->d_name, "..") == NULL && strstr(de->d_name, ".") == NULL) {
                list_dir (dirPathNext);
            }

            if( de->d_type == DT_REG ) {
                read_file(dirPathNext, de->d_name);
            }

            free(dirPathNext);
            de = readdir(dir);      
        }
    }
    closedir(dir);    
}

void read_file(char *path, char *name) {
    FILE *fp;
    fp = fopen(path, "r");
    char buff[255];
    while(!feof(fp)) {
        fscanf(fp, "%s", buff);
        if(feof(fp))
            break;

        file_struct *el;
        if(strstr(buff, "@include") != NULL) {
            fscanf(fp, "%s", buff);

            el = makeElem(path, name, buff, 0);
        }
        if(strstr(buff, "Minotaur") != NULL) {

            el = makeElem(path, name, NULL, 1);
        }
        if(strstr(buff, "Deadlock") != NULL) {

            el = makeElem(path, name, NULL, 2);
        }
        addEl(el); 
    }
    fclose(fp);
}

void addEl(file_struct *el)
{
    if (head == NULL) {
        head = el;
        return;
    }
    file_struct *temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = el;
    el->prev = temp;
}


file_struct* makeElem(char *path, char *name, char *link_name, int type)
{
    file_struct *newEl = (file_struct *) malloc(sizeof(file_struct));
    newEl->file_name = (char *) malloc(strlen(name));
    strcpy(newEl->file_name, name);
    newEl->path = (char *) malloc(strlen(path)+1);
    strcpy(newEl->path, path);
    switch (type) {
    case 0:
        newEl->file_name_link = (char *) malloc(strlen(link_name));
        strcpy(newEl->file_name_link, link_name);
        break;
    case 1:
        newEl->file_name_link = (char *) malloc(strlen("Minotaur"));
        strcpy(newEl->file_name_link, "Minotaur");
        break;
    case 2:
        newEl->file_name_link = (char *) malloc(strlen("Deadlock"));
        strcpy(newEl->file_name_link, "Deadlock");
        break;
    default:
        newEl->file_name_link = NULL;
        break;
    }
    newEl->type = type;
    newEl->next = NULL;
    newEl->prev = NULL;
    return newEl;
}

void showList()
{
    if( head == NULL )
        return;
    file_struct *temp = head;
    while (temp != NULL) {

        temp = temp->next;
    }
}

void removeDeadloks()
{
    if( head == NULL )
        return;
    file_struct *temp = head;
    file_struct *p;
    char *currRemName;
    while (temp != NULL) {
        if(temp->type == 2) {
            currRemName = (char *) malloc(strlen(temp->file_name));
            strcpy(currRemName, temp->file_name);
            p = head;
            while (p != NULL) {

                if(strstr(currRemName, p->file_name) != NULL || strstr(currRemName, p->file_name_link) != NULL) {
                
                    if(p == head){
                        head = head->next;
                        free(p);
                        p = head;
                        continue;
                    }
                
                    file_struct *t = p;
                    p->prev->next = p->next;
                    free(t);
                }
                    p = p->next;

            }
            free(currRemName);
        }

        temp = temp->next;
    }
}

void makePath()
{
    FILE *fp;
    fp = fopen("result.txt", "w");

    file_struct *temp = head;
    char *currRemName = (char *) malloc(strlen("file.txt")+1);
    strcpy(currRemName, "file.txt");

    while ( 1 ) {

        if( strstr(temp->file_name, currRemName) != NULL ) {
            
            fprintf(fp, "%s\n", temp->path);
            free(currRemName);
            currRemName = (char *) malloc(strlen(temp->file_name_link)+1);
            strcpy(currRemName, temp->file_name_link);
            if(strstr(currRemName, "Minotaur")){
                return;
            }
            temp = head;
        } else {
            temp = temp->next;
        }
    }
    fclose(fp);
}

int main()
{
    list_dir("/home/box/labyrinth");

    removeDeadloks();

    makePath();
    return 0;
}

