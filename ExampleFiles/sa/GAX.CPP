#include <math.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <values.h>
#include <string.h>
#include <graphics.h>
#include <dos.h>

/* Constants */
#define	END_LINE	-1
#define MAX_NODES	      	100
#define MAX_EDGES	      	300

								/* Parameters */
//============================== Genetic Algorithm============================
#define Crossover_Rate		0.75
#define Mutation_rate		0.1
#define GENERATION	      5000
#define Pop_size   	      48 //12|48|108|192|300|...  are the best for presentation on the screen

#define Selective_pressure 2.0 //between (1.0 , 2.0] for Linear Ranking Fitness assingment
#define a                  1.34//between (1.0 , 2.0) for Linear Transformation Fitness assingment
#define b                 -0.1 //between (-1.0 ,0.1)   F(i)= a*F(i)+b ;

#define CrossPoints        4   //Number of cross point in Multipoint crossover
#define Square_edge        160 //The edge length of the square in Rect_Crossover
#define Neighbors_num      5   //The number of neighbors in neighborhood crossover
#define CrossVariance		100 // variance of cross over in ConvexHull crossover
#define MutVariance  		10000 // variance of Miutation
#define Mut_Sqr_edge  		48  // (480/10) The length of the square edge in Edge_Mutation2
#define Mut_Radious  		120 // (480/4)The Radious of Mutation in Edge_Mutation2
#define G_min	4
//====================factors of Evaluation1==================================
#define K_ND	0.075        //(K_ND +K_EL +K_EC +K_NED +K_BL = 1.00 )
#define K_EL	0.05         // These factors are for Evaluation1 function
#define K_EC	0.75
#define K_NED	0.075
#define K_BL   0.05
//====================factors of Evaluation2==================================
#define Alfa   0.02         //for Edge Length Deviation
#define Beta   0.8          //for Edge crossing        //These factors are for Evaluation2
#define Gama   0.18         //for Angle Deviation      // Alfa+Beta+Gama=1.00
//====================factors of Evaluation3==================================
#define 	MNDS      2.00     // Minimum Node Distance Sum
#define 	ELD      -2.00		 // Edge Length Deviation
#define 	ELD_MND  -1.00     // Edge Length Deviation / (Minimum Node Distance)
#define 	NoN_MND2  0.25     // Number of Nodes * pow(Minimum Node Distance,2)
#define 	EC_GS2   -1.00		 // Edge Crossing * pow(Grid Size,2)

struct node_t
{
	int	node_num;
	node_t *next;
};

struct	point_t
{
	float	x,y;
};
struct	edge_t
{
	int node1,node2;
};
typedef struct point_t place_t;
place_t *place[Pop_size], *new_place[Pop_size];
edge_t  edges[MAX_EDGES];

/* Global Variables */
struct node_t * * adjacenty_list;
int number_of_nodes = 0;
int number_of_edges = 0;
int number_of_node_edge = 0;
int number_of_edge_crossings = 0;
float Xmax, Ymax;
//float MaxPenalty1, MaxPenalty2, MaxPenalty3;
float Right[Pop_size], Left[Pop_size], Top[Pop_size], Button[Pop_size];


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
void BFS(int v,int neighbors[])
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
}
//===========================================================================
void BFS()
{
	int visited[MAX_NODES],k=0,v=0;
	Queue q;
	node_t *tmp_ptr;

	for (int i=0; i<MAX_NODES ; i++)
		visited[i]=0;
	number_of_nodes=0;
	q.Add(v);
	while(!q.IsEmpty())
	{
		v=q.Delete();
		if(!visited[v])
				number_of_nodes++;
		visited[v]=1;
		tmp_ptr=adjacenty_list[v];
		while( tmp_ptr->next )    //go to end of edge list
		{
			tmp_ptr = tmp_ptr->next;
			if(!visited[tmp_ptr->node_num])
			{
				q.Add(tmp_ptr->node_num);
			  //	visited[tmp_ptr->node_num]=1;
				edges[k].node1=v;
				edges[k].node2=tmp_ptr->node_num;
				k++;
			}
		}
	}
	delete [] visited;
	number_of_edges=k;
}
//===========================================================================
/* Adds a node to the begin of the list */
int AddTo(node_t * * edge_list, int node_num)
{
	node_t * tmp_ptr;

	tmp_ptr = *(edge_list);
	if (tmp_ptr==NULL)   //edge list is empty
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
}

//===========================================================================
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


//===========================================================================
int Give_Inputs(char * input)
{
  FILE	*input_file;
  char	choice, input_name[100];
  int	node_num, i;

  number_of_nodes = 0;
  //clrscr();
  //printf("Please choice one of the following input modes:\n\n");
  //printf("1. Input from a text file (Adjacenty List)\n");
  //printf("2. Input from standard input (Adjacenty List)\n\n");
  //printf("What do you choice, 1 or 2 ? ");
  //scanf("%c", &choice);
  choice='1'; //temporary

  if (choice == '1')
  {
	//clrscr();
	//printf("Please Enter Input File Name : ");
	//scanf("%s", input_name);
	strcpy(input_name, input);
	if ((input_file = fopen(input_name, "rb")) == NULL)
	{
		clrscr();
		printf("Error : I can not read the input file!");
		return -2;
	}

	adjacenty_list = (node_t * *)(malloc(MAX_NODES*sizeof(long)));
	for (i=0; i<MAX_NODES; i++)
	  adjacenty_list[i] = NULL;

	while (!feof(input_file))
	{
	  do
	  {
		fscanf(input_file, "%d", &node_num);
		if (node_num != END_LINE)
		{
		  AddTo((node_t * *)&adjacenty_list[number_of_nodes], node_num);
		  number_of_edges++;
		}
	  } while (node_num != END_LINE);
	  number_of_nodes++;
	  number_of_edges--;
	}
	number_of_edges /=2;
	fclose(input_file);
	return 0;
  }
  else	if (choice == '2')
  {
	printf("Not supported yet!\n");
	return -1;
  }
  else
  {
	printf("You must choice 1 or 2.");
	return -1;
  }

}

//===========================================================================
void Init_Placement(place_t *place)
{
  for (int i=0; i < number_of_nodes ; i++)
  {
	 place[i].x = random(Right[0]);
	 place[i].y = random(Button[0]);
  }
}
//===========================================================================
float MinimumNodeDistance_Sum(place_t * place)
{
	int i, j;
	float sum,distance,min_dist, Dx, Dy;

	sum = 0;
	for (i=0; i< (number_of_nodes-1); i++)
	{
		min_dist=sqrt(Right[0]*Right[0]+Button[0]*Button[0]);
		for (j=i+1; j<number_of_nodes; j++)
		{
			Dx = place[i].x - place[j].x;
			Dy = place[i].y - place[j].y;
			distance  = sqrt(Dx*Dx + Dy*Dy);
			if(distance < min_dist)
				min_dist=distance;
		}
		if (min_dist < G_min)
			sum += G_min;
		else
			sum += min_dist;
	}
	return sum;
}

