#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node_info.h"
#include "print_debug.h"

////////////////////
// calculate rank //
////////////////////

/* Calculate rank. And updata if necessary */
void calc_next_rank(int next_node_num,int prev_rank){
	int temp_rank;

		temp_rank =  prev_rank - (dag_node[next_node_num].exec_time + dag_node[next_node_num].delay_time);

		/* If prev_rank is 0, update */
		if(dag_node[next_node_num].rank == 0){
			dag_node[next_node_num].rank = temp_rank;
#ifdef PRINT_CALC_RANK
			printf("When =0: %d rank is prev_rank(%d) - exec(%d) = %d\n",next_node_num,prev_rank,dag_node[next_node_num].exec_time,dag_node[next_node_num].rank);
#endif
		}
		/* If prev_rank > new_rank , updata */
		else if(dag_node[next_node_num].rank > temp_rank ){
			dag_node[next_node_num].rank = temp_rank;
#ifdef PRINT_CALC_RANK
			printf("dag_node[next_node_num].rank(%d) > temp_rank(%d)\n",dag_node[next_node_num].rank,temp_rank);
			printf("When >0: %d rank is prev_rank(%d) - exec(%d) = %d\n",next_node_num,prev_rank,dag_node[next_node_num].exec_time,dag_node[next_node_num].rank);
#endif
		}
}

/* endノードの名前とload_arc[i].toが一致するiに対してfromと一致するdag_nodeの引数を返す */
void find_consident_arc(int find_node){
	int from_num_char = 0;
	int from_num = 0;
	char from_node[] = "default";
	int from_node_int[MAX_NODE];
	int i=0;
	int j=0;
	int copy_i;

	for (i=0;i<node_num;i++){
		from_node_int[i] = -1;
	}

	/* find load_arc[].from, and get depend node name */
 	for(from_num_char=0;from_num_char < arc_num ;from_num_char++){ 	
		if(!strcmp(dag_node[find_node].node_name,load_arc[from_num_char].to)){
			strcpy(from_node,load_arc[from_num_char].from);
			
			/* If entry node, return */
			for(from_num=0;from_num< arc_num;from_num++){
				if(!strcmp(from_node,"default")){
					return;
				}

				/* If it is not entry node, calculate rank and recursive*/
				if(!strcmp(from_node,dag_node[from_num].node_name)){
					calc_next_rank(from_num,dag_node[find_node].rank); // calculate rank
					find_consident_arc(from_num); // recursive
					from_node_int[j] =  from_num;
					j++;
					break;
				}
			}
		}
	}
#ifdef PRINT_CALC_RANK
	printf("%s depend ",dag_node[find_node].node_name);
	for(i=0;i<node_num;i++){
		if(from_node_int[i] != -1)
			printf(" %d",from_node_int[i]);
	}
	printf("\n\n");
#endif

	for(copy_i=0; copy_i < node_num; copy_i++){
		dag_node[find_node].depend_node[copy_i] = from_node_int[copy_i];
	}
}

/////////////
// Ranking //
/////////////

void ranking(){
int i;
int j;

	// エンドノードの場合
	// rank = deadline - (exec+delay)
	for(i=0;i<deadline_num; i++){
		for(j=0;j<=node_num; j++){
			if(!strcmp(load_deadline[i].end_node_name,dag_node[j].node_name)){
				dag_node[j].rank = load_deadline[i].deadline - (dag_node[j].exec_time/* + dag_node[j].delay_time*/); //hlbs
				find_consident_arc(j);
			}
		}
	}
}

