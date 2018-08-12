#include "server/grid/lbs_grid.h"

#include <iostream>

int lbs_grid_init(lbs_grid_t* lbs_grid, double lon1, double lon2, double lat1, double lat2, int row_num, int col_num)
{
	lbs_grid->row_num = row_num;
	lbs_grid->col_num = col_num;
	
	lbs_grid->lon_min = lon1;
	lbs_grid->lat_min = lat1;

	lbs_grid->cell_width = (lon2 - lon1)/col_num;
	lbs_grid->cell_height = (lat2 - lat1)/row_num;

	lbs_hashtable_init(&lbs_grid->hash_table);

	lbs_grid->cell = new lbs_cell_t[col_num*row_num];
	
	if(!lbs_grid->cell)
	{
		return -1;
	}

	for(int i = 0; i < col_num*row_num ; i++)
	{
		lbs_queue_init(&(lbs_grid->cell[i].dummy_node.queue));
	}

	return 0;
}

int lbs_grid_destroy(lbs_grid_t* lbs_grid)
{
	for( int i = 0; i < lbs_grid->row_num * lbs_grid->col_num; i++)
	{
		lbs_queue_t* temp = &(lbs_grid->cell[i].dummy_node.queue);
		while(!(lbs_queue_empty(temp)))
		{
			lbs_mov_node_t* current = (lbs_mov_node_t*)temp->next;
			lbs_queue_remove((lbs_queue_t*)current);

			delete current;
		}
	}

	delete lbs_grid->cell;

	return 0;
}

int lbs_grid_update(lbs_grid_t* lbs_grid, double lon, double lat, uint64_t timestamp, uint32_t id)
{
	int row = lbs_grid_cell_row(lbs_grid, lat);
	int col = lbs_grid_cell_col(lbs_grid, lon);
	uint32_t cell_id = lbs_grid_cell_id(lbs_grid, row, col);
	
	lbs_hashnode_t* head = lbs_hashtable_get(&lbs_grid->hash_table, id);

	if( head == nullptr || (lbs_queue_empty(&head->queue)))
	{
		lbs_mov_node_t* new_mov = new lbs_mov_node_t;

		new_mov->id = id;
		new_mov->lat = lat;
		new_mov->lon = lon;
		new_mov->timestamp = timestamp;
		
		lbs_queue_init(&new_mov->queue);
		lbs_queue_insert_head(&lbs_grid->cell[cell_id].dummy_node.queue, &new_mov->queue);
		
		lbs_hashtable_set(&lbs_grid->hash_table, id, new_mov, cell_id);
		
		return 0;
	}
	else
	{
		lbs_hashnode_t* ret_node = lbs_hashtable_get(&lbs_grid->hash_table, id);
		
		ret_node->mov_node->lat = lat;
		ret_node->mov_node->lon = lon;
		ret_node->mov_node->timestamp = timestamp;

		if(ret_node->cell_id == cell_id)
		{
			return 0;
		}
		else
		{
			lbs_queue_remove(&ret_node->mov_node->queue);
			lbs_queue_insert_head(&lbs_grid->cell[cell_id].dummy_node.queue, &ret_node->mov_node->queue);
			ret_node->cell_id = cell_id;
			
			return 0;
		}
	}

}

int lbs_grid_cell_row(lbs_grid_t* lbs_grid, double lat)
{
	return (lat - lbs_grid->lat_min)/lbs_grid->cell_height;
}

int lbs_grid_cell_col(lbs_grid_t* lbs_grid, double lon)
{
	return (lon - lbs_grid->lon_min)/lbs_grid->cell_width;
}

int lbs_grid_cell_id(lbs_grid_t* lbs_grid, int cell_row, int cell_col)
{
	return cell_row*lbs_grid->col_num+cell_col;
}

void lbs_grid_cell_row_col(lbs_grid_t* lbs_grid, int cell_id, int* cell_row, int* cell_col)
{
	*cell_row = cell_id/lbs_grid->col_num;
	*cell_col = cell_id%lbs_grid->col_num;
}

lbs_cell_t* lbs_grid_cell(lbs_grid_t* lbs_grid, int cell_id)
{
	return &lbs_grid->cell[cell_id];
}
