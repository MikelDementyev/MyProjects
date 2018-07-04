#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>


typedef struct _file_struct file_struct;

struct _file_struct
{
    char *file_name; // имя файла
    char *path; //полный путь к файлу
    char *file_name_link; // имя файла на который ссылается
    int type; //0 - файл со ссылкой на следующий, 2 - файл тупик, 1 - минотавр
    file_struct *next; //ссылка на следующий элемент
    file_struct *prev; //ссылка на следующий элемент
};

file_struct* head=NULL;

void read_file(char *path, char *name);
file_struct* makeElem(char *path, char *name, char *link_name, int type);
void showList();
void addEl(file_struct *el);

void list_dir(const char *dirPath)
{
    DIR *dir = opendir(dirPath);    // "открываем" директорию
    if (dir) { // если это удалось успешно
        struct dirent *de = readdir(dir); // получаем очередной элемент открытой директории
        while (de) {             // если это удалось
//            printf("%s/%s\n", dirPath, de->d_name); //печатаем имя этого элемента
            //выделяем память под строку хранящую полный путь
            char *dirPathNext = (char*) malloc( strlen(dirPath) + strlen(de->d_name) + 2 );
            //формируем полный путь к файлу
            sprintf(dirPathNext, "%s/%s", dirPath, de->d_name );

            //если текущий файл - каталог рекурсивно открываем его для сканирования
            if( de->d_type == DT_DIR && strstr(de->d_name, "..") == NULL && strstr(de->d_name, ".") == NULL) {
                list_dir (dirPathNext);
            }
            //если файл - файл, то вычитываем его содержимое
            if( de->d_type == DT_REG ) {
                read_file(dirPathNext, de->d_name);
            }
            //не забываем освободить память
            free(dirPathNext);
            de = readdir(dir);      // снова получаем очередной элемент открытой директории
        }
    }
    closedir(dir);    // не забываем "закрыть" директорию
}

void read_file(char *path, char *name) {
    FILE *fp;
    fp = fopen(path, "r");
    char buff[255];
    while(!feof(fp)) {
        fscanf(fp, "%s", buff);
        if(feof(fp))
            break;
//        printf("\t%s\n", buff); //печатаем имя этого элемента

        file_struct *el;
        if(strstr(buff, "@include") != NULL) {
            fscanf(fp, "%s", buff);
//            printf("\t\t%s\n", buff); //печатаем имя этого элемента
            el = makeElem(path, name, buff, 0);
        }
        if(strstr(buff, "Minotaur") != NULL) {
//            printf("\t\t%s\n", buff); //печатаем имя этого элемента
            el = makeElem(path, name, NULL, 1);
        }
        if(strstr(buff, "Deadlock") != NULL) {
//            printf("\t\t%s\n", buff); //печатаем имя этого элемента
            el = makeElem(path, name, NULL, 2);
        }
        addEl(el); //добавляем
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
        printf("%s, %s, %s, %i\n", temp->file_name, temp->path, temp->file_name_link, temp->type);
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
                    //Удаляем голову особым образом
                    if(p == head){
                        head = head->next;
                        free(p);
                        p = head;
                        continue;
                    }
                    //Удаляем элемент в середине
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
    if(head == NULL)
        return;
    FILE *fp;
    fp = fopen("./result.txt", "w");

    file_struct *temp = head;
    char *currRemName = (char *) malloc(strlen("file.txt")+1);
    strcpy(currRemName, "file.txt");

    while ( 1 ) {

        if( strstr(temp->file_name, currRemName) != NULL ) {
            //printf("%s\n",temp->path);
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
    list_dir("./labyrinth");
//    showList();
//    printf("\n\n");
    removeDeadloks();
//    showList();
//    printf("\n\n");
    makePath();
    return 0;
}
