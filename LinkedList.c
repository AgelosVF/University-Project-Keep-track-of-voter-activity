#include "MVote.h"
#include <stdio.h>
#include <stdlib.h>




struct list_node{
	int tk;
	int people;
	voter **voter_list;
	struct list_node *next_n;
	struct list_node *prev_n;
	

};
//-------------------------------------------------------------------------------------
//private functions
int delete_node( list_node *node){
	int i=0;
	if ( node == NULL){
		printf("Deleting empty node in ll\n");
		return 0;
	}
	//fix prev and next node pointers sustain the list in case we delete middle node
	if(node->prev_n !=NULL){
		node->prev_n->next_n=node->next_n;
	}
	if (node->next_n != NULL){
		node->next_n->prev_n=node->prev_n;
	}
	
	i+=my_free(node->voter_list);
	//the voters get freed by the hashtble destroy so no need to free them here
	i+=my_free(node);
	return i;
}


//add voter to node. Return 1 for success , if voter already in list returns 0.
int add_voter_to_node(list_node *node, voter *vtr){
	int i=ll_find_voter(node, vtr->pin);
	if (i==1){
		printf("Voter %d already in list\n",vtr->pin);
		return 0;
	}
	int index= node->people;
	node->people=(node->people) + 1;
	voter **tmp;
	tmp=realloc(node->voter_list, sizeof(voter *) * ( node->people ) );
	if(tmp == NULL){
		printf("Error: could realocate space for voter in linked list\n");
		exit(-1);
	}
	node->voter_list=tmp;
	node->voter_list[index]=vtr;

	return 1;

}
//---------------------------------------------------------------------------------------

//public functions

//create a new node for the list containing a voter and insert it between prev and next 
list_node * ll_create_node(voter *vtr,list_node *prev, list_node *next){
	
	//allocate memory
	list_node * new_node=malloc( sizeof(*new_node) );
	if (new_node == NULL)
	{
		perror("Error:malloc failed to alocate space for list node\n");
		exit(-1);
	}
	//add next and prev nodes
	new_node->next_n=next;
	new_node->prev_n=prev;
	
	//change prev and next nodes to point to the new node as their next and prev
	if (prev != NULL){
		prev->next_n=new_node;
	}
	if (next != NULL){
		next->prev_n=new_node;
	}

	// add the rest of the data
	new_node->tk=vtr->zip;
	new_node->people=1;

	//alocate space for the table and link the first voter
	new_node->voter_list=calloc( sizeof( voter *), new_node->people );
	new_node->voter_list[0]=vtr;


	return new_node;
}

// go throught the list search and return the node with tk=zip code. If you dont find it return NULL.
list_node * ll_find_tk(list_node * lnode, int zip_code){
	//recursively go throught each node until you find the one with the zip code i am seatching
	//or reach the end of the list
	if ( (lnode->tk) == zip_code ){
		//it's the node i am looking for return it
		return lnode;
	}
	else if ( (lnode->next_n) != NULL){
		// check the next node
		return ll_find_tk( (lnode->next_n), zip_code);
	}
	else {
		//reached end of list and didn't find it
		return NULL;
	}
}

// search node for the voter with pin. If found return 1 else return 0
int ll_find_voter(list_node * snode, int spin){

	voter * tmp;
	for(int i=0;i<snode->people; i++){	
		tmp=snode->voter_list[i];
		if ( (tmp->pin) == spin ){
			//found the voter
			return 1;
		}
	}
	//didnt find the voter
	return 0;
}


//add voter to the list. Search for a node with the same t.k. if not found make a new node else call 
//add_voter_to_node to add him there.
int ll_add_voter_to_list(list_node *node,voter *vtr){
	

	list_node *tmp=ll_find_tk(node, vtr->zip);
	if (tmp==NULL){
		//he is the first to vote with zip code so we create a new node at the end of the list
		tmp=node;
		//find the last node
		while( tmp->next_n != NULL){
			tmp=tmp->next_n;
		}
		//create and add the new node to the end of the list
		tmp->next_n=ll_create_node(vtr, tmp, NULL);
		return 1;
	}
	else{
		//there is already a node in the list with that zip code
		int i=add_voter_to_node(tmp,vtr);
		if ( i==0 ){
			printf("%d has already voted\n",vtr->pin);
		}
		return i;
	}
}

