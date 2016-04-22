#include <stdio.h>
#include <stdlib.h>
#include "node_info.h"
#include "print_debug.h"

void print_dag_node(int node_num){
  int i;
  int j;

  for(i=0;i<node_num;i++){
    printf("%s %d %d %d \n"
        ,dag_node[i].node_name
        ,dag_node[i].exec_time
        ,dag_node[i].delay_time
        ,dag_node[i].rank);
    printf("dependancy has");
    for(j=0;j<node_num;j++){
      printf(" %d",dag_node[i].depend_node[j]);
    }
    printf("\n\n");
	}
}

void print_priority_queue(int *sorted_rank){
  int i;

  printf("\n---- prioritu queue ----\n");
  for(i=0;i<node_num;i++){
	    printf("sorted_rank[%d] = %d\nrank = %d\n\n",i, sorted_rank[i],dag_node[sorted_rank[i]].rank);
	  }
}


void print_est_and_eft(){
  int i;
  printf("\n---- est and eft ----\n");
  for(i=0;i<node_num;i++){
    printf("dag_node[%d].est = %d\n",i, dag_node[i].est);
    printf("dag_node[%d].eft = %d\n",i, dag_node[i].eft);    
  }
}


void print_core_sum_time(){
	int i;
  printf("\n---- sum_time for each core----\n");
  for(i=0;i<MAX_PROCESSER;i++){
    printf("processor[%d].sum_time = %d\n",i,processor[i].sum_time);
  }
}


void print_figure(int *sorted_rank){
  int i,j,temp;
  printf("\n----- figure ----\n");

  for(i=0;i<MAX_PROCESSER;i++){
    printf("core [%d] ",i);

        printf("\n");

    for(j=0;j<node_num;j++){
        for(temp=0;temp<node_num;temp++){
          if(processor[i].assign_node_suffix[j] == sorted_rank[temp]){
              printf("|%d %s(%d) %d| "
                  ,dag_node[sorted_rank[temp]].est
                  ,dag_node[sorted_rank[temp]].node_name
                  ,dag_node[sorted_rank[temp]].exec_time
                  ,dag_node[sorted_rank[temp]].eft);
          }
        }
      }
    printf("\n");
  }
}

write_data_for_evaluations(){
int i;
FILE *fp_lb,*fp_sp,*fp_e,*fp_makespan,*fp_slr;
/////////////////////////////////////////// LB
  //compute LB
  double avg=0;
  double lb=0;
  double makespan=0;
  int cp_p=-1;
  for(i=0;i<3;i++){
    avg += processor[i].sum_time;
    if(makespan < processor[i].sum_time){
      makespan = processor[i].sum_time;
      cp_p=i;
    } 
  }
  avg /= 3;
  lb = makespan / avg;
  printf("lb = %lf\n",lb);
if ((fp_lb = fopen("./evaluate/hlbs_lb_30.txt", "a")) == NULL) {
    printf("file open error!!\n");
    exit(EXIT_FAILURE); /* (3)エラーの場合は通常、異常終了する */
}
fprintf(fp_lb,"%lf\n",lb);
fclose(fp_lb);


//////////////////////////////////////////////
//compute Sp

  double sum=0;
  double sp=0;
  for(i=0;i<node_num;i++){
    sum += dag_node[i].exec_time;
  }
  sp = sum / makespan;
  printf("sp = %lf\n",sp);
if ((fp_sp = fopen("./evaluate/hlbs_sp_30.txt", "a")) == NULL) {
    printf("file open error!!\n");
    exit(EXIT_FAILURE); /* (3)エラーの場合は通常、異常終了する */
}
fprintf(fp_sp,"%lf\n",sp);
fclose(fp_sp);
/////////////////////////////////////////////////
//compute E

  double e;
  e= sp / 3;

if ((fp_e = fopen("./evaluate/hlbs_e_30.txt", "a")) == NULL) {
    printf("file open error!!\n");
    exit(EXIT_FAILURE); /* (3)エラーの場合は通常、異常終了する */
}
printf("e = %lf\n",e);
fprintf(fp_e,"%lf\n",e);
fclose(fp_e);
///////////////////////////////////////////////////
// makespan
  
if ((fp_makespan = fopen("./evaluate/hlbs_makespan_30.txt", "a")) == NULL) {
    printf("file open error!!\n");
    exit(EXIT_FAILURE); /* (3)エラーの場合は通常、異常終了する */
}
printf("makespan = %lf\n",makespan);
fprintf(fp_makespan,"%lf\n",makespan);
fclose(fp_makespan);


////////////////////////////////|///////////////////
// SLR

  double slr= 0;
  int cp = 0;
  int min_rank=1000;
  int cp_deadline;
  int cp_node=-1;

  for(i=0;i<node_num;i++){
    if(min_rank > dag_node[i].rank){
      min_rank = dag_node[i].rank;
    }
  }

  for(i=0;i<node_num;i++){
    if(processor[cp_p].assign_node_suffix[i] != -1){
      printf("suffix = %d\n",i);
      cp_node = i;
    }
  }

  cp_deadline = load_deadline[cp_node].deadline;
  cp = /*cp_deadline*/500 - min_rank;
  printf("cp = %d, cp_deadline = %d, min_rank = %d, cp_node = %d\n",cp,cp_deadline,min_rank,cp_node);
  slr = makespan / (double)cp*2;
  printf("slr = %lf\n",slr);
  if ((fp_slr = fopen("./evaluate/hlbs_slr_10.txt", "a")) == NULL) {
    printf("file open error!!\n");
    exit(EXIT_FAILURE); /* (3)エラーの場合は通常、異常終了する */
  }
  fprintf(fp_slr,"%lf\n",slr);
  fclose(fp_e);
}
