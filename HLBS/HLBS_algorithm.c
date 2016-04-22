/* DAG scheduler */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node_info.h"
#include "print_debug.h"


/* Sort in order of priority */
void sort_suffix(int *dag_node_suffix){
  int i,j,tmp;
  int temp_dag_node[MAX_NODE]; // only use for sorting dag_node's suffix

  for(i=0;i<node_num;i++){
    dag_node_suffix[i]=i;
    temp_dag_node[i]=dag_node[i].rank;
  }
      
  for (i=0; i<node_num; ++i) {
    for (j=i+1; j<node_num; ++j) {
      if (temp_dag_node[i] > temp_dag_node[j]) {
      tmp = temp_dag_node[i];
      temp_dag_node[i] = temp_dag_node[j];
      temp_dag_node[j] = tmp;

      /* sort suffix */
      tmp=dag_node_suffix[i];
      dag_node_suffix[i] =dag_node_suffix[j];
      dag_node_suffix[j] = tmp;
      }
    }
  }
}

int main(int argc, char* argv[])
{
	int use_cores; 
	int suffix_sorted_rank[MAX_NODE];
	
	node_num = 0;
	deadline_num = 0;
	arc_num = 0;
	type_num = 0;

	if(argc!=2){
		printf("Error: invalid option\n");
		printf("usage: limited_core_num (e.g., ./test 3)\n");
		exit(EXIT_FAILURE);
	}
	use_cores = atoi(argv[1]);

	// Read parameter from *.tgff
	read_dag_depend();
	
	// Set parameter to dag_node(structure)
	int i;
	for(i=0;i<node_num;i++){
		set_param(i);
	}

	// Prioritization
	ranking();

	// Store node in order by descending
	sort_suffix(suffix_sorted_rank);

	// Allocation
	allocate_task(use_cores,suffix_sorted_rank); 

////////////
// option //
////////////
#ifdef PRINT_DAG_NODE
	print_dag_node(node_num);
#endif
#ifdef PRINT_PRIORITY_QUEUE 
	print_priority_queue(suffix_sorted_rank);
#endif

	return 0;
}
