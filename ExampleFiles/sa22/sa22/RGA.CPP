#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <values.h>
#include <string.h>
#include <graphics.h>
#include <dos.h>
#include <iostream.h>

/* Constants */
//#define DEBUG  //For debuging, it should be turn on.
#define	END_LINE	      -1
#define  HEAD_LINES_NUM  1
#define  MAX_FNAME	    100
#define  LINE_LEN	       80
#define  ESC_CODE        27

#define  MAX_NODES	    100
#define  MAX_EDGES	    500

/*
Types of graph nodes
1.      Ordinary= 0
2.      Bend    = 1
3.      Cross   = 2
4.      Virtual = 3 (these nodes should be omitted at the last)
*/
#define ORDINARY	0
#define BEND		1
#define CROSS		2
#define VIRTUAL	3

#define SHOW_TYPE 2 //( All=1  &  Best=2)
								/* Parameters */
int MAX_GRAPH_NODES	= 100;
int MAX_SKEL_NODES	= 100;
//============================== Genetic Algorithm============================
#define GENERATION			10000
#define Pop_size   	      48 //12|48|108|192|300|...  are the best for presentation on the screen
#define Sel_Num            54 //Number of Selected parents(for marriage)in each ganerationfloat   Crossover_Rate	  =-0.5;
float   Crossover_Rate	  = .75;
float   Mutation_Rate	  = .6;

#define Selective_pressure 1.0 //between (1.0 , 2.0] for Linear Ranking Fitness assingment
#define a                  1.34//between (1.0 , 2.0) for Linear Transformation Fitness assingment
#define b                 -0.1 //between (-1.0 ,0.1)   F(i)= a*F(i)+b ;

#define CrossPoints        3   //Number of cross point in Multipoint crossover
#define Square_edge        100 //The edge length of the square in Rect_Crossover
#define Neighbors_num      10   //The number of neighbors in neighborhood crossover
int     CrossVariance	  =10000;// variance of cross over in ConvexHull crossover
int     MutVariance  	  =10000;// variance of Miutation
int     Tiny_Mut_Radius	  =30;// The Radius of Mutation in Tiny[Edge]_Mutation
int     Mut_Radius  	     =100;// The Radius of Mutation in [Two]Edge_Mutation2
//====================factors of Evaluation4==================================
int     G_min  = 10;
int     FACTOR	= 1;   // LANDA3 is multiplied by this factor, for virtual nodes.

float   LANDA1 = 0.0;   // Node distribution [10000]
float   LANDA2 = 0.0;   // Border line
float   LANDA3 = 0.7;   // Edge len [10]
float   LANDA4 = 6.2;   // Edge Crossing
//float LANDA4 = (LANDA5/(G_min*G_min));  // Edge Crossing
float   LANDA5 = 0.0;   // Node-Edge distribution
float   LANDA6 = 0.000007; //0.00008 for mesh
float   LANDA7 = .1; //2.1 for mesh
float   LANDA8 =  0;
//================
int ITERATION	= 0;	       /* 30 moves per node */ //[30]
int inner_loop_criterion = 0;
int Final_generation=150;
//============================================================================
struct node_t
{
	int	node_num;
	node_t *next;
};
struct polygon_node_t
{
	int node_num;
	int x, y;
	polygon_node_t * next, *prev;
};
struct polygon_node_list_t
{
	int number_of_nodes;
	polygon_node_t * head, *tail;
};
struct crossing_node_t
{
	polygon_node_t * node1, * node2; // begin & end nodes of crossing edge of polygon
	int x, y;						    	// coordinations of crossing point
	crossing_node_t * next, * prev;  // links to next and previous elements of the list
};
struct xy_node_t
{
	int node_num;
	int x, y;
	xy_node_t * next;
};
struct skel_nodes_t
{
	int x, y;
	int element;
	node_t * neighbors;
};
struct graph_nodes_t
{
	int element;
	node_t * neighbors;
	short int type;
};
struct point_list_t
{
	int x, y;
	point_list_t *next;
};
struct	point_t
{
	int x,y;
};
struct	edge_t
{
	int node1,node2;
};
typedef struct point_t place_t;
edge_t  edges[MAX_EDGES];
place_t Coordinates[MAX_NODES];

/* Global Variables */
struct node_t * * adjacenty_list;
struct skel_nodes_t * skel_nodes_list = NULL;
struct graph_nodes_t * graph_nodes_list ;
//struct polygon_node_t * polygon_nodes_list = NULL;// NEW
struct polygon_node_list_t polygon_nodes_list;
struct xy_node_t * fix_nodes_list = NULL;
struct xy_node_t * prelocated_nodes_list = NULL;

int number_of_graph_nodes= 0;
int number_of_ordinary_graph_nodes= 0;
int number_of_skel_nodes = 0;
int number_of_polygon_nodes = 0;
int number_of_fix_nodes = 0;
int number_of_prelocated_nodes = 0;

int number_of_edges = 0;
int number_of_node_edge = 0;
int number_of_edge_crossings = 0;
int number_of_pseudo_edge_crossings = 0;
int Xmax, Ymax;
int Right=479, Left=0, Top=0, Button=479;

float Max_BorderLines, Max_Node_Overlapping, Max_EdgeCrossing;
float Max_Node_Distribution, Max_EdgeLength, Max_Length;
float Max_NodeEdgeDistances, Min_BorderLines;


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//###########################################################################
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//===========================================================================
class Queue
{
public:
	Queue(int MaxQueueSize = 200);
	int IsFull();
	int Add(int x);
	int IsEmpty();
	int Delete();
private:
	int front,rear;
	int * queue;
	int MaxSize;
};
//=================
	Queue::Queue(MaxQueueSize)
	{
		MaxSize=MaxQueueSize;
		queue= new int[MaxSize];
		front= rear = -1;
	}
//==================
	Queue::IsFull()
	{
		if(rear==MaxSize-1) return 1;
		else return 0;
	}
//==================
	Queue::IsEmpty()
	{
		if(front==rear) return 1;
		else return 0;
	}
//==================
	Queue::Add(x)
	{
		if(IsFull()) return 0;
		else{ queue[++rear]=x; return 1;}
	}
//===================
	Queue::Delete()
	{
		if(IsEmpty()) return NULL;
		else { return queue[++front];}
	}