//go through each node and add the people in each. Return the sum
int ll_total_voted( list_node *node){
	int i=node->people;
	while (node->next_n != NULL){
		node=node->next_n;
		i+=node->people;
	}
	return i;
}

//go throught each node and print the tk and the number of people. I use a 2d array were i add the values of each node and keep it sorted so I can print them in order.
void ll_print_tk_num_voters(list_node *node){

	//malloc a 2d array to store people,voters using a pointer to a table of pointers
	int **array2d;
	int rows=1,columns=2;
	array2d=calloc(rows, sizeof(int *) );
	if(array2d==NULL){
		perror("Error:couldnt allocate memory for 2d array in ll_print_tk_num_voters\n");
		exit(-1);
	}
	array2d[0]=calloc(columns, sizeof(int));
	if(array2d[0]==NULL){
		perror("Error:couldnt allocate memory for 2d array in ll_print_tk_num_voters\n");
		exit(-1);
	}


	array2d[0][0]=node->people;
	array2d[0][1]=node->tk;

	int **tmp;

	while(node->next_n != NULL){
		node=node->next_n;
		rows+=1;
		//make space for another entry
		tmp=realloc(array2d, sizeof(int*)*rows);
		if(tmp){
			array2d=tmp;
		}
		else {
			perror("Error:couldnt allocate memory for 2d array in ll_print_tk_num_voters\n");
			exit(-1);
		}
		array2d[rows-1]=calloc( columns, sizeof(int*) );
		array2d[rows-1][0]=node->people;
		array2d[rows-1][1]=node->tk;

		// bubble sort the new element at the correct position to maintain sorted order
		//from more people to less
		int i = rows - 1;
		while (i > 0 && array2d[i][0] > array2d[i - 1][0]) {
		    // Swap the current element with the previous one
		    int *swap_temp = array2d[i];
		    array2d[i] = array2d[i - 1];
		    array2d[i - 1] = swap_temp;
		    i--;
		}
	}

	for(int i=0;i<rows;i++){	
		printf("- %d : %d voted\n", array2d[i][1], array2d[i][0]);
	}
	
	//free the memory
	for(int i=0;i<rows;i++){
		free(array2d[i]);
	}
	free(array2d);
}

//go through the array in the node and print the data in each voter that voter_list[i] points to
void ll_print_tk_id_voters(list_node *node,int zipcode){
	list_node *tk_node=ll_find_tk(node,zipcode);
	voter *tmp;
	if( tk_node != NULL){
		printf(	 "-%d voted  in  %d\n",tk_node->people ,tk_node->tk);
		for(int i=0; i< tk_node->people; i++){
			tmp=tk_node->voter_list[i];
			printf("%d\n", tmp->pin);
		}

	}
	else {
		printf("No one has voted to that zipcode\n");
	}
}

//call delete_node for each node and return the sum of memory freed
int ll_destroy(list_node *node)
{
	int i=0;
	list_node *next_node;
	while( node != NULL){
		next_node=node->next_n;
		i+=delete_node(node);
		node=next_node;
	}
	return i;
}


//Not used in main. Go backwards to find and return the head of the list
list_node * ll_get_head_of_list(list_node *node){
	if (node->prev_n ==NULL){
		return node;
	}
	else{
		printf("Updated head to %d from %d\n",node->prev_n->tk, node->tk);
		ll_print_tk_num_voters(node->prev_n);
		list_node * tmp=node;
		while( tmp->prev_n!=NULL ){
			tmp=tmp->prev_n;
		}
		node=tmp;

		return node;
	}

}













