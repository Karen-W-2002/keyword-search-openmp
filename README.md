# keyword-search-openmp
A Producer-Consumer Program written in C with OpenMP

### Description
Producers read text from a collection of files, **one per producer**

Consumers take lines of text and tokenise them, and if a keyword is found, the keyword count increases by one

Implemented using queues (thread safe) and multithreads

### Program Functions
- **void Get_Keywords(char \*argv[], char \*keywords[], int keyword_count)**

Gets the inputted keywords from argv

- **void Get_File(char \*argv[], FILE \*fp[], int file_count, int keyword_count)**

Gets input files and save to fp

- **void Read_File(struct Queue \*q, FILE \*fp)**

Reads the fp line by line and stores the lines into a queue using enqueue() function

- **void Close_File(FILE \*fp[], int file_count)**
Close fp files

- **void Tokenise(char \*buf, int global_count[], char \*keywords[], int keyword_count)**

Tokenises the lines inside the queue and compares the tokenised lines with the keywords, if found, it will add to the global count

- **void producer_consumer_func(int producer_count, int consumer_count, FILE \*fp[], int file_count, char \*keywords[], int keyword_count)**

Producer-Consumer function for reading file and tokenise

- **void init_arr(int arr[], int n)**

Initialises integer array to 0

- **void print_keywords_count(int global_count[], char \*keywords[], int keyword_count)**

Prints global count of each keyword

### Queue Structure and Functions
```c
struct QNode {
    char *buf;
    struct QNode *next;
};

struct Queue {
    struct QNode *head, *rear;
};
```

- **struct Queue \*create_queue()**

Function to create a queue

- **struct QNode \*new_node(char \*buf)**

Function to create a node

- **void enqueue(struct Queue \*q, char \*buf)**

Enqueues a new node with data buf into Queue q

- **struct QNode \*dequeue(struct Queue \*q)**

Dequeues from Queue q

- **void print_queue(struct Queue \*q)**

Prints Queue q

### Is the Queue Thread Safe?

Yes! With the use of OpenMP Directives such as

\# pragma omp critical

### Usage
`gcc -g -Wall -fopenmp -o keyword keyword.c`

./keyword [# of producers] [# of consumers] [# of keywords to search for] [keyword1] [keyword2] ... [file1] [file2] ...

Example:
`./keyword 2 2 2 Lorem Ipsum file1.txt file2.txt`

