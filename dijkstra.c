#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "math.h"


//ENUM FOR NODE DESCRIPTION
enum COLOUR {WHITE,GREY,BLACK};


//LENGTH OF QUEUE AND GRAPH
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

node* GRAPH_getNode(node*, node*);
void Queue_input(node**, node*,size_t*);
void DIJKSTRA(node*,int [27][27],node*[]);
size_t GRAPH_getIndexOfNode(node*,char);
int NODE_hasNeighbour(node**,char,size_t*);


node* GRAPH_getNode(node* Node, node*Graph)
{
    for(size_t i=0;i<NODE_count;i++)
    {
        if(Graph[i].name==Node->name)
            return Graph+i;
    }
return NULL;
}

node* QUEUE_extractFirst(node**Queue,size_t* length)
{
    node* first=Queue[0];
    if(*length==1)
    {
        *length-=1;
        return first;

    }
    for(size_t i=0;i<*length;i++)
    {
        Queue[i]=Queue[i+1];
    }
    *length-=1;
    return first;
}

node* QUEUE_extractNode(node** queue,size_t* length,char name)
{
    node* temp;
    if(*length==1)
    {
        temp=queue[0];
        queue[0]=(node*)NULL;
        *length=0;
        return temp;
    }
    if(queue[*length-1]->name==name)
    {
        temp=queue[*length-1];
        queue[*length-1]=(node*)NULL;
        *length-=1;
        return temp;

    }
    else
    {

        for(size_t i=0;i<*length;i++)
        {
            if(queue[i]->name==name)
            {
                temp=queue[i];
                for(size_t j=i;j<*length-1;j++)queue[j]=queue[j+1];
                return temp;
            }

        }
    }
}

size_t GRAPH_getIndexOfNode(node* Graph,char name)
{
    for(size_t i=0;i<27;i++)
    {
        if(Graph[i].name == name)return i;
        else continue;
    }
    return -1;
}

node* GRAPH_getNodeByName(node Graph[], char name)
{
    node* dummy=(node*)NULL;

    for(size_t i=0;i<NODE_count;i++)
    {
        if(Graph[i].name==name)
            return &Graph[i];

    }
    return dummy;
}

int NODE_hasNeighbour(node** neighbours,char name,size_t* length)
{
    for(size_t u=0;u<*length;u++)
    {
        if(neighbours[u]->name==name)return 1;

    }
    return 0;
}

void Queue_input(node** Queue, node* Node,size_t* QUEUE_length)
{
    if(*QUEUE_length==0)Queue[0]=Node;
    else
    {
        for(size_t u=0;u<*QUEUE_length;u++)
        {
            if((u==*QUEUE_length-1)&&((Node->distance)>=(Queue[u]->distance)))
            {
                Queue[u+1]=Node;
                break;
            }
            if((Node->distance)>=(Queue[u]->distance))continue;
            if((Node->distance)<(Queue[u]->distance))
            {
                for(size_t p=*QUEUE_length;p>u;p--)
                {
                    Queue[p]=Queue[p-1];
                }
                Queue[u]=Node;
                break;
            }

        }
    }
    *QUEUE_length+=1;
}