//===========================================================================
//     Bearth-First Search of Graph
/*void BFS(int v,int neighbors[])
{
	int visited[MAX_NODES],k=0;
	Queue q;
	node_t *tmp_ptr;

	for (int i=0; i<MAX_NODES ; i++)
		visited[i]=0;
	visited[v]=1;
//	neighbors[k]=v;
	q.Add(v);
	while(!q.IsEmpty())
	{
		v=q.Delete();
		neighbors[k]=v;
		k++;
		if (k==Neighbors_num)
			break;
		tmp_ptr=adjacenty_list[v];
		while( tmp_ptr->next )    //go to end of edge list
		{
			tmp_ptr = tmp_ptr->next;
			if(!visited[tmp_ptr->node_num])
			{
				q.Add(tmp_ptr->node_num);
				visited[tmp_ptr->node_num]=1;
			}
		}
	}
	delete [] visited;
}  */
//===========================================================================
void BFS()
{
	int visited[MAX_NODES],k=0,v=0;
	int finished[MAX_NODES];
	Queue q;
	node_t  *tmp_ptr;

	for (int i=0; i<MAX_NODES ; i++)
	{	visited[i]=0; finished[i]=0;}
	number_of_graph_nodes=0;
	number_of_edges=0;
	q.Add(v);
	visited[v]=1;
	while(!q.IsEmpty())
	{
		v=q.Delete();
		finished[v]=1;
		number_of_graph_nodes++;
		tmp_ptr=(node_t*)(&graph_nodes_list[v]);
		tmp_ptr->node_num=graph_nodes_list[v].element;
		tmp_ptr->next=graph_nodes_list[v].neighbors;
//		next=(node_t*)(&graph_nodes_list[v]);
		while( tmp_ptr->next )    //go to end of edge list
		{
			tmp_ptr = tmp_ptr->next;
			if(!finished[tmp_ptr->node_num])
			{
					edges[k].node1=v;
					edges[k].node2=tmp_ptr->node_num;
					k++;
				if(!visited[tmp_ptr->node_num])
				{
					q.Add(tmp_ptr->node_num);
					visited[tmp_ptr->node_num]=1;
				}
			}
		}
	}
//	delete [] visited;
	delete [] finished;
	number_of_edges=k;
}
//===========================================================================
int Print(struct graph_nodes_t * graph, int graph_nodes_no)
{
	int i, j;
	struct node_t * tmp_ptr;

	if (!graph)
	{
		printf("Graph is empty!, press any key to exit ...\n");
		return -1;
	}
	printf("\nGraph Nodes : \n");
	for (i=0; i< graph_nodes_no; i++)
	{
		printf("%d , element: %d \n", i, graph[i].element);
		tmp_ptr = graph[i].neighbors;
		while(tmp_ptr)
		{
			printf("  node: %d \n", tmp_ptr->node_num);
			tmp_ptr = tmp_ptr->next;
		}
		printf("press any key ...\n");
		getch();
	}
	return 0;
}
//============================================================================
/* Prints Polygon Nodes List */
int Print(struct polygon_node_list_t * nodes_list)
{
	int i, j;
	struct polygon_node_t * tmp_ptr;

	tmp_ptr = nodes_list->head;
	if (!tmp_ptr)
	{
		printf("Polygon Nodes List is empty!, press any key to exit ...\n");
		return -1;
	}
	printf("\nPolygon Nodes:\n");
	while (tmp_ptr)
	{
		printf("Node %d (%.1f , %.1f) ", tmp_ptr->node_num,
		tmp_ptr->x, tmp_ptr->y);
		tmp_ptr = tmp_ptr->next;
	}
	printf("\nPress any key ...\n");
	getch();
	return 0;
}
//============================================================================
/* Adds a node to the end of the list
int AddTo(node_t * * edge_list, int node_num)
{
	node_t * tmp_ptr;

	tmp_ptr = *(edge_list);
	if (!tmp_ptr)   //edge list is empty
	{
	  *(edge_list) = (node_t *)malloc(sizeof(node_t));
	  (*(edge_list))->node_num = node_num;
	  (*(edge_list))->next = NULL;
	}
	else
	{
	  while(tmp_ptr->next)     //go to end of edge list
		 tmp_ptr = tmp_ptr->next;
	  tmp_ptr->next = (node_t *)malloc(sizeof(node_t));
	  tmp_ptr->next->node_num = node_num;
	  tmp_ptr->next->next = NULL;
	}
	return 0;
}                                   */
//===========================================================================
/* Adds a node to the end of a nodes list */
int AddTo(node_t * * nodes_list, int node_num)
{
	node_t * tmp_ptr;

	tmp_ptr = *(nodes_list);
	if (!tmp_ptr)
	{
		(*(nodes_list)) = (node_t *)malloc(sizeof(node_t));
		if (*(nodes_list) == NULL)
		{
			//outtextxy(0, Ymax-10, "There is not enough memory!, press any key to quit ... ");
			printf("There is not enough memory!, press any key to quit ... \n");
			getch();
			return -1;
		}
		(*(nodes_list))->node_num = node_num;
		(*(nodes_list))->next = NULL;
	}
	else
	{
		while(tmp_ptr->next)
			tmp_ptr = tmp_ptr->next;

		tmp_ptr->next = (node_t *)malloc(sizeof(node_t));
		if (tmp_ptr->next == NULL)
		{
			//outtextxy(0, Ymax-10, "There is not enough memory!, press any key to quit ... ");
			printf("There is not enough memory!, press any key to quit ... \n");
			getch();
			return -1;
		}

		tmp_ptr->next->node_num = node_num;
		tmp_ptr->next->next = NULL;
	}
	return 0;
}
//============================================================================
/* Adds a node to the end of a path list */
int AddTo(point_list_t * * path_list, int x, int y)
{
	point_list_t * tmp_ptr;

	tmp_ptr = *(path_list);
	if (!tmp_ptr)
	{
	  (*(path_list)) = (point_list_t *)malloc(sizeof(point_list_t));
	  if (*(path_list) == NULL)
	  {
		 //outtextxy(0, Ymax-10, "There is not enough memory!, press any key to quit ... ");
		 printf("There is not enough memory!, press any key to quit ... \n");
		 getch();
		 return -1;
	  }
	  (*(path_list))->x = x;
	  (*(path_list))->y = y;
	  (*(path_list))->next = NULL;
	}
	else
	{
	  while(tmp_ptr->next)
		 tmp_ptr = tmp_ptr->next;

	  tmp_ptr->next = (point_list_t *)malloc(sizeof(point_list_t));
	  if (tmp_ptr->next == NULL)
	  {
		 //outtextxy(0, Ymax-10, "There is not enough memory!, press any key to quit ... ");
		 printf("There is not enough memory!, press any key to quit ... \n");
		 getch();
		 return -1;
	  }

	  tmp_ptr->next->x = x;
	  tmp_ptr->next->y = y;
	  tmp_ptr->next->next = NULL;
	}
	return 0;
}
//============================================================================
int AddTo(struct polygon_node_list_t * nodes_list, int node_num, int x, int y)
{
	polygon_node_t * tmp_ptr;

	tmp_ptr = nodes_list->head;
	if (!tmp_ptr)
	{
		nodes_list->head = (polygon_node_t *)malloc(sizeof(polygon_node_t));
		if (!nodes_list->head)
		{
			//outtextxy(0, Ymax-10, "There is not enough memory!, press any key to quit ... ");
			printf("There is not enough memory!, press any key to quit ... \n");
			getch();
			return -1;
		}
		nodes_list->head->node_num = node_num;
		nodes_list->head->x = x;
		nodes_list->head->y = y;
		nodes_list->head->next = NULL;
		nodes_list->head->prev = NULL;
		nodes_list->tail = nodes_list->head;
		nodes_list->number_of_nodes = 1;
	}
	else
	{
		//while(tmp_ptr->next)
		//  tmp_ptr = tmp_ptr->next;
		tmp_ptr = nodes_list->tail; //NEW

		tmp_ptr->next = (polygon_node_t *)malloc(sizeof(polygon_node_t));
		if (tmp_ptr->next == NULL)
		{
			//outtextxy(0, Ymax-10, "There is not enough memory!, press any key to quit ... ");
			printf("There is not enough memory!, press any key to quit ... \n");
			getch();
			return -1;
		}

		tmp_ptr->next->node_num = node_num;
		tmp_ptr->next->x = x;
		tmp_ptr->next->y = y;
		tmp_ptr->next->next = NULL;
		tmp_ptr->next->prev = tmp_ptr;
		nodes_list->tail = tmp_ptr->next;
		nodes_list->number_of_nodes++;
	}
	return 0;
}
//============================================================================
int AddTo(struct xy_node_t * * nodes_list, int node_num, int x, int y)
{
	xy_node_t * tmp_ptr;

	tmp_ptr = *(nodes_list);
	if (!tmp_ptr)
	{
		(*(nodes_list)) = (xy_node_t *)malloc(sizeof(xy_node_t));
		if (*(nodes_list) == NULL)
		{
			//outtextxy(0, Ymax-10, "There is not enough memory!, press any key to quit ... ");
			printf("There is not enough memory!, press any key to quit ... \n");
			getch();
			return -1;
		}
		(*(nodes_list))->node_num = node_num;
		(*(nodes_list))->x = x;
		(*(nodes_list))->y = y;
		(*(nodes_list))->next = NULL;
	}
	else
	{
	 while(tmp_ptr->next)
		tmp_ptr = tmp_ptr->next;

	 tmp_ptr->next = (xy_node_t *)malloc(sizeof(xy_node_t));
	 if (tmp_ptr->next == NULL)
	 {
		 //outtextxy(0, Ymax-10, "There is not enough memory!, press any key to quit ... ");
		 printf("There is not enough memory!, press any key to quit ... \n");
		 getch();
		 return -1;
	 }

	 tmp_ptr->next->node_num = node_num;
	 tmp_ptr->next->x = x;
	 tmp_ptr->next->y = y;
	 tmp_ptr->next->next = NULL;
	}
	return 0;
}
//============================================================================
/* Removs all nodes from the list */
int RemoveAllNode(node_t * nodes_list)
{
	node_t * tmp_ptr;

	while (nodes_list)
	{
	  tmp_ptr = nodes_list->next;
	  free(nodes_list);
	  nodes_list = tmp_ptr;
	}
	return 0;
}
//============================================================================
/* Removs all nodes from the list */
int RemoveAll(node_t * edge_list)
{
	node_t * tmp_ptr;

	while (edge_list)
	{
	  tmp_ptr = edge_list->next;
	  free(edge_list);
	  edge_list = tmp_ptr;
	}
	return 0;
}
//============================================================================
int sget(char * str, int index, char * res_str)
{
	int i, j, k;

	j = 0;
	for (i=0 ; i<index; i++)
	{
		while ( (str[j] == ' ') || (str[j] == 9) )
			j++;
		k = 0;
		memset(res_str, 0, 20);
		while ( (str[j] != ' ') && (str[j] != 10)
						&& (str[j] != 13) && (str[j] != 0) )
		{
			res_str[k] = str[j];
			j++;
			k++;
		}
	}
	return 0;
}
//===========================================================================
int Give_Inputs(char * input)
{
	FILE	*input_file;
	FILE	*parent_file;
	FILE	*prms_file;
	char   a_line[LINE_LEN];
	char   tmp_str[20];
	char	 prms_name[100];
	char   type[6];
	char	 choice, input_name[100], parent_name[100],s[100];
	int	 node_num, i, j, index, x, y;
	int    ch, len;
	int	 first, first_node;
	int    node_type;

// clrscr();
// number_of_holes = 0; FELLAN
// printf("Please Enter Input File Name : ");
// scanf("%s", input_name);
	strcpy(input_name, input);
	strcpy(prms_name, "params.txt");

	/* Reading parameters */
	if ((prms_file = fopen(prms_name, "rb")) == NULL)
	{
		//clrscr();
		printf("Error : I can not read the parameters file!");
		return -2;
	}
	fscanf(prms_file, "%f", &LANDA3); // Edge-length
	fscanf(prms_file, "%f", &LANDA4); // Corssings
	fscanf(prms_file, "%f", &LANDA5); // Node-Edge distribution
	fscanf(prms_file, "%f", &LANDA6); // CSDE
	fscanf(prms_file, "%f", &LANDA7); // MSEE
	fscanf(prms_file, "%f", &LANDA8); // Bends

	fclose(prms_file);
  /*  End of reading parameters */

	if ((input_file = fopen(input_name, "rb")) == NULL)
	{
		//clrscr();
		printf("Error : I can not read the input file!");
		return -2;
	}

	number_of_graph_nodes = 0;
	number_of_ordinary_graph_nodes = 0;
	number_of_skel_nodes = 0;
	//number_of_polygon_nodes = 0; NEW
	polygon_nodes_list.number_of_nodes = 0;
	number_of_fix_nodes = 0;
	number_of_prelocated_nodes = 0;

	graph_nodes_list = (struct graph_nodes_t *)(malloc(MAX_GRAPH_NODES*sizeof(struct graph_nodes_t)));
	if (graph_nodes_list == NULL)
	{
		printf("There is not enough memory!, press any key to quit ... ");
		getch();
		return -2;
	}

	skel_nodes_list = (struct skel_nodes_t *)(malloc(MAX_SKEL_NODES*sizeof(struct skel_nodes_t)));
	if (skel_nodes_list == NULL)
	{
		printf("There is not enough memory!, press any key to quit ... ");
		getch();
		return -2;
	}

	for (i=0; i<MAX_GRAPH_NODES; i++)
	{
		graph_nodes_list[i].element = i;
		graph_nodes_list[i].neighbors = NULL;
		graph_nodes_list[i].type = ORDINARY;
	}


	for (i=0; i<MAX_SKEL_NODES; i++)
	{
		skel_nodes_list[i].x = 0;
		skel_nodes_list[i].y = 0;
		skel_nodes_list[i].element = i;
		skel_nodes_list[i].neighbors = NULL;
	}
	for (i=0; i<MAX_NODES; i++)
	{  Coordinates[i].x=0;
		Coordinates[i].y=0;
	}

	/* Following instructions skip unusefull informations */
	for (i=0; i < HEAD_LINES_NUM+1; i++)
		fgets(a_line, LINE_LEN, input_file);

	/* Constructs Polygon Nodes list */
	while (a_line[0] != '%')
	{
		sscanf(a_line, "%d %d %d", &node_num, &x, &y);
		if (AddTo(&polygon_nodes_list, node_num, x, y) < 0)
		{
			printf("GiveInput: I can't add to the list!, press any key to quit ... ");
			getch();
			return -1;
		}
		fgets(a_line, LINE_LEN, input_file);
		number_of_polygon_nodes++;
	}

  /* Constructs Nodes List of the Skeleton */
	fgets(a_line, LINE_LEN, input_file);
	while (a_line[0] != '%')
	{
		sscanf(a_line, "%d %d %d", &node_num, &x, &y);
		index = number_of_skel_nodes;
		skel_nodes_list[index].x = x;
		skel_nodes_list[index].y = y;
		skel_nodes_list[index].element = node_num;
		skel_nodes_list[index].neighbors = NULL;
		i = 4;
		sget(a_line, i, tmp_str);
		node_num = atoi(tmp_str);
		while (node_num != END_LINE)
		{
			AddTo(&(skel_nodes_list[index].neighbors), node_num);
			sget(a_line, ++i, tmp_str);
			node_num = atoi(tmp_str);
		}
		fgets(a_line, LINE_LEN, input_file);
		number_of_skel_nodes++;
	}

	/* Constructs Nodes List of the Graph */
	fgets(a_line, LINE_LEN, input_file);
	while (a_line[0] != '%')
	{
		sscanf(a_line, "%d %d", &node_type, &node_num);
		index = number_of_graph_nodes;
		graph_nodes_list[index].element = node_num;
		graph_nodes_list[index].neighbors = NULL;
		graph_nodes_list[index].type = node_type;
		if (node_type == ORDINARY)
			number_of_ordinary_graph_nodes++;
		i = 3;
		sget(a_line, i, tmp_str);
		node_num = atoi(tmp_str);
		while (node_num != END_LINE)
		{
			AddTo(&(graph_nodes_list[index].neighbors), node_num);
			number_of_edges++;
			sget(a_line, ++i, tmp_str);
			node_num = atoi(tmp_str);
		}
		fgets(a_line, LINE_LEN, input_file);
		number_of_graph_nodes++;
		number_of_edges--;
	}
	number_of_edges /=2;

	// Add Prelocated Nodes to the Prelocated nodes list
	fgets(a_line, LINE_LEN, input_file);
	index = 0;
	while (a_line[0] != '%')
	{
		sscanf(a_line, "%d %d", &x, &y);
		AddTo(&prelocated_nodes_list, node_num, x, y);
		Coordinates[index].x=x;
		Coordinates[index].y=y;
		index++;
		fgets(a_line, LINE_LEN, input_file);
	}
	// Add Fixed Nodes to the fixed nodes list
	fgets(a_line, LINE_LEN, input_file);
	while (a_line[0] != '%')
	{
		sscanf(a_line, "%d %f %f", &node_num, &x, &y);
		index = number_of_fix_nodes;
		AddTo(&fix_nodes_list, node_num, x, y);
		fgets(a_line, LINE_LEN, input_file);
		number_of_fix_nodes++;
	}

	fclose(input_file);
	return 0;
}
//===========================================================================
void Init_Placement(place_t *place)
{
	int i, m, used[MAX_NODES];
	for (i=0; i < number_of_graph_nodes ; i++)
		used[i]=0;
	m = random(number_of_graph_nodes);
	for (i=0; i < number_of_graph_nodes ; i++)
	{
		while(used[m])
			m=random(number_of_graph_nodes);
		used[m]=1;
		place[i].x = Coordinates[m].x;
		place[i].y = Coordinates[m].y;
		m = random(number_of_graph_nodes);
	}
}
//===========================================================================
// Adds an element to the head of the list.
int AddTo(crossing_node_t * * crossing_list, polygon_node_t * p1,
								polygon_node_t * p2, float x, float y)
{
	struct crossing_node_t * tmp_ptr;

	tmp_ptr = *crossing_list;
	*crossing_list = (crossing_node_t *)malloc(sizeof(crossing_node_t));
	(*crossing_list)->x = x;
	(*crossing_list)->y = y;
	(*crossing_list)->node1 = p1;
	(*crossing_list)->node2 = p2;
	(*crossing_list)->next = tmp_ptr;
	(*crossing_list)->prev = NULL;
	if (tmp_ptr)
		tmp_ptr->prev = *crossing_list;
	return 0;
}
//============================================================================
// dose line t1-t2 crosses the edge p1-p2?
int has_crossing(place_t * t1, place_t * t2,polygon_node_t * p1, polygon_node_t * p2)
{
	float u2, d;
	int X1, Y1, X2, Y2, x1, y1, x2, y2;

	X1 = p1->x;
	Y1 = p1->y;
	X2 = p2->x;
	Y2 = p2->y;

	x1 = t1->x;
	y1 = t1->y;
	x2 = t2->x;
	y2 = t2->y;

	//printf("X1 = %f , Y1 = %f    X2 = %f , Y2 = %f \n", X1, Y1, X2, Y2);
	//printf("x1 = %f , x1 = %f    x2 = %f , x2 = %f \n", x1, x1, x2, x2);

	d  = (y1-y2)*(X1-X2) - (x1-x2)*(Y1-Y2);
	if (d)
	{
		//u1 = ( (x2 -X2)*(Y1 -Y2) - (y2-Y2)*(X1-X2) ) / d;
		u2 =  ( (X2 -x2)*(y1 -y2) - (Y2-y2)*(x1-x2) ) / (-d);
		if ((0.0 <= u2) &&(u2 <= 1.0))
			return 1;
	}
	return 0;
}
//============================================================================
/*
  The following function finds all polygon crossings of an edge.
  There are some special cases that I didn't consider here!
			  /                                \
			/____                                \ ____
	 This case    / |     should be ignored and this case |\    should
			 /   |                                     |  \
  be considered as just one crossing.
*/
int find_polygon_corssings(place_t * place1, place_t * place2,

	crossing_node_t * * crossing_list)
{
	float x1, y1, x2, y2;
	int k;
	polygon_node_t  *p1, *p2;
	float u1, u2, sum, d;
	float X1, Y1, X2, Y2, x, y;
	float x_in, y_in, T1, T2, L1, L2, B1, B2, R1, R2, TT1, TT2;
	int result = 0;

	x1 = place1->x; y1 = place1->y;
	x2 = place2->x; y2 = place2->y;

	p1 = polygon_nodes_list.head;
	if (p1)
		p2 = p1->next;
	else
		p2 = NULL;

	while ((p1 != NULL) && (p2 != NULL))
	{
		X1 = p1->x;
		Y1 = p1->y;

		X2 = p2->x;
		Y2 = p2->y;

		d  = (y1-y2)*(X1-X2) - (x1-x2)*(Y1-Y2);
		if (d)
		{
			u1 = ( (x2 -X2)*(Y1 -Y2) - (y2-Y2)*(X1-X2) ) / d;
			u2 =  ( (X2 -x2)*(y1 -y2) - (Y2-y2)*(x1-x2) ) / (-d);
			// If u2 == 1 then point (X1, Y1) is on line (x1, y1)-(x2, y2).
			// If u2 == 0 then point (X2, Y2) is on line (x1, y1)-(x2, y2).
			// So coordinate of the crossing point will be :
			// x = u2*X1 + (1-u2)*X2 and y = u2*Y1 + (1-u2)*Y2
			if ((0.0 <= u1) && (u1 <= 1.0) && (0.0 < u2) &&(u2 <= 1.0))
			//if ((0.0 < u1) && (u1 < 1.0) && (0.0 < u2) &&(u2 <= 1.0))
			{
				x = u2*X1 + (1-u2)*X2;
				y = u2*Y1 + (1-u2)*Y2;
				AddTo(crossing_list, p1, p2, x, y);
				result++;
			}
		}
		if (p2 == polygon_nodes_list.head)
			break;
		p1 = p2;
		p2 = p1->next;
		if (p2 == NULL)
			p2 = polygon_nodes_list.head;
	}
	return result;
}
//============================================================================
void sort_on_x(struct crossing_node_t * crossing_list, int n)
{
	int i, j;
	struct crossing_node_t * tmp_ptr1, * tmp_ptr2, tmp_node;

	if (!crossing_list)
		return;

	for (i=n-1; i > 0; i--)
	{
		tmp_ptr1 = crossing_list;
		tmp_ptr2 = tmp_ptr1->next;
		for (j=1; j <= i; j++)
		{
			if (!tmp_ptr1 || !tmp_ptr2)
				break;
			if (tmp_ptr1->x > tmp_ptr2->x)
			{
				//swap tmp_ptr1 & tmp_ptr2
				tmp_node.node1 = tmp_ptr1->node1;
				tmp_node.node2 = tmp_ptr1->node2;
				tmp_node.x = tmp_ptr1->x;
				tmp_node.y = tmp_ptr1->y;

				tmp_ptr1->node1 = tmp_ptr2->node1;
				tmp_ptr1->node2 = tmp_ptr2->node2;
				tmp_ptr1->x = tmp_ptr2->x;
				tmp_ptr1->y = tmp_ptr2->y;

				tmp_ptr2->node1 = tmp_node.node1;
				tmp_ptr2->node2 = tmp_node.node2;
				tmp_ptr2->x = tmp_node.x;
				tmp_ptr2->y = tmp_node.y;
			}
			// NEW, to go forward:
			tmp_ptr1 = tmp_ptr2;
			tmp_ptr2 = tmp_ptr2->next;
		}
	}
}
//============================================================================
void sort_on_y(struct crossing_node_t * crossing_list, int n)
{
	int i, j;
	struct crossing_node_t * tmp_ptr1, * tmp_ptr2, tmp_node;

	if (!crossing_list)
		return;

	for (i=n-1; i > 0; i--)
	{
		tmp_ptr1 = crossing_list;
		tmp_ptr2 = tmp_ptr1->next;
		for (j=1; j <= i; j++)
		{
			if (!tmp_ptr1 || !tmp_ptr2)
				break;
			if (tmp_ptr1->y > tmp_ptr2->y)
			{
				//swap tmp_ptr1 & tmp_ptr2
				tmp_node.node1 = tmp_ptr1->node1;
				tmp_node.node2 = tmp_ptr1->node2;
				tmp_node.x = tmp_ptr1->x;
				tmp_node.y = tmp_ptr1->y;

				tmp_ptr1->node1 = tmp_ptr2->node1;
				tmp_ptr1->node2 = tmp_ptr2->node2;
				tmp_ptr1->x = tmp_ptr2->x;
				tmp_ptr1->y = tmp_ptr2->y;

				 tmp_ptr2->node1 = tmp_node.node1;
				 tmp_ptr2->node2 = tmp_node.node2;
				 tmp_ptr2->x = tmp_node.x;
				 tmp_ptr2->y = tmp_node.y;
			}
			// NEW, to go forward:
			tmp_ptr1 = tmp_ptr2;
			tmp_ptr2 = tmp_ptr2->next;
		}
	}
}
//============================================================================
void free_crossing_list(struct crossing_node_t * * crossing_list)
{
	struct crossing_node_t * cross_tmp_ptr2;
	struct crossing_node_t * cross_tmp_ptr1 = *crossing_list;

	while (cross_tmp_ptr1)
	{
		cross_tmp_ptr2 = cross_tmp_ptr1;
		cross_tmp_ptr1 = cross_tmp_ptr1->next;
		free(cross_tmp_ptr2);
	}
	*crossing_list = NULL;
}
//============================================================================
void free_path_list(point_list_t * * path_list)
{
	point_list_t * path_tmp_ptr2;
	point_list_t * path_tmp_ptr1 = *path_list;

	while (path_tmp_ptr1)
	{
		path_tmp_ptr2 = path_tmp_ptr1;
		path_tmp_ptr1 = path_tmp_ptr1->next;
		free(path_tmp_ptr2);
	}
	*path_list = NULL;
}
//============================================================================
void find_path(struct crossing_node_t * cross_node_ptr1,
	struct crossing_node_t * cross_node_ptr2, point_list_t * * path_list,
	place_t * graph_node_ptr1,
	place_t * graph_node_ptr2)
{
	struct polygon_node_t * p1, * p2, * q1, *q2, *p3, *p4;

	//printf("find_path 1 \n");

	p1 = cross_node_ptr1->node1;
	p2 = cross_node_ptr1->node2;
	q1 = cross_node_ptr2->node1;
	q2 = cross_node_ptr2->node2;

	//printf("find_path 2 \n");

	AddTo(path_list, cross_node_ptr1->x, cross_node_ptr1->y);
	//printf("find_path 3 \n");
	AddTo(path_list, p2->x, p2->y);
	//printf("find_path 4 \n");
	p3 = p2;
	while ((p3 != q1))
	{
		p4 = p3->next;
		if (!p4)
			p4 = polygon_nodes_list.head;
		//printf("find_path 5 p3->node= %d p4->node=%d\n", p3->node_num, p4->node_num);
		if (has_crossing(graph_node_ptr1, graph_node_ptr2, p3, p4))
			goto next_step;
		//printf("find_path 6 \n");
		AddTo(path_list, p3->x, p3->y);
		//printf("find_path 7 \n");
		//p3 = p3 ->next; BUG FIXED NEW 31 Ordibehesht 82
		p3 = p4;
	}
	//printf("find_path 8 \n");
	if (p2 != q1)
		AddTo(path_list, q1->x, q1->y);
	//printf("find_path 9 \n");
	AddTo(path_list, cross_node_ptr2->x, cross_node_ptr2->y);
	//printf("find_path 10 \n");
	return;

next_step:
	free_path_list(path_list);

	AddTo(path_list, cross_node_ptr1->x, cross_node_ptr1->y);
	AddTo(path_list, p1->x, p1->y);
	p3 = p1;
	while ((p3 != q2))
	{
		p4 = p3->prev;
		if (!p4)
			p4 = polygon_nodes_list.tail;
		AddTo(path_list, p3->x, p3->y);
		//p3 = p3 ->prev; BUG FIXED 31 Ordibehesht 82
		p3 = p4;
	}
	if (p1 != q2)
		AddTo(path_list, q2->x, q2->y);
	AddTo(path_list, cross_node_ptr2->x, cross_node_ptr2->y);
	return;
}
//============================================================================
int RemoveGraphEdge(int node1, int node2)
{
	node_t * neighbor2;
	node_t * neighbor = graph_nodes_list[node1].neighbors;

	if (neighbor->node_num == node2)
	{
		graph_nodes_list[node1].neighbors = neighbor->next;
		free(neighbor);
		return 0;
	}

	while (neighbor->next)
	{
		neighbor2 = neighbor->next;
		if (neighbor2->node_num == node2)
		{
			neighbor->next = neighbor2->next;
			free(neighbor2);
			return 0;
		}
		else
			neighbor = neighbor2;
	}
	return 1;
}
//============================================================================
int update_graph_place(int node1, int node2, point_list_t * path_list,place_t * place)
{
	node_t * neighbor;
	point_list_t  *t2; //*t1;

	if (!path_list)
		return 1;

	//remove graph edge node1 --> node2
	if (RemoveGraphEdge(node1, node2))
		return 1;

	//remove graph edge node2 --> node1
	if (RemoveGraphEdge(node2, node1))
		return 1;

	//create new node t
	graph_nodes_list[number_of_graph_nodes].element = number_of_graph_nodes;
	graph_nodes_list[number_of_graph_nodes].neighbors = NULL;
	graph_nodes_list[number_of_graph_nodes].type = BEND;

	//set t = path_list
	place[number_of_graph_nodes].x = path_list->x;
	place[number_of_graph_nodes].y = path_list->y;

	//add node1 -->t
	AddTo(&(graph_nodes_list[node1].neighbors), number_of_graph_nodes);

	//add t -->node1
	AddTo(&(graph_nodes_list[number_of_graph_nodes].neighbors), node1);

	number_of_graph_nodes++;

	//t1 = path_list;
	t2 = path_list->next;

	while (t2)
	{
	  //create a new graph_node and set to t2
	  graph_nodes_list[number_of_graph_nodes].element = number_of_graph_nodes;
	  graph_nodes_list[number_of_graph_nodes].neighbors = NULL;
	  graph_nodes_list[number_of_graph_nodes].type = BEND;
	  place[number_of_graph_nodes].x = t2->x;
	  place[number_of_graph_nodes].y = t2->y;

	  //add t1 -->t2
	  AddTo(&(graph_nodes_list[number_of_graph_nodes-1].neighbors), number_of_graph_nodes);
	  //add t2 -->t1
	  AddTo(&(graph_nodes_list[number_of_graph_nodes].neighbors), number_of_graph_nodes-1);

	  number_of_graph_nodes++;

	  //t1 = t2;
	  t2 = t2->next;
	}

	//add node2 -->t1
	AddTo(&(graph_nodes_list[node2].neighbors), number_of_graph_nodes-1);

	//add t1 -->node2
	AddTo(&(graph_nodes_list[number_of_graph_nodes-1].neighbors), node2);

	return 0;
}
//============================================================================
/*
  This function works for convex & rectilinear polygons.
  It puts all non-fixed and non-prelocated nodes at the
  location of their first fixed or prelocated predecessor.
  Then it replace all edges of the graph which cross the
  polygon with a path that is not crossing
  the polygon.
*/
int Final_Placement4(place_t *place)
{
	int i, j, neighbor;
	point_list_t * path_tmp_ptr, *tmp_path_list;
	node_t * tmp_ptr;
	point_list_t * path_list = NULL;
	int node1, node2, crossings;
	place_t tmp;
	int XX, YY;
	struct crossing_node_t * tmp_ptr2, * crossing_list = NULL;
	struct crossing_node_t * cross_tmp_ptr1, * cross_tmp_ptr2;

	// for all edges of the graph that have two fixed
	// end points, check if they cross the boundary of
	// the polygon.
	for (i=0; i< number_of_graph_nodes; i++)
	{
		node1 = graph_nodes_list[i].element;
		tmp_ptr = graph_nodes_list[i].neighbors;
		while(tmp_ptr)
		{
			node2 = tmp_ptr->node_num;
			tmp_ptr = tmp_ptr->next;

			if ((node2 < node1) || (node2 >= number_of_graph_nodes))
				continue;

			crossings = find_polygon_corssings(&place[node1], &place[node2], &crossing_list);
			if (!crossings)
					continue;

			if (crossings%2)
			{
//				 printf("ERROR 1 - There is an error in crossing list!* %d  %d\n",node1,node2);
				free_crossing_list(&crossing_list);
				 continue;
			}
			// sorting is required for grouping
			// crossing points 2-by-2.
			if (place[node1].y == place[node2].y)
					 sort_on_x(crossing_list, crossings);
			else
					sort_on_y(crossing_list, crossings);

			tmp_ptr2 = crossing_list;
			while (tmp_ptr2)
			{
				if (tmp_ptr2->next == NULL)
				{
//							 printf("ERROR 2 - There is an error in crossing list!\n");
							 goto end2;
				}
				tmp_path_list = NULL;
				find_path(tmp_ptr2, tmp_ptr2->next, &tmp_path_list,
											&place[node1], &place[node2]);
				if (path_list == NULL)
					path_list = tmp_path_list;
				else
				{
							 path_tmp_ptr = path_list;
							 while (path_tmp_ptr->next)
								 path_tmp_ptr = path_tmp_ptr->next;
							 path_tmp_ptr->next = tmp_path_list;
				}
				tmp_ptr2 = tmp_ptr2->next->next;
			}
			if (place[node1].y == place[node2].y)
			{
				if (place[node1].x < place[node2].x)
						update_graph_place(node1, node2, path_list, place);
				else
						update_graph_place(node2, node1, path_list, place);
			}
			else
			{
				if (place[node1].y < place[node2].y)
							update_graph_place(node1, node2, path_list, place);
				else
							update_graph_place(node2, node1, path_list, place);
			}
end2:		free_path_list(&path_list);
			free_crossing_list(&crossing_list);
		} //while
	} // for
  return 0;
}
//============================================================================
int HasPolygonCrossing(int x1, int y1,
	  int x2, int y2);
