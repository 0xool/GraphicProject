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

/* Parameters */
#define MAX_NODES	500
#define INIT_TEMP	24000.0
#define FINAL_TEMP	0.1
#define ITERATION	20	/* 50 moves per node */

#define LANDA1	4800
#define LANDA2	40
#define LANDA3	0.005
#define LANDA4 5000
#define LANDA5	4800
#define G_min	4

struct node_t
{
	int	node_num;
	node_t *next;
};

struct	point_t
{
	float	x,y;
};
typedef struct point_t place_t;
place_t *place[12], *new_place[12];


/* Global Variables */
struct node_t * * adjacenty_list;
int number_of_nodes = 0;
int inner_loop_criterion = 0;
float Xmax, Ymax;
float MaxPenalty1, MaxPenalty2, MaxPenalty3;
float Right[12], Left[12], Top[12], Button[12];

int number_of_edge_crossings = 0;
//===========================================================================
/* Adds a node to the begin of the list */
int AddTo(node_t * * edge_list, int node_num)
{
	node_t * tmp_ptr;

	tmp_ptr = *(edge_list);
	if (tmp_ptr==NULL)
	{
	  *(edge_list) = (node_t *)malloc(sizeof(node_t));
	  (*(edge_list))->node_num = node_num;
	  (*(edge_list))->next = NULL;
	}
	else
	{
	  while(tmp_ptr->next)
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
		  AddTo((node_t * *)&adjacenty_list[number_of_nodes], node_num);
	  } while (node_num != END_LINE);
	  number_of_nodes++;
	}
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
void Init_Placement(place_t *place, int n,int k)
{
  int i;

  if (number_of_nodes)
  {
	  MaxPenalty1 = (MAXLONG/(number_of_nodes-1));
	  MaxPenalty2 = (MAXLONG/(number_of_nodes-1)*(number_of_nodes-1));
	  MaxPenalty3 = (MAXLONG/(number_of_nodes-1)*(number_of_nodes-1)
			*(number_of_nodes-1));
  }
  else
  {
	  MaxPenalty1 = MAXLONG;
	  MaxPenalty2 = MAXLONG;
	  MaxPenalty3 = MAXLONG;
  }
//  randomize();
  for (i=0; i < n; i++)
  {
	 place[i].x = (int)( Left[k] + (double)(rand() * (Right[k] - Left[k])) / RAND_MAX );
	 place[i].y = (int)( Top[k] + (double)(rand() * (Button[k] - Top[k])) / RAND_MAX );
  }
}

//===========================================================================

/* All nodes place in one location! *//*
float Init_Placement(place_t *place, int n)
{
  int i;
  int XX, YY;

  if (number_of_nodes)
  {
	  MaxPenalty1 = (MAXLONG/(number_of_nodes-1));
	  MaxPenalty2 = (MAXLONG/(number_of_nodes-1)*(number_of_nodes-1));
	  MaxPenalty3 = (MAXLONG/(number_of_nodes-1)*(number_of_nodes-1)
			*(number_of_nodes-1));
  }
  else
  {
	  MaxPenalty1 = MAXLONG;
	  MaxPenalty2 = MAXLONG;
	  MaxPenalty3 = MAXLONG;
  }

  XX = (float)( Left + (float)(Right - Left)/2 );
  YY = (float)( Top + (float)(Button - Top)/2 );

  for (i=0; i < n; i++)
  {
	 place[i].x = XX;
	 place[i].y = YY;
  }
  return 0;
}                                       */


//===========================================================================
void Perturb(place_t * place, place_t *new_place,
	float RadiusX, float RadiusY,int x)
{
  static int i=0;
  static int j=0;
  int k;

  if (!number_of_nodes)
    return;

  for (k=0; k < number_of_nodes; k++)
  {
    new_place[k].x = place[k].x;
	 new_place[k].y = place[k].y;
  }
  //new_place[i].x = (int)(Left + (rand() * (Right - Left)) / RAND_MAX);
  //new_place[i].y = (int)(Top + (rand() * (Button - Top)) / RAND_MAX);
  if ((float)(rand()) < (float)((float)RAND_MAX/2))
    new_place[i].x += (float)((float)(rand() * RadiusX) / RAND_MAX);
  else
    new_place[i].x -= (float)((float)(rand() * RadiusX) / RAND_MAX);

  if ((float)(rand()) < (float)((float)RAND_MAX/2))
	 new_place[i].y += (float)((float)(rand() * RadiusY) / RAND_MAX);
  else
    new_place[i].y -= (float)((float)(rand() * RadiusY) / RAND_MAX);

  if (new_place[i].x < Left[x])
    new_place[i].x = Left[x];
  if (new_place[i].x > Right[x])
	 new_place[i].x = Right[x];
  if (new_place[i].y < Top[x])
    new_place[i].y = Top[x];
  if (new_place[i].y > Button[x])
    new_place[i].y = Button[x];
  i++;
  i=i%number_of_nodes;
  if (!i)
    j++;
  if (j==ITERATION)
  {
	 inner_loop_criterion = 1;
	 j = 0;
  }
}


