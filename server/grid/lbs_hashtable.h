#include <pthread.h>

#include "server/grid/lbs_defs.h"

typedef struct lbs_hashnode_s
{
  lbs_queue_t queue;

  lbs_mov_node_t* mov_node;

  uint32_t cell_id;
} lbs_hashnode_t;

typedef struct lbs_hastable_s
{
  pthread_mutex_t mutex;

  int size;

  int capacity;

  lbs_hashnode_t* hash_nodes;
} lbs_hashtable_t;


// Initialization
int lbs_hashtable_init(lbs_hashtable_t* lbs_hashtable);

// Destructor
int lbs_hashtable_destroy(lbs_hashtable_t* lbs_hash_table);

// Setter
int lbs_hashtable_set(lbs_hashtable_t* lbs_hashtable, uint32_t id, lbs_mov_node_t* lbs_mov_node, int cell_id);

// Getter
lbs_hashnode_t* lbs_hashtable_get(lbs_hashtable_t* hash_table, uint32_t id);
