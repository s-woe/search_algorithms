#include <stdlib.h>
#include <stdio.h>
#include "string.h"


//ENUM FOR NODE DESCRIPTION
enum COLOUR {WHITE,GREY,BLACK};

//ENUM FOR QUEUE OPS
enum MODE {ENQUEUE,DEQUEUE};

//LENGTH OF QUEUE AND GRAPH
size_t QUEUE_length=0;
size_t NODE_count=0;

//TO TALK a node with 'node' not 'struct node'
typedef struct node node;

//typedef to describe one node
typedef struct node
{
    node* parent;
    node* neigbourlist[27];
    size_t neighbourcount;
    char name;
    enum COLOUR colour;
    size_t distance;

}*node_p;


//FUNCTIONS
node* QUEUE_OP(enum MODE,node**,node*);
node* GRAPH_getNode(node*, node*);
void GRAPH_BFS(node*);
void GRAPH_DELETE(node*);

node* GRAPH_getNode(node* Node, node*Graph)
{
    for(size_t i=0;i<NODE_count;i++)
    {
        if(Graph[i].name==Node->name)
            return &Graph[i];
    }
return(0);
}

node* GRAPH_getNodeByName(node *Graph, char name)
{
    node* dummy=(node*)NULL;

    for(size_t i=0;i<NODE_count;i++)
    {
        if(Graph[i].name==name)
            return &Graph[i];

    }
    return dummy;
}


node* QUEUE_OP(enum MODE Mode, node** Queue,node* Node)
{

    switch(Mode)
    {
        case ENQUEUE:
        {
            Queue[QUEUE_length]=Node;
            QUEUE_length++;
            return *Queue;
            break;
        }
        case DEQUEUE:
        {

            node* temp = Queue[0];
            for(size_t i=1;i<QUEUE_length;i++)Queue[i-1]=Queue[i];
            QUEUE_length--;
            return temp;
            break;

        }

        default:break;

    }
    return NULL;
}

void GRAPH_BFS(node* graph)
{
    node* queue[26];
    node* temp;

    QUEUE_length=0;
    graph[0].colour=GREY;

    //ENQUEUE FirstElement of Graph
    QUEUE_OP(ENQUEUE,queue,&graph[0]);

    while(QUEUE_length!=0)
    {
        temp=QUEUE_OP(DEQUEUE,queue,&graph[0]);
        for(size_t i=0;i<temp->neighbourcount;i++)
            if(temp->neigbourlist[i]->colour==WHITE)
            {
                temp->neigbourlist[i]->colour=GREY;
                QUEUE_OP(ENQUEUE,queue,temp->neigbourlist[i]);
            }
        temp->colour=BLACK;
        printf("%c",temp->name);

    }
    printf("\n");
}

int main(void)
{
    size_t stringlength=0;
    size_t count=0;
    char input[27];
    node GRAPH[27];
    while(scanf("%s",input)!=EOF)
    {
        stringlength=strlen(input);
        GRAPH[count].name=input[0];
        GRAPH[count].colour=WHITE;
        GRAPH[count].neighbourcount=0;
        NODE_count++;
        for(size_t i=2;i<stringlength;i++)
        {
            node* tempNode=GRAPH_getNodeByName(GRAPH,input[i]);
            if(tempNode!=(node*)NULL)
            {
               tempNode->neigbourlist[tempNode->neighbourcount]=&GRAPH[count];
               tempNode->neighbourcount+=1;
               GRAPH[count].neigbourlist[GRAPH[count].neighbourcount]=tempNode;
               GRAPH[count].neighbourcount+=1;
            }
         }
         count++;
    }
    NODE_count=count;
    GRAPH_BFS(GRAPH);
    return(0);

}



