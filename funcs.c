// Gil ben hamo
#include "headers.h"

///////program funcs/////////
//init funcs
char** BuildSecondaryMemo()
{
    int size = HD_SIZE;
    char** arr = (char**)malloc(sizeof(char*)*size);
    for(int i=0;i<size;i++)
    {
        arr[i] = (char*)malloc(sizeof(PAGE_SIZE)+1);
        strcpy(arr[i],"");
    }
    return arr;
}

void initMainMemo()
{
    for(int i=0;i<RAM->capacity;i++)
        Enqueue(RAM,BuildFrame(i,HD[i]));
}

//handle memo funcs
void readFromFile(char* input, char* output)
{
    // Setup
    FILE *file;
    char temp[MAX_BUFF_LEN], operation[OPER_LEN], index_buff[INDEX_LEN], text[TEXT_LEN];
    file = fopen(input,"r");
    int counter = 0,index=0;

    // Check if the file is open
    if(!file)
    {
        printf("\033[0;31mCould not open the file\033[0m\n");
        exit(1);
    }    

    // Insert string to the file
    while(fgets(temp, MAX_BUFF_LEN, file)) 
    {
        //get rid of \n
        char *newline = strchr(temp,'\n');
        if(newline)
            *newline = 0; 

        //get the operation
        strcpy(operation,strtok(temp," "));
        if(strcmp(operation,"print")==0)
        {
            printDiskToFile(output);
            continue;
        }

        //get the index
        strcpy(index_buff,strtok(NULL," "));
        index=atoi(index_buff);
        
        if(strcmp(operation,"read")!=0)
            strcpy(text,strtok(NULL," "));

        if(strcmp(operation,"read")==0)
            read(index);
        
        if(strcmp(operation,"write")==0)
            write(index,text);
    }
    fclose(file);
}

void printDiskToFile(char* path)
{
    // Setup
    FILE *file;
    file = fopen(path,"a");

    // Check if the file is open
    if(!file)
    {
        printf("\033[0;31mCould not open the file\033[0m\n");
        exit(1);
    } 
    fprintf(file, "secondaryMemory=["); 
    for(int i=0;i<HD_SIZE;i++)
    {
        fprintf(file, " %s," ,HD[i]);    
    }  
    fprintf(file, "]\n"); 
    
    fclose(file);
}

Frame* readFromMemo(int key)
{
   Frame *f = FindInQueue(RAM,key), *new_f;
   if(f==NULL)
   {
        new_f = BuildFrame(key,HD[key]);
        if(!isFull(RAM))
            Enqueue(RAM,new_f);
        else
            pageFault(new_f);
        return new_f;
   }
   else
   {
       //if the page allready in ram
        if(pf_algorithm==LRU)
            moveNodeToEnd(RAM,f->id_num);
        else
            f->referenced=1;
   } 
   return f;
}

char* read(int key)
{
    Frame* f = readFromMemo(key);
    return f->content;
}

void write(int key, char* _content)
{
    Frame* f = readFromMemo(key);
    f->modified = 1;
    strcat(f->content,_content);
}

//page fault
void PF_LRU(Frame* f)
{
    Frame *rm_frame;
    rm_frame = Dequeue(RAM);
    if(rm_frame->modified==1)
        strcpy(HD[rm_frame->id_num],rm_frame->content);
    Enqueue(RAM,f);
}

void PF_SCFIFO(Frame* f)
{
    Frame *rm_frame;
    int flag = 1;
    do
    {
        rm_frame = Dequeue(RAM);
        if(rm_frame->referenced==0)
        {
            if(rm_frame->modified==1)
                strcpy(HD[rm_frame->id_num],rm_frame->content);
            Enqueue(RAM,f);
            flag = 0;
        }
        else
        {
            rm_frame->referenced=0;
            Enqueue(RAM,rm_frame);
        }
    }
    while(flag);
}

void moveNodeToEnd(Queue *q, int id)
{
    Frame *curr=q->head, *prev=NULL;
    if(q->size<=1 || q->tail && q->tail->id_num == id)
        return;
    while(curr!=NULL)
    {
        if(curr->id_num==id)
        {
            if(prev!=NULL)
                prev->next = curr->next;
            else
                q->head=curr->next;
            curr->next = NULL;
            q->tail->next=curr;
            q->tail=curr;
        }
        prev=curr;
        curr=prev->next;
    }
}

void pageFault(Frame *f)
{
    if(pf_algorithm==LRU)
    {
        PF_LRU(f);
    }
    else
    {
        PF_SCFIFO(f);
    }
}

//free
void FreeMemo()
{
    //////// FREE HD
    for(int i=0;i<HD_SIZE;i++)
    {
        if(HD[i])
            free(HD[i]);

    }
    if(HD)
        free(HD);

    //////// FREE RAM
    Frame *temp=RAM->head,*next = NULL;
    while(temp)
    {
        next = temp->next;
        if(temp->content)
            free(temp->content);
        free(temp);
        temp = next;
    }
    free(RAM);
    printf("free allocated memory\n");
}


//Queue funcs
Queue* BuildQueue(int size)
{
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->head = NULL;
    q->tail=0;
    q->capacity=size;
    q->size=0;
    return q;
}

int isFull(Queue* q)
{
    return q->capacity==q->size;
}

int isEmpty(Queue* q)
{
    return q->size==0;
}

void Enqueue(Queue* q, Frame* f)
{
    if(!isFull(q))
    {
        if(isEmpty(q))
        {
            q->head=f;
            q->tail=f;
        }
        else
        {
            q->tail->next=f;
            q->tail=f;
        }
        q->size++;
    }
    else
        printf("The queue is full\n");
}

Frame* Dequeue(Queue* q)
{
    if(!isEmpty(q))
    {
        Frame* f = q->head;
        if(f==q->tail)
            q->tail=NULL;
        q->head = q->head->next;
        q->size--;
        return f;
    }
    else
    {
        printf("The queue is Empty\n");
        return NULL;
    }
}

Frame* FindInQueue(Queue* q,int id)
{
    Frame* temp = q->head;
    while(temp!=NULL)
    {
        if(temp->id_num==id)
            return temp;
        temp=temp->next;
    }
    return NULL;
}

//Frame funcs
Frame* BuildFrame(int _id, char* _content)
{
    Frame* f = (Frame*)malloc(sizeof(Frame));
    f->next=NULL;
    f->id_num=_id;
    f->content = (char*)malloc(sizeof(PAGE_SIZE)+1);
    strcpy(f->content,_content);
    //f->content=_content;
    f->modified=0;
    f->referenced=0;
    return f;
}

void PrintFrameList(Frame *temp)
{//printing the list organs
	printf("\nThe list is: ");
	while (temp != NULL)
	{
		printf("{%d,%s}, ",temp->id_num, temp->content);
		temp = temp->next;
	}
	printf("\n");
}