//===========================================================================
float Node_distribution(place_t * place)
{
	int i, j;
   float sum, distance2, Dx, Dy;

   sum = 0;
	for (i=0; i< (number_of_nodes-1); i++)
	 for (j=i+1; j<number_of_nodes; j++)
	 {
	   Dx = place[i].x - place[j].x;
	   Dy = place[i].y - place[j].y;
	   distance2 = Dx*Dx + Dy*Dy;
	   if (distance2 < (G_min*G_min))
	     sum += (float)LANDA1 / (G_min*G_min);
	   else
	     sum += (float)LANDA1 / distance2;
	 }
   return sum;
}

//===========================================================================
float BorderLines(place_t * place,int k)
{
   int i, j;
   float sum, Ri, Li, Ti, Bi;

   sum = 0;
	for (i=0; i<number_of_nodes; i++)
   {
	   Ri = (Right[k] - place[i].x);
	   Li = (place[i].x - Left[k]);
	   Ti = (place[i].y - Top[k]);
	   Bi = (Button[k] - place[i].y);
	   if (Ri*Ri < (G_min*G_min))
	     sum += LANDA2 / (G_min*G_min);
	   else
	     sum += LANDA2/(Ri*Ri);
		if (Li*Li < (G_min*G_min))
		  sum += LANDA2 / (G_min*G_min);
	   else
		  sum += LANDA2/(Li*Li);
		if (Ti*Ti < (G_min*G_min))
		  sum += LANDA2 / (G_min*G_min);
	   else
	     sum += LANDA2/(Ti*Ti);
	   if (Bi*Bi < (G_min*G_min))
	     sum += LANDA2 / (G_min*G_min);
		else
	     sum += LANDA2/(Bi*Bi);
   }
   return sum;
}

//===========================================================================
float EdgeLengths(place_t * place)
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
		sum += LANDA3*edge_len2;
		next = next->next;
	 }
	}
	return sum/2; /* Note that each edge repeats two times */
}

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
		  {
		    distance2 =(x3-x1)*(x3-x1)
					+ (y3-y1)*(y3-y1);
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
		  if (!d2) printf("ERROR: DIVIDED BY ZERRO!!!!!!!\n");
		  distance2 = (d1*d1) / d2;
		}
		if (distance2 < (G_min*G_min))
		  sum += LANDA5 / (G_min*G_min); // it was before this LANDA4 !
		else
		  sum += LANDA5/distance2;

	      }
			next = next->next;
	    }
	  }
	}

  return sum/2; /* Note That each edge repreats two times */
}

//===========================================================================
float EdgeCrossings(place_t * place)
{
  int i, j, node_numi, node_numj;
  int neighbor_numi, neighbor_numj;
  node_t * next1, *next2;
  float x1, y1, x2, y2, X1, Y1, X2, Y2;
  float u1, u2, sum, d;

  sum = 0; number_of_edge_crossings = 0;
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
		     sum += LANDA4;
			  number_of_edge_crossings++;
		   }
		 }
	       }
	       next2 = next2->next;
	     }
	     next1 = next1->next;
		}
	 }
		 }
	  }
	return sum/2;
}

//===========================================================================
/* The Cost Function */
float Cost(place_t *place,int k)
{
  float tmp;

  tmp =  Node_distribution(place); //OK LANDA1=1000 & expo/1000.0
  tmp += BorderLines(place,k); //OK LADNDA2=1000 & expo/1000.0
  tmp += EdgeLengths(place);
  tmp += EdgeCrossings(place);
  tmp += NodeEdgeDistances(place);
  return tmp;
}

//===========================================================================
void Swap(place_t **place, place_t **new_place)
{
  place_t * tmp_ptr;

  tmp_ptr = *place;
  *place = *new_place;
  *new_place = tmp_ptr;
}