//============================================================================
int IsFixed(int i, place_t * a_place)
{
	xy_node_t * fix_node;

	fix_node = fix_nodes_list;
	while (fix_node)
	{
		if (fix_node->node_num == i)
		{
			if (a_place)
			{
				a_place->x = fix_node->x;
				a_place->y = fix_node->y;
			}
			return 1;
		}
		fix_node = fix_node->next;
	}
	return 0;
}
//============================================================================
int IsPrelocated(int i, place_t * a_place)
{
	xy_node_t * pre_node;

	pre_node = prelocated_nodes_list;
	while (pre_node)
	{
		if (pre_node->node_num == i)
		{
			if (a_place)
			{
				a_place->x = pre_node->x;
				a_place->y = pre_node->y;
			}
			return 1;
		}
		pre_node = pre_node->next;
	}
	return 0;
}
//============================================================================
int Perturb(place_t * place, place_t *new_place,float RadiusX, float RadiusY)
{
	int        k, a_random, ch;
// node_t  *  next;
	static int i=0;
	static int j=0;
	int        x1, y1, x2, y2;
	int        hole_crossing, node_num, neighbor_num, polygon_crossing;
	struct     node_t * neighbor;
	place_t    tmp;


	for (k=0; k < number_of_graph_nodes; k++)
	{
		new_place[k].x = place[k].x;
		new_place[k].y = place[k].y;
	}

	// This statement fixes the root (node 1)
	//if (i==0)
	//  i++;

	// This loop statement fixes fix nodes
	// and when finds the first non-fixed node
	// breaks and then a random coordination is
	// slected for that node.
	k = i;
	while (IsFixed(i, &tmp))
	{
		i++;
		i=i%number_of_graph_nodes;
		if (!i)
			j++;
		if (j==ITERATION)
		{
			inner_loop_criterion = 1;
			j = 0;
		}
		if (i == k) // all nodes are fix!
			return 1;  // breaks the infinite loop
	}

	hole_crossing = 0; //FELLAN
	polygon_crossing = 1;

	while (hole_crossing || polygon_crossing)
	{
		if (kbhit())
		{
			ch = getch();
			if (ch == ESC_CODE)
			{
				//outtextxy(0, Ymax-10, "Perturb : Good bye! Press any key ... ");
				printf("Perturb : Good bye! Press any key ... \n");
				getch();
				return -1;
			}
		}
		// this loops select a random x coordinate for ith point.
		while (1)
		{
			if ((float)(rand()) < (float)((float)RAND_MAX/2))
			{
				a_random = (int)(new_place[i].x + ((float)(rand() * RadiusX) / RAND_MAX));
				if (a_random < Xmax)
				{
					x1 = a_random;
					break;
				}
			}
			else
			{
				a_random = (int)(new_place[i].x - ((float)(rand() * RadiusX) / RAND_MAX));
				if (a_random > 0)
				{
					x1 = a_random;
					break;
				}
			}
		}  //while(1)
		// this loops select a random y coordinate for ith point.
		while (1)
		{
			if ((float)(rand()) < (float)((float)RAND_MAX/2))
			{
				a_random = (int)(new_place[i].y + ((float)(rand() * RadiusY) / RAND_MAX));
				if (a_random < Ymax)
				{
					y1 = a_random;
					break;
				}
			}
			else
			{
				a_random = (int)(new_place[i].y - ((float)(rand() * RadiusY) / RAND_MAX));
				if (a_random > 0)
				{
					y1 = a_random;
					break;
				}
			}
		}

		// Test of being out of polygon region.
		node_num = graph_nodes_list[i].element;
		if (node_num != i)
		{
			//outtextxy(0, Ymax-10, "The node_number is not correct in the graph nodes list!!!");
			printf("The node_number is not correct in the graph nodes list!!!\n");
			getch();
			return -1;
		}
		if (node_num > number_of_graph_nodes)
		{
			//outtextxy(0, Ymax-10, "(17) The node_number is greater than number_of_graph_nodes!!!");
			printf("(17) The node_number is greater than number_of_graph_nodes!!!\n");
			getch();
			return -1;
		}

		polygon_crossing = 0;
		neighbor = graph_nodes_list[i].neighbors;
		while (neighbor != NULL)
		{
			neighbor_num = neighbor->node_num;
			if (neighbor_num > number_of_graph_nodes)
			{
				//outtextxy(0, Ymax-10, " 18 The node_number is greater than number_of_graph_nodes!!!");
				printf("18 The node_number is greater than number_of_graph_nodes!!!\n");
				getch();
				return -1;
			}
			x2 = place[neighbor_num].x;
			y2 = place[neighbor_num].y;
			if (HasPolygonCrossing(x1, y1, x2, y2))
				polygon_crossing = 1;
			neighbor = neighbor->next;
		}
	}

	new_place[i].x = x1;
	new_place[i].y = y1;

	i++;
	i=i%number_of_graph_nodes;
	if (!i)
		 j++;
	if (j==ITERATION)
	{
		 inner_loop_criterion = 1;
		 j = 0;
	}
	return 0;
}
//============================================================================
float Node_distribution(place_t * place)
{
	int i, j;
	float sum, distance2, Dx, Dy;

	sum = 0;
	for (i=0; i< (number_of_graph_nodes-1); i++)
	 for (j=i+1; j<number_of_graph_nodes; j++)
	 {
		if ((graph_nodes_list[i].type == VIRTUAL) ||
				(graph_nodes_list[j].type == VIRTUAL))
			continue;
		Dx = place[i].x - place[j].x;
		Dy = place[i].y - place[j].y;
		distance2  = Dx*Dx + Dy*Dy;
		if (distance2 < G_min*G_min)
		  sum += 1.0/(G_min*G_min);
		else
		  sum += 1.0/distance2;
	 }
	return sum;
}
//===========================================================================
float BorderLines(place_t * place)
{
	int    i, j;
	int    node_num, neighbor_num;
	float  sum;
	int    Ri, Li, Ti, Bi;
	struct polygon_node_t *p1, *p2;
	struct node_t * neighbor;
	int    x1, y1, x2, y2, x3, y3;
	float  distance2, d1, d2, u;

   sum = 0;

   /* Penalty distance of Borders of the polygon */
	for (i=0; i<number_of_graph_nodes; i++)
   {
		if (graph_nodes_list[i].type == VIRTUAL)
			continue;

		x3 = place[i].x;
		y3 = place[i].y;

		p1 = polygon_nodes_list.head;
		if (p1)
			p2 = p1->next;
		else
			p2 = NULL;

		while (p1 && p2)
		{
			x1 = p1->x;
			y1 = p1->y;

			x2 = p2->x;
			y2 = p2->y;

			d1 =(y1-y2)*(x3-x2)-(x1-x2)*(y3-y2);
			if (d1 == 0.0)
			{
				if ((x1==x2)&&(y1==y2))
				{
					distance2 =(x3-x1)*(x3-x1) + (y3-y1)*(y3-y1);
				}
				else
				{
					if (x1!=x2)
						u = (x3-x2)/(x1-x2);
					else
						if (y1!=y2)
							u = (y3-y2)/(y1-y2);

					if ( u > 1.0 )
						distance2 = (x3-x1)*(x3-x1) + (y3-y1)*(y3-y1);
					else
						if (u < 0.0)
							distance2 = (x3-x2)*(x3-x2) + (y3-y2)*(y3-y2);
						else
							distance2 = 0;
				}
			}
			else
			{
				d2 = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
				if (!d2)
				{
					//outtextxy(0, Ymax-10, "ERROR 2 : DIVIDED BY ZERRO!!!!!!!");
					printf("ERROR 2 : DIVIDED BY ZERRO!!!!!!!\n");
					getch();
				}
				distance2 = (d1*d1) / d2;
			}
			if (distance2 < (G_min*G_min))
				sum += 1.0 / (G_min*G_min);
			else
				sum += 1.0 /distance2;

			if (p2 == polygon_nodes_list.head)
				break;
			p1 = p2;
			p2 = p1->next;
			if (p2 == NULL)
			p2 = polygon_nodes_list.head;
		}
	}

	/* penalty distance of corners of the polygon */
	p1 = polygon_nodes_list.head;
	while (p1)
	{
		x3 = p1->x;
		y3 = p1->y;

		for(j=0; j<number_of_graph_nodes; j++)
		{
			if (graph_nodes_list[j].type == VIRTUAL)
				continue;
			node_num = graph_nodes_list[j].element;
			if (node_num > number_of_graph_nodes)
			{
				//outtextxy(0, Ymax-10, " 9 The node_number is greater than number_of_graph_nodes!!!");
				printf("9 The node_number is greater than number_of_graph_nodes!!!\n");
				getch();
				return sum/2;
			}
			neighbor = graph_nodes_list[j].neighbors;
			while (neighbor != NULL)
			{
				neighbor_num = neighbor->node_num;
				if (neighbor_num > number_of_graph_nodes)
				{
					//outtextxy(0, Ymax-10, " 10 The node_number is greater than number_of_graph_nodes!!!");
					printf("10 The node_number is greater than number_of_graph_nodes!!!\n");
					getch();
					return sum/2;
				}

				x1 = place[node_num].x;
				y1 = place[node_num].y;
				x2 = place[neighbor_num].x;
				y2 = place[neighbor_num].y;
				d1 =(y1-y2)*(x3-x2)-(x1-x2)*(y3-y2);
				if (d1 == 0.0)
				{
					if ((x1==x2)&&(y1==y2))
					{
						distance2 =(x3-x1)*(x3-x1) + (y3-y1)*(y3-y1);
					}
					else
					{
						if (x1!=x2)
							u = (x3-x2)/(x1-x2);
						else
							if (y1!=y2)
								u = (y3-y2)/(y1-y2);

						if ( u > 1.0 )
							distance2 = (x3-x1)*(x3-x1) + (y3-y1)*(y3-y1);
						else
							if (u < 0.0)
								distance2 = (x3-x2)*(x3-x2) + (y3-y2)*(y3-y2);
							else
								distance2 = 0;
					}
				}
				else
				{
					d2 = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
					if (!d2)
					{
						//outtextxy(0, Ymax-10, "ERROR 6 : DIVIDED BY ZERRO!!!!!!!");
						printf("ERROR 6 : DIVIDED BY ZERRO!!!!!!!\n");
						getch();
					}
					distance2 = (d1*d1) / d2;
				}
				if (distance2 < (G_min*G_min))
					//sum += MaxPenalty1;
					sum += 1.0 / (G_min*G_min);
				else
					sum += 1.0 /distance2;

				neighbor = neighbor->next;
			}
		}
		p1 = p1->next;
	}

	if (sum < 0)
	{
		//outtextxy(0, Ymax-10, "BorderLines: !!!!!! sum < 0 !!!!!!!!!");
		printf("BorderLines: !!!!!! sum < 0 !!!!!!!!!\n");
		getch();
		return 0;
	}

	return sum;
}
//============================================================================
float EdgeLengths(place_t * place)
{
	int i, j;
	float length2, sum, Dx, Dy;

	sum=0;
	for (i=0; i< (number_of_edges); i++)
		{
			Dx = place[edges[i].node1].x - place[edges[i].node2].x;
			Dy = place[edges[i].node1].y - place[edges[i].node2].y;
			length2  = Dx*Dx + Dy*Dy;
			if(length2< G_min*G_min)
				sum += (G_min*G_min);
			else
				sum += length2;
		}
	return sum;
}
//===========================================================================
float __EdgeLengths(place_t * place)
{
	int i, j;
	int node_num;
	//node_t * next;
	int neighbor_num;
	float sum, edge_len2, Dx, Dy;
	struct node_t * neighbor;

	sum = 0.0;
	for (i=0; i<number_of_graph_nodes; i++)
	{
		node_num = graph_nodes_list[i].element;
		if (node_num > number_of_graph_nodes)
		{
			//outtextxy(0, Ymax-10, " 5 The node_number is greater than number_of_graph_nodes!!!");
			printf("5 The node_number is greater than number_of_graph_nodes!!!\n");
			getch();
			return sum;
		}
		neighbor = graph_nodes_list[i].neighbors;
		while (neighbor != NULL)
		{
			neighbor_num = neighbor->node_num;
			if (neighbor_num > number_of_graph_nodes)
			{
				//outtextxy(0, Ymax-10, " 6 The node_number is greater than number_of_graph_nodes!!!");
				printf("6 The node_number is greater than number_of_graph_nodes!!!\n");
				getch();
				return sum;
			}

			// each edge should be considered in one direction
			if (node_num >= neighbor_num)
			{
				neighbor = neighbor->next;
					continue;
			}

			Dx = place[node_num].x - place[neighbor_num].x;
			Dy = place[node_num].y - place[neighbor_num].y;
			edge_len2 = Dx*Dx + Dy*Dy;
			if ((graph_nodes_list[neighbor_num].type == VIRTUAL) ||
					 (graph_nodes_list[i].type == VIRTUAL))
				sum += edge_len2*50*FACTOR;
			else
				if ((graph_nodes_list[neighbor_num].type == BEND) ||
						(graph_nodes_list[node_num].type == BEND) )
					sum += edge_len2*10*FACTOR;
				else
					sum += edge_len2;

			if (sum < 0)
			{
				//outtextxy(0, Ymax-10, "EdgeLengths: !!!!!! sum < 0 !!!!!!!!!");
				printf("EdgeLengths: !!!!!! sum < 0 !!!!!!!!!\n");
				getch();
				return 0;
			}
			neighbor = neighbor->next;
		}
	}

	if (sum < 0)
	{
		//outtextxy(0, Ymax-10, "EdgeLengths: !!!!!! sum < 0 !!!!!!!!!");
		printf("EdgeLengths: !!!!!! sum < 0 !!!!!!!!!\n");
		getch();
		return 0;
	}

	//return sum/2; NEW
	return sum;
}
//============================================================================
int TotalMeanEdgeLength(place_t * place, float & total, float & mean)
{
	int     i, j;
	int     node_num;
// node_t  * next;
	int     neighbor_num;
	float   edge_len2;
	float     Dx, Dy;
	struct  node_t * neighbor;

	total = 0.0;
	mean  = 0.0;

	for (i=0; i<number_of_graph_nodes; i++)
	{
		node_num = graph_nodes_list[i].element;
		if (node_num > number_of_graph_nodes)
		{
			//outtextxy(0, Ymax-10, " 5 The node_number is greater than number_of_graph_nodes!!!");
			printf("5 The node_number is greater than number_of_graph_nodes!!!\n");
			getch();
			return -1;
		}

		if (graph_nodes_list[node_num].type == VIRTUAL)
			continue;

		neighbor = graph_nodes_list[i].neighbors;
		while (neighbor != NULL)
		{
			neighbor_num = neighbor->node_num;
			if (neighbor_num > number_of_graph_nodes)
			{
			  //outtextxy(0, Ymax-10, " 6 The node_number is greater than number_of_graph_nodes!!!");
			  printf("6 The node_number is greater than number_of_graph_nodes!!!\n");
			  getch();
			  return -1;
			}

			// each edge should be considered in one direction
			// and virtual nodes should not be considered.
			if ((graph_nodes_list[neighbor_num].type == VIRTUAL) ||
				 (node_num >= neighbor_num))
			{
				neighbor = neighbor->next;
				continue;
			}

			Dx = place[node_num].x - place[neighbor_num].x;
			Dy = place[node_num].y - place[neighbor_num].y;
			edge_len2 = Dx*Dx + Dy*Dy;

			total += sqrtl(edge_len2); // I think it's OK

			if (total < 0.0)
			{
				//outtextxy(0, Ymax-10, "EdgeLengths: !!!!!! total < 0 !!!!!!!!!");
				printf("EdgeLengths: !!!!!! sum < 0 !!!!!!!!!\n");
				getch();
				return -1;
			}
			neighbor = neighbor->next;
		}
	}

	if (total < 0)
	{
		//outtextxy(0, Ymax-10, "EdgeLengths: !!!!!! total < 0 !!!!!!!!!");
		printf("EdgeLengths: !!!!!! total < 0 !!!!!!!!!\n");
		getch();
		return -1;
	}

	if (number_of_ordinary_graph_nodes > 1)
		mean =  total / (number_of_ordinary_graph_nodes-1);
	else
		mean = 0;
	return 0;
}
//============================================================================
float NextSegmentLength(place_t * place, int node, int forbidden)
{
	struct node_t * neighbor;
	float segment_len2;
	int Dx, Dy;

	if ((node < 0) || (node >= number_of_graph_nodes))
		return 0;

	if ((forbidden < 0) || (forbidden >= number_of_graph_nodes))
		return 0;

	if (graph_nodes_list[node].type != BEND)
		return 0;

	neighbor = graph_nodes_list[node].neighbors;

	if (!neighbor)
		return 0;

	if (neighbor->node_num == forbidden)
		neighbor = neighbor->next;

	if (!neighbor)
		return 0;

	if (graph_nodes_list[neighbor->node_num].type == VIRTUAL)
		neighbor = neighbor->next;

	if (!neighbor)
		return 0;

	Dx = place[node].x - place[neighbor->node_num].x;
	Dy = place[node].y - place[neighbor->node_num].y;
	segment_len2 = Dx*Dx + Dy*Dy;

	return (sqrt(segment_len2) + NextSegmentLength(place, neighbor->node_num, node));
}
//============================================================================
int MaxDeviationEdgeLength(place_t * place, float mean,
	float & max, float & deviation)
{
	int    i, j;
	int    node_num;
	//node_t * next;
	int    neighbor_num;
	float  edge_len2, edge_len;
	float    Dx, Dy;
	struct node_t * neighbor;

	max = 0.0;
	deviation = 0.0;

	for (i=0; i<number_of_graph_nodes; i++)
	{
		node_num = graph_nodes_list[i].element;
		if (node_num > number_of_graph_nodes)
		{
			//outtextxy(0, Ymax-10, " 5 The node_number is greater than number_of_graph_nodes!!!");
			printf("5 The node_number is greater than number_of_graph_nodes!!!\n");
			getch();
			return -1;
		}

		if (graph_nodes_list[node_num].type != ORDINARY)
			continue;

		neighbor = graph_nodes_list[i].neighbors;
		while (neighbor != NULL)
		{
			neighbor_num = neighbor->node_num;
			if (neighbor_num > number_of_graph_nodes)
			{
				//outtextxy(0, Ymax-10, " 6 The node_number is greater than number_of_graph_nodes!!!");
				printf("6 The node_number is greater than number_of_graph_nodes!!!\n");
				getch();
				return -1;
			}

			// each edge should be considered in one direction
			// and virtual nodes should not be considered.
			if (graph_nodes_list[neighbor_num].type == VIRTUAL)
			{
				neighbor = neighbor->next;
				continue;
			}

			Dx = place[node_num].x - place[neighbor_num].x;
			Dy = place[node_num].y - place[neighbor_num].y;
			edge_len2 = Dx*Dx + Dy*Dy;

			edge_len = sqrt(edge_len2); // I think it's OK

			if ((graph_nodes_list[neighbor_num].type == BEND))
				 //|| (graph_nodes_list[neighbor_num].type == CROSS)
					// we don't consider crosses as nodes in this version
				edge_len += NextSegmentLength(place, neighbor_num, node_num);

			if (edge_len < 0)
			{
				//outtextxy(0, Ymax-10, "EdgeLength: !!!!!! edge_len < 0 !!!!!!!!!");
				printf("EdgeLength: !!!!!! edge_len < 0 !!!!!!!!!\n");
				getch();
				return -1;
			}

			if (edge_len > max)
				max = edge_len;

			deviation += (edge_len - mean)*(edge_len - mean);

			neighbor = neighbor->next;
		}
	}

	if (number_of_ordinary_graph_nodes > 1)
	{
		deviation = deviation / (2*(number_of_ordinary_graph_nodes-1));
		deviation = sqrt(deviation);
	}
	else
		deviation = 0;

	return 0;
}
//============================================================================
int TotalMeanEdgeBend(int & total, float & mean)
{
	int i;

	total = 0;
	for (i=0; i < number_of_graph_nodes; i++)
		if (graph_nodes_list[i].type == BEND)
			total++;

	if (number_of_ordinary_graph_nodes > 1)
		mean = total / (number_of_ordinary_graph_nodes - 1);
	else	mean = 0;

	return 0;
}
//============================================================================
int NextEdgeBends(int node, int forbidden)
{
	struct node_t * neighbor;

	if ((node < 0) || (node >= number_of_graph_nodes))
	return 0;

	if ((forbidden < 0) || (forbidden >= number_of_graph_nodes))
	return 0;

	if (graph_nodes_list[node].type != BEND)
	return 0;

	neighbor = graph_nodes_list[node].neighbors;

	if (!neighbor)
	return 0;

	if (neighbor->node_num == forbidden)
	neighbor = neighbor->next;

	if (!neighbor)
	return 0;

	if (graph_nodes_list[neighbor->node_num].type == VIRTUAL)
	neighbor = neighbor->next;

	if (!neighbor)
	return 0;

	return (1 + NextEdgeBends(neighbor->node_num, node));
}
//============================================================================
int MaxDeviationEdgeBend(float mean,
	int & max, float & deviation)
{
	int    i, j;
	int    node_num;
// node_t * next;
	int    neighbor_num;
	int    bends;
	float  Dx, Dy;
	struct node_t * neighbor;

	max = 0.0;
	deviation = 0.0;

	for (i=0; i<number_of_graph_nodes; i++)
	{
		node_num = graph_nodes_list[i].element;
		if (node_num > number_of_graph_nodes)
		{
			//outtextxy(0, Ymax-10, " 5 The node_number is greater than number_of_graph_nodes!!!");
			printf("5 The node_number is greater than number_of_graph_nodes!!!\n");
			getch();
			return -1;
		}

		if (graph_nodes_list[node_num].type != ORDINARY)
			continue;

		neighbor = graph_nodes_list[i].neighbors;
		while (neighbor != NULL)
		{
			neighbor_num = neighbor->node_num;
			if (neighbor_num > number_of_graph_nodes)
			{
				  //outtextxy(0, Ymax-10, " 6 The node_number is greater than number_of_graph_nodes!!!");
				  printf("6 The node_number is greater than number_of_graph_nodes!!!\n");
				  getch();
				  return -1;
			}

			// virtual nodes should not be considered.
			if (graph_nodes_list[neighbor_num].type == VIRTUAL)
			{
				neighbor = neighbor->next;
					continue;
			}

			bends = 0;

			if ((graph_nodes_list[neighbor_num].type == BEND))
				 //|| (graph_nodes_list[neighbor_num].type == CROSS)
				 // we don't consider crosses as nodes in this version
				bends += NextEdgeBends(neighbor_num, node_num);

			if (bends > max)
				max = bends;

			deviation += (bends - mean)*(bends - mean);

			neighbor = neighbor->next;
		}
	}

	if (number_of_ordinary_graph_nodes > 1)
	{
			deviation = deviation / (2*(number_of_ordinary_graph_nodes-1));
			deviation = sqrt(deviation);
	}
	else
		deviation = 0;

	return 0;
}
//============================================================================
// This function computes the minimum distance between
// nodes and bends, but I should also consider edges and crosses.
int MinDistance(place_t * place, float & min)
{
	int i, j;
	float distance, Dx, Dy;

	min = Xmax;

	// computes minimm distance between nodes and bends
	for (i=0; i< (number_of_graph_nodes-1); i++)
		for (j=i+1; j<number_of_graph_nodes; j++)
		{
			if ((graph_nodes_list[i].type == VIRTUAL) ||
					(graph_nodes_list[j].type == VIRTUAL))
				continue;
			Dx = place[i].x - place[j].x;
			Dy = place[i].y - place[j].y;
			distance = Dx*Dx + Dy*Dy;
			distance = sqrt(distance);
			if (distance < min)
				min = distance;
		}
	return 0;
}
//============================================================================
float ClosenessToSkeleton(place_t * place)
{
	int i, j, k, t;
	//node_t * next;
	//corner_t * p1, *p2;

	int    node_num, neighbor_num;
	int    x1, y1, x2, y2, x3, y3;
	float  distance2, sum, d1, d2, u;
	struct node_t * neighbor;

	sum = 0;

	// Considers Closeness of nodes of graph to edges of skeleton
	for (i=0; i<number_of_graph_nodes; i++)
	{
		x3 = place[i].x;
		y3 = place[i].y;

		if (graph_nodes_list[i].type == VIRTUAL)
			continue;

		for(j=0; j<number_of_skel_nodes; j++)
		{
			node_num = skel_nodes_list[j].element;
			if (node_num > number_of_skel_nodes)
			{
				//outtextxy(0, Ymax-10, " 7 The node_number is greater than number_of_graph_nodes!!!");
				printf("7 The node_number is greater than number_of_graph_nodes!!!\n");
				getch();
				return sum/2;
			}
			if ( node_num != i )
			{
				neighbor = skel_nodes_list[j].neighbors;
				while (neighbor != NULL)
				{
					neighbor_num = neighbor->node_num;
					if (neighbor_num > number_of_skel_nodes)
					{
						//outtextxy(0, Ymax-10, " 8 The node_number is greater than number_of_graph_nodes!!!");
						printf("8 The node_number is greater than number_of_graph_nodes!!!\n");
						getch();
						return sum/2;
					}
					if ( neighbor_num != i )
					{
						x1 = skel_nodes_list[node_num].x;
						y1 = skel_nodes_list[node_num].y;
						x2 = skel_nodes_list[neighbor_num].x;
						y2 = skel_nodes_list[neighbor_num].y;
						d1 =(y1-y2)*(x3-x2)-(x1-x2)*(y3-y2);
						if (d1 == 0.0)
						{
							if ((x1==x2)&&(y1==y2))
							{
								distance2 =(x3-x1)*(x3-x1) + (y3-y1)*(y3-y1);
							}
							else
							{
								if (x1!=x2)
									u = (x3-x2)/(x1-x2);
								else
									if (y1!=y2)
										u = (y3-y2)/(y1-y2);

								if ( u > 1.0 )
									distance2 = (x3-x1)*(x3-x1) + (y3-y1)*(y3-y1);
								else
									if (u < 0.0)
										distance2 = (x3-x2)*(x3-x2) + (y3-y2)*(y3-y2);
									else
										distance2 = 0;
							}
						}
						else
						{
							d2 = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
							if (!d2)
							{
								//outtextxy(0, Ymax-10, "ERROR 1 : DIVIDED BY ZERRO!!!!!!!");
								printf("ERROR 1 : DIVIDED BY ZERRO!!!!!!!\n");
								getch();
							}
							distance2 = (d1*d1) / d2;
						}
						sum += LANDA8*distance2;
						if (sum < 0)
						{
							//outtextxy(0, Ymax-10, "Closeness_distance: !!!!!! sum < 0 !!!!!!!!!");
							printf("Closeness_distance: !!!!!! sum < 0 !!!!!!!!!\n");
							getch();
							return 0;
						}
					}
					neighbor = neighbor->next;
				}
			}
		}
	}

	// Considers Closeness of nodes of skeleton to edges of graph
	for (i=0; i<number_of_skel_nodes; i++)
	{
		x3 = skel_nodes_list[i].x;
		y3 = skel_nodes_list[i].y;

		for(j=0; j<number_of_graph_nodes; j++)
		{
			node_num = graph_nodes_list[j].element;
			if (node_num > number_of_graph_nodes)
			{
				//outtextxy(0, Ymax-10, " 7 The node_number is greater than number_of_graph_nodes!!!");
				printf("7 The node_number is greater than number_of_graph_nodes!!!\n");
				getch();
				return sum/2;
			}
			if ( node_num != i )
			{
				neighbor = graph_nodes_list[j].neighbors;
				while (neighbor != NULL)
				{
					neighbor_num = neighbor->node_num;
					if (neighbor_num > number_of_graph_nodes)
					{
						//outtextxy(0, Ymax-10, " 8 The node_number is greater than number_of_graph_nodes!!!");
						printf("8 The node_number is greater than number_of_graph_nodes!!!\n");
						getch();
						return sum/2;
					}
					if ( neighbor_num != i )
					{
						x1 = place[node_num].x;
						y1 = place[node_num].y;
						x2 = place[neighbor_num].x;
						y2 = place[neighbor_num].y;
						d1 =(y1-y2)*(x3-x2)-(x1-x2)*(y3-y2);
						if (d1 == 0.0)
						{
							if ((x1==x2)&&(y1==y2))
							{
								distance2 =(x3-x1)*(x3-x1) + (y3-y1)*(y3-y1);
							}
							else
							{
								if (x1!=x2)
									u = (x3-x2)/(x1-x2);
								else
									if (y1!=y2)
										 u = (y3-y2)/(y1-y2);

								if ( u > 1.0 )
									distance2 = (x3-x1)*(x3-x1) + (y3-y1)*(y3-y1);
								else
									if (u < 0.0)
										distance2 = (x3-x2)*(x3-x2) + (y3-y2)*(y3-y2);
								else
									distance2 = 0;
							}
						}
						else
						{
							d2 = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
							if (!d2)
							{
								//outtextxy(0, Ymax-10, "ERROR 1 : DIVIDED BY ZERRO!!!!!!!");
								printf("ERROR 1 : DIVIDED BY ZERRO!!!!!!!\n");
								getch();
							}
							distance2 = (d1*d1) / d2;
						}
						sum += LANDA8*distance2;
						if (sum < 0)
						{
							//outtextxy(0, Ymax-10, "Closseness_distance: !!!!!! sum < 0 !!!!!!!!!");
							printf("Closseness_distance: !!!!!! sum < 0 !!!!!!!!!\n");
							getch();
							return 0;
						}
					}
					neighbor = neighbor->next;
				}
			}
		}
	}
	if (sum < 0)
	{
		//outtextxy(0, Ymax-10, "Closeness: !!!!!! sum < 0 !!!!!!!!!");
		printf("Closeness: !!!!!! sum < 0 !!!!!!!!!\n");
		getch();
		return 0;
	}
	return sum/2;
}
//============================================================================
float NodeEdgeDistances(place_t * place)
{
  int i, j, k, t;
  int node_num, neighbor_num;
  float distance2, sum, d1, d2, u;
  node_t * next;
  float x1, y1, x2, y2, x3, y3;
  struct node_t * neighbor;

	sum = 0;
	number_of_pseudo_edge_crossings = 0;
	for (i=0; i<number_of_graph_nodes; i++)
	{
		if (graph_nodes_list[i].type == VIRTUAL)
			continue;

		x3 = place[i].x;
		y3 = place[i].y;

		for(j=0; j<number_of_graph_nodes; j++)
		{
			if (graph_nodes_list[j].type == VIRTUAL)
				continue;
			node_num = graph_nodes_list[j].element;
			if (node_num > number_of_graph_nodes)
			{
				//outtextxy(0, Ymax-10, " 7 The node_number is greater than number_of_graph_nodes!!!");
				printf("7 The node_number is greater than number_of_graph_nodes!!!\n");
				getch();
				return sum/2;
			}
			if ( node_num != i )
			{
				neighbor = graph_nodes_list[j].neighbors;
				while (neighbor != NULL)
				{
					neighbor_num = neighbor->node_num;
					if (graph_nodes_list[neighbor_num].type == VIRTUAL)
					{
						neighbor = neighbor->next;
							continue;
					}
					if (neighbor_num > number_of_graph_nodes)
					{
						 //outtextxy(0, Ymax-10, " 8 The node_number is greater than number_of_graph_nodes!!!");
						 printf("8 The node_number is greater than number_of_graph_nodes!!!\n");
						 getch();
						 return sum/2;
					}
					if ( neighbor_num != i )
					{
						x1 = place[node_num].x;
						y1 = place[node_num].y;
						x2 = place[neighbor_num].x;
						y2 = place[neighbor_num].y;
						d1 =(y1-y2)*(x3-x2)-(x1-x2)*(y3-y2);

						if (x1!=x2)
							u = (x3-x2)/(x1-x2);
						else
							if (y1!=y2)
								u = (y3-y2)/(y1-y2);
						if (d1 == 0.0 || u<0.0 || 1.0<u )
						{
							if ((x1==x2)&&(y1==y2))
								distance2 =(x3-x1)*(x3-x1) + (y3-y1)*(y3-y1);
							else
							{
								if ( u > 1.0 )
									distance2 = (x3-x1)*(x3-x1) + (y3-y1)*(y3-y1);
								else
									if (u < 0.0)
										distance2 = (x3-x2)*(x3-x2) + (y3-y2)*(y3-y2);
									else
										distance2 = 0;
							}//else
						}//if
						else
						{
							d2 = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
							if (!d2) 	printf("ERROR: DIVIDED BY ZERRO!!!!!!!\n");
							distance2 = (d1*d1) / d2;
						}
						if (distance2 < G_min*G_min)
						{
							sum += 1.0/(G_min*G_min); // it was before this LANDA4 !
							number_of_node_edge++;
							number_of_pseudo_edge_crossings++;
						}
						else
						{
							sum += 1.0/distance2;
							number_of_node_edge++;
						}
					}//if
					neighbor = neighbor->next;
				}//while
			}//if
		}//for(j .....)
	}//for(i .....)
	number_of_node_edge/=2 ;
	number_of_pseudo_edge_crossings/=2 ;
	return sum/2; /* Note That each edge repeats two times */
}
//===========================================================================
int EdgeCrossings(place_t * place)
{
	int    i, j, node_numi, node_numj, sum;
	int    neighbor_numi, neighbor_numj;
//	node_t * next1, *next2;
	struct node_t *neighbor1, *neighbor2;
	int    x1, y1, x2, y2, X1, Y1, X2, Y2;
	float  u1, u2, d;

	sum = 0; number_of_edge_crossings = 0;
	for (i=0; i<(number_of_graph_nodes-1); i++)
	{
		node_numi = graph_nodes_list[i].element;
		if (node_numi > number_of_graph_nodes)
		{
			//outtextxy(0, Ymax-10, " 1 The node_number is greater than number_of_graph_nodes!!!");
			printf("1 The node_number is greater than number_of_graph_nodes!!!\n");
			getch();
			return sum;
		}

		if (graph_nodes_list[node_numi].type == VIRTUAL)
			continue;

		for (j=i+1; j<number_of_graph_nodes; j++)
		{
			node_numj = graph_nodes_list[j].element;
			if (node_numj > number_of_graph_nodes)
			{
			  //outtextxy(0, Ymax-10, " 2 The node_number is greater than number_of_graph_nodes!!!");
				printf("2 The node_number is greater than number_of_graph_nodes!!!\n");
				getch();
				return sum;
			}

			if (graph_nodes_list[node_numj].type == VIRTUAL)
				continue;

			if (node_numi == node_numj)
			{
			  clrscr();
			  printf("Error E1: Two Col of the Edge List are equal!\n");
			  return sum;
			}
			neighbor1 = graph_nodes_list[i].neighbors;
			while (neighbor1 != NULL)
			{
				neighbor_numi = neighbor1->node_num;

				if (neighbor_numi > number_of_graph_nodes)
				{
				//outtextxy(0, Ymax-10, " 3 The node_number is greater than number_of_graph_nodes!!!");
					printf("3 The node_number is greater than number_of_graph_nodes!!!\n");
				getch();
				return sum;
				}

				if (graph_nodes_list[neighbor_numi].type == VIRTUAL)
				{
					neighbor1 = neighbor1->next;
					continue;
				}

				if (neighbor_numi == node_numj) // edges should not be incident
				{
					neighbor1 = neighbor1->next;
					continue;
				}

				neighbor2 = graph_nodes_list[j].neighbors;
				while ( neighbor2 != NULL)
				{
					neighbor_numj = neighbor2->node_num;
					if (neighbor_numj > number_of_graph_nodes)
					{
						//outtextxy(0, Ymax-10, " 4 The node_number is greater than number_of_graph_nodes!!!");
						printf("4 The node_number is greater than number_of_graph_nodes!!!\n");
						getch();
						return sum;
					}

					if (graph_nodes_list[neighbor_numj].type == VIRTUAL)
					{
						 neighbor2 = neighbor2->next;
							 continue;
					}

					if (// edges should not be incident
						(neighbor_numj != node_numi) &&
						(neighbor_numj != neighbor_numi) &&
						// NEW each edge should consider in one direction
						(node_numi < neighbor_numi) &&  // each edge consider once
						(node_numj < neighbor_numj))	// each edge consider once
					{
						x1 = place[node_numi].x;
						y1 = place[node_numi].y;
						x2 = place[neighbor_numi].x;
						y2 = place[neighbor_numi].y;
						X1 = place[node_numj].x;
						Y1 = place[node_numj].y;
						X2 = place[neighbor_numj].x;
						Y2 = place[neighbor_numj].y;
						d  = (y1-y2)*(X1-X2) - (x1-x2)*(Y1-Y2);
						if (d)
						{
							u1 = ( (x2 -X2)*(Y1 -Y2) - (y2-Y2)*(X1-X2) ) / d;
							u2 =  ( (X2 -x2)*(y1 -y2) - (Y2-y2)*(x1-x2) ) / (-d);
							if ( ( 0.0 <= u1) && (u1 <= 1.0) &&(0.0 <= u2) &&(u2 <=1.0) )
							{
								sum ++;
								number_of_edge_crossings++;
							}
						}//if(d)
					} //if(.........)
					neighbor2 = neighbor2->next;
				}//while(neighbor2!=NULL)
				neighbor1 = neighbor1->next;
			}//while(neighbor1!=NULL)
		}//for(j .....)
	} //for(i ......)
	number_of_edge_crossings;
	return sum;
}
//===========================================================================
int TotalEdgeCrossings(place_t * place, int & total)
{
	int    i, j, node_numi, node_numj;
	int    neighbor_numi, neighbor_numj;
// node_t * next1, *next2;
	struct node_t *neighbor1, *neighbor2;
	int    x1, y1, x2, y2, X1, Y1, X2, Y2;
	float  u1, u2, d;

	total = 0;
	for (i=0; i<(number_of_graph_nodes-1); i++)
	{
		node_numi = graph_nodes_list[i].element;
		if (node_numi > number_of_graph_nodes)
		{
			//outtextxy(0, Ymax-10, " 1 The node_number is greater than number_of_graph_nodes!!!");
			printf("1 The node_number is greater than number_of_graph_nodes!!!\n");
			getch();
			return -1;
		}

		if (graph_nodes_list[node_numi].type == VIRTUAL)
			continue;

		for (j=i+1; j<number_of_graph_nodes; j++)
		{
			node_numj = graph_nodes_list[j].element;
			if (node_numj > number_of_graph_nodes)
			{
				//outtextxy(0, Ymax-10, " 2 The node_number is greater than number_of_graph_nodes!!!");
				printf("2 The node_number is greater than number_of_graph_nodes!!!\n");
				getch();
				return -1;
			}

			if (graph_nodes_list[node_numj].type == VIRTUAL)
				continue;

			if (node_numi == node_numj)
			{
				//clrscr();
				//outtextxy(0, Ymax-10, "Error E1: Two Col of the Edge List are equal!");
				printf("Error E1: Two Col of the Edge List are equal!\n");
				getch();
				return -1;
			}
			neighbor1 = graph_nodes_list[i].neighbors;
			while (neighbor1 != NULL)
			{
				neighbor_numi = neighbor1->node_num;

				if (neighbor_numi > number_of_graph_nodes)
				{
					//outtextxy(0, Ymax-10, " 3 The node_number is greater than number_of_graph_nodes!!!");
					printf("3 The node_number is greater than number_of_graph_nodes!!!\n");
					getch();
					return -1;
				}

				if (graph_nodes_list[neighbor_numi].type == VIRTUAL)
				{
					neighbor1 = neighbor1->next;
					continue;
				}

				if (neighbor_numi == node_numj) // edges should not be incident
				{
					neighbor1 = neighbor1->next;
					continue;
				}

				neighbor2 = graph_nodes_list[j].neighbors;
				while ( neighbor2 != NULL)
				{
					neighbor_numj = neighbor2->node_num;
					if (neighbor_numj > number_of_graph_nodes)
					{
						//outtextxy(0, Ymax-10, " 4 The node_number is greater than number_of_graph_nodes!!!");
						printf("4 The node_number is greater than number_of_graph_nodes!!!\n");
						getch();
						return -1;
					}

					if (graph_nodes_list[neighbor_numj].type == VIRTUAL)
					{
						neighbor2 = neighbor2->next;
						continue;
					}

					if (// edges should not be incident
						(neighbor_numj != node_numi) &&
						(neighbor_numj != neighbor_numi) &&
						// NEW each edge should consider in one direction
						(node_numi < neighbor_numi) &&  // each edge consider once
						(node_numj < neighbor_numj))	// each edge consider once
					{
						x1 = place[node_numi].x;
						y1 = place[node_numi].y;
						x2 = place[neighbor_numi].x;
						y2 = place[neighbor_numi].y;
						X1 = place[node_numj].x;
						Y1 = place[node_numj].y;
						X2 = place[neighbor_numj].x;
						Y2 = place[neighbor_numj].y;
						d  = (y1-y2)*(X1-X2) - (x1-x2)*(Y1-Y2);
						if (d)
						{
							u1 = ( (x2 -X2)*(Y1 -Y2) - (y2-Y2)*(X1-X2) ) / d;
							u2 =  ( (X2 -x2)*(y1 -y2) - (Y2-y2)*(x1-x2) ) / (-d);
							if ( ( 0.0 <= u1) && (u1 <= 1.0) &&(0.0 <= u2) &&(u2 <=1.0) )
								total++; // I think it's OK
						}
					}
					neighbor2 = neighbor2->next;
				}
				neighbor1 = neighbor1->next;
			}
		}
	}
	total /=2;
	return 0;
}
//============================================================================
int IsInPolygon(int x1, int y1)
{
	int   crossings=0;
	polygon_node_t  *p1, *p2;
	float u1, u2;
	int   X1, Y1, X2, Y2;

	p1 = polygon_nodes_list.head;
	if (p1)
		p2 = p1->next;
	else
		p2 = NULL;

	while ((p1 != NULL) && (p2 != NULL))
	{
		X1 = p1->x;
		Y1 = p1->y;

		X2 = p2->x;
		Y2 = p2->y;

		if (Y2 != Y1)
		{
			u2 = (Y2-y1)/(Y2-Y1);
			u1 = X2-x1 + u2*(X1-X2);
			if (u1 == 0.0)
				return 0;
			if ((0.0 < u1) && (0.0 < u2) &&(u2 < 1.0))
				crossings++;
		}
		if (p2 == polygon_nodes_list.head)
			break;
		p1 = p2;
		p2 = p1->next;
		if (p2 == NULL)
			p2 = polygon_nodes_list.head;
	}
	if (crossings%2)
		return 1;
	else
		return 0;
}
//============================================================================
/*
  The following function check Polygon Crossing of an edge.
  NEW : For my purpose if one of nodes u1, u2 or both of them of
  an edge u1-u2 lies on the polygon it doesn't consider as
  a crossing. It is useful when an edge which its two end-points
  are fixed crosses the polygon and we replace it with a polyline
  some part of which lie on the polygon edges.
*/
int HasPolygonCrossing(int x1, int y1,
			  int x2, int y2)
{
	int k;
	polygon_node_t  *p1, *p2;
	float u1, u2, sum, d;
	int   X1, Y1, X2, Y2;
	int   x_in, y_in, T1, T2, L1, L2, B1, B2, R1, R2, TT1, TT2;

  //   If Initial point be interior point of polygon,
  //   I can ignore being out of polygon checking, because
  //   it is not possible that in the same time all nodes be
  //   out of polygon. Nodes one by one moves so if one node
  //   be out of polygon its edge crosses the polygon.

	p1 = polygon_nodes_list.head;
	if (p1)
		p2 = p1->next;
	else
		p2 = NULL;

	while ((p1 != NULL) && (p2 != NULL))
	{
		X1 = p1->x;
		Y1 = p1->y;

		X2 = p2->x;
		Y2 = p2->y;

		d  = (y1-y2)*(X1-X2) - (x1-x2)*(Y1-Y2);
		if (d)
		{
			u1 = ( (x2 -X2)*(Y1 -Y2) - (y2-Y2)*(X1-X2) ) / d;
			u2 =  ( (X2 -x2)*(y1 -y2) - (Y2-y2)*(x1-x2) ) / (-d);
		//if ((0.0 <= u1) && (u1 <= 1.0) && (0.0 < u2) &&(u2 <= 1.0)) NEW
			if ((0.0 < u1) && (u1 <= 1.0) && (0.0 < u2) &&(u2 <= 1.0))
			{
				return 1;
			}
			else
				if ((0.0 == u1) && (0.0 < u2) &&(u2 <= 1.0))
				{
		// We node x1, y1 choose at random while we
		// know x2, y2 is on the polygon.
		// So we should check if x1, y1 is in polygon edge or not.
					if (!IsInPolygon(x1, y1))
						return 1;
				}
		}
		if (p2 == polygon_nodes_list.head)
			return 0;
		p1 = p2;
		p2 = p1->next;
		if (p2 == NULL)
			p2 = polygon_nodes_list.head;
	}
	return 0;
}
//============================================================================
float MeanSquareError_of_EdgeLength(place_t * place)
{
	int i, j;
	float length,sum,L,error, Dx, Dy;
	sum=0;
	for (i=0; i< (number_of_edges); i++)
		{
			Dx = place[edges[i].node1].x - place[edges[i].node2].x;
			Dy = place[edges[i].node1].y - place[edges[i].node2].y;
			length  = sqrtl(pow(Dx,2) + pow(Dy,2));
			if(length< G_min)
				sum += G_min;
			else
				sum += length;
		}
	L  = sum/number_of_edges;
	sum= 0;
	for (i=0; i< (number_of_edges); i++)
	{
		Dx       = place[edges[i].node1].x - place[edges[i].node2].x;
		Dy       = place[edges[i].node1].y - place[edges[i].node2].y;
		length   = sqrtl(pow(Dx,2) + pow(Dy,2));
		error    = (length-L)/L;
		sum 	  += powl(error,2);
	}
	return sum / number_of_graph_nodes;
}
//===========================================================================
float CumulativeSquareDeviation_of_EdgeAngles(place_t * place)
{
	int i, j , center_node, next_node, neighbors=0;
	float angle_deviation,angle_delta,angle,sum, Dx, Dy;
	float pi=3.141592654,angles[100],tmp;
	node_t * neighbor;

	sum= 0;
	for (i=0; i< (number_of_graph_nodes); i++)
	{
		for(j=0 ; j<100 ;j++)
			angles[j]   =-1;
		neighbors   = 0;
		center_node = graph_nodes_list[i].element;
//		if (graph_nodes_list[i]->neighbor)
			neighbor = graph_nodes_list[i].neighbors;
		while (neighbor)
		{
				next_node         = neighbor->node_num;
				Dx                = place[next_node].x - place[center_node].x ;
				Dy                = place[next_node].y - place[center_node].y ;
				if(Dx!=0)
					angle 	      = atan(Dy/Dx)*360/(2*pi);
				else
					if (Dy!=0)
						angle				= 90.0;
					else
						angle				= 00.0;

				if((Dx>=0)&&(Dy>=0)) angle = angle;
				if((Dx< 0)&&(Dy>=0)) angle = angle + 180.0;
				if((Dx< 0)&&(Dy< 0)) angle = angle + 180.0;
				if((Dx>=0)&&(Dy< 0)) angle = angle + 360.0;
				angles[neighbors] = angle;
				neighbors++;
				neighbor              = neighbor->next;
		}//while
		if(neighbors>=2)   //sortion of angles[]
			for(int k=0 ; k<neighbors-1 ; k++)
				for(j=k+1 ; j<neighbors ;j++)
					if(angles[j] < angles[k])
						{
							tmp       = angles[k];
							angles[k] = angles[j];
							angles[j] = tmp;
						}
		for(j=1 ; j<neighbors ; j++)
		{
				angle_delta     = fabs(angles[j]-angles[j-1]);
				angle_deviation = angle_delta-(360.0 / neighbors);
				sum            += pow(angle_deviation,2);
		}
		angle_delta     = 360.0 - fabs(angles[0]-angles[neighbors-1]);
		angle_deviation = angle_delta-(360.0 / neighbors);
		sum            += pow(angle_deviation,2);
	}//main for
	return sum/number_of_graph_nodes;
}
//===========================================================================
void Perturb(place_t *offspring)
{
	static int i=0, j=0;
	place_t tmp_node;

	if (!number_of_graph_nodes)
		return;
	if (i==j) j++;

	tmp_node     = offspring[i];
	offspring[i] = offspring[j];
	offspring[j] = tmp_node;

	j++;
	j=j%(number_of_graph_nodes-1) ;
	if(j==0)
	{
		i++;j=i+1;
	}
	i=i%number_of_graph_nodes;
}
//============================================================================
int Polygon_Crossing(place_t *place)
{
	int i, j, neighbor;
	point_list_t * path_tmp_ptr, *tmp_path_list;
	node_t * tmp_ptr;
	point_list_t * path_list = NULL, *ptr;
	int node1, node2, crossings=0, number_of_bends=0, bends=0;
	place_t tmp;
	int XX, YY, prv_x, prv_y;
	struct crossing_node_t * tmp_ptr2, * crossing_list = NULL;
	struct crossing_node_t * cross_tmp_ptr1, * cross_tmp_ptr2;

	// for all edges of the graph,
	// check if they cross the boundary of the polygon.
	for (i=0; i< number_of_graph_nodes; i++)
	{
		node1 = graph_nodes_list[i].element;
		tmp_ptr = graph_nodes_list[i].neighbors;
		while(tmp_ptr)
		{
			node2 = tmp_ptr->node_num;
			tmp_ptr = tmp_ptr->next;
			if ((node2 < node1) || (node2 >= number_of_graph_nodes))
					continue;

			crossings = find_polygon_corssings(&place[node1], &place[node2], &crossing_list);
			if (!crossings)
					continue;
			if (crossings%2)
			{
//						 printf("ERROR 1 -** There is an error in crossing list! **\n");
						 free_crossing_list(&crossing_list);
						 continue;
			}

			// sorting is required for grouping
			// crossing points 2-by-2.
			if (place[node1].y == place[node2].y)
				 sort_on_x(crossing_list, crossings);
			else
				sort_on_y(crossing_list, crossings);
			tmp_ptr2 = crossing_list;
			while (tmp_ptr2)
			{
				if (tmp_ptr2->next == NULL)
				{
//					 printf("ERROR 2 -## There is an error in crossing list! ##\n");
						 goto end;
				}
				tmp_path_list = NULL;
				find_path(tmp_ptr2, tmp_ptr2->next, &tmp_path_list,
									&place[node1], &place[node2]);
				if (path_list == NULL)
						path_list = tmp_path_list;
				else
				{
					 path_tmp_ptr = path_list;
					 while (path_tmp_ptr->next)
						 path_tmp_ptr = path_tmp_ptr->next;
					 path_tmp_ptr->next = tmp_path_list;
				}
				tmp_ptr2 = tmp_ptr2->next->next;
			}
			bends=0;
			if(path_list)
			{
				ptr   = path_list;
				bends++;
				prv_x = ptr->x;
				prv_y = ptr->y;
				while(ptr->next)
				{
					ptr=ptr->next;
					if(!(ptr->x == prv_x && ptr->y == prv_y))
					{
						prv_x=ptr->x;
						prv_y=ptr->y;
						bends++;
					}
				}
				if(bends-crossings>number_of_polygon_nodes/2)
					bends= number_of_polygon_nodes-(bends-crossings)+crossings;
				number_of_bends += bends;
			}
end:		free_path_list(&path_list);
			free_crossing_list(&crossing_list);
		} //while
	} // for
	return number_of_bends;
}
//===========================================================================
/* The Cost Function */
float Cost2(place_t *place)
{
	float tmp;

	tmp  = float(LANDA1)*Node_distribution(place); //OK LANDA1=1000 & expo/1000.0
	tmp += float(LANDA2)*BorderLines(place); //OK LADNDA2=1000 & expo/1000.0

	tmp += float(LANDA3)*__EdgeLengths(place); // OK LANDA3 = 0.0005 & expo/1000.0
	tmp += float(LANDA4)*EdgeCrossings(place);// OK LANDA4 = LANDA5/(G_min*G_min), G_min = 4
	tmp += float(LANDA5)*NodeEdgeDistances(place); // OK LANDA5 = 4000
	return tmp;
}
//============================================================================
/* The Cost Function */
float Evaluation4(place_t *place)
{
	float tmp=0;
	float tmp2=0;

	tmp  = float(LANDA3)*EdgeLengths(place);
	tmp += float(LANDA4)*EdgeCrossings(place);
	tmp += float(LANDA5)*NodeEdgeDistances(place);
	tmp += float(LANDA4)*number_of_pseudo_edge_crossings;
	tmp += float(LANDA6)*CumulativeSquareDeviation_of_EdgeAngles(place);
	tmp += float(LANDA7)*MeanSquareError_of_EdgeLength(place);
	tmp2 = float(LANDA8)*Polygon_Crossing(place);
	return tmp;
}
//===========================================================================
void Swap(point_t x, point_t y)
{
	point_t tmp;

	tmp = x;
	x   = y;
	y   = tmp;
}
//===========================================================================
void Swap(place_t **place, place_t **new_place)
{
	place_t  *tmp_ptr;

	tmp_ptr    = *place;
	*place     = *new_place;
	*new_place = tmp_ptr;
}
//===========================================================================
void Fitness_Sortion2(float Fitness[],place_t *place[],int size)
{
	int ofsp_num=size;
	float F_tmp;
	place_t *tmp_ptr;

	for(int i=0 ; i<size; i++)
		if(Fitness[i]==0)
		{
			ofsp_num=int(i);
			break;
		}
	for (i=0 ; i<ofsp_num-1 ; i++)
		for (int j=i+1 ; j<ofsp_num ; j++)
			if (Fitness[i] >= Fitness[j])
			{
				F_tmp        = Fitness[i];
				Fitness[i]   = Fitness[j];
				Fitness[j]   = F_tmp;
				tmp_ptr      = place[i];
				place[i]     = place[j];
				place[j]     = tmp_ptr;
			}
}
//===========================================================================
void LinearRanking_Fitness_assingment(float Fitness[],float Selection_prob[])
{
	float SP=Selective_pressure,sum=0,tmp[Pop_size];
	for(int i=0 ; i<Pop_size ; i++)
		tmp[i]=Fitness[i];
	for(i=0 ; i<Pop_size ; i++)
	{
		tmp[i]= 2 - SP + 2*(SP-1.0)*(Pop_size-1-i)/(Pop_size-1);
		sum  += tmp[i];
	}
	for(i=0 ; i<Pop_size ; i++)
		Selection_prob[i] = tmp[i]/sum;
}
//===========================================================================
void LinearNormalization_Fitness_assingment(float Fitness[],float Selection_prob[])
{
	float sum=0,tmp[Pop_size];
	for(int i=0 ; i<Pop_size ; i++)
		tmp[i]=Fitness[i];
	for(i=0 ; i<Pop_size ; i++)
	{
		tmp[i]= Pop_size - i;
		sum += tmp[i];
	}
	for(i=0 ; i<Pop_size ; i++)
		Selection_prob[i] = tmp[i]/sum;
}
//===========================================================================
void RoulleteWheele_Selection(float Selection_prob[],int number_of_selection,int Selected_parent[])
{
	float Prob_distribution[Pop_size], sum=0, tmp;
	int i,j,tmp2;
	for(i=0 ; i<Pop_size ; i++)
	{
		sum 	+= Selection_prob[i] ;
		Prob_distribution[i] = sum ;
	}
	for(i=0 ; i<number_of_selection ; i++)
	{
		tmp=rand() / float(RAND_MAX); // create a float number between [0.0 , 1.0]
		for(j=0 ; j<Pop_size ; j++)
			if(tmp <= Prob_distribution[j])
			{
				Selected_parent[i] = j;
				break;
			}
	}
	for(i=0 ; i<number_of_selection ; i+=2)
		if( Selected_parent[i] == Selected_parent[i+1])
			for(j=i+2; j<number_of_selection ; j++)
				if (Selected_parent[i+1] != Selected_parent[j])
				{
					tmp2=Selected_parent[i+1];
					Selected_parent[i+1]=Selected_parent[j];
					Selected_parent[j]=tmp2;
					break;
				}
	for(i=number_of_selection-1 ; i>=0 ; i-=2)
		if( Selected_parent[i] == Selected_parent[i-1])
			for(j=i-2; j<=0 ; j--)
				if (Selected_parent[i-1] != Selected_parent[j])
				{
					tmp2=Selected_parent[i-1];
					Selected_parent[i-1]=Selected_parent[j];
					Selected_parent[j]=tmp2;
					break;
				}
	for(i=number_of_selection ; i<2*Pop_size ; i++)
		Selected_parent[i]=-1;
}
//===========================================================================
void StochasticUniversalSampling_Selection(float Selection_prob[],int number_of_selection,int Selected_parent[])
{
	float Prob_distribution[Pop_size], sum=0, tmp=0;
	float ptr_distance=0, start_pointer=0;
	int i,j,tmp2,Rand_limit=0;
	for(i=0 ; i<Pop_size ; i++)
	{
		sum 	+= Selection_prob[i] ;
		Prob_distribution[i] = sum ;
	}
	ptr_distance= 1.0 /float(number_of_selection);
	if (0.1<=ptr_distance && ptr_distance<1.0)
	{ 	Rand_limit= int(ptr_distance*10000.0); start_pointer= random(Rand_limit) /10000.0;}
	if (0.01<=ptr_distance  && ptr_distance<0.1)
	{  Rand_limit= int(ptr_distance*100000.0);start_pointer= random(Rand_limit) /100000.0;}
	if (0.001<=ptr_distance && ptr_distance<0.01)
	{	Rand_limit= int(ptr_distance*1000000.0);start_pointer= random(Rand_limit) /1000000.0;}

	tmp=start_pointer;
	for(i=0 ; i<number_of_selection ; i++)
	{
		for(j=0 ; j<Pop_size ; j++)
			if(tmp <= Prob_distribution[j])
			{
				Selected_parent[i] = j;
				break;
			}
		tmp+=ptr_distance;
	}
	for(i=0 ; i<number_of_selection ; i+=2)
		if( Selected_parent[i] == Selected_parent[i+1])
			for(j=i+2; j<number_of_selection ; j++)
				if (Selected_parent[i+1] != Selected_parent[j])
				{
					tmp2=Selected_parent[i+1];
					Selected_parent[i+1]=Selected_parent[j];
					Selected_parent[j]=tmp2;
					break;
				}
	for(i=number_of_selection-1 ; i>=0 ; i-=2)
		if( Selected_parent[i] == Selected_parent[i-1])
			for(j=i-2; j<=0 ; j--)
				if (Selected_parent[i-1] != Selected_parent[j])
				{
					tmp2=Selected_parent[i-1];
					Selected_parent[i-1]=Selected_parent[j];
					Selected_parent[j]=tmp2;
					break;
				}
	for(i=number_of_selection ; i<2*Pop_size ; i++)
		Selected_parent[i]=-1;
}
//===========================================================================
void Uniform_Crossover(place_t *parent0,place_t *parent1,place_t *offspring0,place_t *offspring1)
{
	short int sample0[MAX_NODES],sample1[MAX_NODES];
	for (int i=0 ; i<number_of_graph_nodes; i++)
	{
		sample0[i]=random(2);
		sample1[i]=random(2);
	}
	for (i=0 ; i<number_of_graph_nodes; i++)
	{  //sample0[i]++;
		if(sample0[i]==0)
			offspring0[i]=parent0[i];
		else
			offspring0[i]=parent1[i];
		if(sample1[i]==1)
			offspring1[i]=parent1[i];
		else
			offspring1[i]=parent0[i];
	}
}
//===========================================================================
void SinglePoint_Crossover(place_t *parent0,place_t *parent1,place_t *offspring0,place_t *offspring1)
{
	short int CrossPoint=0, found=0, i, j;
	place_t tmp_node;

	CrossPoint=random(number_of_graph_nodes-1);
	for(i=0 ; i<number_of_graph_nodes ; i++)
	{
		offspring0[i]=parent0[i];
		offspring1[i]=parent1[i];
		j++;
	}
	for(i=CrossPoint+1 ; i<number_of_graph_nodes ; i++)
	{
		if( (parent0[i].x!=parent1[i].x) || (parent0[i].y!=parent1[i].y) )
		{
			j=-1;
			found=0;
			while(!found)
			{
				j++;
				if((parent0[j].x==parent1[i].x)&&(parent0[j].y==parent1[i].y))
					found=1;
			}
			tmp_node      = offspring0[i];
			offspring0[i] = offspring0[j];
			offspring0[j] = tmp_node;
			j=-1;
			found=0;
			while(!found)
			{
				j++;
				if((parent0[i].x==parent1[j].x)&&(parent0[i].y==parent1[j].y))
					found=1;
			}
			tmp_node      = offspring1[i];
			offspring1[i] = offspring1[j];
			offspring1[j] = tmp_node;
		}
		else
			offspring0[i]=offspring1[i]=parent1[i];
	}
}
//===========================================================================
void MultiPoint_Crossover(place_t *parent0,place_t *parent1,place_t *offspring0,place_t *offspring1)
{
	short int points[CrossPoints],p_tmp,flage;
	points[0]=random(number_of_graph_nodes-1);
	for (int i=1 ; i<CrossPoints ; i++)  //Generate m random crosspoints
	{
L:		points[i]=random(number_of_graph_nodes-1);
		for (int j=0; j<i ; j++)
			if(points[i]==points[j])
				goto L;
	}
	for (i=0 ; i<(CrossPoints)-1 ; i++)   //sort ascending the crosspoints
		for (int j=i+1 ; j< CrossPoints ; j++)
			if (points[i] > points[j])
			{
				p_tmp       = points[i];
				points[i]   = points[j];
				points[j]   = p_tmp;
			}
	flage=0;
	for(i=0 ; i<=points[0] ; i++)
	{  offspring0[i]=parent0[i]; offspring1[i]=parent1[i];}
	flage=1;
	for(i=1 ; i< CrossPoints ; i++)
	{
		if (flage==0)
			for(int j=points[i-1]+1 ; j<=points[i] ; j++)
			{  offspring0[j]=parent0[j]; offspring1[j]=parent1[j];}
		else
			for(int j=points[i-1]+1 ; j<=points[i] ; j++)
			{  offspring0[j]=parent1[j]; offspring1[j]=parent0[j];}
		if(flage==0) flage=1;
		else         flage=0;
	 }
	 if(flage==0)
		for(i=points[CrossPoints-1]+1 ; i<number_of_graph_nodes ; i++)
		{  offspring0[i]=parent0[i]; offspring1[i]=parent1[i];}
	 else
		for(i=points[CrossPoints-1]+1 ; i<number_of_graph_nodes ; i++)
		{	offspring0[i]=parent1[i]; offspring1[i]=parent0[i];}
}
//===========================================================================
void Mutation(place_t *offspring,int variance)
{
	int  X,Y;
	float deviation,mean,Z,r0,r1;
	deviation=sqrtl(variance);
//	for(int i=0 ; i<number_of_graph_nodes ; i++)
	int i=random(number_of_graph_nodes);
	{
		mean = offspring[i].x;
		r0   = (random(1000)+1)/1000.0;
		r1   = (random(1000)+1)/1000.0;
		Z    = sqrtl(-2.0*log(r0))*cos(360.0*r1);
		X    = int(Z*deviation + mean);	offspring[i].x=X;

		mean = offspring[i].y;
		r0   = (random(1000)+1)/1000.0;
		r1   = (random(1000)+1)/1000.0;
		Z    = sqrtl(-2.0*log(r0))*cos(360.0*r1);
		Y    = int(Z*deviation + mean);	offspring[i].y=Y;

		if(offspring[i].x<Left)
			offspring[i].x=Left;
		if(offspring[i].x>Right)
			offspring[i].x=Right;
		if(offspring[i].y<Top)
			offspring[i].y=Top;
		if(offspring[i].y>Button)
			offspring[i].y=Button;
	}
}
//===========================================================================
void Single_Mutation(place_t *offspring)
{
	int Rand_node,Rand_x,Rand_y;
	Rand_node = random(number_of_graph_nodes);

	offspring[Rand_node].x = Rand_x;
	offspring[Rand_node].y = Rand_y;
}
//===========================================================================
void Small_Mutation(place_t *offspring)
{
	int Rand_node1,Rand_node2;
	point_t tmp;
	Rand_node1 = random(number_of_graph_nodes);
	Rand_node2 = random(number_of_graph_nodes);
	tmp                   = offspring[Rand_node1];
	offspring[Rand_node1] = offspring[Rand_node2];
	offspring[Rand_node2] = tmp;
}
//===========================================================================
void LeftRotation_Mutation(place_t *offspring)
{
	int i;
	place_t tmp_node;

	tmp_node = offspring[0];
	for(i=0 ; i<number_of_graph_nodes-1 ; i++)
		offspring[i] = offspring[i+1];
	offspring[number_of_graph_nodes-1] = tmp_node;
}
//===========================================================================
void RightRotation_Mutation(place_t *offspring)
{
	int i;
	place_t tmp_node;

	tmp_node = offspring[number_of_graph_nodes-1];
	for(i=number_of_graph_nodes-1 ; i>0 ; i--)
		offspring[i] = offspring[i-1];
	offspring[0] = tmp_node;
}
//===========================================================================
void Shuffle_Mutation(place_t *offspring)
{
	int i, n = number_of_graph_nodes;
	place_t tmp_ofsp[MAX_NODES];
	place_t tmp_node;

	for(i=0 ; i<number_of_graph_nodes ; i++)
		tmp_ofsp[i] = offspring[i];
	if(n%2==1)
		n++;
	for(i=0 ; i<n/2 ; i++)
		offspring[i] = tmp_ofsp[2*i];
	if(number_of_graph_nodes%2==1)
		for(i=n/2 ; i<n-1 ; i++)
			offspring[i] = tmp_ofsp[2*i-n+1];
	else
		for(i=n/2 ; i<n ; i++)
			offspring[i] = tmp_ofsp[2*i-n+1];

}
//===========================================================================
void Unshuffle_Mutation(place_t *offspring)
{
	int i, n = number_of_graph_nodes;
	place_t tmp_ofsp[MAX_NODES];
	place_t tmp_node;

	for(i=0 ; i<number_of_graph_nodes ; i++)
		tmp_ofsp[i] = offspring[i];
	if(n%2==1)
		n++;
	for(i=0 ; i<n/2 ; i++)
		offspring[2*i] = tmp_ofsp[i];
	if(number_of_graph_nodes%2==1)
		for(i=0 ; i<n/2-1 ; i++)
			offspring[2*i+1] = tmp_ofsp[i+n/2];
	else
		for(i=0 ; i<n/2 ; i++)
			offspring[2*i+1] = tmp_ofsp[i+n/2];
}
//===========================================================================
void Edge_Mutation1(place_t *offspring)
{
	int selected_edge,node1,node2;
	selected_edge=random(number_of_edges);
	node1=edges[selected_edge].node1;
	node2=edges[selected_edge].node2;
	offspring[node1].x=random(Right);
	offspring[node1].y=random(Button);
	offspring[node2].x=random(Right);
	offspring[node2].y=random(Button);
}
//===========================================================================
void Edge_Mutation2(place_t *offspring,int Radius)
{
	int selected_edge,node1,node2,delta_x,delta_y;
	selected_edge=random(number_of_edges);
	node1=edges[selected_edge].node1;
	node2=edges[selected_edge].node2;
	delta_x=random(2*Radius)-(Radius);
	delta_y=random(2*Radius)-(Radius);

	offspring[node1].x += delta_x;
	offspring[node1].y += delta_y;
	offspring[node2].x += delta_x;
	offspring[node2].y += delta_y;
}
//===========================================================================
void TwoEdgeReverse_Mutation(place_t *offspring)
{
	int selected_edge1,selected_edge2;
	int node1_1,node1_2,node2_1,node2_2;
	place_t tmp_node;

	selected_edge1=random(number_of_edges);
	node1_1 = edges[selected_edge1].node1;
	node1_2 = edges[selected_edge1].node2;
L4:selected_edge2=random(number_of_edges);
	if(number_of_edges>=2)
		if (selected_edge2==selected_edge1)
			goto L4;
	node2_1=edges[selected_edge2].node1;
	node2_2=edges[selected_edge2].node2;

	tmp_node           = offspring[node1_1];
	offspring[node1_1] = offspring[node1_2];
	offspring[node1_2] = tmp_node;

	tmp_node           = offspring[node2_1];
	offspring[node2_1] = offspring[node2_2];
	offspring[node2_2] = tmp_node;
}
//===========================================================================
void OneEdgeReverse_Mutation(place_t *offspring)
{
	int selected_edge,node1,node2;
	place_t tmp_node;

	selected_edge=random(number_of_edges);
	node1=edges[selected_edge].node1;
	node2=edges[selected_edge].node2;

	tmp_node         = offspring[node1];
	offspring[node1] = offspring[node2];
	offspring[node2] = tmp_node;
}
//===========================================================================
void Elitism_Insertion(place_t *parents[],float Prnt_Fit[],
								place_t *offsprings[],float Ofsp_Fit[],
								place_t *newGeneration[],float New_Fit[])
{
	int i=0, j=0, k=0 ;
	while( (i<Pop_size-Sel_Num) && (j<Sel_Num) && (k<Pop_size))
	{
		if(Prnt_Fit[i] >= Ofsp_Fit[j])
			{
				for(int m=0 ; m < number_of_graph_nodes ; m++)
					newGeneration[k][m]=parents[i][m];
				New_Fit[k] = Prnt_Fit[i];
				i++;
			}
		else
			{
				for(int m=0 ; m < number_of_graph_nodes ; m++)
					newGeneration[k][m]=offsprings[j][m];
				New_Fit[k] = Ofsp_Fit[j];
				j++;
			}
		k++;
	}
	while ( (k<Pop_size) && (i<Pop_size-Sel_Num) )
	{
		for(int m=0 ; m < number_of_graph_nodes ; m++)
			newGeneration[k][m]=parents[i][m];
		New_Fit[k] = Prnt_Fit[i];
		i++;   k++;
	}
	while ( (k<Pop_size) && (j<Sel_Num) )
	{
		for(int m=0 ; m < number_of_graph_nodes ; m++)
			newGeneration[k][m]=offsprings[j][m];
		New_Fit[k] = Ofsp_Fit[j];
		j++;  k++;
	}
}
//===========================================================================
void Elitism_Insertion2(place_t *parents[],float Prnt_Fit[],
								place_t *offsprings[],float Ofsp_Fit[],
								place_t *newGeneration[],float New_Fit[])
{
	int i=0, j=0, k=0 ;
	while( (i<Pop_size-Sel_Num) && (j<Sel_Num) && (k<Pop_size))
	{
		if(Prnt_Fit[i] < Ofsp_Fit[j])
			{
				for(int m=0 ; m < number_of_graph_nodes ; m++)
					newGeneration[k][m]=parents[i][m];
				New_Fit[k] = Prnt_Fit[i];
				i++;
			}
		else
			{
				for(int m=0 ; m < number_of_graph_nodes ; m++)
					newGeneration[k][m]=offsprings[j][m];
				New_Fit[k] = Ofsp_Fit[j];
				j++;
			}
		k++;
	}
	while ( (k<Pop_size) && (i<Pop_size-Sel_Num) )
	{
		for(int m=0 ; m < number_of_graph_nodes ; m++)
			newGeneration[k][m]=parents[i][m];
		New_Fit[k] = Prnt_Fit[i];
		i++;   k++;
	}
	while ( (k<Pop_size) && (j<Sel_Num) )
	{
		for(int m=0 ; m < number_of_graph_nodes ; m++)
			newGeneration[k][m]=offsprings[j][m];
		New_Fit[k] = Ofsp_Fit[j];
		j++;  k++;
	}
}
//===========================================================================
void FitnessBased_Insertion(place_t *parents[],float Prnt_Fit[],
								place_t *offsprings[],float Ofsp_Fit[],
								place_t *newGeneration[],float New_Fit[])
{
	int i=0,j=0,k=0;
	while((k<Pop_size))
	{
		if(Prnt_Fit[i] > Ofsp_Fit[j])
			{
				for(int m=0 ; m < number_of_graph_nodes ; m++)
					newGeneration[k][m] = parents[i][m];
				New_Fit[k]       = Prnt_Fit[i];
				i++;
			}
		else
			{
				for(int m=0 ; m < number_of_graph_nodes ; m++)
					newGeneration[k][m] = offsprings[j][m];
				New_Fit[k]       = Ofsp_Fit[j];
				j++;
			}
		k++;
	}
}
//===========================================================================
void FitnessBased_Insertion2(place_t *parents[],float Prnt_Fit[],
								place_t *offsprings[],float Ofsp_Fit[],
								place_t *newGeneration[],float New_Fit[])
{
	int i=0,j=0,k=0;
	while((k<Pop_size))
	{
		if(Prnt_Fit[i] < Ofsp_Fit[j])
			{
				for(int m=0 ; m < number_of_graph_nodes ; m++)
					newGeneration[k][m] = parents[i][m];
				New_Fit[k]       = Prnt_Fit[i];
				i++;
			}
		else
			{
				for(int m=0 ; m < number_of_graph_nodes ; m++)
					newGeneration[k][m] = offsprings[j][m];
				New_Fit[k]       = Ofsp_Fit[j];
				j++;
			}
		k++;
	}
}
//===========================================================================
int InitGraph()
{
  int gdriver = DETECT, gmode, errorcode;
  int x, y;
  initgraph(&gdriver, &gmode, "C:\\borlandc\\bgi");
  errorcode = graphresult();
  if (errorcode != grOk)  /* an error occurred */
  {
	 printf("Graphics error: %s\n", grapherrormsg(errorcode));
	 return -1;
  }
  Xmax = getmaxx();
  Ymax = getmaxy();
  return 0;
}

