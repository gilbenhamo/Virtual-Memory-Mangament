// Gil ben hamo

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

////////////////// Define Zone /////////////////
#define MAX_BUFF_LEN 50
#define OPER_LEN 32
#define INDEX_LEN 32
#define TEXT_LEN 32
#define PAGE_SIZE 1024

#define ALGO_INDEX 1
#define INPUT_INDEX 2
#define OUTPUT_INDEX 3
#define M_INDEX 4
#define N_INDEX 5
/////////////////////////////////////////////////

////////////////// Structs + enums //////////////
typedef struct frame Frame;
struct frame
{
    char* content;
    int id_num;
    int modified;
    int referenced;
    Frame* next;
};

//structs
typedef struct Queue
{
    Frame* head;
    Frame* tail;
    int size;
    int capacity;
}Queue;

enum algorithm
{
    SCFIFO,
    LRU
};
/////////////////////////////////////////////////

////// GLOBAL vars - represent the memories /////
int pf_algorithm = 0,HD_SIZE=0;
char** HD = NULL;
Queue* RAM = NULL;
////////////////////////////////////////////////

////////////////////funcs///////////////////////
//Queue funcs
Queue* BuildQueue(int size);
int isFull(Queue* q);
int isEmpty(Queue* q);
void Enqueue(Queue* q, Frame* t);
Frame* Dequeue(Queue* q);
Frame* FindInQueue(Queue* q,int id);

//Frame funcs
Frame* BuildFrame(int _id,char* _content);
void PrintFrameList(Frame *temp);


//program funcs
char** BuildSecondaryMemo();
void initMainMemo();
void readFromFile(char* input, char* output);
void printDiskToFile(char* path);
Frame* readFromMemo(int key);
char* read(int key);
void write(int key, char* _content);
void PF_LRU(Frame* f);
void PF_SCFIFO(Frame* f);
void moveNodeToEnd(Queue *q, int id);
void pageFault(Frame *f);

void FreeMemo();