//===========================================================================
float MinimumNodeDistance(place_t * place)
{
	int i, j;
	float distance,min_dist, Dx, Dy;

	min_dist=sqrt(Right[0]*Right[0]+Button[0]*Button[0]);
	for (i=0; i< (number_of_nodes-1); i++)
		for (j=i+1; j<number_of_nodes; j++)
		{
			Dx = place[i].x - place[j].x;
			Dy = place[i].y - place[j].y;
			distance  = sqrt(Dx*Dx + Dy*Dy);
			if(distance < min_dist)
				min_dist=distance;
		}
	if(min_dist< G_min)
		return G_min;
	else
		return min_dist;

}
//===========================================================================
float MinimumEdgeLength(place_t * place)
{
	int i, j;
	float length,min_length, Dx, Dy;

	min_length=sqrt(Right[0]*Right[0]+Button[0]*Button[0]);
	for (i=0; i< (number_of_edges); i++)
		{
			Dx = place[edges[i].node1].x - place[edges[i].node2].x;
			Dy = place[edges[i].node1].y - place[edges[i].node2].y;
			length  = sqrt(Dx*Dx + Dy*Dy);
			if(length < min_length)
				min_length=length;
		}
	if(min_length< G_min)
		return G_min;
	else
		return min_length;

}
//===========================================================================
float EdgeLengths(place_t * place)
{
	int i, j;
	float length,sum, Dx, Dy;

	sum=0;
	for (i=0; i< (number_of_edges); i++)
		{
			Dx = place[edges[i].node1].x - place[edges[i].node2].x;
			Dy = place[edges[i].node1].y - place[edges[i].node2].y;
			length  = sqrt(Dx*Dx + Dy*Dy);
			if(length< G_min)
				sum += G_min;
			else
				sum += length;
		}
	return sum;
}
//===========================================================================
float EdgeLengthDeviation(place_t * place,float Optimal)
{
	int i, j;
	float length,sum,deviation, Dx, Dy;

	sum=0;
	for (i=0; i< (number_of_edges); i++)
		{
			Dx = place[edges[i].node1].x - place[edges[i].node2].x;
			Dy = place[edges[i].node1].y - place[edges[i].node2].y;
			length  = sqrt(Dx*Dx + Dy*Dy);
			deviation=fabs(length-Optimal);
			if(deviation< G_min)
				sum += G_min;
			else
				sum += deviation;
		}
	return sum;
}
//===========================================================================
float MeanSquareError_of_EdgeLength(place_t * place)
{
	int i, j;
	float length,sum,L,error, Dx, Dy;

	L  = Right[0]/sqrt(number_of_nodes);
	sum= 0;
	for (i=0; i< (number_of_edges); i++)
	{
		Dx       = place[edges[i].node1].x - place[edges[i].node2].x;
		Dy       = place[edges[i].node1].y - place[edges[i].node2].y;
		length   = sqrt(Dx*Dx + Dy*Dy);
		error    = (length-L)/L;
		sum 	  += pow(error,2);
	}
	return sum/number_of_nodes;
}
//===========================================================================
float CumulativeSquareDeviation_of_EdgeAngles(place_t * place)
{
	int i, j , center_node, next_node, neighbors=0;
	float angle_deviation,angle_delta,angle,sum, Dx, Dy;
	float pi=3.141592654,angles[100],tmp;
	node_t * next;

	sum= 0;
	for (i=0; i< (number_of_nodes); i++)
	{
		for(j=0 ; j<100 ;j++)
			angles[j]   =-1;
		neighbors   = 0;
		center_node = adjacenty_list[i]->node_num;
		next        = adjacenty_list[i]->next;
		while (next)
		{
				next_node         = next->node_num;
				Dx                = place[next_node].x - place[center_node].x ;
				Dy                = place[next_node].y - place[center_node].y ;
				angle             = atan(Dy/Dx)*360/(2*pi);
				if((Dx>=0)&&(Dy>=0)) angle = angle;
				if((Dx< 0)&&(Dy>=0)) angle = angle + 180.0;
				if((Dx< 0)&&(Dy< 0)) angle = angle + 180.0;
				if((Dx>=0)&&(Dy< 0)) angle = angle + 360.0;
				angles[neighbors] = angle;
				neighbors++;
				next              = next->next;
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
	return sum;
}
//===========================================================================
float Node_distribution(place_t * place)
{
	int i, j;
	float sum, distance, Dx, Dy;

	sum = 0;
	for (i=0; i< (number_of_nodes-1); i++)
	 for (j=i+1; j<number_of_nodes; j++)
	 {
		Dx = place[i].x - place[j].x;
		Dy = place[i].y - place[j].y;
		distance  = sqrt(Dx*Dx + Dy*Dy);
		if (distance < G_min)
		  sum += G_min;
		else
		  sum += distance;
	 }
	return sum;
}
//===========================================================================
float BorderLines(place_t * place)
{
	int i, j;
	float sum, Ri, Li, Ti, Bi;

	sum = 0;
	for (i=0; i<number_of_nodes; i++)
	{
		Ri = (Right[0]   - place[i].x);
		Li = (place[i].x - Left[0]);
		Ti = (place[i].y - Top[0]);
		Bi = (Button[0]  - place[i].y);
		if (Ri < G_min)  sum += G_min;
		else 		  		  sum += Ri;
		if (Li < G_min)  sum += G_min;
		else             sum += Li;
		if (Ti < G_min)  sum += G_min;
		else  			  sum += Ti;
		if (Bi < G_min)  sum += G_min;
		else				  sum += Bi;
	}
	return sum;
}

//===========================================================================
/*float EdgeLengths(place_t * place)
{
	int i, j;
	float Dx, Dy;
	float sum, edge_len2;
	int node_num;
	node_t * next;
	int neighbor_num;

	sum = 0;
	for (i=0; i<number_of_nodes; i++)
	if (adjacenty_list[i])
	{
		node_num = adjacenty_list[i]->node_num;
		next = adjacenty_list[i]->next;
		while (next)
		{
			neighbor_num = next->node_num;
			Dx = place[node_num].x - place[neighbor_num].x;
			Dy = place[node_num].y - place[neighbor_num].y;
			edge_len2 = Dx*Dx + Dy*Dy;
			sum += sqrt(edge_len2);
			next = next->next;
		}
	}
	return sum/2; // Note that each edge repeats two times
}
	 */
//===========================================================================
float NodeEdgeDistances(place_t * place)
{
  int i, j;
  int node_num, neighbor_num;
  float distance2, sum, d1, d2, u;
  node_t * next;
  //int x1, y1, x2, y2, x3, y3;
  float x1, y1, x2, y2, x3, y3;

  sum = 0;
  for (i=0; i<number_of_nodes; i++)
	for(j=0; j<number_of_nodes; j++)
	if (adjacenty_list[j])
	{
	  node_num = adjacenty_list[j]->node_num;
	  if ( node_num != i )
	  {
		 next = adjacenty_list[j]->next;
		 while (next)
		 {
			neighbor_num = next->node_num;
			if ( neighbor_num != i )
			{
				x1 = place[node_num].x;
				y1 = place[node_num].y;
				x2 = place[neighbor_num].x;
				y2 = place[neighbor_num].y;
				x3 = place[i].x;
				y3 = place[i].y;
				d1 =(y1-y2)*(x3-x2)-(x1-x2)*(y3-y2);
				if (d1 == 0.0)
				{
				  if ((x1==x2)&&(y1==y2))
					distance2 =(x3-x1)*(x3-x1) + (y3-y1)*(y3-y1);
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
				  }//else
				}//if
				else
				{
					d2 = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
					if (!d2) printf("ERROR: DIVIDED BY ZERRO!!!!!!!\n");
					distance2 = (d1*d1) / d2;
				}
				if (sqrt(distance2) < G_min)
				{
					sum += G_min; // it was before this LANDA4 !
					number_of_node_edge++;
				}
				else
				{
					sum += sqrt(distance2);
					number_of_node_edge++;
				}
			}//if
			next = next->next;
		 }//while
	  }//if
	}//if
	number_of_node_edge /= 2;
	return sum/2; /* Note That each edge repeats two times */
}

//===========================================================================
int EdgeCrossings(place_t * place)
{
  int i, j, node_numi, node_numj;
/*  int neighbor_numi, neighbor_numj;
  node_t * next1, *next2;
  float x1, y1, x2, y2, X1, Y1, X2, Y2;
  float u1, u2, sum, d;

  number_of_edge_crossings = 0;
  for (i=0; i<(number_of_nodes-1); i++)
	  for (j=i+1; j<number_of_nodes; j++)
	  {
		 if (adjacenty_list[i])
		 {
			node_numi = adjacenty_list[i]->node_num;
			if (adjacenty_list[j])
			{
				node_numj = adjacenty_list[j]->node_num;
				if (node_numi == node_numj)
				{
					clrscr();
					printf("Error E1: Two Col of the Edge List are equal!\n");
					return 0;
				}
				next1 = adjacenty_list[i]->next;
				while (next1)
				{
					neighbor_numi = next1->node_num;
					next2 = adjacenty_list[j]->next;
					if (neighbor_numi != node_numj)
						while ( next2)
						{
							neighbor_numj = next2->node_num;
							if ( (neighbor_numj != node_numi) &&
									(neighbor_numj != neighbor_numi))
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
									//	sum += LANDA4;
										number_of_edge_crossings++;
									}//if
								}//if
							}//if
							next2 = next2->next;
						}//while
						next1 = next1->next;
				}//while
			}//if
		 }//if
	 }//for
//	 return number_of_edge_crossings/2;  /* Note That each edge repeats two times */
//##############################################*/
	float E1N1_x,E1N1_y,E1N2_x,E1N2_y,E2N1_x,E2N1_y,E2N2_x,E2N2_y;
	float E1L,E1R,E1T,E1B,E2L,E2R,E2T,E2B;
	float a1,a2,b1,b2,CrsPt_x,CrsPt_y;
	number_of_edge_crossings=0;
	for(i=0; i<number_of_edges; i++)
	{
		E1N1_x=place[edges[i].node1].x; 	E1N1_y=place[edges[i].node1].y;
		E1N2_x=place[edges[i].node2].x; 	E1N2_y=place[edges[i].node2].y;
		if ( E1N1_x < E1N2_x)  	{E1L=E1N1_x ; E1R=E1N2_x;}
		else                   	{E1L=E1N2_x ; E1R=E1N1_x;}
		if ( E1N1_y < E1N2_y)  	{E1T=E1N1_y ; E1B=E1N2_y;}
		else                   	{E1T=E1N2_y ; E1B=E1N1_y;}
		for(j=0 ; j<number_of_edges ; j++)
		{
			if( (j!=i) && (edges[i].node1!=edges[j].node1) && (edges[i].node1!=edges[j].node2)
						  && (edges[i].node2!=edges[j].node1) && (edges[i].node2!=edges[j].node2) )
			{
				E2N1_x=place[edges[j].node1].x; 	E2N1_y=place[edges[j].node1].y;
				E2N2_x=place[edges[j].node2].x; 	E2N2_y=place[edges[j].node2].y;

				if ( E2N1_x < E2N2_x)  	{E2L=E2N1_x ; E2R=E2N2_x;}
				else                   	{E2L=E2N2_x ; E2R=E2N1_x;}
				if ( E2N1_y < E2N2_y)  	{E2T=E2N1_y ; E2B=E2N2_y;}
				else                   	{E2T=E2N2_y ; E2B=E2N1_y;}

				a1=(E1N2_y - E1N1_y) / (E1N2_x - E1N1_x);
				a2=(E2N2_y - E2N1_y) / (E2N2_x - E2N1_x);
				b1= -a1*(E1N1_x) + E1N1_y;
				b2= -a2*(E2N1_x) + E2N1_y;
				if( (a1-a2)!=0)
				{
					CrsPt_x=(b2-b1) / (a1-a2);
					CrsPt_y=a1 * CrsPt_x + b1 ;
					if ( (E1L<=CrsPt_x) && (CrsPt_x<=E1R) && (E1T<=CrsPt_y) && (CrsPt_y<=E1B)
						&&(E2L<=CrsPt_x) && (CrsPt_x<=E2R) && (E2T<=CrsPt_y) && (CrsPt_y<=E2B))
							number_of_edge_crossings++;
				}//if
			}//if
		}//for 2
	}// main for
	return number_of_edge_crossings;

}

//===========================================================================
/* The Cost Function */
float Cost(place_t *place)
{
  float tmp;

  tmp =  Node_distribution(place); //OK LANDA1=1000 & expo/1000.0
  tmp += BorderLines(place); //OK LADNDA2=1000 & expo/1000.0
  tmp += EdgeLengths(place);
  tmp += EdgeCrossings(place);
  tmp += NodeEdgeDistances(place);
  return tmp;
}
//===========================================================================
/* The Fitness Function */
float Evaluation1(place_t *place)
{
	float tmp,Max_BorderLines,Max_Length;
	float Max_Node_distribution,Max_EdgeLength;
	float Max_EdgeCrossing,Max_NodeEgdeDistances;
	float F_ND,F_EC,F_NED,F_EL,F_BL,Fitness;

	tmp = NodeEdgeDistances(place);
	Max_Length=sqrt((Right[0])*(Right[0]) + (Button[0])*(Button[0]));

	Max_Node_distribution= number_of_nodes*(number_of_nodes-1)*Max_Length;
	Max_EdgeLength       = number_of_edges *  Max_Length;
//	Max_EdgeCrossing     = (number_of_edges) * (number_of_edges-1) / 4;
	Max_NodeEgdeDistances= number_of_node_edge * Max_Length;
	Max_BorderLines      = (Right[0]+Button[0])*number_of_nodes;
						  //((Xmax/2)+(Xmax/2)+(Ymax/2)+(Ymax/2))*number_of_nodes;
	F_ND  = Node_distribution(place) / Max_Node_distribution;
	F_BL  = BorderLines(place) / Max_BorderLines;
	F_EL  = EdgeLengths(place) / Max_EdgeLength;
	F_EC  = 1/(EdgeCrossings(place)+1);
//	tmp2  = EdgeCrossings(place);
	F_NED = tmp / Max_NodeEgdeDistances;

	Fitness = K_ND*F_ND + K_BL*F_BL + K_NED*F_NED + K_EC*F_EC + K_EL*F_EL;
	return Fitness;
}
//===========================================================================
/* The Fitness Function */
float Evaluation2(place_t *place)
{
	float EA_Dev, EL_Dev, EC , Fitness;

	EA_Dev =  MeanSquareError_of_EdgeLength(place);
	EL_Dev = CumulativeSquareDeviation_of_EdgeAngles(place);
	EC     = EdgeCrossings(place);
	Fitness = (Alfa / (1+EL_Dev)) + (Beta / (1+EC)) + (Gama / (1+EA_Dev));
	return Fitness;
}
//===========================================================================
/* The Fitness Function */
float Evaluation3(place_t *place)
{
	float  Fitness=0,Optimal_Length,tmp1,tmp2,tmp3;
   float  EdgeCross,EdgeLength_Dev,MinNodeDist,MinNodeDist_Sum;

	Optimal_Length = MinimumEdgeLength(place) + 20.0;
	EdgeCross = EdgeCrossings(place);
	EdgeLength_Dev = EdgeLengthDeviation(place,Optimal_Length);
	MinNodeDist = MinimumNodeDistance(place);
	MinNodeDist_Sum = MinimumNodeDistance_Sum(place);
	tmp1 = (EdgeLength_Dev / MinNodeDist);
	tmp2 = (number_of_nodes * pow(MinNodeDist,2));
	tmp3 = (EdgeCross * pow(100,2));
	Fitness = MNDS*MinNodeDist_Sum +
				 ELD*EdgeLength_Dev +
				 ELD_MND*(EdgeLength_Dev / MinNodeDist) +
				 NoN_MND2*(number_of_nodes * pow(MinNodeDist,2))+
				 EC_GS2*(EdgeCross * pow(100,2));

	return Fitness;
}
//===========================================================================
void Fitness_Sortion(float Fitness[],place_t *place[])
{
	float F_tmp;
	place_t *tmp_ptr;
	for (int i=0 ; i<(Pop_size)-1 ; i++)
		for (int j=i+1 ; j< Pop_size ; j++)
			if (Fitness[i] <= Fitness[j])
			{
				F_tmp        = Fitness[i];
				Fitness[i]   = Fitness[j];
				Fitness[j]   = F_tmp;
				tmp_ptr     = place[i];
				place[i]    = place[j];
				place[j]    = tmp_ptr;
			}
}
//===========================================================================
void Rough_Fitness_assingment(float Fitness[],float Selection_prob[])
{
	float sum=0;
	for(int i=0 ; i<Pop_size ; i++)
		sum += Fitness[i];
	for(i=0 ; i<Pop_size ; i++)
		Selection_prob[i] = Fitness[i]/sum;
}
//===========================================================================
void LinearRanking_Fitness_assingment(float Fitness[],float Selection_prob[])
{
	float SP=Selective_pressure,sum=0;
	for(int i=0 ; i<Pop_size ; i++)
	{
		Fitness[i]= 2 - SP + 2*(SP-1.0)*(Pop_size-1-i)/(Pop_size-1);
		sum += Fitness[i];
	}
	for(i=0 ; i<Pop_size ; i++)
		Selection_prob[i] = Fitness[i]/sum;
}
//===========================================================================
void LinearNormalization_Fitness_assingment(float Fitness[],float Selection_prob[])
{
	float sum=0;
	for(int i=0 ; i<Pop_size ; i++)
	{
		Fitness[i]= Pop_size - i;
		sum += Fitness[i];
	}
	for(i=0 ; i<Pop_size ; i++)
		Selection_prob[i] = Fitness[i]/sum;
}
//===========================================================================
void LinearTransformation_Fitness_assingment(float Fitness[],float Selection_prob[])
{
	float sum=0;
	for(int i=0 ; i<Pop_size ; i++)
	{
		Fitness[i]= Fitness[i]*a + b;
		sum += Fitness[i];
	}
	for(i=0 ; i<Pop_size ; i++)
		Selection_prob[i] = Fitness[i]/sum;
}
//===========================================================================
void Exponential_Fitness_assingment(float Fitness[],float Selection_prob[])
{
	float sum=0;
	for(int i=0 ; i<Pop_size ; i++)
	{
		Fitness[i]= sqrt(Fitness[i] + 1);
		sum += Fitness[i];
	}
	for(i=0 ; i<Pop_size ; i++)
		Selection_prob[i] = Fitness[i]/sum;
}
//===========================================================================
void Windowing_Fitness_assingment(float Fitness[],float Selection_prob[])
{
	float sum=0;
	for(int i=0 ; i<Pop_size ; i++)
	{
		Fitness[i]= Fitness[i] - Fitness[Pop_size-1] ;
		sum += Fitness[i];
	}
	for(i=0 ; i<Pop_size ; i++)
		Selection_prob[i] = Fitness[i]/sum;
}
//===========================================================================
void RoulleteWheele_Selection(float Selection_prob[],int number_of_selection,int Selected_parent[])
{
	float Prob_distribution[Pop_size], sum=0, tmp;
	int i,j,tmp2;
	for(i=0 ; i<Pop_size ; i++)
	{
		Prob_distribution[i] = Selection_prob[i] + sum;
		sum                  = Prob_distribution[i];
	}
//	randomize();
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
}
//===========================================================================
void StochasticUniversalSampling_Selection(float Selection_prob[],int number_of_selection,int Selected_parent[])
{
	float Prob_distribution[Pop_size], sum=0, tmp=0;
	float ptr_distance=0, start_pointer=0;
	int i,j,tmp2,Rand_limit=0;
	for(i=0 ; i<Pop_size ; i++)
	{
		Prob_distribution[i] = Selection_prob[i] + sum;
		sum                  = Prob_distribution[i];
	}
//	randomize();
	ptr_distance= 1.0 /float(number_of_selection);
	if (ptr_distance>=0.1   && ptr_distance<1.0)
	{ 	Rand_limit= int(ptr_distance*10000.0); start_pointer= random(Rand_limit) /10000.0;}
	if (ptr_distance>=0.01  && ptr_distance<0.1)
	{  Rand_limit= int(ptr_distance*100000.0);start_pointer= random(Rand_limit) /100000.0;}
	if (ptr_distance>=0.001 && ptr_distance<0.01)
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
}
//===========================================================================
void Uniform_Crossover(place_t *parent0,place_t *parent1,place_t *offspring0,place_t *offspring1)
{
	short int sample0[MAX_NODES],sample1[MAX_NODES];
//	randomize();
	for (int i=0 ; i<number_of_nodes; i++)
	{
		sample0[i]=random(2);
		sample1[i]=random(2);
	}
	for (i=0 ; i<number_of_nodes; i++)
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
	short int CrossPoint=0;
//	randomize();
	CrossPoint=random(number_of_nodes-1);
	for(int i=0 ; i<=CrossPoint ; i++)
	{
		offspring0[i]=parent0[i];
		offspring1[i]=parent1[i];
	}
	for(i=CrossPoint+1 ; i<number_of_nodes ; i++)
	{
		offspring0[i]=parent1[i];
		offspring1[i]=parent0[i];
	}
}
//===========================================================================
void MultiPoint_Crossover(place_t *parent0,place_t *parent1,place_t *offspring0,place_t *offspring1)
{
	short int points[CrossPoints],p_tmp,flage;
//	randomize();
	points[0]=random(number_of_nodes-1);
	for (int i=1 ; i<CrossPoints ; i++)  //Generate m random crosspoints
	{
L:		points[i]=random(number_of_nodes-1);
		for (int j=i-1; j>=0 ; j--)
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
		for(i=points[CrossPoints-1]+1 ; i<number_of_nodes ; i++)
		{  offspring0[i]=parent0[i]; offspring1[i]=parent1[i];}
	 else
		for(i=points[CrossPoints-1]+1 ; i<number_of_nodes ; i++)
		{	offspring0[i]=parent1[i]; offspring1[i]=parent0[i];}
}
//===========================================================================
void ConvexHull_Crossover(place_t *parent0,place_t *parent1,place_t *offspring0,place_t *offspring1,float variance)
{
	float deviation,mean,Z,r0,r1,X,Y;
	deviation=sqrt(variance);
	for(int i=0 ; i<number_of_nodes ; i++)
	{
		mean = (parent0[i].x+parent1[i].x)/2;
		r0   = (random(1000)+1)/1000.0;
		r1   = (random(1000)+1)/1000.0;
		Z    = sqrt(-2.0*log(r0))*cos(360.0*r1);
		X    = float(int(Z*deviation + mean));	offspring0[i].x=X;
		r0   = (random(1000)+1)/1000.0;
		r1   = (random(1000)+1)/1000.0;
		Z    = sqrt(-2.0*log(r0))*cos(360.0*r1); // | Z=sqrt(-2.0*log(r0))*sin(360.0*r1);
		X    = float(int(Z*deviation + mean));	offspring1[i].x=X;

		mean = (parent0[i].y+parent1[i].y)/2;
		r0   = (random(1000)+1)/1000.0;
		r1   = (random(1000)+1)/1000.0;
		Z    = sqrt(-2.0*log(r0))*cos(360.0*r1);
		Y    = float(int(Z*deviation + mean));	offspring0[i].y=Y;
		r0   = (random(1000)+1)/1000.0;
		r1   = (random(1000)+1)/1000.0;
		Z    = sqrt(-2.0*log(r0))*cos(360.0*r1);
		Y    = float(int(Z*deviation + mean));	offspring1[i].y=Y;
	}
}
//===========================================================================
void PlaneCut_Crossover(place_t *parent0,place_t *parent1,place_t *offspring0,place_t *offspring1)
{
	float x0,x1,y0,y1,m,c;
	x0=random(480); y0=random(480);
	x1=random(480); y1=random(480);
	m=(y1-y0)/(x1-x0);
	c=y0-m*x0;
	for(int i=0 ; i<number_of_nodes ; i++)
	{
		if((parent0[i].y - m*parent0[i].x)<=c) //or if((parent0[i].y - m*parent0[i].x)>=c)
		{
			offspring0[i] = parent0[i];
			offspring1[i] = parent1[i];
		}
		else
		{
			offspring0[i] = parent1[i];
			offspring1[i] = parent0[i];
		}
	}
}
//===========================================================================
void Rect_Crossover(place_t *parent0,place_t *parent1,place_t *offspring0,place_t *offspring1)
{
	int P_rect_x,P_rect_y,O_rect_x,O_rect_y,delta_x,delta_y;
	int P_L,P_R,P_T,P_B,O_L,O_R,O_T,O_B;
	P_rect_x = random(480-Square_edge); 	P_rect_y = random(480-Square_edge);
	O_rect_x = random(480-Square_edge); 	O_rect_y = random(480-Square_edge);
	delta_x=O_rect_x - P_rect_x;     delta_y=O_rect_y - P_rect_y;
	P_L = P_rect_x ; 	P_R = P_rect_x + Square_edge;
	P_T = P_rect_y ; 	P_B = P_rect_y + Square_edge;
	O_L = O_rect_x ; 	O_R = O_rect_x + Square_edge;
	O_T = O_rect_y ; 	O_B = O_rect_y + Square_edge;
	for(int i=0 ; i<number_of_nodes ; i++)
	{
		if((parent0[i].x>=P_L)&&(parent0[i].x<=P_R)&&(parent0[i].y>=P_T)&&(parent0[i].y<=P_B))
		{  offspring1[i].x=parent0[i].x + delta_x; 	offspring1[i].y=parent0[i].y + delta_y;}
		else
			if((parent0[i].x>=O_L)&&(parent0[i].x<=O_R)&&(parent0[i].y>=O_T)&&(parent0[i].y<=O_B))
				offspring0[i] = parent1[i];
			else
				offspring0[i] = parent0[i];
		if((parent1[i].x>=P_L)&&(parent1[i].x<=P_R)&&(parent1[i].y>=P_T)&&(parent1[i].y<=P_B))
		{	offspring0[i].x=parent1[i].x + delta_x; 	offspring0[i].y=parent1[i].y + delta_y;}
		else
			if((parent1[i].x>=O_L)&&(parent1[i].x<=O_R)&&(parent1[i].y>=O_T)&&(parent1[i].y<=O_B))
				offspring1[i] = parent0[i];
			else
				offspring1[i] = parent1[i];
	}
}
//===========================================================================
void Neighborhood_Crossover(place_t *parent0,place_t *parent1,place_t *offspring0,place_t *offspring1)
{
	int Rand_node,neighbors[Neighbors_num];
	int flag=0;
	node_t *tmp_ptr;
	Queue q;
L1:Rand_node=random(number_of_nodes);
	for(int i=0; i<Neighbors_num ;i++)
		neighbors[i]=-1;
	BFS(Rand_node,neighbors);
	if(neighbors[Neighbors_num-1] == -1)  // test the last value.
		goto L1;
	for (i=0 ; i<number_of_nodes ; i++)
	{
		flag=0;
		for(int j=0 ;j<Neighbors_num ; j++)
			if (i == neighbors[j])
				flag=1;
		if(flag)
		{
			offspring0[i]=parent1[i];
			offspring1[i]=parent0[i];
		}
		else
		{
			offspring0[i]=parent0[i];
			offspring1[i]=parent1[i];
		}
	}
}
//===========================================================================
void Mutation(place_t *offspring,float variance)
{
	float deviation,mean,Z,r0,r1,X,Y;
	deviation=sqrt(variance);
	for(int i=0 ; i<number_of_nodes ; i++)
	{
		mean = offspring[i].x;
		r0   = (random(1000)+1)/1000.0;
		r1   = (random(1000)+1)/1000.0;
		Z    = sqrt(-2.0*log(r0))*cos(360.0*r1);
		X    = float(int(Z*deviation + mean));	offspring[i].x=X;

		mean = offspring[i].y;
		r0   = (random(1000)+1)/1000.0;
		r1   = (random(1000)+1)/1000.0;
		Z    = sqrt(-2.0*log(r0))*cos(360.0*r1);
		Y    = float(int(Z*deviation + mean));	offspring[i].y=Y;
	}
}
//===========================================================================
void Single_Mutation(place_t *offspring)
{
	int Rand_node,Rand_x,Rand_y;
	Rand_node = random(number_of_nodes);
	Rand_x    = random(Right[0]);
	Rand_y    = random(Button[0]);
	offspring[Rand_node].x = Rand_x;
	offspring[Rand_node].y = Rand_y;
}
//===========================================================================
void Small_Mutation(place_t *offspring)
{
	int Rand_node1,Rand_node2;
	point_t tmp;
	Rand_node1 = random(number_of_nodes);
	Rand_node2 = random(number_of_nodes);
	tmp                   = offspring[Rand_node1];
	offspring[Rand_node1] = offspring[Rand_node2];
	offspring[Rand_node2] = tmp;
}
//===========================================================================
void LargeCont_Mutation(place_t *offspring)
{
	short int rect1_x,rect1_y,rect2_x,rect2_y,delta_x,delta_y;
	short int L1,R1,T1,B1,L2,R2,T2,B2;
	rect1_x = random(480-Square_edge); 	rect1_y = random(480-Square_edge);
	rect2_x = random(480-Square_edge); 	rect2_y = random(480-Square_edge);
	delta_x=rect2_x - rect1_x;     delta_y=rect2_y - rect1_y;
	L1 = rect1_x ; 	R1 = rect1_x + Square_edge;
	T1 = rect1_y ; 	B1 = rect1_y + Square_edge;
	L2 = rect2_x ; 	R2 = rect2_x + Square_edge;
	T2 = rect2_y ; 	B2 = rect2_y + Square_edge;
	for(int i=0 ; i<number_of_nodes ; i++)
	{
		if((offspring[i].x>=L1)&&(offspring[i].x<=R1)&&(offspring[i].y>=T1)&&(offspring[i].y<=B1))
		{  offspring[i].x += delta_x; 	offspring[i].y += delta_y;}
		else
			if((offspring[i].x>=L2)&&(offspring[i].x<=R2)&&(offspring[i].y>=T2)&&(offspring[i].y<=B2))
			{  offspring[i].x -= delta_x; 	offspring[i].y -= delta_y;}
	}
}
//===========================================================================
void Edge_Mutation1(place_t *offspring)
{
	int selected_edge,node1,node2;
	selected_edge=random(number_of_edges);
	node1=edges[selected_edge].node1;
	node2=edges[selected_edge].node2;
	offspring[node1].x=random(Right[0]);
	offspring[node1].y=random(Button[0]);
	offspring[node2].x=random(Right[0]);
	offspring[node2].y=random(Button[0]);
}
//===========================================================================
void Edge_Mutation2(place_t *offspring,int Radious)
{
	int selected_edge,node1,node2,delta_x,delta_y;
	selected_edge=random(number_of_edges);
	node1=edges[selected_edge].node1;
	node2=edges[selected_edge].node2;
	delta_x=random(2*Radious)-(Radious);
	delta_y=random(2*Radious)-(Radious);

	offspring[node1].x += delta_x;
	offspring[node1].y += delta_y;
	offspring[node2].x += delta_x;
	offspring[node2].y += delta_y;
	if(offspring[node1].x>Right[0])   offspring[node1].x=Right[0];
	if(offspring[node1].y>Button[0])  offspring[node1].y=Button[0];
	if(offspring[node2].x>Right[0])   offspring[node2].x=Right[0];
	if(offspring[node2].y>Button[0])  offspring[node2].y=Button[0];
}
//===========================================================================
void TwoEdge_Mutation(place_t *offspring, int Radious)
{
	int selected_edge1,selected_edge2,neighbor_edges[MAX_EDGES],k;
	int node1_1,node1_2,node2_1,node2_2,delta_x,delta_y;
	int found,visited[MAX_EDGES],num_of_visited=0;
	for(int i=0; i<MAX_EDGES; i++)
		visited[i]=0;
L3:selected_edge1=random(number_of_edges);
	if(visited[selected_edge1])
		goto L3;
	visited[selected_edge1]=1;
	num_of_visited++;
	node1_1 = edges[selected_edge1].node1;
	node1_2 = edges[selected_edge1].node2;
	for(i=0; i<MAX_EDGES; i++)
		neighbor_edges[i]=-1;
	found=0;
	k=0;
	for(i=0 ; i<number_of_edges ; i++)
		if(((edges[i].node1==node1_1) || (edges[i].node2==node1_2)) && (i!=selected_edge1))
		{
			neighbor_edges[k]=i;
			found=1;  k++;
		}
	if((!found)&&(num_of_visited<number_of_edges))
		goto L3;

	delta_x=random(2*Radious)-(Radious);
	delta_y=random(2*Radious)-(Radious);

	offspring[node1_1].x += delta_x;	offspring[node1_1].y += delta_y;
	offspring[node1_2].x += delta_x;	offspring[node1_2].y += delta_y;
	if(offspring[node1_1].x>Right[0])   offspring[node1_1].x=Right[0];
	if(offspring[node1_1].y>Button[0])  offspring[node1_1].y=Button[0];
	if(offspring[node1_2].x>Right[0])   offspring[node1_2].x=Right[0];
	if(offspring[node1_2].y>Button[0])  offspring[node1_2].y=Button[0];
	if(found)
		selected_edge2=neighbor_edges[random(k)];
	else
		{
		if(number_of_edges==1)
			return;
L4:	selected_edge2=random(number_of_edges);
		if (selected_edge2==selected_edge1)
			goto L4;
		}
		node2_1=edges[selected_edge2].node1;
		node2_2=edges[selected_edge2].node2;
		offspring[node2_1].x += delta_x; offspring[node2_1].y += delta_y;
		offspring[node2_2].x += delta_x; offspring[node2_2].y += delta_y;
		if(offspring[node2_1].x>Right[0])   offspring[node2_1].x=Right[0];
		if(offspring[node2_1].y>Button[0])  offspring[node2_1].y=Button[0];
		if(offspring[node2_2].x>Right[0])   offspring[node2_2].x=Right[0];
		if(offspring[node2_2].y>Button[0])  offspring[node2_2].y=Button[0];
}
//===========================================================================
void TinyEdge_Mutation(place_t *offspring)
{
	int selected_edge,node1,node2,delta_x,delta_y;
	selected_edge=random(number_of_edges);
	node1=edges[selected_edge].node1;
	node2=edges[selected_edge].node2;
	delta_x=random(2*Mut_Sqr_edge)-Mut_Sqr_edge;
	delta_y=random(2*Mut_Sqr_edge)-Mut_Sqr_edge;

	offspring[node1].x += delta_x;
	offspring[node1].y += delta_y;
	offspring[node2].x += delta_x;
	offspring[node2].y += delta_y;
	if(offspring[node1].x>Right[0])   offspring[node1].x=Right[0];
	if(offspring[node1].y>Button[0])  offspring[node1].y=Button[0];
	if(offspring[node2].x>Right[0])   offspring[node2].x=Right[0];
	if(offspring[node2].y>Button[0])  offspring[node2].y=Button[0];
}
//===========================================================================
void Tiny_Mutation(place_t *offspring)
{
	int Rand_node,delta_x,delta_y;
	Rand_node = random(number_of_nodes);
	delta_x    = random(2*Mut_Sqr_edge)-Mut_Sqr_edge;
	delta_y    = random(2*Mut_Sqr_edge)-Mut_Sqr_edge;
	offspring[Rand_node].x += delta_x;
	offspring[Rand_node].y += delta_y;
}
//===========================================================================
void Elitism_Insertion(place_t *parents[],float Prnt_Fit[],
								place_t *offsprings[],float Ofsp_Fit[],
								place_t *newGeneration[],float New_Fit[])
{
	int i=0, j=0, k=0 ;
	for (i=0 ; i<Pop_size/3 ; i++)
		{ newGeneration[k]=parents[i]; k++; }
	while( (i<Pop_size) && (j<Pop_size) && (k<Pop_size))
	{
		if(Prnt_Fit[i] >= Ofsp_Fit[j])
			{  newGeneration[k]=parents[i]; i++; }
		else
			{  newGeneration[k]=offsprings[j]; j++; }
		k++;
	}
}
//===========================================================================
void FitnessBased_Insertion(place_t *parents[],float Prnt_Fit[],
								place_t *offsprings[],float Ofsp_Fit[],
								place_t *newGeneration[],float New_Fit[])
{
	int i=0,j=0,k=0;
	while( (i<Pop_size) && (j<Pop_size) && (k<Pop_size))
	{
		if(Prnt_Fit[i] >= Ofsp_Fit[j])
			{  newGeneration[k]=parents[i]; i++; }
		else
			{  newGeneration[k]=offsprings[j]; j++; }
		k++;
	}
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
int InitGraph()
{
  /* request auto detection */
  int gdriver = DETECT, gmode, errorcode;
  int x, y;

  /* initialize graphics mode */
  initgraph(&gdriver, &gmode, "C:\\borlandc\\bgi");

  /* read result of initialization */
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
void DrawGraph(place_t * place)
{
  int i, index1, index2;
  struct node_t * next;
  char tmp_str[5];

  cleardevice();
  for (i=0; i<number_of_nodes; i++)
  {
	 setcolor(YELLOW);
	 circle(place[i].x, place[i].y, 2);
//    sprintf(tmp_str, "%d", i+1);
//    setcolor(YELLOW);
//    outtextxy(place[i].x-1, place[i].y-2, tmp_str);
	 setcolor(WHITE);
  }
  for (i=0; i<number_of_nodes; i++)
	 if (adjacenty_list[i])
	 {
		next = adjacenty_list[i]->next;
		index1 = adjacenty_list[i]->node_num;
		while(next)
		{
	index2 = next->node_num;
	line(place[index1].x, place[index1].y, place[index2].x, place[index2].y);
	next = next->next;
		}
	 }
}
//===========================================================
void Draw_AllGraph(place_t *place[])
{
	int i,j, index1, index2;
	struct node_t * next;
	char tmp_str[5];
		cleardevice();

		j=0;
//	for (j=0; j<Pop_size ;j++)
		for (i=0; i<number_of_nodes; i++)
		{
			 setcolor(YELLOW);
			 circle(place[j][i].x, place[j][i].y, 2);
			 sprintf(tmp_str, "%d", i);
			 setcolor(YELLOW);
			 outtextxy(place[j][i].x+8, place[j][i].y, tmp_str);
		}
		  setcolor(WHITE);
/*	  for (j=0; j<Pop_size ;j++)
		if (j!=Fitness_order[0])
		  for (i=0; i<number_of_nodes; i++)
			 if (adjacenty_list[i])
			 {
				next = adjacenty_list[i]->next;
				index1 = adjacenty_list[i]->node_num;
				while(next)
				{
					index2 = next->node_num;
					line(place[j][index1].x, place[j][index1].y, place[j][index2].x, place[j][index2].y);
					next = next->next;
				}
			 }
*/
//===========
		 setcolor(RED);
		j=0;
		for (i=0; i<number_of_nodes; i++)
			 if (adjacenty_list[i])
			 {
				next = adjacenty_list[i]->next;
				index1 = adjacenty_list[i]->node_num;
				while(next)
				{
					index2 = next->node_num;
					line(place[j][index1].x, place[j][index1].y, place[j][index2].x, place[j][index2].y);
					next = next->next;
				}
			 }

}

//###########################################################################
/* Main */
void main(int argc, char * argv[])
{
	float Fitness[Pop_size], Selection_prob[Pop_size];
	int   Selected_parent[Pop_size];
//	float temp=0;
	float RadiusX[Pop_size], RadiusY[Pop_size];
	place_t *place[Pop_size], *new_place[Pop_size];
	int i,k,w,row_number,col_number;
	FILE * output_file;
	char * output_name = "sa_alg.txt";
	if (argc < 2)
	{
		printf("Usage: sa_gd <input-file-1> ... <input-file-n>");
 //    return;
	}
	if ((output_file = fopen(output_name, "w")) == NULL)
	{
		clrscr();
		printf("Error : I can not read the output file!");
 //    return;
	}
	InitGraph();
	/* The following parameters can be given from the user. */
//  CloseGraph();
	 clrscr();
//  printf("Evaluation of The SA Algorithm. Please wait ... \n");
	for (k = 1; k < 2 /*argc*/; k++)
	{
		if (Give_Inputs("G.TXT"))//argv[k]))
			continue;
		if (!number_of_nodes)
			continue;
		row_number=sqrt(Pop_size*3/4);
		col_number=sqrt(Pop_size*3/4)*4/3;
		for (int r=0; r<=row_number-1 ; r++)
			for (int c=0; c<=col_number-1 ; c++)
			{
				Top[r*col_number+c] = 0;  //(Ymax/row_number)*r;
				Left[r*col_number+c] = 0;//(Xmax-Ymax)/2; //(Xmax/col_number)*c;
				Right[r*col_number+c] = Ymax; //(Xmax/col_number)*(c+1);
				Button[r*col_number+c] = Ymax;//(Ymax/row_number)*(r+1);
			}
		for(i=0 ; i<Pop_size ; i++)
		{
			Fitness[i]=0;
			Selection_prob[i]=0;
			Selected_parent[i]=-1;
		}
		BFS();
		randomize();
		for(w=0;w<Pop_size;w++)
		{
			place[w] = (place_t *)malloc(number_of_nodes*sizeof(place_t));
			new_place[w] = (place_t *)malloc(number_of_nodes*sizeof(place_t));
			Init_Placement(place[w]);
	//		Fitness[w] = Evaluation1(place[w]);
	//		EdgeLengths(place[0]);
	//		CumulativeSquareDeviation_of_EdgeAngles(place[0]);
			Draw_AllGraph(place);
			EdgeCrossings(place[0]);
	//		MeanSquareError_of_EdgeLength(place[0]);
	//		outtextxy(0, Ymax-10, "Initial Drawing, press any key to continue.");
		}

		Fitness_Sortion(Fitness,place);

//		Rough_Fitness_assingment(Fitness,Selection_prob);
//		LinearNormalization_Fitness_assingment(Fitness,Selection_prob);
//		LinearTransformation_Fitness_assingment(Fitness,Selection_prob);
//		LinearRanking_Fitness_assingment(Fitness,Selection_prob);
//		Exponential_Fitness_assingment(Fitness,Selection_prob);
//		Windowing_Fitness_assingment(Fitness,Selection_prob);

//		RoulleteWheele_Selection(Selection_prob,(Pop_size-Pop_size/3),Selected_parent);
//		StochasticUniversalSampling_Selection(Selection_prob,(Pop_size-Pop_size/3),Selected_parent);

//		Uniform_Crossover(place[0],place[1],new_place[0],new_place[1]);
//		SinglePoint_Crossover(place[0],place[1],new_place[0],new_place[1]);
//		MultiPoint_Crossover(place[0],place[1],new_place[0],new_place[1]);
//		ConvexHull_Crossover(place[0],place[1],new_place[0],new_place[1],100);
//		PlaneCut_Crossover(place[0],place[1],new_place[0],new_place[1]);
//		Rect_Crossover(place[0],place[1],new_place[0],new_place[1]);
//		Neighborhood_Crossover(place[0],place[1],new_place[0],new_place[1]);

//		Mutation(place[0],MutVariance);
//		Single_Mutation(place[0]);
//		Small_Mutation(place[0]);
//		LargeCont_Mutation(place[0]);
//		Edge_Mutation1(place[0]);
//		Edge_Mutation2(place[0],Mut_Radious);
//		TwoEdge_Mutation(place[0],Mut_Radious);
//		TinyEdge_Mutation(place[0]);
//		Tiny_Mutation(place[0]);

//      temp=0;
		Draw_AllGraph(place);
		getch();
//   	printf("%s started. \n", argv[k]);

/*
		while (1)
		{
			for(w=0;w<12;w++)
			{

			//RadiusX[w] = (float)(((Right[w]-Left[w])/2-50)*(temp-FINAL_TEMP)/(INIT_TEMP-FINAL_TEMP)+50);
			//RadiusY[w] = (float)(((Button[w]-Top[w])/2-50)*(temp-FINAL_TEMP)/(INIT_TEMP-FINAL_TEMP)+50);
			//RadiusX = (int)((Right - Left)/2);
			//RadiusY = (int)((Button - Top)/2);
			while ( inner_loop_criterion == 0 )
			{
			  Perturb(place[w], new_place[w], RadiusX[w], RadiusY[w],w);
			  //delta_cost = Cost(new_place) - Cost(place);
			  new_Fitness[w] = Evaluation(new_place[w],w);
			}//while
//			inner_loop_criterion = 0;
		 }//for
//			temp = Schedule(temp);
			DrawGraphx(place);
	  }	//while
	*/
//	  	DrawGraphx(place);
		outtextxy(0, Ymax-10, "press any key to quit:");
		getch();
		CloseGraph();
		free(place[w]);
		free(new_place[w]);
		for (i=0; i<number_of_nodes; i++)
			RemoveAll(adjacenty_list[i]);
		free(adjacenty_list);
//		fprintf(output_file, "The SA Algorithm result: %s  cost= %f , crossings= %d \n", argv[k],	cost, number_of_edge_crossings);
		printf("%s finished.\n", argv[k]);
		fflush(output_file);
	}
  //CloseGraph();
	fclose(output_file);
} /* Main */
