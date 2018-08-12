#include "server/grid/lbs_heap.h"

int lbs_nnheap_init(lbs_nnheap_t* lbs_nnheap)
{
	lbs_nnheap->capacity = 10;
	lbs_nnheap->size = 0;
	
	lbs_nnheap->heap_nodes = new lbs_heapnode_t[lbs_nnheap->capacity];

	if(!lbs_nnheap->heap_nodes)
	{
		return -1;
	}

	for( auto f: lbs_nnheap->heap_nodes)
	{
		f.distance = 0;
		f.is_gird = 0;
		f.cell_id = 0;
		f.node = 0;
	}

	return 0;
}

int lbs_nnheap_destroy(lbs_nnheap_t* lbs_nnheap)
{
	delete lbs_nnheap->heap_nodes;
}

int lbs_nnheap_insert(lbs_nnheap_t* lbs_nnheap, lbs_mov_node_t* lbs_mov_node, int cell_id, uint8_t is_grid, double distance)
{
	if(!( lbs_nnheap->size < capacity ))
	{
		if(lbs_nnheap->heap_nodes)
		{
			auto new_ptr = (lbs_heapnode_t*) realloc( lbs_nnheap->heap_nodes, (capacity+10)*sizeof(lbs_heapnode_t));
			
			if(new_ptr)
			{
				lbs_nnheap->capacity += 10;
				lbs_nnheap->heap_nodes = new_ptr;
			}
		}
	}

	int index = lbs_nnheap->size;
	(lbs_nnheap->size)++;

	lbs_nnheap->heap_nodes[index].node = lbs_mov_node;
	lbs_nnheap->heap_nodes[index].distance = distance;
	lbs_nnheap->heap_nodes[index].cell_id = cell_id;
	lbs_nnheap->heap_nodes[index].is_grid = is_grid;

	while( index !=0 && lbs_nnheap->heap_nodes[lbs_nnheap_parent(index)].distance > lbs_nnheap->heap_nodes[index].distance )
	{
		lbs_nnheap_swap(&lbs_nnheap->heap_nodes[index], &lbs_nnheap->heap_nodes[lbs_nnheap_parent(index)];

		index = lbs_nnheap_parent(index);
	}
}

lbs_heapnode_t* lbs_nnheap_top(lbs_nnheap_t* lbs_nnheap)
{
	return &lbs_nnheap->heap_nodes[0];
}

void lbs_nnheap_pop(lbs_nnheap_t* lbs_nnheap)
{
	if(lbs_nnheap->size == 1)
	{
		(lbs_nnheap->size)--;
		
		return 0;
	}

	lbs_nnheap->heap_nodes[0] = lbs_nnheap->heap_node[lbs_nnheap->size-1];
	(lbs_nnheap->size)--;
	
	lbs_nnheap_MinHeapify( lbs_nnheap, 0 );

	return 0;
	
}

void lbs_nnheap_swap(lbs_mov_node_t* x, lbs_mov_node_t* y)
{
	auto temp = *x;

	*x = *y;
	*y = temp;
}

void lbs_nnheap_MinHeapify(lbs_nnheap_t* lbs_nnheap, int i )
{
	int l = lbs_nnheap_left(i);
	int r = lbs_nnheap_right(i);
	int smallest = i;
	
	if( l<lbs_nnheap->size && lbs_nnheap->heap_nodes[l].distance < lbs_nnheap->nodes[i].distance )
	{
		smallest = l;
	}
	if( r<lbs_nnheap->size && lbs_nnheap->heap_nodes[r].distance < lbs_nnheap->nodes[smallest].distance )
	{
		smallest = r;
	}
	
	if( smallest != i )
	{
		lbs_nnheap_swap(&lbs_nnheap->heap_nodes[i],&lbs_nnheap->heap_nodes[smallest]);
		
		lbs_nnheap_MinHeapify(lbs_nnheap, smallest);
	}
}
