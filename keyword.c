#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define BUFSIZE 1024

void read_files(char *argv[], char buf[][BUFSIZE], int file_count);
void producer_consumer_func(int producer_count, int consumer_count);



int main(int argc, char *argv[])
{
    //int i;
    int producer_count = strtol(argv[1], NULL, 10);
    int consumer_count = strtol(argv[2], NULL, 10);
    int file_count = strtol(argv[3], NULL, 10);
    //FILE *fp[file_count];
    char buffer[file_count][BUFSIZE];

    // Read file
    /*for(i = 0; i < file_count; i++)
    {
        fp[i] = fopen(argv[i + 4], "r");

        if(fp[i] == NULL)
        {
            printf("Error on file open!\n");
            exit(0);
        }
    }*/

    //Get_files(fp, &file_count);
    //fread(&(buffer[0]), 1024, 1, fp[0]);
    read_files(argv, buffer, file_count);
    printf("%s\n", buffer[1]);
    producer_consumer_func(producer_count, consumer_count);

    /*for(i = 0; i < file_count; i++)
    {
        fclose(fp[i]);
    }*/

    return 0;
}

void read_files(char *argv[], char buf[][BUFSIZE], int file_count)
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
    //int thread_count = producer_count + consumer_count;

}
