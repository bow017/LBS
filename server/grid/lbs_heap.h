#ifndef SERVER_GRID_LBS_NN_HEAP_H_
#define SERVER_GRID_LBS_NN_HEAP_H_

#include "server/grid/lbs_defs.h"

typedef struct lbs_heapnode_s
{
  double distance;
  uint8_t is_grid;
  int cell_id;
  lbs_mov_node_t* node;
} lbs_heapnode_t;

typedef struct lbs_nnheap_s
{
  uint32_t capacity;
  uint32_t size;
  lbs_heapnode_t *heap_nodes;
} lbs_nnheap_t;

/* Initialization */
int lbs_nnheap_init(lbs_nnheap_t* lbs_nnheap);

/* Destructor */
int lbs_nnheap_destroy(lbs_nnheap_t* lbs_nnheap);

/* Insert */
int lbs_nnheap_insert(lbs_nnheap_t* lbs_nnheap, lbs_mov_node_t* lbs_mov_node, int cell_id, uint8_t is_grid, double distance);

/* Top */
lbs_heapnode_t* lbs_nnheap_top(lbs_nnheap_t* lbs_nnheap);

/* Pop */
void lbs_nnheap_pop(lbs_nnheap_t* lbs_nnheap);

int lbs_nnheap_parent(int index) { return (index-1)/2 }

int lbs_nnheap_left(int index) { return (index*2+1); }

int lbs_nnheap_right(int index) { return (index*2+2); }

/* Utility function that helps to swap two elements */
void lbs_nnheap_swap(lbs_mov_node_t* x, lbs_mov_node_t* y)

/* Recursive function that re-min-heapify the subtree structure */
void lbs_nnheap_MinHeapify(lbs_nnheap_t* lbs_nnheap, int i );


#endif //SERVER_GRID_LBS_NN_HEAP_S_
