#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "print_debug.h"
#include "node_info.h"

int find_assign_core(int find_node){
	int i,j;
	int assigned_core = 0;

	for(i=0;i<MAX_PROCESSER;i++){
		for(j=0;j<node_num;j++){
			if(find_node == -1){ //entry_node
				assigned_core = -1;
				return assigned_core;
			}
			if(processor[i].assign_node_suffix[j] == find_node){
				assigned_core = i;
				return assigned_core;
			}
		}
	}
	return assigned_core;
}

int check_depend_num(int _dag_node){
	int i;
	int depend_node_num=0;

	for(i=0;i<node_num;i++){
		if(dag_node[_dag_node].depend_node[i] != -1){
			depend_node_num++;
		}
	}
	return depend_node_num;
}

// AFT(_dag_node) in the core 
int avail(int _dag_node,int core){
	int avail_time;

	avail_time = processor[core].sum_time;
	return avail_time;
}

// max(AFT(prev_node) + c(prev_node))
int prev_aft_com(int _dag_node,int core){
	int avail_time=0;
	int depend_node_num;
	int prev_node;
	int prev_node_core;
	int i;
	int prev_value;
	int max_avail_time = 0;

	depend_node_num = check_depend_num(_dag_node);
	for(i=0;i<depend_node_num;i++){
		prev_node = dag_node[_dag_node].depend_node[i];
		prev_node_core = find_assign_core(prev_node);
		if(prev_node_core == core){
		avail_time = processor[core].sum_time;
		}else{
			if(prev_node == -1){
				avail_time = 0;
			}else{
			avail_time = dag_node[prev_node].eft + dag_node[prev_node].delay_time;
			}
		}
		if(max_avail_time < avail_time){
			max_avail_time = avail_time;
		}
	}

	return max_avail_time;
}

int calc_eft(int _dag_node,int core){
	int avail_time1;
	int avail_time2;
	int est=1000;
	int eft=1000;

	avail_time1 = avail(_dag_node,core); // AFT(_dag_node) in the core
	avail_time2 = prev_aft_com(_dag_node,core); // max(AFT(prev_node) + c(prev_node))

	if(avail_time1 > avail_time2){
		est = avail_time1;
	}else{
		est = avail_time2;
	}
	eft = est + dag_node[_dag_node].exec_time;

return eft; 
}

void allocate_task_to_core(int _dag_node, int min_eft, int min_processor){
	int i;

	processor[min_processor].sum_time = min_eft;
	for(i=0;i<node_num;i++){
		if(processor[min_processor].assign_node_suffix[i] == -1){
			processor[min_processor].assign_node_suffix[i] = _dag_node;
			break;
		}
	}
}
///////////////////////////
// Step 3. allocate_task //
///////////////////////////

void allocate_task(int use_cores, int *suffix_sorted_rank){
	int i,j;
	int min_eft=1000;
	int min_processor;
	int _dag_node;
	int temp_eft;
	
	// Inisialize processor
	for(i=0;i<MAX_PROCESSER;i++){
		for(j=0;j<node_num;j++){
			processor[i].assign_node_suffix[j] = -1;
		}
	}
	
	for(i=0;i<node_num;i++){
		_dag_node = suffix_sorted_rank[i];

		// Calculate EFT for each core 
		for(j=0;j<use_cores;j++){
			temp_eft = calc_eft(_dag_node,j);

			// Select minimum EFT
			if(min_eft > temp_eft){
				min_eft = temp_eft;
				dag_node[_dag_node].eft = min_eft;
				dag_node[_dag_node].est = min_eft - dag_node[_dag_node].exec_time;
				min_processor = j;
			}
		}

		allocate_task_to_core(_dag_node,min_eft,min_processor);
		min_eft=1000; // for next loop
	}

	
/////////////
// Options //
/////////////

#ifdef PRINT_CORE_SUM_TIME  
	print_core_sum_time();
#endif
#ifdef PRINT_EST_AND_EFT
	print_est_and_eft();
#endif
#ifdef PRINT_FIGURE
	print_figure(suffix_sorted_rank);
#endif
#ifdef WRITE_DATA
	write_data_for_evaluations();
#endif
}