void DIJKSTRA(node* Graph,int Adj[27][27], node* Queue[27])
{
    size_t QUEUE_length=0;
    size_t distbuff=0;
    //initialize queue with all nodes
    for(size_t h=0; h<NODE_count;h++)
    {
        Queue_input(Queue,GRAPH_getNodeByName(Graph,Graph[h].name),&QUEUE_length);

    }
    while(QUEUE_length!=0)
    {
        node* min;
        min=QUEUE_extractFirst(Queue,&QUEUE_length);

        min->colour=GREY;


        for(size_t i=0;i<min->neighbourcount;i++)
        {
            node* temp;
            //has the node neigbours yet visited and better reachable over this node?
            if((min->neigbourlist[i]->colour==BLACK)&&((min->neigbourlist[i]->distance)>((min->distance)+Adj[GRAPH_getIndexOfNode(Graph,min->neigbourlist[i]->name)][GRAPH_getIndexOfNode(Graph,min->name)])))
            {
                //then actualize the weight, because its the cheapest way to this node
                min->neigbourlist[i]->distance= (min->distance)+(Adj[GRAPH_getIndexOfNode(Graph,min->neigbourlist[i]->name)][GRAPH_getIndexOfNode(Graph,min->name)]);
                //set the actual to a parent of the neighbour
                min->neigbourlist[i]->parent=min;

                continue;
            }
            if((min->neigbourlist[i]->distance)>(min->distance+Adj[GRAPH_getIndexOfNode(Graph,min->neigbourlist[i]->name)][GRAPH_getIndexOfNode(Graph,min->name)]))
            {
                //then actualize the weight, because its the cheapest way to this node
                min->neigbourlist[i]->distance= (min->distance)+(Adj[GRAPH_getIndexOfNode(Graph,min->neigbourlist[i]->name)][GRAPH_getIndexOfNode(Graph,min->name)]);
                //set the actual to a parent of the neighbour
                min->neigbourlist[i]->parent=min;
                //sort queue with new distances
                temp=QUEUE_extractNode(Queue,&QUEUE_length,min->neigbourlist[i]->name);
                Queue_input(Queue,temp,&QUEUE_length);
            }
        }

        //set node as processed
        min->colour=BLACK;
    }
    for(size_t u=0;u<NODE_count;u++)
    {
        node* actual=&Graph[u];

        //print the processed node
        printf("%c-",actual->name);
        distbuff=actual->distance;
        //print every parent until the start node
        while(actual->parent!=(node*)NULL)
        {
            printf("%c-",actual->parent->name);
            actual=actual->parent;
        }
        //print distance
        printf("%i\n",distbuff);
    }
}

int main(void)
{
    size_t count=0;
    char input[7];
    node GRAPH[27];
    node* QUEUE[27];
    //inizialize distance to node to INFINITY because its unknown
    for(size_t k=0;k<27;k++)
    {
       GRAPH[k].distance=(size_t)10000000000;
    }


    int ADJ[27][27];
    //initialize every edge weight to zero
    for(size_t k=0;k<27;k++)
    {
       for(size_t j=0;j<27;j++)
            ADJ[k][j]=0;
    }


    while((scanf("%s",input)!=EOF))
    {
        node* actual;

        if(GRAPH_getNodeByName(GRAPH,input[0])==(node*)NULL)//is this a not yet registred node?
        {
            GRAPH[count].name=input[0];
            GRAPH[count].parent=(node*)NULL;//initialization
            GRAPH[count].colour=WHITE;
            GRAPH[count].neighbourcount=0;
            actual=&GRAPH[count];                           //set an pointer to the currently added node to update his neigbours/weight relations later
            if(count==0)GRAPH[count].distance=0;            //first node gets weight 0
            count++;

        }
        else
        {
            actual=GRAPH_getNodeByName(GRAPH,input[0]);
        }

        node* neighbour=GRAPH_getNodeByName(GRAPH,input[2]);    // register second node which represents neigbour of 'actual' node
        if(neighbour!=(node*)NULL)
        {

           if((NODE_hasNeighbour(neighbour->neigbourlist,actual->name,&(neighbour->neighbourcount))!=0)||
              (NODE_hasNeighbour(actual->neigbourlist,neighbour->name, &(actual->neighbourcount))!=0))continue;


           //update neighbourlist of neighbour
           neighbour->neigbourlist[neighbour->neighbourcount]=actual;
           neighbour->neighbourcount+=1;

           //update adjacency matrix
           ADJ[GRAPH_getIndexOfNode(GRAPH,neighbour->name)][GRAPH_getIndexOfNode(GRAPH,actual->name)]=atoi(input+4);//set entries of the adjacency matrix
           ADJ[GRAPH_getIndexOfNode(GRAPH,actual->name)][GRAPH_getIndexOfNode(GRAPH,neighbour->name)]=atoi(input+4);//set entries of the adjacency matrix

           //update actual shown node's neigbourlist
           actual->neigbourlist[actual->neighbourcount]=neighbour;
           actual->neighbourcount+=1;
        }

        NODE_count=count;

    }
    DIJKSTRA(GRAPH,ADJ,QUEUE);
    return 0;
}



