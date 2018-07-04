#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <limits.h>
#include <string.h>

int strToInt(char *name);

typedef unsigned short word_t;
typedef unsigned char  byte_t;

typedef struct node {
    struct node* next;
    word_t v;
} node_t;

typedef struct {
    node_t* h, *t;
} queue_t;

void   queue_init(queue_t* q){ q->h = q->t = NULL; }
int    queue_empty(queue_t* q){ return (q->h == NULL); }
word_t queue_front(queue_t* q) { return q->h->v; }
int    queue_push(queue_t* q, word_t v);
void   queue_pop(queue_t* q);
void   queue_clear(queue_t* q);

typedef struct {
    word_t v, p;
} vert_t;
byte_t** graph_init(int vn);
void graph_input(byte_t **g, int a, int b);
void     graph_free(byte_t** g, int n);
word_t*  graph_bfs(byte_t** g, int n, word_t s, word_t e, int* m);


typedef struct _file_struct file_struct;

struct _file_struct
{
    char *file_name; // имя файла
    char *path; //полный путь к файлу
    char *file_name_link; // имя файла на который ссылается
    int type; //0 - файл со ссылкой на следующий, 2 - файл тупик, 1 - минотавр
    int id;
    file_struct *next; //ссылка на следующий элемент
    file_struct *prev; //ссылка на следующий элемент
};

file_struct* head=NULL;
int s = 0;
int e = -1;
int n = 0;
byte_t** g;

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
            int a = strToInt(name);
            int b = strToInt(buff);
            graph_input(g, a, b);

            el = makeElem(path, name, buff, 0);
        }
        if(strstr(buff, "Minotaur") != NULL) {
//            printf("\t\t%s\n", buff); //печатаем имя этого элемента
            e = strToInt(name);
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

int strToInt(char *name)
{
    if(strstr(name, "file.txt") != NULL) {
        return 0;
    }
    char dig[5];
    for(int i = 4, j = 0; i < strlen(name)-4; i++, j++){
        dig[j] = name[i];
    }
    int temp = atoi(dig);
    if(n < temp)
        n = temp;
    return temp;
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
    newEl->id = strToInt(name);
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

void makePath(word_t *p, int m)
{
    if(head == NULL)
        return;
    FILE *fp;
    fp = fopen("./result.txt", "w");

    for(int i = 0; i < m; i++) {
        file_struct *temp = head;
        while( temp->next != NULL) {
            if(temp->id == p[i]) {
                break;
            }
            temp = temp->next;
        }
        fprintf(fp, "%s\n", temp->path);
    }

    fclose(fp);
}

int main()
{
    g = graph_init(3000);
    list_dir("./labyrinth");

    int i, m;
    word_t*  p;

    if(g == NULL)
        return 1;
    //Ищем в графе с циклами кратчайший путь
    p = graph_bfs(g, n, s, e, &m);
    if(p != NULL) {
        makePath(p, m);
        free(p);
    }
    /*
    if(p != NULL){
        printf("path: ");
        for(i = 0; i < m; ++i)
            printf("%u ", p[i]);
        putchar('\n');

        free(p);
    } else
        fputs("error find bfs!\n", stderr);
    */
    graph_free(g, n);
    return 0;
}

//поиск в ширину
word_t* graph_bfs(byte_t** g, int n, word_t s, word_t e, int* m){
    int     i, y;
    queue_t q;
    vert_t* vs;
    word_t  v, *p, *d, *j;

    p  = NULL;
    vs = (vert_t*)malloc((size_t)n * sizeof(vert_t));
    if(vs == NULL)
        return NULL;
    for(i = 0; i < n; ++i)
        vs[i].p = vs[i].v = USHRT_MAX;

    vs[s].v = 0;
    queue_init(&q);
    queue_push(&q, s);

    y = 0;
    while(! queue_empty(&q)){
        v = queue_front(&q);
        queue_pop(&q);
        if(v == e){
            y = 1;
            break;
        }
        for(i = 0; i < n; ++i){
            if((g[v][i] != 0) && (vs[i].v > (vs[v].v + 1))){
                queue_push(&q, (word_t)i);
                vs[i].p = v;
                vs[i].v = vs[v].v + 1;
            }
        }
    }
    queue_clear(&q);

    if(y){
        v = e;
        i = 1;
        while(v != s){
            v = vs[v].p;
            ++i;
        }
        p = (word_t*)malloc((size_t)i * sizeof(word_t));
        if(p == NULL)
            return NULL;

        d = p;
        for(*d++ = e; e != s; e = vs[e].p)
            *d++ = vs[e].p;

        for(--d, j = p; j < d; ++j, --d){
            v  = *d;
            *d = *j;
            *j = v;
        }
        *m = i;
    }
    free(vs);
    return p;
}

/*
ввод неориентированного графа, формат
    N - кол-во вершин
*/
byte_t** graph_init(int vn){
    int      i;
    byte_t** g;


    g = (byte_t**)malloc((size_t)vn * sizeof(byte_t*));
    if(g == NULL)
        return NULL;

    for(i = 0; i < vn; ++i){
        g[i] = (byte_t*)calloc((size_t)vn, sizeof(byte_t));
        if(g[i] == NULL){
            i -= 1;
        }
    }
    //*n = vn;
    return g;
}

void graph_input(byte_t **g, int a, int b){
     g[a][b] = g[b][a] = 1;
}

//удаление графа из памяти
void graph_free(byte_t** g, int n){
    int i;
    for(i = 0; i < n; ++i)
        free(g[i]);
    free(g);
}

//---------------
//вставка
int queue_push(queue_t* q, word_t v){
    node_t* p = (node_t*)malloc(sizeof(node_t));
    if(p != NULL){
        p->v    = v;
        p->next = NULL;
        if(q->h == NULL)
            q->h = q->t = p;
        else {
            q->t->next = p;
            q->t = p;
        }
    }
    return (p != NULL);
}

//извлечение
void queue_pop(queue_t* q){
    node_t* t;
    if(q->h != NULL){
        t    = q->h;
        q->h = q->h->next;
        free(t);
        if(q->h == NULL)
            q->t = NULL;
    }
}

//удаление всех
void queue_clear(queue_t* q){
    while(! queue_empty(q))
        queue_pop(q);
}