//===========================================================================
float Schedule(float temp)
{
	if (temp > (float)(INIT_TEMP / 2))
	  return (float)(temp = 0.7 * temp);
	else
	  return (float)(temp = 0.8 * temp);
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
void DrawGraphx(place_t *place[])
{
	int i,j, index1, index2,MinCost_index=0;
	struct node_t * next;
	char tmp_str[5];
	float cost[12];
		cleardevice();
	for(i=0;i<12;i++)
		cost[i]=Cost(place[i],i);
	for(i=1;i<12;i++)
		if (cost[i]<cost[MinCost_index])
			MinCost_index=i;

//	j=MinCost_index;
	for (j=0; j<12 ;j++)
		for (i=0; i<number_of_nodes; i++)
		{
			 setcolor(YELLOW);
			 circle(place[j][i].x, place[j][i].y, 2);
		//    sprintf(tmp_str, "%d", i+1);
		//    setcolor(YELLOW);
		//    outtextxy(place[i].x-1, place[i].y-2, tmp_str);
		  }
		  setcolor(WHITE);
	  for (j=0; j<12 ;j++)
		if (j!=MinCost_index)
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
//===========
		 setcolor(RED);
		j=MinCost_index;
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
  float delta_cost[12];
  float cost[12], new_cost[12];
  float temp;
  float a_random, expo;
  float RadiusX[12], RadiusY[12];
  place_t *place[12], *new_place[12];
  int i, k,w,c;
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
  for (int r=0; r<=2 ; r++)
	  for (int c=0; c<=3 ; c++)
	  {
	  Top[r*4+c] = (Ymax/3)*r;
	  Left[r*4+c] = (Xmax/4)*c;
	  Right[r*4+c] = (Xmax/4)*(c+1);
	  Button[r*4+c] = (Ymax/3)*(r+1);
	  }
	  randomize();
	for(w=0;w<=11;w++)
	{
	  place[w] = (place_t *)malloc(number_of_nodes*sizeof(place_t));
	  new_place[w] = (place_t *)malloc(number_of_nodes*sizeof(place_t));
	  temp = INIT_TEMP;

	  Init_Placement(place[w], number_of_nodes,w);
	  cost[w] = Cost(place[w],w);
//	  DrawGraph(place[w]);getch();
	//outtextxy(0, Ymax-10, "Initial Drawing, press any key to continue.");
	//DrawGraph(place);

	}
	  DrawGraphx(place);getch();
//   printf("%s started. \n", argv[k]);


//	{ temp=INIT_TEMP;
	  while (temp > FINAL_TEMP)
	  {
		for(w=0;w<1;w++)  //0--12
		{
			RadiusX[w] = (float)(((Right[w]-Left[w])/2-50)*(temp-FINAL_TEMP)/(INIT_TEMP-FINAL_TEMP)+50);
			RadiusY[w] = (float)(((Button[w]-Top[w])/2-50)*(temp-FINAL_TEMP)/(INIT_TEMP-FINAL_TEMP)+50);
//			RadiusX[w] = (float)((Right[w] - Left[w])/2);
//			RadiusY[w] = (float)((Button[w] - Top[w])/2);
			while ( inner_loop_criterion == 0 )
			{
			  Perturb(place[w], new_place[w], RadiusX[w], RadiusY[w],w);
			  //delta_cost = Cost(new_place) - Cost(place);
			  new_cost[w] = Cost(new_place[w],w);
			  delta_cost[w] = new_cost[w] - cost[w];
			  if (delta_cost[w] < 0.0)
			  {
				Swap(&place[w], &new_place[w]);
				cost[w] = new_cost[w];
		 //		DrawGraph(place[w]);
			  }
			  else
			  {
				a_random = (long double)((long double)rand()/RAND_MAX);
				expo = (long double)exp((long double)((-delta_cost[w])/temp));
				if (expo == HUGE_VAL)
				{
				  printf("There is an error!\n");
				  continue;
				}
				if (a_random < (expo))
				{
				  Swap(&place[w], &new_place[w]);
				  cost[w] = new_cost[w];
		 //		  DrawGraph(place[w]);
				}
			  }//else
			}//while
			inner_loop_criterion = 0;
		 }//for
			temp = Schedule(temp);
			DrawGraphx(place);
	  }//while

	/**/	 DrawGraphx(place);
	  outtextxy(0, Ymax-10, "press any key to quit:");
	  getch();
	  CloseGraph();
	  free(place[w]);
	  free(new_place[w]);
	  for (i=0; i<number_of_nodes; i++)
		RemoveAll(adjacenty_list[i]);
	  free(adjacenty_list);
	  fprintf(output_file, "The SA Algorithm result: %s  cost= %f , crossings= %d \n", argv[k],	cost, number_of_edge_crossings);
	  printf("%s finished.\n", argv[k]);
	  fflush(output_file);
  }
  //CloseGraph();
  fclose(output_file);
} /* Main */
