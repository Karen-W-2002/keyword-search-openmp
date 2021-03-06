#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define BUFSIZE 1024
#define KEYWORD "Lorem"

struct QNode {
    char *buf;
    struct QNode *next;
};

struct Queue {
    struct QNode *head, *rear;
};

void Get_Keywords(char *argv[], char *keywords[], int keyword_count);
void Get_File(char *argv[], FILE *fp[], int file_count, int keyword_count);
void Read_File(struct Queue *q, FILE *fp);
void Close_File(FILE *fp[], int file_count);
void Tokenise(char *buf, int global_count[], char *keywords[], int keyword_count);
void producer_consumer_func(int producer_count, int consumer_count, FILE *fp[], int file_count, char *keywords[], int keyword_count);
struct Queue *create_queue();
struct QNode *new_node(char *buf);
void enqueue(struct Queue *q, char *buf);
struct QNode *dequeue(struct Queue *q);
void print_queue(struct Queue *q);
void init_arr(int arr[], int n);
void print_keywords_count(int global_count[], char *keywords[], int keyword_count);

int main(int argc, char *argv[])
{
    int producer_count = strtol(argv[1], NULL, 10);
    int consumer_count = strtol(argv[2], NULL, 10);
    int file_count = strtol(argv[1], NULL, 10);
    int keyword_count = strtol(argv[3], NULL, 10);
    char *keywords[keyword_count];
    FILE *fp[file_count];

    Get_Keywords(argv, keywords, keyword_count); 
    Get_File(argv, fp, file_count, keyword_count);
    producer_consumer_func(producer_count, consumer_count, fp, file_count, keywords, keyword_count);
    Close_File(fp, file_count);
    return 0;
}

void Get_Keywords(char *argv[], char *keywords[], int keyword_count)
{
    int i;

    for(i = 0; i < keyword_count; i++)
        keywords[i] = argv[i+4];
}

void Get_File(char *argv[], FILE *fp[], int file_count, int keyword_count)
{
    int i;

    for(i = 0; i < file_count; i++)
    {
        fp[i] = fopen(argv[i+4+keyword_count], "r");
        if(fp[i] == NULL)
        {
            printf("Error on read\n");
            exit(0);
        }
    }
}

void Read_File(struct Queue *q, FILE *fp)
{
    char *line = malloc(BUFSIZE * sizeof(char));
    
    while(fgets(line, BUFSIZE, fp) != NULL)
    {
        line[strcspn(line, "\n")] = 0;
        enqueue(q, line);
        line = malloc(BUFSIZE * sizeof(char));
    }
}

void Close_File(FILE *fp[], int file_count)
{
    int i;

    for(i = 0; i < file_count; i++)
    {
        fclose(fp[i]);
    }
}

void Tokenise(char *buf, int global_count[], char *keywords[], int keyword_count)
{
    char *token = strtok(buf, " ");
    int i;

    while(token != NULL)
    {
        for(i = 0; i < keyword_count; i++)
        {
            if(!strcmp(token, keywords[i]))
            {
                    global_count[i]++;
            }
        }
        token = strtok(NULL, " ");
    }
}

void producer_consumer_func(int producer_count, int consumer_count, FILE *fp[], int file_count, char *keywords[], int keyword_count)
{
    struct Queue *key_q = create_queue();
    int thread_count = producer_count + consumer_count;
    int prod_done_count = 0;
    int global_count[keyword_count];
    init_arr(global_count, keyword_count);

#   pragma omp parallel num_threads(thread_count) default(none) shared(file_count, key_q, fp, producer_count, consumer_count, prod_done_count, global_count, keywords, keyword_count)
    {
        int i = 0;
        int my_rank = omp_get_thread_num();
        
        if(my_rank < producer_count) // Producer Threads
        {
            for(i = my_rank; i < file_count; i += producer_count)
            {
                Read_File(key_q, fp[i]);
            }
#           pragma omp atomic
            prod_done_count++;
        }
        else // Consumer Threads
        {
            struct QNode *temp;        

            while(prod_done_count < producer_count)
            {
#               pragma omp critical
                {
                    temp = dequeue(key_q);
                }  
                
                if(temp != NULL)
#               pragma omp critical
                {
                    Tokenise(temp->buf, global_count, keywords, keyword_count);
                }
            }

            while(key_q->head != NULL) 
            {
#               pragma omp critical
                {
                    temp = dequeue(key_q);
                }

                if(temp != NULL)
#               pragma omp critical
                {
                    Tokenise(temp->buf, global_count, keywords, keyword_count);
                }
            }
        }
    }
    print_keywords_count(global_count, keywords, keyword_count);
}

/*
 * Queue Structure
 */

struct Queue *create_queue()
{
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    q->head = q->rear = NULL;
    return q;
}

struct QNode *new_node(char *buf)
{
    struct QNode *temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->buf = buf;
    temp->next = NULL;
    return temp;
}

void enqueue(struct Queue *q, char *buf)
{
    struct QNode *temp = new_node(buf);

#   pragma omp critical
    if(q->rear == NULL)
    {
        q->head = q->rear = temp;
    }
    else
    {
        q->rear->next = temp;
        q->rear = temp;
    }
}

struct QNode *dequeue(struct Queue *q)
{
    if(q->head == NULL)
            return NULL;
    struct QNode *temp = q->head;

    q->head = q->head->next;

    if(q->head == NULL)
        q->rear = NULL;
    return temp;
}

void print_queue(struct Queue *q)
{
    struct QNode *temp = q->head;

    if(temp == NULL)
    {
        printf("NULL\n");
        return;
    }

    if(temp->next != NULL)
    {
        while(temp)
        {
            printf("%s -> ", temp->buf);
            temp = temp->next;
        }
    }
    else
    {
        printf("%s -> ", temp->buf);
    }

    printf("NULL\n");
}

void init_arr(int arr[], int n)
{
    int i;

    for(i = 0; i < n; i++)
    {
        arr[i] = 0;
    }
}

void print_keywords_count(int global_count[], char *keywords[], int keyword_count)
{
    int i;

    for(i = 0; i < keyword_count; i++)
    {
        printf("%s: %d\n", keywords[i], global_count[i]);
    }
}
