#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define BUFSIZE 1024

void Get_File(char *argv[], FILE *fp[], int file_count);
void Read_File(FILE *fp[], char buf[][BUFSIZE], int file_count);
void Close_File(FILE *fp[], int file_count);
void producer_consumer_func(int producer_count, int consumer_count, FILE *fp[], int file_count);
struct Queue *create_queue();
struct QNode *new_node(char *buf);
void enqueue(struct Queue *q, char *buf);
void dequeue(struct Queue *q);

struct QNode {
    char *buf;
    struct QNode *next;
};

struct Queue {
    struct QNode *head, *rear;
};

int main(int argc, char *argv[])
{
    int producer_count = strtol(argv[1], NULL, 10);
    int consumer_count = strtol(argv[2], NULL, 10);
    int file_count = strtol(argv[3], NULL, 10);
    FILE *fp[file_count];
    char buffer[file_count][BUFSIZE];
 
    Get_File(argv, fp, file_count); 
    //producer_consumer_func(producer_count, consumer_count);

    Close_File(fp, file_count);
    return 0;
}

void Get_File(char *argv[], FILE *fp[], int file_count)
{
    int i;

    for(i = 0; i < file_count; i++)
    {
        fp[i] = fopen(argv[i+4], "r");
        if(fp[i] == NULL)
        {
            printf("Error on read\n");
            exit(0);
        }
    }
}

void Read_File(FILE *fp[], char buf[][BUFSIZE], int file_count)
{
    int i;

    for(i = 0; i < file_count; i++)
    {
        fread(&(buf[i]), 1024, 1, fp[i]);
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

void producer_consumer_func(int producer_count, int consumer_count, FILE *fp[], int file_count)
{
    int thread_count = producer_count + consumer_count;
    
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
    if(q->rear == NULL)
    {
        q->head = q->rear = NULL;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
}

void dequeue(struct Queue *q)
{
    if(q->head == NULL)
            return;

    struct QNode *temp = q->head;

    q->head = q->head->next;

    if(q->head == NULL)
        q->rear = NULL;

    free(temp);
}
