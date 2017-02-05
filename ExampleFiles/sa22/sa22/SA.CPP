/* SA 22 */
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <values.h>
#include <string.h>
#include <graphics.h>
#include <dos.h>

/* Constants */
//#define DEBUG  //For debuging, it should be turn on.
#define	END_LINE	      -1
#define  HEAD_LINES_NUM  1
#define  MAX_FNAME	    100
#define  LINE_LEN	       80
#define  ESC_CODE        27

/*
Types of tree nodes
1.      Ordinary= 0
2.      Bend    = 1
3.      Cross   = 2
4.      Virtual = 3 (these nodes should be omitted at the last)
*/
#define ORDINARY	0
#define BEND		1
#define CROSS		2
#define VIRTUAL	3
/* Parameters */
int MAX_TREE_NODES	= 0;
int MAX_SKEL_NODES	= 0;


float ITERATION	= 0;	       /* 30 moves per node */ //[30]

float G_min	= 0;
float LANDA1	= 0;	// Node distribution [10000]
float LANDA2	= 0;	// Border line
float LANDA3	= 0;	// Edge len [10]
float LANDA5	= 0;	// Node-Edge distribution

//float LANDA4  	= (LANDA5/(G_min*G_min));  // Edge Crossing
float LANDA4  	= 0;	// Edge Crossing

// Are meaningless
//float LANDA6	  = 1000;          // Hole Crossing
//float LANDA7   = 10000;        // Being in Hole

float LANDA8  	= 0;    // Closeness of Tree to Skeleton
float FACTOR	= 1;   // LANDA3 is multiplied by this factor, for virtual nodes.


/* Structures Definition */

struct node_t
{
	int	node_num;
	node_t *next;
};

struct polygon_node_t
{
   int node_num;
   float x, y;
   polygon_node_t * next, *prev;
};

struct polygon_node_list_t
{
   int number_of_nodes;
   polygon_node_t * head, *tail;
};

struct crossing_node_t
{
   polygon_node_t * node1, * node2; // begin & end nodes of crossing
				    // edge of polygon
   float x, y;			    // coordinations of crossing point
   crossing_node_t * next, * prev;  // links to next and previous elements
				    // of the list
};

struct xy_node_t
{
   int node_num;
   float x, y;
   xy_node_t * next;
};

/*struct skel_node_t
{
   int	node_num;
   skel_node_t *next;
};*/

struct skel_nodes_t
{
   float x, y;
   int element;
   //skel_node_t * neighbors;
   node_t * neighbors;
};

/*struct tree_node_t
{
   int	node_num;
   tree_node_t *next;
};*/

struct tree_nodes_t
{
   int element;
   //tree_node_t * neighbors;
   node_t * neighbors;
   short int type;
};

struct point_list_t
{
	float x, y;
	point_list_t *next;
};

struct	point_t
{
	//int	x,y; NEW 82/2/24
	float x, y;
};

typedef struct point_t place_t;

/* Global Variables */
//struct node_t * * adjacenty_list; //???
//struct node_t * * child_list; FELLAN
//struct node_t * * parent_list; FELLAN
//struct point_list_t * * holes; FELLAN

//int use_skel_info = 1;

int number_of_tree_nodes = 0;
int number_of_ordinary_tree_nodes = 0;

int number_of_skel_nodes = 0;
//int number_of_polygon_nodes = 0; NEW
int number_of_fix_nodes = 0;
int number_of_prelocated_nodes = 0;


/* begin Criteria
int total_edge_crossing = 0;	// total number of edge crossings

float total_edge_length = 0;	// sum of all egdge lengths
float mean_edge_length = 0;	// mean of edge length
float edge_length_deviation = 0;	// standard deviation of edge lengths
float max_edge_length = 0;	// maximum edge length

int total_edge_bend = 0;	// sum of all egdge bends
int mean_edge_bend = 0;		// mean of edge bends
int edge_bend_deviation = 0;	// standard deviation of edge bends
int max_edge_bend = 0;		// maximum edge bends

float min_distance = 0;		// minimum distance between nodes,
				// edges, bends and crossings
/* end Criteria */

struct skel_nodes_t * skel_nodes_list = NULL;
struct tree_nodes_t * tree_nodes_list = NULL;
//struct polygon_node_t * polygon_nodes_list = NULL;// NEW
struct polygon_node_list_t polygon_nodes_list;
struct xy_node_t * fix_nodes_list = NULL;
struct xy_node_t * prelocated_nodes_list = NULL;


//int has_hole = 0; FELLAN
//int is_rooted_tree = 0; FELLAN

//int number_of_holes = 0; FELLAN