//===========================================================================
void CloseGraph()
{
  closegraph();
}
//===========================================================================
void DrawPolygon(struct polygon_node_list_t * polygon)
{
	struct polygon_node_t *p1, *p2;

	p1 = polygon->head;
	if (p1)
		p2 = p1->next;
	else
		p2 = NULL;

	while (p1 && p2)
	{
		line(p1->x, p1->y, p2->x, p2->y);
		if (p2 == polygon->head)
			break;
		p1 = p2;
		p2 = p2->next;
		if (p2 == NULL)
			p2 = polygon->head;
	}
}
//============================================================================
void Draw_BestGraph(place_t * place[])
{
	int i, k, index1, index2;
//	struct node_t * next;
	struct node_t *neighbor, *neighbor2;;
	char tmp_str[80];
	float tmp, EL, ND, NED, BL, SD_EA, SD_EL;
	int EC, BND;

	cleardevice();
	setcolor(BLUE);
	DrawPolygon(&polygon_nodes_list);

  //Draw Graph Nodes

	static int j=0;
	for (i=0; i<number_of_graph_nodes; i++)
	{
		if (graph_nodes_list[i].type == BEND)
			continue;
		// Does not draw virtual nodes
		setcolor(YELLOW);
		fillellipse(place[j][i].x, place[j][i].y, 2, 2);
		sprintf(tmp_str, "%d", i);
		outtextxy(place[j][i].x+8, place[j][i].y, tmp_str);
	}

	//Draw graph Edges
	setcolor(LIGHTRED);
	for (i=0; i<number_of_graph_nodes; i++)
	{
		neighbor = graph_nodes_list[i].neighbors;
		index1 = graph_nodes_list[i].element;
		while(neighbor != NULL)
		{
			index2 = neighbor->node_num;
			line(place[j][index1].x, place[j][index1].y, place[j][index2].x, place[j][index2].y);
			neighbor = neighbor->next;
		}
	}

					setcolor(YELLOW);
//					ND=Node_distribution(place[0]);
//					sprintf(tmp_str, "N.D.    =%f",ND);
//					outtextxy(480, 25, tmp_str);
					BND=Polygon_Crossing(place[0]);
					sprintf(tmp_str, "BND     =%d",BND);
					outtextxy(480, 40, tmp_str);
					EL=EdgeLengths(place[0]);
					sprintf(tmp_str, "E.L.    =%8.1f",EL);
					outtextxy(480, 55, tmp_str);
					EC=EdgeCrossings(place[0]);
					sprintf(tmp_str, "E.C.    =%d", EC);
					outtextxy(480, 70, tmp_str);
					NED=NodeEdgeDistances(place[0]);
					sprintf(tmp_str, "N.E.D.  =%f",NED);
					outtextxy(480, 85, tmp_str);
					sprintf(tmp_str, "P.E.C.  = %d", number_of_pseudo_edge_crossings);
					outtextxy(480, 100, tmp_str);
					SD_EA=CumulativeSquareDeviation_of_EdgeAngles(place[0]);
					sprintf(tmp_str, "S.D.E.A.=%f",SD_EA);
					outtextxy(480, 115, tmp_str);
					SD_EL=MeanSquareError_of_EdgeLength(place[0]);
					sprintf(tmp_str, "S.D.E.L.=%f",SD_EL);
					outtextxy(480, 130, tmp_str);

					setcolor(GREEN);
//					sprintf(tmp_str, "L1      =%f",LANDA1);
//					outtextxy(480, 145, tmp_str);
					sprintf(tmp_str, "L3      =%f",LANDA3);
					outtextxy(480, 160, tmp_str);
					sprintf(tmp_str, "L4      =%f",LANDA4);
					outtextxy(480, 175, tmp_str);
					sprintf(tmp_str, "L5      =%f",LANDA5);
					outtextxy(480, 190, tmp_str);
					sprintf(tmp_str, "L6      =%f",LANDA6);
					outtextxy(480, 205, tmp_str);
					sprintf(tmp_str, "L7      =%f",LANDA7);
					outtextxy(480, 220, tmp_str);
					sprintf(tmp_str, "L8      =%f",LANDA8);
					outtextxy(480, 235, tmp_str);

					setcolor(YELLOW);
//					sprintf(tmp_str, "L1*ND   =%f",ND*LANDA1);
//					outtextxy(480, 250, tmp_str);
					sprintf(tmp_str, "L2*BND  =%f",BND*LANDA8);
					outtextxy(480, 265, tmp_str);
					sprintf(tmp_str, "L3*EL   =%f",EL*LANDA3);
					outtextxy(480, 280, tmp_str);
					sprintf(tmp_str, "L4*EC   =%f",EC*LANDA4);
					outtextxy(480, 295, tmp_str);
					sprintf(tmp_str, "L5*NED  =%f",NED*LANDA5);
					outtextxy(480, 310, tmp_str);
					sprintf(tmp_str, "L6*SDEA =%f",SD_EA*LANDA6);
					outtextxy(480, 325, tmp_str);
					sprintf(tmp_str, "L7*SDEL =%f",SD_EL*LANDA7);
					outtextxy(480, 340, tmp_str);

					setcolor(CYAN);
					tmp= EL*LANDA3 +EC*LANDA4 +NED*LANDA5
							+SD_EA*LANDA6 +SD_EL*LANDA7 +BND*LANDA8 ;
					sprintf(tmp_str, "Cost    =%f",tmp);
					outtextxy(480, 355, tmp_str);
					setcolor(GREEN);
					sprintf(tmp_str, "Pop_size =%d",Pop_size);
					outtextxy(480, 380, tmp_str);
					sprintf(tmp_str, "Sel_Num  =%d",Sel_Num);
					outtextxy(480, 395, tmp_str);
					sprintf(tmp_str, "CrossRate=%4.2f",Crossover_Rate);
					outtextxy(480, 410, tmp_str);
					sprintf(tmp_str, "Mut Rate =%4.2f",Mutation_Rate);
					outtextxy(480, 425, tmp_str);


}
//===========================================================================
/*
void Draw_AllGraph(place_t *place[])
{
	int i,j, index1, index2, row_number, col_number ;
	struct node_t * next;
	char tmp_str[5];
	int delta_x[Pop_size], delta_y[Pop_size];
	float scale;

	cleardevice();
	scale= 1 / sqrtl(Pop_size*3/4);
	row_number=sqrtl(Pop_size*3/4);
	col_number=sqrtl(Pop_size*3/4)*4/3;
		for (int r=0; r<=row_number-1 ; r++)
			for (int c=0; c<=col_number-1 ; c++)
			{
				delta_y[r*col_number+c]    = (Ymax/row_number)*r;
				delta_x[r*col_number+c]   = (Xmax/col_number)*c;
//				Right[r*col_number+c]  = (Xmax/col_number)*(c+1);
//				Button[r*col_number+c] = (Ymax/row_number)*(r+1);
			}

	for (j=0 ; j<Pop_size ;j++)
		for (i=0; i<number_of_graph_nodes; i++)
		{
			 setcolor(YELLOW);
			 if(Pop_size==12)
				 circle(place[j][i].x*scale+delta_x[j], place[j][i].y*scale+delta_y[j], 2);
			 else
				 circle(place[j][i].x*scale+delta_x[j], place[j][i].y*scale+delta_y[j], 1);
//			 sprintf(tmp_str, "%d", i);
//			 setcolor(YELLOW);
//			 outtextxy(place[j][i].x+8, place[j][i].y, tmp_str);
		}
	  setcolor(WHITE);
	  for (j=1; j<Pop_size ;j++)
		  for (i=0; i<number_of_graph_nodes; i++)
			 if (adjacenty_list[i])
			 {
				next = adjacenty_list[i]->next;
				index1 = adjacenty_list[i]->node_num;
				while(next)
				{
					index2 = next->node_num;
					line(place[j][index1].x*scale+delta_x[j], place[j][index1].y*scale+delta_y[j], place[j][index2].x*scale+delta_x[j], place[j][index2].y*scale+delta_y[j]);
					next = next->next;
				}
			 }
//===========

		 setcolor(LIGHTRED);
		j=0;
		for (i=0; i<number_of_graph_nodes; i++)
			 if (adjacenty_list[i])
			 {
				next = adjacenty_list[i]->next;
				index1 = adjacenty_list[i]->node_num;
				while(next)
				{
					index2 = next->node_num;
					line(place[j][index1].x*scale+delta_x[j], place[j][index1].y*scale+delta_y[j], place[j][index2].x*scale+delta_x[j], place[j][index2].y*scale+delta_y[j]);
					next = next->next;
				}
			 }
} */
//###########################################################################
/* Main */
void main(int argc, char * argv[])
{
	float Prnt_Fit[Pop_size], Ofsp_Fit[2*Pop_size],NewPop_Fit[Pop_size];
	place_t * Parents[Pop_size], * Offsprings[2*Pop_size], * NewPop[Pop_size];
	struct polygon_node_t * poly_tmp_ptr1, * poly_tmp_ptr2;
	float Selection_prob[Pop_size];
	int   Selected_parents[2*Pop_size];
	float Radius,Cross_variance, Mut_variance;
	int i,k,w,key,ch, total_bends;
	FILE * output_file;
	char * output_name = "sa_alg.txt";
	char tmp_str[80];
	struct  dostime_t t1, t2;
	long int init_time=0, run_time=0;
	float total_edge_length=0, max_edge_length   = 0, edge_length_deviation=0,
			mean_edge_length =0, edge_bend_deviation=0, mean_edge_bend    =   0,
			min_distance     =0;
	int total_edge_crossing=0, total_edge_bend=0, max_edge_bend=0;

	argc=4;
	argv[0]="sa";
	argv[1]="SA";
	argv[2]="sa_alg.txt";
	argv[3]="RG.txt";

	clrscr();
	if (argc < 4)
	{
		printf("Usage: sa_gd <msg> <output-file> <input-file1> ... <input-file-n> \n");
		return;
	}

	if ((output_file = fopen(argv[2], "w")) == NULL)
	{
		printf("Error : I can not create the output file!");
		return;
	}

	for (k = 3; k < argc ; k++)
	{
		printf("%s started ... \n", argv[k]); //NEW
		polygon_nodes_list.head = polygon_nodes_list.tail = NULL;
		if (Give_Inputs(argv[k])) ;
//			goto main_return2;
		#ifdef DEBUG
			Print(&polygon_nodes_list);
			Print(skel_nodes_list, number_of_skel_nodes);
			Print(graph_nodes_list, number_of_graph_nodes);
		#endif

//		if (!number_of_graph_nodes)
//			goto return2;

		randomize();
		for(i=0 ; i<Pop_size ; i++)
		{
			Prnt_Fit[i]    		    	  = 0;
			Ofsp_Fit[i]        			  = 0;
			Ofsp_Fit[i+Pop_size]			  = 0;
			NewPop_Fit[i]      			  = 0;
			Selection_prob[i]   			  = 0;
			Selected_parents[i]			  =-1;
			Selected_parents[i+Pop_size] =-1;
		}
		BFS();

		_dos_gettime(&t1);
		for(w=0;w<Pop_size;w++)
		{
			Parents[w]             = (place_t *)malloc(number_of_graph_nodes*sizeof(place_t));
			Offsprings[w]          = (place_t *)malloc(number_of_graph_nodes*sizeof(place_t));
			Offsprings[w+Pop_size] = (place_t *)malloc(number_of_graph_nodes*sizeof(place_t));
			NewPop[w]              = (place_t *)malloc(number_of_graph_nodes*sizeof(place_t));
			Init_Placement(Parents[w]);
			Prnt_Fit[w] = Evaluation4(Parents[w]);
		}
		Fitness_Sortion2(Prnt_Fit,Parents,sizeof(Prnt_Fit)/4);

		_dos_gettime(&t2);
//	   hundredth of secod
		init_time = (long int)(t2.hour    - t1.hour)*360000 +
						(long int)(t2.minute  - t1.minute)*6000 +
						(long int)(t2.second  - t1.second)*100  +
						(long int)(t2.hsecond - t1.hsecond);

		InitGraph();
				if(SHOW_TYPE==2)
				{
					Draw_BestGraph(Parents);
					setcolor(LIGHTRED);
					sprintf(tmp_str, "Generation= %d", 0);
					outtextxy(480, 10, tmp_str);
				}
				else
				{
					setcolor(YELLOW);
					sprintf(tmp_str, "%s", "Ofsp");
					outtextxy(5, 10, tmp_str);
//					Draw_AllGraph(Parents);
					setcolor(YELLOW);
					sprintf(tmp_str, "%d", 0);
					outtextxy(5, 10, tmp_str);
				}
		outtextxy(0, Ymax-10, "Initial Drawing, press any key to continue...");
		getch();

		_dos_gettime(&t1);
		for(int g=0 ; g<GENERATION ; g++)
		{
			if (kbhit())
			{
				ch = getch();
				if (ch == ESC_CODE)
					break;
			}
//			LinearNormalization_Fitness_assingment (Prnt_Fit,Selection_prob);
			LinearRanking_Fitness_assingment       (Prnt_Fit,Selection_prob);

			RoulleteWheele_Selection             (Selection_prob,Sel_Num,Selected_parents);
//			StochasticUniversalSampling_Selection(Selection_prob,Sel_Num,Selected_parents);


			Radius        = (float)(( Right/2     -50)*(GENERATION-g)/(GENERATION)+50);
			Cross_variance= (float)((CrossVariance-50)*(GENERATION-g)/(GENERATION)+50);
			Mut_variance  = (float)((MutVariance  -50)*(GENERATION-g)/(GENERATION)+50);
			//===========CroosOver============
			for(i=0 ; i<Sel_Num ; i+=2)
			{
				if((float(random(100))/100.0) > Crossover_Rate)
				{
					for(int j=0 ; j<number_of_graph_nodes ; j++)
					{
						Offsprings[i  ][j]=Parents[Selected_parents[i  ]][j];
						Offsprings[i+1][j]=Parents[Selected_parents[i+1]][j];
					}
				}
				else
//					Uniform_Crossover     (Parents[Selected_parents[i]],Parents[Selected_parents[i+1]],Offsprings[i],Offsprings[i+1]);
					SinglePoint_Crossover (Parents[Selected_parents[i]],Parents[Selected_parents[i+1]],Offsprings[i],Offsprings[i+1]);
//					MultiPoint_Crossover  (Parents[Selected_parents[i]],Parents[Selected_parents[i+1]],Offsprings[i],Offsprings[i+1]);
			}
			//=========Mutation=================
			for(i=0 ; i<Sel_Num ; i+=1)
			{
				if((float(random(100))/100.0) <= Mutation_Rate)
//					Perturb           (Offsprings[i]);
//					Mutation          (Offsprings[i],Mut_variance);
//					Single_Mutation   (Offsprings[i]);
//					if(g%100==0)
					{
//						Shuffle_Mutation      (Offsprings[i]);
//						RightRotation_Mutation(Offsprings[i]);
//						Unshuffle_Mutation    (Offsprings[i]);
//						LeftRotation_Mutation (Offsprings[i+1]);
//						TwoEdgeReverse_Mutation(Offsprings[i]);
						Small_Mutation    (Offsprings[i]);
//						OneEdgeReverse_Mutation(Offsprings[i]);
					}
			}

			for(i=0 ; i<Sel_Num  ; i++)
				Ofsp_Fit[i] = Cost2(Offsprings[i]);

			Fitness_Sortion2(Ofsp_Fit,Offsprings,sizeof(Ofsp_Fit)/4);

			if(Sel_Num < Pop_size)
				Elitism_Insertion2(Parents,Prnt_Fit,Offsprings,Ofsp_Fit,NewPop,NewPop_Fit);
			else
				FitnessBased_Insertion2(Parents,Prnt_Fit,Offsprings,Ofsp_Fit,NewPop,NewPop_Fit);

			for(i=0 ; i<Pop_size  ; i++)
			{
				for(int j=0 ; j<number_of_graph_nodes ; j++)
					Parents[i][j] = NewPop[i][j];
				Prnt_Fit[i] = NewPop_Fit[i];
			}
			if(kbhit())
			{	if(27==getch())
					Final_Placement4(Parents[0]);
					Draw_BestGraph(Parents);
					setcolor(LIGHTRED);
					sprintf(tmp_str, "Generation= %d", g);
					outtextxy(480, 10, tmp_str);
					break;
			}
			if(g%50==0 && g!=0)
			{
				if(SHOW_TYPE==2)
				{
					Draw_BestGraph(Parents);
					setcolor(LIGHTRED);
					sprintf(tmp_str, "Generation= %d", g);
					outtextxy(480, 10, tmp_str);
				}
				else
				{
					setcolor(YELLOW);
					sprintf(tmp_str, "%s", "Ofsp");
					outtextxy(5, 10, tmp_str);
//					Draw_AllGraph(Parents);
					setcolor(YELLOW);
					sprintf(tmp_str, "%d", g);
					outtextxy(5, 10, tmp_str);
				}
			}
		}//for(GENERATION)

		main_return1:

		_dos_gettime(&t2);
// 	hundredth of secod
		run_time = (long int)(t2.hour-t1.hour)*360000 +
		(long int)(t2.minute - t1.minute)*6000 +
		(long int)(t2.second - t1.second)*100 +
		(long int)(t2.hsecond - t1.hsecond);

/*		if (TotalEdgeCrossings(Parents[0], total_edge_crossing) ||
				TotalMeanEdgeLength(Parents[0], total_edge_length, mean_edge_length) ||
				MaxDeviationEdgeLength(Parents[0], mean_edge_length,
				max_edge_length, edge_length_deviation) ||
				TotalMeanEdgeBend(total_edge_bend, mean_edge_bend) ||
				MaxDeviationEdgeBend(mean_edge_bend,
				max_edge_bend, edge_bend_deviation) ||
				MinDistance(Parents[0], min_distance))
		{
			fprintf(output_file, "Error in computation %s Algorithm for graph %s!\n",
			argv[1], argv[k]);
		}
		else
		{
			fprintf(output_file, "Result of %s Algorithm for graph %s : \
					init_time= %ld , run_time= %ld , total_crossings= %d , \
					total_edge_len= %f , max_edge_len= %f , edge_len_deviation= %f , \
					mean_edge_len= %f , total_bends= %d , max_bends= %d , \
					bends_deviation= %f , mean_bends= %f , min_dist= %f \n",
					argv[1], argv[k],
					init_time, run_time, total_edge_crossing,
					total_edge_length, max_edge_length, edge_length_deviation,
					mean_edge_length, total_edge_bend, max_edge_bend,
					edge_bend_deviation, mean_edge_bend, min_distance);
		} */
		fflush(output_file);
//		printf("%s finished. \n", argv[k]); //NEW

		main_return2:
		for (i=0; i<number_of_graph_nodes; i++)
			RemoveAllNode(graph_nodes_list[i].neighbors);
		free(graph_nodes_list);
		graph_nodes_list = NULL;
		number_of_graph_nodes = 0;

		for (i=0; i<number_of_skel_nodes; i++)
			RemoveAllNode(skel_nodes_list[i].neighbors);
		free(skel_nodes_list);
		skel_nodes_list = NULL;
		number_of_skel_nodes = 0;

		poly_tmp_ptr1 = polygon_nodes_list.head;
		while(poly_tmp_ptr1)
		{
			poly_tmp_ptr2 = poly_tmp_ptr1;
			poly_tmp_ptr1 = poly_tmp_ptr1->next;
			free(poly_tmp_ptr2);
		}
		polygon_nodes_list.head = NULL;


		setcolor(YELLOW);
		outtextxy(0, Ymax-10, "press any key to quit...");
		getch();
		CloseGraph();
		free(Parents);
		free(Offsprings);
		free(NewPop);
//		fprintf(output_file, "The SA Algorithm result: %s  cost= %f , crossings= %d \n", argv[k],	cost, number_of_edge_crossings);
//		printf("%s finished.\n", argv[k]);
		fflush(output_file);

	}	//main for(k)
	fclose(output_file);
} /* Main */