#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node_info.h"
#include "print_debug.h"
///////////////////
// set parametar //
///////////////////

/* search coincident type, and return coincident number*/
int search_type(int _load_node_num){
	int coincide_num;
	int res=-1;
	
	for(coincide_num = 0; coincide_num < type_num; coincide_num++){
		if(load_node[_load_node_num].type == load_value[coincide_num].type){
			res = coincide_num;
			break;
		}
	}
	return res;
}

//読み取った情報をツリー用の構造体に変換し作成
void set_param(int i){
	int coincide_type = 0;

	/* load_node.typeに一致するload_valueの配列番号を返す */
	coincide_type = search_type(i);
	/* set parameter */
	strcpy(dag_node[i].node_name,load_node[i].node_name);  // name
	dag_node[i].exec_time = load_value[coincide_type].exec_time;   // exec_time
	dag_node[i].delay_time = load_value[coincide_type].delay_time;   // delay_time
	dag_node[i].rank = 0;
	dag_node[i].est = 0;
	dag_node[i].eft = 0;
	
}

////////////////////////////////////////////////
// Function to extract information to be used //
////////////////////////////////////////////////

// @TASK_GRAPH
/* set task, type */
void extract_node(char *line){
	char blank[] = " \tTASKTYPE";
	char *temp_char;
	int temp_int=0;

	/* extract TASK */
	temp_char = strtok(line,blank);
	strcpy(load_node[node_num].node_name,temp_char);
#ifdef PRINT_LOAD_NODE
	printf("task_name = %s\n",load_node[node_num].node_name);
#endif

	/* extract TYPE */
	temp_char = strtok(NULL,blank);
	temp_int = atoi(temp_char);
	load_node[node_num].type = temp_int;
#ifdef PRINT_LOAD_NODE
	printf("TYPE = %d\n\n",load_node[node_num].type);
#endif
}

/* set ARC, FROM, TO */
void extract_arc(char *line){
	char blank[] = " \tARCFROMTOTYPE";
	char *temp_char;
	int temp_int=0;

	/* extract ARC */
	temp_char = strtok(line,blank);
	strcpy(load_arc[arc_num].link_name,temp_char);
#ifdef PRINT_LOAD_ARC
	printf("link_name = %s\n",load_arc[arc_num].link_name);
#endif
	/* extract FROM */
	temp_char = strtok(NULL,blank);
	strcpy(load_arc[arc_num].from,temp_char);
#ifdef PRINT_LOAD_ARC
	printf("link_from = %s\n",load_arc[arc_num].from);
#endif
	/* extract TO */
	temp_char = strtok(NULL,blank);
	strcpy(load_arc[arc_num].to,temp_char);
#ifdef PRINT_LOAD_ARC
	printf("link_to = %s\n\n",load_arc[arc_num].to);
#endif
}

/* set deadline */
void extract_deadline(char *line){
	char blank[] = " \tHARDDEADLINEONAT";
	char *temp_char;
	int temp_int=0;;

	/* extract "_" */
	temp_char = strtok(line,blank);

	/* extract deadline name */
	temp_char = strtok(NULL,blank);
	strcpy(load_deadline[deadline_num].deadline_name,temp_char);
#ifdef PRINT_LOAD_DEADLINE
	printf("deadline_name = %s\n",load_deadline[deadline_num].deadline_name);
#endif
	/* extract end node name */
	temp_char = strtok(NULL,blank);
	strcpy(load_deadline[deadline_num].end_node_name,temp_char);
#ifdef PRINT_LOAD_DEADLINE
	printf("end_node = %s\n",load_deadline[deadline_num].end_node_name); 	
#endif
	/* extract deadline */
	temp_char = strtok(NULL,blank);
	temp_int = atoi(temp_char);
	load_deadline[deadline_num].deadline = temp_int;
#ifdef PRINT_LOAD_DEADLINE
	printf("deadline = %d\n\n",load_deadline[deadline_num].deadline);
#endif
}

// @COMMON
/* set type, exec_time, delay_time */
void extract_node_info(char *line){
	char blank[] = " \t";
	char *temp_char;
	int temp_int=0;

	/* extract type */
	temp_char = strtok(line,blank);
	temp_int = atoi(temp_char);
	load_value[type_num].type = temp_int;
#ifdef PRINT_LOAD_VALUE
	printf("type = %d\n",load_value[type_num].type); 
#endif

	/* extract exec_time */
	temp_char = strtok(NULL,blank);
	temp_int = atoi(temp_char);
	load_value[type_num].exec_time = temp_int;
#ifdef PRINT_LOAD_VALUE
	printf("exec_time = %d\n",load_value[type_num].exec_time); 
#endif

	/* extract delay_time */
	temp_char = strtok(NULL,blank);
	temp_int = atoi(temp_char);
	load_value[type_num].delay_time = temp_int;
#ifdef PRINT_LOAD_VALUE
	printf("dealy_time = %d\n",load_value[type_num].delay_time); 
#endif
}

////////////////////////////////////////
// Set information of DAG from *.tgff //
////////////////////////////////////////

/* read information of node and dependance from *tgff */
void read_dag_depend(){
	char buff[256];
	FILE *node_fp;
	char *find_task = "TASK";
	char *find_arc = "ARC";
	char *find_deadline = "HARD_DEADLINE";
	char *find_exec_time = "exec_time";
	int information_flag=0;

 if((node_fp = fopen("./Sample_DAG/DAG_sample.tgff", "r")) == NULL) {
	//if((node_fp = fopen("./Sample_DAG/DAG_sample_deadline.tgff", "r")) == NULL) { 
	//if((node_fp = fopen("./Sample_DAG/multi_entry.tgff", "r")) == NULL) { 
    printf("failed to open input file．\n");
    exit(1);
  }
	
 	/* get TASK_GRAPH */
	while(fgets(buff,sizeof(buff),node_fp) != NULL){		

		/* extract line of node */
		if(strstr(buff,find_task) != NULL && strstr(buff,"@") == NULL){
			extract_node(buff);
			node_num++;
		}

		/* extract line of dependance */
		else if(strstr(buff,find_arc) != NULL ){
			extract_arc(buff);
			arc_num++;
		}

		/* extract line of deadline */
		else if(strstr(buff,find_deadline) != NULL ){
			extract_deadline(buff);
			deadline_num++;
		}
		else if(strstr(buff,"}") != NULL ){ 
			break;
		}
	}

	int node_num_t = node_num;
	/*get COMMON */
	while(fgets(buff,sizeof(buff),node_fp) != NULL){		
		/* extract line of node */
		/* finish when find "}" of COMMON */
		if(strstr(buff,"}") != NULL ){ 
			break;
		}

		/* extract exec_time and delay_time */
		if(strstr(buff,find_exec_time) != NULL){
			information_flag=1;
			fgets(buff,sizeof(buff),node_fp);
		}

		if(information_flag == 1){
			extract_node_info(buff); 
			type_num++;
		}
	}
	fclose(node_fp);
	node_num = node_num_t;
}