int inner_loop_criterion = 0;
float Xmax, Ymax;
//float MaxPenalty1, MaxPenalty2, MaxPenalty3;
//double Right, Left, Top, Button; // ????
//int X1hole, Y1hole, X2hole, Y2hole;
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//###########################################################################
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/* Prints Nodes List of the Tree */
int Print(struct tree_nodes_t * tree, int tree_nodes_no)
{
	int i, j;
	struct node_t * tmp_ptr;

	if (tree==NULL)
	{
		printf("Tree is empty!, press any key to exit ...\n");
		return -1;
	}
	printf("\nTree Nodes : \n");
	for (i=0; i< tree_nodes_no; i++)
	{
		printf("%d , element: %d \n", i, tree[i].element);
		tmp_ptr = tree[i].neighbors;
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
/* Prints Nodes List of the Skeleton */
int Print(struct skel_nodes_t * skel, int skel_nodes_no)
{
	int i, j;
	struct node_t * tmp_ptr;

	if (skel==NULL)
	{
		printf("Skeleton is empty!, press any key to exit ...\n");
		return -1;
   }
   printf("\nSkeleton Nodes : \n");
   for (i=0; i< skel_nodes_no; i++)
   {
      printf("%d , element: %d , (%.1f , %.1f)\n", i, skel[i].element,
      skel[i].x, skel[i].y);
		tmp_ptr = skel[i].neighbors;
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
   if (tmp_ptr==NULL)
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
/* Prints Fix Nodes List */
int Print(struct xy_node_t * nodes_list)
{
	int i, j;
	struct xy_node_t * tmp_ptr;

	if (nodes_list==NULL)
	{
		printf("Fix Nodes List is empty!, press any key to exit ...\n");
		return -1;
	}
	printf("\nFix Nodes:\n");
	while (nodes_list)
	{
		printf("Node %d (%.1f , %.1f) ", nodes_list->node_num,
	 nodes_list->x, nodes_list->y);
		nodes_list = nodes_list->next;
	}
	printf("\nPress any key ...\n");
	getch();
	return 0;
}
//============================================================================
/* Adds a node to the end of a nodes list */
int AddTo(node_t * * nodes_list, int node_num)
{
	node_t * tmp_ptr;

	tmp_ptr = *(nodes_list);
	if (tmp_ptr==NULL)
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
int AddTo(point_list_t * * path_list, float x, float y)
{
	point_list_t * tmp_ptr;

	tmp_ptr = *(path_list);
	if (tmp_ptr==NULL)
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
int AddTo(struct polygon_node_list_t * nodes_list, int node_num, float x, float y)
{
	polygon_node_t * tmp_ptr;

	tmp_ptr = nodes_list->head;
	if (tmp_ptr==NULL)
	{
		nodes_list->head = (polygon_node_t *)malloc(sizeof(polygon_node_t));
		if (nodes_list->head == NULL)
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
int AddTo(struct xy_node_t * * nodes_list, int node_num, float x, float y)
{
	xy_node_t * tmp_ptr;

	tmp_ptr = *(nodes_list);
	if (tmp_ptr==NULL)
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
//============================================================================
int Give_Inputs(char * input)
{
	FILE	*input_file;
	FILE	*parent_file;
	FILE	*prms_file;
	char   a_line[LINE_LEN];
	char   tmp_str[20];
	char	prms_name[100];
	char   type[6];
	char	choice, input_name[100], parent_name[100];
	int	   node_num, i, j, index;
//	int	Xhole, Yhole; FELLAN
	int   ch, len;
	int	first, first_node;
	float x, y;
	int node_type;

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

	fscanf(prms_file, "%f", &MAX_TREE_NODES);
	fscanf(prms_file, "%f", &MAX_SKEL_NODES);
	fscanf(prms_file, "%f", &LANDA3_I); // At now, it is not used
	fscanf(prms_file, "%f", &LANDA3_F); // At now, it is not used
	fscanf(prms_file, "%f", &LANDA1_I); // At now, it is not used
	fscanf(prms_file, "%f", &LANDA1_F); // At now, it is not used

	fscanf(prms_file, "%f", &INIT_TEMP1);
	fscanf(prms_file, "%f", &FINAL_TEMP1);
	fscanf(prms_file, "%f", &INIT_TEMP2);
	fscanf(prms_file, "%f", &FINAL_TEMP2);

	fscanf(prms_file, "%f", &ITERATION);

	fscanf(prms_file, "%f", &G_min);
	fscanf(prms_file, "%f", &LANDA1); // Node distribution
	fscanf(prms_file, "%f", &LANDA2); // Border
	fscanf(prms_file, "%f", &LANDA3); // Edge-len
	fscanf(prms_file, "%f", &LANDA5); // Node-Edge distribution

	fscanf(prms_file, "%f", &LANDA4); // Corssing

	fscanf(prms_file, "%f", &LANDA8); //closeness to skeleton

	fscanf(prms_file, "%f", &FACTOR);

// fscanf(prms_file, "%d", &use_skel_info);

	fclose(prms_file);
  /*  End of reading parameters */


	if ((input_file = fopen(input_name, "rb")) == NULL)
	{
		//clrscr();
		printf("Error : I can not read the input file!");
		return -2;
	}

	number_of_tree_nodes = 0;
	number_of_ordinary_tree_nodes = 0;
	number_of_skel_nodes = 0;
	//number_of_polygon_nodes = 0; NEW
	polygon_nodes_list.number_of_nodes = 0;
	number_of_fix_nodes = 0;
	number_of_prelocated_nodes = 0;

	tree_nodes_list = (struct tree_nodes_t *)(malloc(MAX_TREE_NODES*sizeof(struct tree_nodes_t)));
	if (tree_nodes_list == NULL)
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

	for (i=0; i<MAX_TREE_NODES; i++)
	{
		tree_nodes_list[i].element = i;
		tree_nodes_list[i].neighbors = NULL;
		tree_nodes_list[i].type = ORDINARY;
	}


	for (i=0; i<MAX_SKEL_NODES; i++)
	{
		skel_nodes_list[i].x = 0;
		skel_nodes_list[i].y = 0;
		skel_nodes_list[i].element = i;
		skel_nodes_list[i].neighbors = NULL;
	}

	/* Following instructions skip unusefull informations */
	for (i=0; i < HEAD_LINES_NUM+1; i++)
		fgets(a_line, LINE_LEN, input_file);

	/* Constructs Polygon Nodes list */
	while (a_line[0] != '%')
	{
		sscanf(a_line, "%d %f %f", &node_num, &x, &y);
		if (AddTo(&polygon_nodes_list, node_num, x, y) < 0)
		{
			printf("GiveInput: I can't add to the list!, press any key to quit ... ");
			getch();
			return -1;
		}
		fgets(a_line, LINE_LEN, input_file);
		//number_of_polygon_nodes++; NEW
	}

  /* Constructs Nodes List of the Skeleton */
	fgets(a_line, LINE_LEN, input_file);
	while (a_line[0] != '%')
	{

		sscanf(a_line, "%d %f %f", &node_num, &x, &y);
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
			//sscanf(a_line, "%d", &node_num);
			sget(a_line, ++i, tmp_str);
			node_num = atoi(tmp_str);
		}
		fgets(a_line, LINE_LEN, input_file);
		number_of_skel_nodes++;
	}

	/* Constructs Nodes List of the Tree */
	fgets(a_line, LINE_LEN, input_file);
	while (a_line[0] != '%')
	{
		sscanf(a_line, "%d %d", &node_type, &node_num);
		index = number_of_tree_nodes;
		tree_nodes_list[index].element = node_num;
		tree_nodes_list[index].neighbors = NULL;
		tree_nodes_list[index].type = node_type;
		if (node_type == ORDINARY)
			number_of_ordinary_tree_nodes++;
		//sscanf(a_line, "%d", &node_num);
		i = 3;
		sget(a_line, i, tmp_str);
		node_num = atoi(tmp_str);
		while (node_num != END_LINE)
		{
			AddTo(&(tree_nodes_list[index].neighbors), node_num);
			//sscanf(a_line, "%d", &node_num);
			sget(a_line, ++i, tmp_str);
			node_num = atoi(tmp_str);
		}
		fgets(a_line, LINE_LEN, input_file);
		number_of_tree_nodes++;
	}

	/* Constructs Prelocated Nodes List of the Tree */
	fgets(a_line, LINE_LEN, input_file);
	while (a_line[0] != '%')
	{
		sscanf(a_line, "%d %f %f", &node_num, &x, &y);
		index = number_of_prelocated_nodes;
		AddTo(&prelocated_nodes_list, node_num, x, y);
		fgets(a_line, LINE_LEN, input_file);
		number_of_prelocated_nodes++;
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
//============================================================================
/*
  The Centroid is not allways in interior of the polygon!
  So we select a node (not vertex) of the skeleton as
  an intrior point of the polygon.

  search skeleton nodes list
  remove ones there are in polygon list.
  select one of them as the intrior point.
*/
int FindAnInteriorPoint(float & XX, float &YY)
{
	int i;
	struct polygon_node_t * tmp_ptr;

	for (i=0; i<number_of_skel_nodes; i++)
	{
		tmp_ptr = polygon_nodes_list.head;
		while (tmp_ptr)
		{
			if (tmp_ptr->node_num == skel_nodes_list[i].element)
				break;
			tmp_ptr = tmp_ptr->next;
		}
		if (tmp_ptr == NULL)
		{
			XX = skel_nodes_list[i].x;
			YY = skel_nodes_list[i].y;
			return 0;
		}
	}
	printf("I can not find any interior point for polygon!\n");
	return -1;
}
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
/*
This function puts the nodes of the given tree at random
locations in the interior of the given rectangle.

int Init_Placement1(place_t *place, int n)
{
	int i;

	if (number_of_tree_nodes)
	{
		MaxPenalty1 = (MAXLONG/(number_of_tree_nodes-1));
		MaxPenalty2 = (MAXLONG/(number_of_tree_nodes-1)*(number_of_tree_nodes-1));
		MaxPenalty3 = (MAXLONG/(number_of_tree_nodes-1)*(number_of_tree_nodes-1)
			*(number_of_tree_nodes-1));
	}
	else
	{
		MaxPenalty1 = MAXLONG;
		MaxPenalty2 = MAXLONG;
		MaxPenalty3 = MAXLONG;
	}
	randomize();
	for (i=0; i < n; i++)
	{
		// It will bet difficult to determine if a point is in a polygon!
		// It itself requires an algorithm.
		// So we don use this function.
		place[i].x = (int)( Left + (float)(rand() * (Right - Left)) / RAND_MAX );
		place[i].y = (int)( Top + (float)(rand() * (Button - Top)) / RAND_MAX );
	}
	return 0;
}
*/
//============================================================================
/*
	Puts all non-fixed nodes at one interior location of
	the given polygon. (It works just for convex polygons. ?)
	I think it should also work for non-convex polygons,
	but is not very good.
*/
int Init_Placement2(place_t *place, int n)
{
	int i;
	float XX, YY;

	if (FindAnInteriorPoint(XX, YY) < 0)
		return -1;

	#ifdef DEBUG
		char tmp_str[5];
		sprintf(tmp_str, "%.1f", XX);
		outtextxy(0, Ymax-10, "interior x: ");
		outtextxy(150, Ymax-10, tmp_str);
		sprintf(tmp_str, "%.1f", YY);
		outtextxy(300, Ymax-10, "interior y: ");
		outtextxy(450, Ymax-10, tmp_str);
		getch();
	#endif

	for (i=0; i < n; i++)
	{
		// IsFixed Checks if node i is fixed then sets its x and y, and
		// return 1; otherwise return 0.
		if ((!IsFixed(i, &(place[i]))) && (!IsPrelocated(i, &(place[i]))))
		{
			place[i].x = (int)XX;
			place[i].y = (int)YY;
		}
	}
	return 0;
}
//============================================================================
int HasLocatedNeighbor(int node, int * is_located, int &neighbor)
{
	node_t * neighbors = tree_nodes_list[node].neighbors;
	while (neighbors)
	{
		neighbor = neighbors->node_num;
		if (is_located[neighbor])
			return 1;
		neighbors = neighbors->next;
	}
	return 0;
}
//============================================================================
/*
  This function works for convex & rectilinear polygons.
  It puts all non-fixed nodes at the location of their
  first fixed predecessor.
*/
int Init_Placement3(place_t *place, int n)
{
	int    i, unlocateds, neighbor;
	int  * is_located;
	float  XX, YY;

	is_located = (int *)malloc(n*sizeof(int));
	unlocateds = 0;
	for (i=0; i < n; i++)
	{
		if (IsFixed(i, &(place[i])) || IsPrelocated(i, &(place[i])))
			is_located[i] = 1;
		else
		{
			is_located[i] = 0;
			unlocateds++;
		}
	}

	if (unlocateds == n)
	{
		free(is_located);
		if (FindAnInteriorPoint(XX, YY) < 0)
			return -1;
		for (i=0; i < n; i++)
		{
			place[i].x = (int)XX;
			place[i].y = (int)YY;
		}
		return 0;
	}

	while (unlocateds)
	{
		for (i=0; i < n; i++)
		{
			if (!is_located[i])
			{
				if (HasLocatedNeighbor(i, is_located, neighbor))
				{
					is_located[i] = 1;
					place[i].x = place[neighbor].x;
					place[i].y = place[neighbor].y;
					unlocateds--;
				}
			}
		}
	}
	free(is_located);
	return 0;
}
//============================================================================
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
	float X1, Y1, X2, Y2, x1, y1, x2, y2;

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
	place_t * tree_node_ptr1,
	place_t * tree_node_ptr2)
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
		if (has_crossing(tree_node_ptr1, tree_node_ptr2, p3, p4))
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
int RemoveTreeEdge(int node1, int node2)
{
	node_t * neighbor2;
	node_t * neighbor = tree_nodes_list[node1].neighbors;

	if (neighbor->node_num == node2)
	{
		tree_nodes_list[node1].neighbors = neighbor->next;
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
int update_tree_place(int node1, int node2, point_list_t * path_list,place_t * place)
{
	node_t * neighbor;
	point_list_t  *t2; //*t1;

	if (!path_list)
		return 1;

	//remove tree edge node1 --> node2
	if (RemoveTreeEdge(node1, node2))
		return 1;

	//remove tree edge node2 --> node1
	if (RemoveTreeEdge(node2, node1))
		return 1;

	//create new node t
	tree_nodes_list[number_of_tree_nodes].element = number_of_tree_nodes;
	tree_nodes_list[number_of_tree_nodes].neighbors = NULL;
	tree_nodes_list[number_of_tree_nodes].type = BEND;

	//set t = path_list
	place[number_of_tree_nodes].x = path_list->x;
	place[number_of_tree_nodes].y = path_list->y;

	//add node1 -->t
	AddTo(&(tree_nodes_list[node1].neighbors), number_of_tree_nodes);

	//add t -->node1
	AddTo(&(tree_nodes_list[number_of_tree_nodes].neighbors), node1);

	number_of_tree_nodes++;

	//t1 = path_list;
	t2 = path_list->next;

	while (t2)
	{
	  //create a new tree_node and set to t2
	  tree_nodes_list[number_of_tree_nodes].element = number_of_tree_nodes;
	  tree_nodes_list[number_of_tree_nodes].neighbors = NULL;
	  tree_nodes_list[number_of_tree_nodes].type = BEND;
	  place[number_of_tree_nodes].x = t2->x;
	  place[number_of_tree_nodes].y = t2->y;

	  //add t1 -->t2
	  AddTo(&(tree_nodes_list[number_of_tree_nodes-1].neighbors), number_of_tree_nodes);
	  //add t2 -->t1
	  AddTo(&(tree_nodes_list[number_of_tree_nodes].neighbors), number_of_tree_nodes-1);

	  number_of_tree_nodes++;

	  //t1 = t2;
	  t2 = t2->next;
	}

	//add node2 -->t1
	AddTo(&(tree_nodes_list[node2].neighbors), number_of_tree_nodes-1);

	//add t1 -->node2
	AddTo(&(tree_nodes_list[number_of_tree_nodes-1].neighbors), node2);

	return 0;
}
//============================================================================
/*
  This function works for convex & rectilinear polygons.
  It puts all non-fixed and non-prelocated nodes at the
  location of their first fixed or prelocated predecessor.
  Then it replace all edges of the tree which cross the
  polygon with a path that is not crossing
  the polygon.
*/
int Init_Placement4(place_t *place, int n)
{
	int i, j, unlocateds, neighbor;
	int * is_located, * is_fixed_or_prelocated;
	point_list_t * path_tmp_ptr, *tmp_path_list;
	node_t * tmp_ptr;
	point_list_t * path_list = NULL;
	int node1, node2, crossings;
	place_t tmp;
	int XX, YY;
	struct crossing_node_t * tmp_ptr2, * crossing_list = NULL;
	struct crossing_node_t * cross_tmp_ptr1, * cross_tmp_ptr2;

	//printf("INIT 1 \n");
	is_located = (int *)malloc(n*sizeof(int));
	is_fixed_or_prelocated = (int *)malloc(n*sizeof(int));
	unlocateds = 0;
	for (i=0; i < n; i++)
	{
		if (IsFixed(i, &(place[i])) || IsPrelocated(i, &(place[i])))
		{
			is_located[i] = 1;
			is_fixed_or_prelocated[i] = 1;
		}
		else
		{
			is_located[i] = 0;
			is_fixed_or_prelocated[i] = 0;
			unlocateds++;
		}
	}

	//printf("INIT 2 \n");

	if (unlocateds == n)
	{
		free(is_located);
		free(is_fixed_or_prelocated);
		if (FindAnInteriorPoint(XX, YY) < 0)
			return -1;
		for (i=0; i < n; i++)
		{
			place[i].x = (int)XX;
			place[i].y = (int)YY;
		}
		return 0;
	}

	//printf("INIT 3 \n");

	while (unlocateds)
	{
		for (i=0; i < n; i++)
		{
			if (!is_located[i])
			{
				if (HasLocatedNeighbor(i, is_located, neighbor))
				{
					is_located[i] = 1;
					place[i].x = place[neighbor].x;
					place[i].y = place[neighbor].y;
					unlocateds--;
				}
			}
		}
	}
	free(is_located);
	//printf("INIT 4 \n");

	// for all edges of the tree that have two fixed
	// end points, check if they cross the boundary of
	// the polygon.

	for (i=0; i< n; i++)
	{
		node1 = tree_nodes_list[i].element;
		if (tree_nodes_list[i].type == VIRTUAL)
			continue;
		if (is_fixed_or_prelocated[node1])
		{
			tmp_ptr = tree_nodes_list[i].neighbors;
			while(tmp_ptr)
			{
				node2 = tmp_ptr->node_num;
				tmp_ptr = tmp_ptr->next;

				if ((node2 < node1) || (node2 >= n))
					continue;

				if (tree_nodes_list[node2].type == VIRTUAL)
					continue;

				if (is_fixed_or_prelocated[node2])
				{
					//printf("INIT 4.1 \n");
					crossings = find_polygon_corssings(&place[node1], &place[node2], &crossing_list);
					//printf("INIT 4.2 \n");
					if (!crossings)
						continue;

					if (crossings%2)
					{
						 printf("ERROR 1 - There is an error in crossing list!\n");
						 continue;
					}

					// sorting is required for grouping
					// crossing points 2-by-2.
					//printf("INIT 4.3 \n");
					if (place[node1].y == place[node2].y)
						 sort_on_x(crossing_list, crossings);
					else
						sort_on_y(crossing_list, crossings);

					//printf("INIT 4.4 \n");

					tmp_ptr2 = crossing_list;
					while (tmp_ptr2)
					{
						if (tmp_ptr2->next == NULL)
						{
								 printf("ERROR 2 - There is an error in crossing list!\n");
							 break;
						}
						tmp_path_list = NULL;
						//printf("INIT 4.5 \n");
						find_path(tmp_ptr2, tmp_ptr2->next, &tmp_path_list,
											&place[node1], &place[node2]);
						//printf("INIT 4.6 \n");
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

					//printf("INIT 4.7 \n");
					if (place[node1].y == place[node2].y)
					{
						if (place[node1].x < place[node2].x)
							update_tree_place(node1, node2, path_list, place);
						else
							update_tree_place(node2, node1, path_list, place);
					}
					else
					{
						if (place[node1].y < place[node2].y)
							update_tree_place(node1, node2, path_list, place);
						else
							update_tree_place(node2, node1, path_list, place);
					}
					//printf("INIT 4.8 \n");

					free_path_list(&path_list);
					//printf("INIT 4.9 \n");
					free_crossing_list(&crossing_list);
					//printf("INIT 4.10 \n");
				}  //if
			} //while
		}  //if
	} // for
  //printf("INIT 5.0 \n");
  free(is_fixed_or_prelocated);
  //printf("INIT 5 \n");
  return 0;
}
//============================================================================
int HasHoleCrossing(float x1, float y1,
	  float x2, float y2);

int HasPolygonCrossing(float x1, float y1,
	  float x2, float y2);
//============================================================================
int Perturb(place_t * place, place_t *new_place,float RadiusX, float RadiusY)
{
	int k, a_random, ch;
	//node_t * next;
	static int i=0;
	static int j=0;
	float x1, y1, x2, y2;
	int hole_crossing, node_num, neighbor_num, polygon_crossing;
	struct node_t * neighbor;
	place_t tmp;


	for (k=0; k < number_of_tree_nodes; k++)
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
		i=i%number_of_tree_nodes;
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

	//hole_crossing = 1; FELLAN
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
		node_num = tree_nodes_list[i].element;
		if (node_num != i)
		{
			//outtextxy(0, Ymax-10, "The node_number is not correct in the tree nodes list!!!");
			printf("The node_number is not correct in the tree nodes list!!!\n");
			getch();
			return -1;
		}
		if (node_num > number_of_tree_nodes)
		{
			//outtextxy(0, Ymax-10, "(17) The node_number is greater than number_of_tree_nodes!!!");
			printf("(17) The node_number is greater than number_of_tree_nodes!!!\n");
			getch();
			return -1;
		}

		polygon_crossing = 0;
		neighbor = tree_nodes_list[i].neighbors;
		while (neighbor != NULL)
		{
			neighbor_num = neighbor->node_num;
			if (neighbor_num > number_of_tree_nodes)
			{
				//outtextxy(0, Ymax-10, " 18 The node_number is greater than number_of_tree_nodes!!!");
				printf("18 The node_number is greater than number_of_tree_nodes!!!\n");
				getch();
				return -1;
			}
			x2 = place[neighbor_num].x;
			y2 = place[neighbor_num].y;
			if (HasPolygonCrossing(x1, y1, x2, y2))
				polygon_crossing = 1;
			neighbor = neighbor->next;
		}
	}//while(polygon_crossing || ... )

	new_place[i].x = x1;
	new_place[i].y = y1;

	i++;
	i=i%number_of_tree_nodes;
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
	for (i=0; i< (number_of_tree_nodes-1); i++)
		for (j=i+1; j<number_of_tree_nodes; j++)
		{
			if ((tree_nodes_list[i].type == VIRTUAL) ||
					(tree_nodes_list[j].type == VIRTUAL))
				continue;
			Dx = place[i].x - place[j].x;
			Dy = place[i].y - place[j].y;
			distance2 = Dx*Dx + Dy*Dy;
			//if (!distance2)
				//  sum += MaxPenalty2;
			if (distance2 < (G_min*G_min))
				sum += (float)LANDA1 / (G_min*G_min);
			else
				sum += (float)LANDA1 / distance2;
			if (sum < 0)
			{
				//outtextxy(0, Ymax-10, "Node_distribution: !!!!!! sum < 0 !!!!!!!!!");
				printf("Node_distribution: !!!!!! sum < 0 !!!!!!!!!\n");
				getch();
				return 0;
			}
		}
	if (sum < 0)
	{
			//outtextxy(0, Ymax-10, "Node_distribution: !!!!!! sum < 0 !!!!!!!!!");
			printf("Node_distribution: !!!!!! sum < 0 !!!!!!!!!\n");
			getch();
			return 0;
	}
	return sum;
}
//============================================================================
float BorderLines(place_t * place)
{
	int i, j;
	int node_num, neighbor_num;
	float sum, Ri, Li, Ti, Bi;
	struct polygon_node_t *p1, *p2;
	struct node_t * neighbor;
	float x1, y1, x2, y2, x3, y3;
	float distance2, d1, d2, u;

   sum = 0;

   /* Penalty distance of Borders of the polygon */
   for (i=0; i<number_of_tree_nodes; i++)
   {
      if (tree_nodes_list[i].type == VIRTUAL)
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
				//sum += MaxPenalty1;
				sum += LANDA2 / (G_min*G_min);
			else
				sum += LANDA2/distance2;

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

		for(j=0; j<number_of_tree_nodes; j++)
		{
			if (tree_nodes_list[j].type == VIRTUAL)
				continue;
			node_num = tree_nodes_list[j].element;
			if (node_num > number_of_tree_nodes)
			{
				//outtextxy(0, Ymax-10, " 9 The node_number is greater than number_of_tree_nodes!!!");
				printf("9 The node_number is greater than number_of_tree_nodes!!!\n");
				getch();
				return sum/2;
			}
			neighbor = tree_nodes_list[j].neighbors;
			while (neighbor != NULL)
			{
				neighbor_num = neighbor->node_num;
				if (neighbor_num > number_of_tree_nodes)
				{
					//outtextxy(0, Ymax-10, " 10 The node_number is greater than number_of_tree_nodes!!!");
					printf("10 The node_number is greater than number_of_tree_nodes!!!\n");
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
					sum += LANDA2 / (G_min*G_min);
				else
					sum += LANDA2/distance2;

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
float EdgeLengths(place_t * place, int phase)
{
	int i, j;
	int node_num;
	//node_t * next;
	int neighbor_num;
	float sum, edge_len2, Dx, Dy;
	struct node_t * neighbor;

	sum = 0.0;
	for (i=0; i<number_of_tree_nodes; i++)
	{
		node_num = tree_nodes_list[i].element;
		if (node_num > number_of_tree_nodes)
		{
			//outtextxy(0, Ymax-10, " 5 The node_number is greater than number_of_tree_nodes!!!");
			printf("5 The node_number is greater than number_of_tree_nodes!!!\n");
			getch();
			return sum;
		}
		neighbor = tree_nodes_list[i].neighbors;
		while (neighbor != NULL)
		{
			neighbor_num = neighbor->node_num;
			if (neighbor_num > number_of_tree_nodes)
			{
				//outtextxy(0, Ymax-10, " 6 The node_number is greater than number_of_tree_nodes!!!");
				printf("6 The node_number is greater than number_of_tree_nodes!!!\n");
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
			if ((tree_nodes_list[neighbor_num].type == VIRTUAL) ||
					 (tree_nodes_list[i].type == VIRTUAL))
				sum += LANDA3*edge_len2*50*FACTOR;
			else
				if ((tree_nodes_list[neighbor_num].type == BEND) ||
						(tree_nodes_list[node_num].type == BEND) )
					sum += LANDA3*edge_len2*10*FACTOR;
				else
					sum += LANDA3*edge_len2;

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
	int i, j;
	int node_num;
	//node_t * next;
	int neighbor_num;
	float edge_len2, Dx, Dy;
	struct node_t * neighbor;

	total = 0.0;
	mean = 0.0;

	for (i=0; i<number_of_tree_nodes; i++)
	{
		node_num = tree_nodes_list[i].element;
		if (node_num > number_of_tree_nodes)
		{
			//outtextxy(0, Ymax-10, " 5 The node_number is greater than number_of_tree_nodes!!!");
			printf("5 The node_number is greater than number_of_tree_nodes!!!\n");
			getch();
			return -1;
		}

		if (tree_nodes_list[node_num].type == VIRTUAL)
			continue;

		neighbor = tree_nodes_list[i].neighbors;
		while (neighbor != NULL)
		{
			neighbor_num = neighbor->node_num;
			if (neighbor_num > number_of_tree_nodes)
			{
			  //outtextxy(0, Ymax-10, " 6 The node_number is greater than number_of_tree_nodes!!!");
			  printf("6 The node_number is greater than number_of_tree_nodes!!!\n");
			  getch();
			  return -1;
			}

			// each edge should be considered in one direction
			// and virtual nodes should not be considered.
			if ((tree_nodes_list[neighbor_num].type == VIRTUAL) ||
				 (node_num >= neighbor_num))
			{
				neighbor = neighbor->next;
				continue;
			}

			Dx = place[node_num].x - place[neighbor_num].x;
			Dy = place[node_num].y - place[neighbor_num].y;
			edge_len2 = Dx*Dx + Dy*Dy;

			total += sqrt(edge_len2); // I think it's OK

			if (total < 0)
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

	if (number_of_ordinary_tree_nodes > 1)
		mean =  total / (number_of_ordinary_tree_nodes-1);
	else
		mean = 0;
	return 0;
}
//============================================================================
float NextSegmentLength(place_t * place, int node, int forbidden)
{
	struct node_t * neighbor;
	float segment_len2;
	float Dx, Dy;

	if ((node < 0) || (node >= number_of_tree_nodes))
		return 0;

	if ((forbidden < 0) || (forbidden >= number_of_tree_nodes))
		return 0;

	if (tree_nodes_list[node].type != BEND)
		return 0;

	neighbor = tree_nodes_list[node].neighbors;

	if (!neighbor)
		return 0;

	if (neighbor->node_num == forbidden)
		neighbor = neighbor->next;

	if (!neighbor)
		return 0;

	if (tree_nodes_list[neighbor->node_num].type == VIRTUAL)
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
	int i, j;
	int node_num;
	//node_t * next;
	int neighbor_num;
	float edge_len2, edge_len, Dx, Dy;
	struct node_t * neighbor;

	max = 0.0;
	deviation = 0.0;

	for (i=0; i<number_of_tree_nodes; i++)
	{
		node_num = tree_nodes_list[i].element;
		if (node_num > number_of_tree_nodes)
		{
			//outtextxy(0, Ymax-10, " 5 The node_number is greater than number_of_tree_nodes!!!");
			printf("5 The node_number is greater than number_of_tree_nodes!!!\n");
			getch();
			return -1;
		}

		if (tree_nodes_list[node_num].type != ORDINARY)
			continue;

		neighbor = tree_nodes_list[i].neighbors;
		while (neighbor != NULL)
		{
			neighbor_num = neighbor->node_num;
			if (neighbor_num > number_of_tree_nodes)
			{
				//outtextxy(0, Ymax-10, " 6 The node_number is greater than number_of_tree_nodes!!!");
				printf("6 The node_number is greater than number_of_tree_nodes!!!\n");
				getch();
				return -1;
			}

			// each edge should be considered in one direction
			// and virtual nodes should not be considered.
			if (tree_nodes_list[neighbor_num].type == VIRTUAL)
			{
				neighbor = neighbor->next;
				continue;
			}

			Dx = place[node_num].x - place[neighbor_num].x;
			Dy = place[node_num].y - place[neighbor_num].y;
			edge_len2 = Dx*Dx + Dy*Dy;

			edge_len = sqrt(edge_len2); // I think it's OK

			if ((tree_nodes_list[neighbor_num].type == BEND))
				 //|| (tree_nodes_list[neighbor_num].type == CROSS)
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

	if (number_of_ordinary_tree_nodes > 1)
	{
		deviation = deviation / (2*(number_of_ordinary_tree_nodes-1));
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
	for (i=0; i < number_of_tree_nodes; i++)
		if (tree_nodes_list[i].type == BEND)
			total++;

	if (number_of_ordinary_tree_nodes > 1)
		mean = total / (number_of_ordinary_tree_nodes - 1);
	else	mean = 0;

	return 0;
}
//============================================================================
int NextEdgeBends(int node, int forbidden)
{
	struct node_t * neighbor;

	if ((node < 0) || (node >= number_of_tree_nodes))
	return 0;

	if ((forbidden < 0) || (forbidden >= number_of_tree_nodes))
	return 0;

	if (tree_nodes_list[node].type != BEND)
	return 0;

	neighbor = tree_nodes_list[node].neighbors;

	if (!neighbor)
	return 0;

	if (neighbor->node_num == forbidden)
	neighbor = neighbor->next;

	if (!neighbor)
	return 0;

	if (tree_nodes_list[neighbor->node_num].type == VIRTUAL)
	neighbor = neighbor->next;

	if (!neighbor)
	return 0;

	return (1 + NextEdgeBends(neighbor->node_num, node));
}
//============================================================================
int MaxDeviationEdgeBend(float mean,
	int & max, float & deviation)
{
	int i, j;
	int node_num;
	//node_t * next;
	int neighbor_num;
	float bends, Dx, Dy;
	struct node_t * neighbor;

	max = 0.0;
	deviation = 0.0;

	for (i=0; i<number_of_tree_nodes; i++)
	{
		node_num = tree_nodes_list[i].element;
		if (node_num > number_of_tree_nodes)
		{
			//outtextxy(0, Ymax-10, " 5 The node_number is greater than number_of_tree_nodes!!!");
			printf("5 The node_number is greater than number_of_tree_nodes!!!\n");
			getch();
			return -1;
		}

		if (tree_nodes_list[node_num].type != ORDINARY)
			continue;

		neighbor = tree_nodes_list[i].neighbors;
		while (neighbor != NULL)
		{
			neighbor_num = neighbor->node_num;
			if (neighbor_num > number_of_tree_nodes)
			{
				  //outtextxy(0, Ymax-10, " 6 The node_number is greater than number_of_tree_nodes!!!");
				  printf("6 The node_number is greater than number_of_tree_nodes!!!\n");
				  getch();
				  return -1;
			}

			// virtual nodes should not be considered.
			if (tree_nodes_list[neighbor_num].type == VIRTUAL)
			{
				neighbor = neighbor->next;
					continue;
			}

			bends = 0;

			if ((tree_nodes_list[neighbor_num].type == BEND))
				 //|| (tree_nodes_list[neighbor_num].type == CROSS)
				 // we don't consider crosses as nodes in this version
				bends += NextEdgeBends(neighbor_num, node_num);

			if (bends > max)
				max = bends;

			deviation += (bends - mean)*(bends - mean);

			neighbor = neighbor->next;
		}
	}

	if (number_of_ordinary_tree_nodes > 1)
	{
			deviation = deviation / (2*(number_of_ordinary_tree_nodes-1));
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
	for (i=0; i< (number_of_tree_nodes-1); i++)
		for (j=i+1; j<number_of_tree_nodes; j++)
		{
			if ((tree_nodes_list[i].type == VIRTUAL) ||
					(tree_nodes_list[j].type == VIRTUAL))
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

	int node_num, neighbor_num;
	float x1, y1, x2, y2, x3, y3;
	float distance2, sum, d1, d2, u;
	struct node_t * neighbor;

	sum = 0;

	// Considers Closeness of nodes of tree to edges of skeleton
	for (i=0; i<number_of_tree_nodes; i++)
	{
		x3 = place[i].x;
		y3 = place[i].y;

		if (tree_nodes_list[i].type == VIRTUAL)
			continue;

		for(j=0; j<number_of_skel_nodes; j++)
		{
			node_num = skel_nodes_list[j].element;
			if (node_num > number_of_skel_nodes)
			{
				//outtextxy(0, Ymax-10, " 7 The node_number is greater than number_of_tree_nodes!!!");
				printf("7 The node_number is greater than number_of_tree_nodes!!!\n");
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
						//outtextxy(0, Ymax-10, " 8 The node_number is greater than number_of_tree_nodes!!!");
						printf("8 The node_number is greater than number_of_tree_nodes!!!\n");
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

	// Considers Closeness of nodes of skeleton to edges of tree
	for (i=0; i<number_of_skel_nodes; i++)
	{
		x3 = skel_nodes_list[i].x;
		y3 = skel_nodes_list[i].y;

		for(j=0; j<number_of_tree_nodes; j++)
		{
			node_num = tree_nodes_list[j].element;
			if (node_num > number_of_tree_nodes)
			{
				//outtextxy(0, Ymax-10, " 7 The node_number is greater than number_of_tree_nodes!!!");
				printf("7 The node_number is greater than number_of_tree_nodes!!!\n");
				getch();
				return sum/2;
			}
			if ( node_num != i )
			{
				neighbor = tree_nodes_list[j].neighbors;
				while (neighbor != NULL)
				{
					neighbor_num = neighbor->node_num;
					if (neighbor_num > number_of_tree_nodes)
					{
						//outtextxy(0, Ymax-10, " 8 The node_number is greater than number_of_tree_nodes!!!");
						printf("8 The node_number is greater than number_of_tree_nodes!!!\n");
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
	//node_t * next;
	//corner_t * p1, *p2;

	int node_num, neighbor_num;
	float x1, y1, x2, y2, x3, y3;
	float distance2, sum, d1, d2, u;
	struct node_t * neighbor;

	sum = 0;
	for (i=0; i<number_of_tree_nodes; i++)
	{
		if (tree_nodes_list[i].type == VIRTUAL)
			continue;

		x3 = place[i].x;
		y3 = place[i].y;

		for(j=0; j<number_of_tree_nodes; j++)
		{
			if (tree_nodes_list[j].type == VIRTUAL)
				continue;
			node_num = tree_nodes_list[j].element;
			if (node_num > number_of_tree_nodes)
			{
				//outtextxy(0, Ymax-10, " 7 The node_number is greater than number_of_tree_nodes!!!");
				printf("7 The node_number is greater than number_of_tree_nodes!!!\n");
				getch();
				return sum/2;
			}
			if ( node_num != i )
			{
				neighbor = tree_nodes_list[j].neighbors;
				while (neighbor != NULL)
				{
					neighbor_num = neighbor->node_num;
					if (tree_nodes_list[neighbor_num].type == VIRTUAL)
					{
						neighbor = neighbor->next;
							continue;
					}
					if (neighbor_num > number_of_tree_nodes)
					{
						 //outtextxy(0, Ymax-10, " 8 The node_number is greater than number_of_tree_nodes!!!");
						 printf("8 The node_number is greater than number_of_tree_nodes!!!\n");
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
								distance2 =(x3-x1)*(x3-x1)+(y3-y1)*(y3-y1);
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
							}//else
						}//if(d1==0)
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
						 if (distance2 < (G_min*G_min))
							 sum += LANDA5 / (G_min*G_min); // it was before this LANDA4 !
						 else
							sum += LANDA5/distance2;
						 if (sum < 0)
						 {
								//outtextxy(0, Ymax-10, "EdgeNode_distance: !!!!!! sum < 0 !!!!!!!!!");
								printf("EdgeNode_distance: !!!!!! sum < 0 !!!!!!!!!\n");
								getch();
								return 0;
						 }
				  }//if(neighbor_num!=i)
				  neighbor = neighbor->next;
				}//while(neighbor!=NULL)
			}//if(node_num!=i)
		}//for(j .....)
	}//for(i .....)
	if (sum < 0)
	{
		//outtextxy(0, Ymax-10, "NodeEdgeDistance: !!!!!! sum < 0 !!!!!!!!!");
		printf("NodeEdgeDistance: !!!!!! sum < 0 !!!!!!!!!\n");
		getch();
		return 0;
	}
	return sum/2; /* Note That each edge repreats two times */
}
//============================================================================
float EdgeCrossings(place_t * place)
{
	int i, j, node_numi, node_numj;
	int neighbor_numi, neighbor_numj;
	//node_t * next1, *next2;
	struct node_t *neighbor1, *neighbor2;
	float x1, y1, x2, y2, X1, Y1, X2, Y2;
	float u1, u2, sum, d;

	sum = 0;
	for (i=0; i<(number_of_tree_nodes-1); i++)
	{
		node_numi = tree_nodes_list[i].element;
		if (node_numi > number_of_tree_nodes)
		{
			//outtextxy(0, Ymax-10, " 1 The node_number is greater than number_of_tree_nodes!!!");
			printf("1 The node_number is greater than number_of_tree_nodes!!!\n");
			getch();
			return sum;
		}

		if (tree_nodes_list[node_numi].type == VIRTUAL)
			continue;

		for (j=i+1; j<number_of_tree_nodes; j++)
		{
			node_numj = tree_nodes_list[j].element;
			if (node_numj > number_of_tree_nodes)
			{
			  //outtextxy(0, Ymax-10, " 2 The node_number is greater than number_of_tree_nodes!!!");
				printf("2 The node_number is greater than number_of_tree_nodes!!!\n");
				getch();
				return sum;
			}

			if (tree_nodes_list[node_numj].type == VIRTUAL)
				continue;

			if (node_numi == node_numj)
			{
				//clrscr();
				//outtextxy(0, Ymax-10, "Error E1: Two Col of the Edge List are equal!");
				printf("Error E1: Two Col of the Edge List are equal!\n");
				getch();
				return 0;
			}
			neighbor1 = tree_nodes_list[i].neighbors;
			while (neighbor1 != NULL)
			{
				neighbor_numi = neighbor1->node_num;

				if (neighbor_numi > number_of_tree_nodes)
				{
				//outtextxy(0, Ymax-10, " 3 The node_number is greater than number_of_tree_nodes!!!");
					printf("3 The node_number is greater than number_of_tree_nodes!!!\n");
				getch();
				return sum;
				}

				if (tree_nodes_list[neighbor_numi].type == VIRTUAL)
				{
					neighbor1 = neighbor1->next;
					continue;
				}

				if (neighbor_numi == node_numj) // edges should not be incident
				{
					neighbor1 = neighbor1->next;
					continue;
				}

				neighbor2 = tree_nodes_list[j].neighbors;
				while ( neighbor2 != NULL)
				{
					neighbor_numj = neighbor2->node_num;
					if (neighbor_numj > number_of_tree_nodes)
					{
						//outtextxy(0, Ymax-10, " 4 The node_number is greater than number_of_tree_nodes!!!");
						printf("4 The node_number is greater than number_of_tree_nodes!!!\n");
						getch();
						return sum;
					}

					if (tree_nodes_list[neighbor_numj].type == VIRTUAL)
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
								sum += LANDA4;
							}

							if (sum < 0)
							{
							  //outtextxy(0, Ymax-10, "EdgeCrossing: !!!!!! sum < 0 !!!!!!!!!");
								  printf("EdgeCrossing: !!!!!! sum < 0 !!!!!!!!!\n");
							  getch();
							  return 0;
							}
						}//if(d)
					} //if(.........)
					neighbor2 = neighbor2->next;
			  }//while(neighbor2!=NULL)
			  neighbor1 = neighbor1->next;
			}//while(neighbor1!=NULL)
		}//for(j .....)
	} //for(i ......)
	if (sum < 0)
	{
		//outtextxy(0, Ymax-10, "EdgeCrossing: !!!!!! sum < 0 !!!!!!!!!");
		printf("EdgeCrossing: !!!!!! sum < 0 !!!!!!!!!\n");
		getch();
		return 0;
	}

	//return sum/2; NEW
	return sum;
}
//============================================================================
int TotalEdgeCrossings(place_t * place, int & total)
{
	int i, j, node_numi, node_numj;
	int neighbor_numi, neighbor_numj;
	//node_t * next1, *next2;
	struct node_t *neighbor1, *neighbor2;
	float x1, y1, x2, y2, X1, Y1, X2, Y2;
	float u1, u2, d;

	total = 0;
	for (i=0; i<(number_of_tree_nodes-1); i++)
	{
		node_numi = tree_nodes_list[i].element;
		if (node_numi > number_of_tree_nodes)
		{
			//outtextxy(0, Ymax-10, " 1 The node_number is greater than number_of_tree_nodes!!!");
			printf("1 The node_number is greater than number_of_tree_nodes!!!\n");
			getch();
			return -1;
		}

		if (tree_nodes_list[node_numi].type == VIRTUAL)
			continue;

		for (j=i+1; j<number_of_tree_nodes; j++)
		{
			node_numj = tree_nodes_list[j].element;
			if (node_numj > number_of_tree_nodes)
			{
				//outtextxy(0, Ymax-10, " 2 The node_number is greater than number_of_tree_nodes!!!");
				printf("2 The node_number is greater than number_of_tree_nodes!!!\n");
				getch();
				return -1;
			}

			if (tree_nodes_list[node_numj].type == VIRTUAL)
				continue;

			if (node_numi == node_numj)
			{
				//clrscr();
				//outtextxy(0, Ymax-10, "Error E1: Two Col of the Edge List are equal!");
				printf("Error E1: Two Col of the Edge List are equal!\n");
				getch();
				return -1;
			}
			neighbor1 = tree_nodes_list[i].neighbors;
			while (neighbor1 != NULL)
			{
				neighbor_numi = neighbor1->node_num;

				if (neighbor_numi > number_of_tree_nodes)
				{
					//outtextxy(0, Ymax-10, " 3 The node_number is greater than number_of_tree_nodes!!!");
					printf("3 The node_number is greater than number_of_tree_nodes!!!\n");
					getch();
					return -1;
				}

				if (tree_nodes_list[neighbor_numi].type == VIRTUAL)
				{
					neighbor1 = neighbor1->next;
					continue;
				}

				if (neighbor_numi == node_numj) // edges should not be incident
				{
					neighbor1 = neighbor1->next;
					continue;
				}

				neighbor2 = tree_nodes_list[j].neighbors;
				while ( neighbor2 != NULL)
				{
					neighbor_numj = neighbor2->node_num;
					if (neighbor_numj > number_of_tree_nodes)
					{
						//outtextxy(0, Ymax-10, " 4 The node_number is greater than number_of_tree_nodes!!!");
						printf("4 The node_number is greater than number_of_tree_nodes!!!\n");
						getch();
						return -1;
					}

					if (tree_nodes_list[neighbor_numj].type == VIRTUAL)
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
  return 0;
}
//============================================================================
int IsInPolygon(float x1, float y1)
{
	int crossings=0;
	polygon_node_t  *p1, *p2;
	float u1, u2;
	float X1, Y1, X2, Y2;

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
int HasPolygonCrossing(float x1, float y1,
			  float x2, float y2)
{
	int k;
	polygon_node_t  *p1, *p2;
	float u1, u2, sum, d;
	float X1, Y1, X2, Y2;
	float x_in, y_in, T1, T2, L1, L2, B1, B2, R1, R2, TT1, TT2;

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
/* The Cost Function */
float Cost1(place_t *place)
{
	float tmp;

// tmp  = Node_distribution(place); //OK LANDA1=1000 & expo/1000.0
// tmp += BorderLines(place); //OK LADNDA2=1000 & expo/1000.0
	tmp += EdgeLengths(place, 1); // OK LANDA3 = 0.0005 & expo/1000.0
// tmp += EdgeCrossings(place);// OK LANDA4 = LANDA5/(G_min*G_min), G_min = 4
// tmp += NodeEdgeDistances(place); // OK LANDA5 = 4000
// tmp += HoleCrossings(place); // This statement must be removed
	return tmp;
}
//============================================================================
/* The Cost Function */
float Cost2(place_t *place)
{
	float tmp;

	tmp  = Node_distribution(place); //OK LANDA1=1000 & expo/1000.0
	tmp += BorderLines(place); //OK LADNDA2=1000 & expo/1000.0
	tmp += EdgeLengths(place, 2); // OK LANDA3 = 0.0005 & expo/1000.0
	tmp += EdgeCrossings(place);// OK LANDA4 = LANDA5/(G_min*G_min), G_min = 4
	tmp += NodeEdgeDistances(place); // OK LANDA5 = 4000
// tmp += ClosenessToSkeleton(place); // Under test
												  // It is checked in perturb
	return tmp;
}
//============================================================================
void Swap(place_t **place, place_t **new_place)
{
	place_t * tmp_ptr;

	tmp_ptr    = *place;
	*place     = *new_place;
	*new_place = tmp_ptr;
}
//============================================================================
float Schedule1(float temp)
{

	if (temp > (float)(INIT_TEMP1 / 2))
		temp = (float)(0.60 * (float)temp);
	else
		temp = (float)(0.65 * (float)temp);

	// LANDA3 = ((double)(LANDA3_I - LANDA3_F)/(INIT_TEMP1 - FINAL_TEMP1))*(temp - FINAL_TEMP1)+ LANDA3_F;
	// LANDA1 = ((double)(LANDA1_I - LANDA1_F)/(INIT_TEMP1 - FINAL_TEMP1))*(temp - FINAL_TEMP1)+ LANDA1_F;

	return temp;
}
//============================================================================
float Schedule2(float temp)
{

	if (temp > (float)(INIT_TEMP2 / 2))
		temp = (float)(0.65 * (float)temp);
	else
		temp = (float)(0.75 * (float)temp);

	// LANDA3 = ((double)(LANDA3_I - LANDA3_F)/(INIT_TEMP2 - FINAL_TEMP2))*(temp - FINAL_TEMP2)+ LANDA3_F;
	// LANDA1 = ((double)(LANDA1_I - LANDA1_F)/(INIT_TEMP2 - FINAL_TEMP2))*(temp - FINAL_TEMP2)+ LANDA1_F;

	return temp;
}
//============================================================================
int InitGraph()
{
	int gdriver = DETECT, gmode, errorcode;

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
//============================================================================
void CloseGraph()
{
	closegraph();
}
//============================================================================
void DrawSkeleton(struct skel_nodes_t * skel, int nodes_num)
{
	int i;
	float x1, y1, x2, y2;
	struct node_t * neighbor;
	char tmp_str[10];

	for (i=0; i< nodes_num; i++)
	{
		x1 = skel[i].x;
		y1 = skel[i].y;
		//itoa(i, tmp_str, 10);
		//setcolor(WHITE);
		//outtextxy((int)(4*x1 - 300), (int)y1, tmp_str);
		neighbor = skel[i].neighbors;
		while (neighbor)
		{
			x2 = skel[neighbor->node_num].x;
			y2 = skel[neighbor->node_num].y;
			//itoa(neighbor->node_num, tmp_str, 10);
			//setcolor(WHITE);
			//outtextxy((int)(4*x2 - 300), (int)(y2), tmp_str);
			//setcolor(YELLOW);
			line(x1, y1, x2, y2);
			neighbor = neighbor->next;
		}
	}
}
//============================================================================
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
void DrawTree(place_t * place)
{
	char tmp_str[5];
	struct node_t *neighbor, *neighbor2;;
	int i, k, index1, index2;
	//int x1, y1, x2, y2;
	//corner_t  *p1, *p2;

	cleardevice();
	setcolor(BLUE);
	DrawPolygon(&polygon_nodes_list);
	setcolor(YELLOW);
	DrawSkeleton(skel_nodes_list, number_of_skel_nodes);
	setcolor(WHITE);

	//Draw Tree Edges
	for (i=0; i<number_of_tree_nodes; i++)
	{
		// Does not draw edges of virtual nodes
		if (tree_nodes_list[i].type == VIRTUAL)
			continue;
		neighbor = tree_nodes_list[i].neighbors;
		index1 = tree_nodes_list[i].element;
		while(neighbor != NULL)
		{
			index2 = neighbor->node_num;
			if (tree_nodes_list[index2].type == VIRTUAL)
			{
			//#ifdef DEBUG
				setcolor(RED);
				line((int)place[index1].x, (int)place[index1].y, (int)place[index2].x, (int)place[index2].y);
				setcolor(WHITE);
			//#endif
				neighbor = neighbor->next;
			}
			else
			{
				line((int)place[index1].x, (int)place[index1].y, (int)place[index2].x, (int)place[index2].y);
				neighbor = neighbor->next;
			}
		}
	}

  //Draw Tree Nodes

	for (i=0; i<number_of_tree_nodes; i++)
	{
		if (tree_nodes_list[i].type == BEND)
			continue;
		// Does not draw virtual nodes
		if (tree_nodes_list[i].type == VIRTUAL)
		{
		//#ifdef DEBUG
			setcolor(RED);
			setfillstyle(SOLID_FILL, RED);
			fillellipse((int)place[i].x, (int)place[i].y, 6, 6);
			sprintf(tmp_str, "%d", i);
			setcolor(WHITE);
			setfillstyle(SOLID_FILL, WHITE);
			outtextxy((int)place[i].x-3, (int)place[i].y-4, tmp_str);
		//#endif

	  }
	  else
	  {
			setcolor(WHITE);
			fillellipse((int)place[i].x, (int)place[i].y, 6, 6);
			sprintf(tmp_str, "%d", i);
			setcolor(RED);
			outtextxy((int)place[i].x-3, (int)place[i].y-4, tmp_str);
		}
	}
}
//============================================================================
//############################################################################
//============================================================================
/* Main */
void main(int argc, char * argv[])
{
	float delta_cost, cost, new_cost;
	float temp;
	float a_random, expo;
	float RadiusX, RadiusY;
	place_t *place = NULL, *new_place = NULL;
	int i, ch, idle, k, progress = 0, cx, cy;
	struct xy_node_t * fix_tmp_ptr1, * fix_tmp_ptr2;
	struct xy_node_t * pre_tmp_ptr1, * pre_tmp_ptr2;
	struct polygon_node_t * poly_tmp_ptr1, * poly_tmp_ptr2;
	FILE * output_file;
	struct  dostime_t t1, t2;
	float total_edge_length=0, max_edge_length=0,
	edge_length_deviation=0, mean_edge_length=0, edge_bend_deviation=0,
	mean_edge_bend=0, min_distance=0;
	int total_edge_crossing=0, total_edge_bend=0, max_edge_bend=0;
	long int init_time=0, run_time=0;

	argc=4;
	argv[0]="sa";
	argv[1]="SA";
	argv[2]="sa_alg.txt";
	argv[3]="rec2_t06.ttt";
	if (argc < 4)
	{
		printf("Usage: sa_gd <msg> <output-file> <input-file1> ... <input-file-n> \n");
		return;
	}

	if ((output_file = fopen(argv[2], "w")) == NULL)
	{
//		clrscr();
		printf("Error : I can not create the output file!");
		return;
	}

// InitGraph();
// CloseGraph(); //NEW

//	clrscr();
	printf("Evaluation of %s Algorithm. Please wait ... \n", argv[1]); //NEW

	for (k = 3; k < argc; k++)
	{
		printf("%s started ... \n", argv[k]); //NEW
		polygon_nodes_list.head = polygon_nodes_list.tail = NULL;
		if (Give_Inputs(argv[k]))
			goto main_return2;

		#ifdef DEBUG
			Print(&polygon_nodes_list);
			Print(skel_nodes_list, number_of_skel_nodes);
			Print(tree_nodes_list, number_of_tree_nodes);
			Print(fix_nodes_list);
		#endif

		if (!number_of_tree_nodes)
			goto main_return2;

//	  	place = (place_t *)malloc(number_of_tree_nodes*sizeof(place_t));
		place = (place_t *)malloc(MAX_TREE_NODES*sizeof(place_t)); // NEW
		if (place == NULL)
		{
			printf("There is not enough memory!, press any key to quit ...");
			getch();
			goto main_return2;
		}

//		new_place = (place_t *)malloc(number_of_tree_nodes*sizeof(place_t));
		new_place = (place_t *)malloc(MAX_TREE_NODES*sizeof(place_t));
		if (new_place == NULL)
		{
			printf("There is not enough memory!, press any key to quit ...");
			getch();
			goto main_return2;
		}

//	   InitGraph();
//	   setcolor(WHITE); //NEW
//	   setbkcolor(BLACK); //NEW

		#ifdef DEBUG
			randomize();
			for (i=0; i<number_of_tree_nodes; i++)
			{
				place[i].x = (int)(((float)(rand() * Xmax)) / RAND_MAX);
				place[i].y = (int)(((float)(rand() * Ymax)) / RAND_MAX);
			}
//	   	DrawTree(place); //NEW
			goto main_return1;
		#endif

		_dos_gettime(&t1);
//	  	printf("%s started 2 ... \n", argv[k]); //NEW
//	  	if (Init_Placement2(place, number_of_tree_nodes))
//	  	if (Init_Placement3(place, number_of_tree_nodes))
		if (Init_Placement4(place, number_of_tree_nodes))
			goto main_return1;
//	  	printf("%s started 3 ... \n", argv[k]); //NEW
		_dos_gettime(&t2);
//	   hundredth of secod
		init_time = (long int)(t2.hour-t1.hour)*360000 +
		(long int)(t2.minute - t1.minute)*6000 +
		(long int)(t2.second - t1.second)*100 +
		(long int)(t2.hsecond - t1.hsecond);

//	  	DrawTree(place); //NEW
//	  	outtextxy(0, Ymax-10, "Initial Drawing, press any key to continue."); //NEW
//	  	getch(); //NEW

		/* Phase II */
		_dos_gettime(&t1);
		temp = INIT_TEMP2;
		cost = Cost2(place);

		cx = wherex();
		cy = wherey();

		while ( temp > FINAL_TEMP2 )
		{
			if (kbhit())
			{
				ch = getch();
				if (ch == ESC_CODE)
					break;
			}

			RadiusX = 100;
			RadiusY = 100;
//	 		I selected 400 as initial radius, beacuase area of drawing is
// 		a polygon now and not a rectangle. (200 as final radius)

//			RadiusX = (int)((400-200)*(temp-FINAL_TEMP2)/(INIT_TEMP2-FINAL_TEMP2)+200);
//			if (RadiusX < 50)
//			RadiusX = 50;

//			RadiusY = (int)((400-200)*(temp-FINAL_TEMP2)/(INIT_TEMP2-FINAL_TEMP2)+200);
//			if (RadiusY < 50)
//			RadiusY = 50;

//			RadiusX = (int)(((Right-Left)/5-10)*(temp-FINAL_TEMP2)/(INIT_TEMP2-FINAL_TEMP2)+10);
//			RadiusY = (int)(((Button-Top)/5-10)*(temp-FINAL_TEMP2)/(INIT_TEMP2-FINAL_TEMP2)+10);
			idle = 0;
			while ( inner_loop_criterion == 0 )
			{
				if (kbhit())
				{
					ch = getch();
					if (ch == ESC_CODE)
						goto main_return1;
				}

				//NEW
				gotoxy(cx, cy);
				if (progress == 0)
					printf("/\n");
				else
					if (progress == 1)
						printf("-\n");
					else
						if (progress == 2)
						printf("\\\n");
					else
						if (progress == 3)
							printf("|\n");

				if (++progress == 4)
					progress = 0;

				if (Perturb(place, new_place, RadiusX, RadiusY))
					goto main_return1;
				new_cost = Cost2(new_place);
				delta_cost = new_cost - cost;

				// Because the delta_cost is too big, we decrease it
				delta_cost = delta_cost / 1;

				idle++;

				if (delta_cost < 0.0)
				{
					Swap(&place, &new_place);
					cost = new_cost;
//			 		DrawTree(place); //NEW
					idle=0;
				}
				else
				{
					a_random = (float)((float)rand()/RAND_MAX);
					expo = (float)exp((float)((-delta_cost)/temp));
					if (expo == HUGE_VAL)
					{
						//outtextxy(0, Ymax-10, "There is an error!");
						printf("There is an error!\n");
						getch();
						continue;
					}
					// Note: I added /1000.0 on 24 Ordibehesht 1382 again
					if (a_random < expo/10.0)
					{
						Swap(&place, &new_place);
						cost = new_cost;
//						DrawTree(place); //NEW
						idle=0;
					}
				}
			}
			inner_loop_criterion = 0;
			temp = Schedule2(temp);
		}

		main_return1:

		_dos_gettime(&t2);
// 	hundredth of secod
		run_time = (long int)(t2.hour-t1.hour)*360000 +
		(long int)(t2.minute - t1.minute)*6000 +
		(long int)(t2.second - t1.second)*100 +
		(long int)(t2.hsecond - t1.hsecond);

		if (TotalEdgeCrossings(place, total_edge_crossing) ||
				TotalMeanEdgeLength(place, total_edge_length, mean_edge_length) ||
				MaxDeviationEdgeLength(place, mean_edge_length,
				max_edge_length, edge_length_deviation) ||
				TotalMeanEdgeBend(total_edge_bend, mean_edge_bend) ||
				MaxDeviationEdgeBend(mean_edge_bend,
				max_edge_bend, edge_bend_deviation) ||
				MinDistance(place, min_distance))
		{
			fprintf(output_file, "Error in computation %s Algorithm for tree %s!\n",
			argv[1], argv[k]);
		}
		else
		{
			fprintf(output_file, "Result of %s Algorithm for tree %s : \
					init_time= %ld , run_time= %ld , total_crossings= %d , \
					total_edge_len= %f , max_edge_len= %f , edge_len_deviation= %f , \
					mean_edge_len= %f , total_bends= %d , max_bends= %d , \
					bends_deviation= %f , mean_bends= %f , min_dist= %f \n",
					argv[1], argv[k],
					init_time, run_time, total_edge_crossing,
					total_edge_length, max_edge_length, edge_length_deviation,
					mean_edge_length, total_edge_bend, max_edge_bend,
					edge_bend_deviation, mean_edge_bend, min_distance);
		}
		fflush(output_file);

//		setviewport(0, Ymax-20, Xmax-5, Ymax, 1); //NEW
//		clearviewport(); //NEW
//		outtextxy(0, 10, "press any key to quit:"); //NEW
//		getch(); //NEW
// 	CloseGraph(); //NEW

		gotoxy(cx, cy); //NEW
		printf("%s finished. \n", argv[k]); //NEW

		main_return2:

		/* FELLAN
		for (i=0; i<number_of_holes; i++)
			RemoveAllCorner(holes[i]);
		free(holes);
		*/

		for (i=0; i<number_of_tree_nodes; i++)
			RemoveAllNode(tree_nodes_list[i].neighbors);
		free(tree_nodes_list);
		tree_nodes_list = NULL;
		number_of_tree_nodes = 0;

		for (i=0; i<number_of_skel_nodes; i++)
			RemoveAllNode(skel_nodes_list[i].neighbors);
		free(skel_nodes_list);
		skel_nodes_list = NULL;
		number_of_skel_nodes = 0;

		free(place); place = NULL;
		free(new_place); new_place = NULL;

//		free(fix_nodes_list); BUG-FIXED
		fix_tmp_ptr1 = fix_nodes_list;
		while(fix_tmp_ptr1)
		{
			fix_tmp_ptr2 = fix_tmp_ptr1;
			fix_tmp_ptr1 = fix_tmp_ptr1->next;
			free(fix_tmp_ptr2);
		}
		fix_nodes_list = NULL;

		pre_tmp_ptr1 = prelocated_nodes_list;
		while(pre_tmp_ptr1)
		{
			pre_tmp_ptr2 = pre_tmp_ptr1;
			pre_tmp_ptr1 = pre_tmp_ptr1->next;
			free(pre_tmp_ptr2);
		}
		prelocated_nodes_list = NULL;

		//free(polygon_nodes_list); BUG-FIXED
		poly_tmp_ptr1 = polygon_nodes_list.head;
		while(poly_tmp_ptr1)
		{
			poly_tmp_ptr2 = poly_tmp_ptr1;
			poly_tmp_ptr1 = poly_tmp_ptr1->next;
			free(poly_tmp_ptr2);
		}
		polygon_nodes_list.head = NULL;
	}
	fclose(output_file);
} /* Main */