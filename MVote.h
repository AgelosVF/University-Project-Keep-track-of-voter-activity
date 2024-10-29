#include <stdlib.h>
#include <string.h>

#define MAX_NAME 256

typedef struct voter{
	char fname[MAX_NAME];
	char lname[MAX_NAME];
	int pin;
	int zip;
	char voted;
}voter;
//-------------------------------------------------------------------------------------------------
//my_free calls malloc_usable_size to find the size of the memory ptr points before freeing it. Returns the
//size.
size_t my_free(void *ptr);


//-------------------------------------------------------------------------------------------------
//hash table funtions

typedef struct hash_table hash_table;

//create a table with size buckets that can hold cells without overflowing
hash_table *hashtable_create(unsigned int size,int cells);

//delete the hash table
int hashtable_destroy(hash_table *ht);

//print the hash table
void hashtable_print(hash_table *ht);

//add to the hashtable pointed by ht the voter person. Will return 1 for success and 0 for faillure. Insert will call the hashfunction to find the corresponding bucket. needs to check if it's needed to add buckets
int hashtable_insert(hash_table *ht, voter *person);

//search and return the voter with pin
voter * hashtable_search(hash_table *ht, int pin);
//search for the voter with pin remove him from the table and return a pointer to him
voter * hashtable_remove(hash_table *ht, int pin);

//return the number of objects in the hashtable
int hashtable_total_voters(hash_table *ht);

//-----------------------------------------------------------------------------------------------
//linked list functions

typedef struct list_node list_node;

//create a new node for the list containing a voter and insert it between prev and next
list_node *ll_create_node(voter *vtr, list_node *prev, list_node *next);

//go thought the list and return the node with tk=zip_code. If node doesnt exist returns NULL 
list_node *ll_find_tk(list_node *lnode, int zip_code);

// search node for the voter with pin. If found return 1 else return 0
int ll_find_voter(list_node * snode, int spin);

//add voter to the list.If voter already exists print a message and returns 0 else returns 1.
int ll_add_voter_to_list(list_node *node,voter *vtr);

//returns the total number of people that voted
int ll_total_voted( list_node *node);

//prints the number of people that have voted in each zipcode in order from zipcode with the most to the one with the least.
void ll_print_tk_num_voters(list_node *node);

//print the id of each person that has voted with zipcode
void ll_print_tk_id_voters(list_node *node,int zipcode);

//destroy the list and free the memory. Doesnt free the voters that should be done by calling hashtable destroy
int ll_destroy(list_node *node);


