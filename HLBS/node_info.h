#define MAX_NODE 50
#define NODE_NAME 30
#define MAX_PROCESSER 10

typedef struct _dag_node{
	char node_name[NODE_NAME];
	int rank;
	int exec_time;
	int delay_time;
	int est;
	int eft;
	int depend_node[MAX_NODE];
}Dag_node;

typedef struct _processor{
	int assign_node_suffix[MAX_NODE];
	int sum_time;
}Processer;

/* for input by *.tgff */
//##############################
typedef struct _load_node{
	char node_name[NODE_NAME];
	int	type;
}Load_node;

typedef struct _load_arc{
	char link_name[NODE_NAME];
	char from[NODE_NAME];
	char to[NODE_NAME];
}Load_arc;

typedef struct _load_value{
	int type;
	int exec_time;
	int delay_time;
}Load_value;

typedef struct _load_deadlien{
	int deadline;
	char deadline_name[NODE_NAME];
	char end_node_name[NODE_NAME];
}Load_deadline;
//###############################


Dag_node dag_node[MAX_NODE];
Load_node load_node[MAX_NODE];
Load_arc load_arc[MAX_NODE];
Load_value load_value[MAX_NODE];
Load_deadline load_deadline[MAX_NODE];

Processer processor[MAX_PROCESSER];


int node_num;
int deadline_num;
int arc_num;
int type_num;
