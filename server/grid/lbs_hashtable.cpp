#define CAP_LIMIT 20000

#include "server/grid/lbs_hashtable.h"

#include <iostream>
int lbs_hashtable_init(lbs_hashtable_t* lbs_hashtable)
{
	lbs_hashtable->size = 0;

	lbs_hashtable->capacity = CAP_LIMIT;

	lbs_hashtable->hash_nodes = new lbs_hashnode_t[CAP_LIMIT];

	if(!lbs_hashtable->hash_nodes)
	{
		return -1;	
	}

	for(int i = 0; i < CAP_LIMIT; i++)
	{
		lbs_queue_init(&lbs_hashtable->hash_nodes[i].queue);
		lbs_hashtable->hash_nodes[i].mov_node = NULL;
		lbs_hashtable->hash_nodes[i].cell_id = 0;
	}

	return 0;
}

int lbs_hashtable_destroy(lbs_hashtable_t* lbs_hash_table)
{
	for( int i = CAP_LIMIT - 1; i >= 0; i--)
	{
		lbs_queue_t* temp = &(lbs_hash_table->hash_nodes[i].queue);
		while(!(lbs_queue_empty(temp)))
		{
			lbs_hashnode_t* current = (lbs_hashnode_t*)lbs_hash_table->hash_nodes[i].queue.next;
			lbs_queue_remove(lbs_hash_table->hash_nodes[i].queue.next);

			delete current;
		}

	}
	delete lbs_hash_table->hash_nodes;

	return 0;
}

int lbs_hashtable_set(lbs_hashtable_t* lbs_hashtable, uint32_t id, lbs_mov_node_t* lbs_mov_node, int cell_id)
{
	int hash_index = id % CAP_LIMIT;
	
	lbs_hashnode_t* new_node = new (lbs_hashnode_t);

	if(!new_node)
	{
		return -1;
	}

	lbs_queue_init(&new_node -> queue);

	new_node -> cell_id = cell_id;

	new_node -> mov_node = lbs_mov_node;

	lbs_queue_insert_head(&lbs_hashtable->hash_nodes[hash_index].queue, &new_node->queue);

	return 0;
}

lbs_hashnode_t* lbs_hashtable_get(lbs_hashtable_t* hash_table, uint32_t id)
{
	int hash_index = id % CAP_LIMIT;
	
	lbs_hashnode_t* current = (lbs_hashnode_t*)hash_table -> hash_nodes[hash_index].queue.next;

	lbs_hashnode_t* head = (lbs_hashnode_t*)&hash_table->hash_nodes[hash_index].queue;
	if(current == nullptr)
	{
		return nullptr;
	}

	while(current != head  && current -> mov_node -> id != id)
	{
		std::cout<<"in while loop" << std::endl;
		if( current == nullptr)
		{
			return nullptr;
		}
		current = (lbs_hashnode_t*)current->queue.next;
	}


	return current == head ? NULL : current;
}
