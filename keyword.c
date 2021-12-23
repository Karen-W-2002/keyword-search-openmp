#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define BUFSIZE 1024

void Read_File(char *argv[], char buf[][BUFSIZE], int file_count);
void producer_consumer_func(int producer_count, int consumer_count);
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
    char buffer[file_count][BUFSIZE];
    
    Read_File(argv, buffer, file_count);
    producer_consumer_func(producer_count, consumer_count);

    return 0;
}

void Read_File(char *argv[], char buf[][BUFSIZE], int file_count)
{
    FILE *fp[file_count];
    int i;

    for(i = 0; i < file_count; i++)
    {
        fp[i] = fopen(argv[i+4], "r");

        if(fp[i] == NULL)
        {
            printf("Error on read!\n");
            exit(0);
        }
    
        fread(&(buf[i]), 1024, 1, fp[i]);
        fclose(fp[i]);
    }
}

void producer_consumer_func(int producer_count, int consumer_count)
{
    int thread_count = producer_count + consumer_count;
    
}

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
