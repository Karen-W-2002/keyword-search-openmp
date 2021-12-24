#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define BUFSIZE 1024

struct QNode {
    char *buf;
    struct QNode *next;
};

struct Queue {
    struct QNode *head, *rear;
};

void Get_File(char *argv[], FILE *fp[], int file_count);
void Read_File(struct Queue *q, FILE *fp);
void Close_File(FILE *fp[], int file_count);
void Tokenise(char *buf);
void producer_consumer_func(int producer_count, int consumer_count, FILE *fp[], int file_count);
struct Queue *create_queue();
struct QNode *new_node(char *buf);
void enqueue(struct Queue *q, char *buf);
struct QNode *dequeue(struct Queue *q);
void print_queue(struct Queue *q);

int main(int argc, char *argv[])
{
    //struct Queue *q = create_queue();
    int producer_count = strtol(argv[1], NULL, 10);
    int consumer_count = strtol(argv[2], NULL, 10);
    int file_count = strtol(argv[3], NULL, 10);
    FILE *fp[file_count];
 
    Get_File(argv, fp, file_count);
    //Read_File(q, fp[0], file_count);
    producer_consumer_func(producer_count, consumer_count, fp, file_count);
    //print_queue(q);
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

void Read_File(struct Queue *q, FILE *fp)
{
    char *line = malloc(BUFSIZE * sizeof(char));
    
    while(fgets(line, BUFSIZE, fp) != NULL)
    {
        line[strcspn(line, "\n")] = 0;
        enqueue(q, line);
        line = malloc(BUFSIZE * sizeof(char));
    }

    // maybe close file can be here instead of a function
}

void Close_File(FILE *fp[], int file_count)
{
    int i;

    for(i = 0; i < file_count; i++)
    {
        fclose(fp[i]);
    }
}

void Tokenise(char *buf)
{
    printf("BUF: %s\n", buf);
}

void producer_consumer_func(int producer_count, int consumer_count, FILE *fp[], int file_count)
{
    struct Queue *key_q = create_queue();
    int thread_count = producer_count + consumer_count;
    int prod_done_count = 0;

#   pragma omp parallel num_threads(thread_count) default(none) shared(file_count, key_q, fp, producer_count, consumer_count, prod_done_count)
    {
        int i = 0;
        int my_rank = omp_get_thread_num();
        
        if(my_rank < producer_count) // Producer Threads
        {
            for(i = my_rank; i < file_count; i += producer_count)
            {
                // writing to the q is critical tho?
                Read_File(key_q, fp[i]);
            }
#           pragma omp atomic
            prod_done_count++;
        }
        else // Consumer Threads
        {
            struct QNode *temp;        

            while(prod_done_count < producer_count || key_q->head != NULL) 
            {
#               pragma omp critical
                {
                    if(key_q->head != NULL) 
                        temp = dequeue(key_q);
                }
                if(temp != NULL)
                {
                    Tokenise(temp->buf);
                    free(temp);
                }
            }
        }
    }
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

//#   pragma omp critical
    {
        q->head = q->head->next;

        if(q->head == NULL)
            q->rear = NULL;
    }
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
