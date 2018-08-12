#include "server/grid/lbs_index.h"

#include <stdio.h>
#include <stdlib.h>

#include "common/log.h"

#include "server/grid/lbs_distance.h"
#include "server/grid/lbs_grid.h"

#define LBS_LON_MIN 116
#define LBS_LON_MAX 117
#define LBS_LAT_MIN 39
#define LBS_LAT_MAX 41

#define LBS_ROW_NUM 200
#define LBS_COL_NUM 100


static lbs_grid_t grid;
// 初始化网格索引
int lbs_grid_index_init() {
  
  lbs_grid_init(&grid, LBS_LON_MIN, LBS_LON_MAX, LBS_LAT_MIN, LBS_LAT_MAX, LBS_ROW_NUM, LBS_COL_NUM);

  return 0;
}

// 更新接口[出租车位置更新]
int lbs_grid_index_update(double lon,
                           double lat,
                           uint64_t timestamp,
                           uint32_t id) {

  lbs_grid_update(&grid, lon, lat, timestamp, id);

  return 0;
}

// 范围查询接口[查询某一范围内的所有出租车信息]
int lbs_grid_index_range_query(double lon1, double lon2, double lat1, double lat2, lbs_res_node_t* out)
{

  int row_start = lbs_grid_cell_row(&grid, lat1);
  int row_end = lbs_grid_cell_row(&grid, lat2);

  int col_start = lbs_grid_cell_col(&grid, lon1);
  int col_end = lbs_grid_cell_col(&grid, lon2);

  for( int r = row_start; r <= row_end; r++)
  {
  	for( int c = col_start; c <= col_end; c++)
	{
		int cell_id = lbs_grid_cell_id(&grid, r, c);
		
		lbs_queue_t* head = &grid.cell[cell_id].dummy_node.queue;

		if( !(lbs_queue_empty(head)) )
		{
			lbs_queue_t* current = head->next;

			while( current != head )
			{
				lbs_mov_node_s* temp = (lbs_mov_node_s*) current;
				
				if(temp->lon >= lon1 && temp->lat >= lat1 && temp->lon <= lon2 && temp->lat <= lat2)
				{
					lbs_res_node_t* new_res = new lbs_res_node_t;
					
					new_res->lon = temp->lon;
					new_res->lat = temp->lat;

					new_res->timestamp = temp->timestamp;
					new_res->id = temp->id;

					lbs_queue_init(&new_res->queue);

					lbs_queue_insert_head(&(out->queue),&(new_res->queue));

				}

				current = current->next;
			}
		}
	}
  }
  return 0;
}

// NN查询接口[查询离lon,lat最近的出租车]
int lbs_grid_index_nn_query(double lon, double lat, lbs_res_node_t* out) {
  // TODO: by student
  return 0;
}

void lbs_grid_min_dist(int cell_id, double lon, double lat, double** ret)
{
	int cell_row, cell_col;

	lbs_grid_cell_row_col(&grid, cell_id, &cell_row, &cell_col);

	double cell_min_lat = cell_row * cell_height;
	double cell_min_lon = cell_col * cell_width;

	double cell_max_lat = (cell_row + 1) * cell_height;
	double cell_max_lon = (cell_col + 1) * cell_width;
	
	double cell_lat2 = (cell_row) * cell_height;
	double cell_lon2 = (cell_col + 1)* cell_width;

	double cell_lat3 = (cell_row + 1)* cell_height;
	double cell_lon3 = (cell_col) * cell_width;

	ret[0] = lbs_distance(lon, lat, cell_min_lon, cell_min_lat);
	ret[7] = lbs_distance(lon, lat, cell_max_lon, cell_max_lat);i
	
	ret[2] = lbs_distance(lon, lat, cell_lon2, cell_lat2);
	ret[5] = lbs_distance(lon, lat, cell_lon3, cell_lat3);

	ret[1] = lbs_distance(lon, lat, lon, cell_min_lat);
	ret[6] = lbs_distance(lon, lat, lon, cell_max_lat);

	ret[3] = lbs_distance(lon, lat, cell_min_lon, lat);
	ret[4] = lbs_distance(lon, lat, cell_max_lon, lat);
}
