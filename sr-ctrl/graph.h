#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdint.h>

#include "arraylist.h"
#include "hashmap.h"

struct node {
	unsigned int id;
	void *data;
};

struct edge {
	struct node *local;
	struct node *remote;
	unsigned int id;
	uint32_t metric;
	uint32_t bw;
	uint32_t ava_bw;
	uint32_t delay;
};

struct nodepair {
	struct node *local;
	struct node *remote;
};

/* (local,remote) => minimal edge (or null) */
/* node => neighbors */

struct graph {
	struct arraylist *nodes;
	struct arraylist *edges;
	unsigned int last_node;
	unsigned int last_edge;
	struct hashmap *min_edges;
	struct hashmap *neighs;
};

struct dres {
	struct hashmap *dist;
	struct hashmap *path;
	int n_nodes;
};

struct graph *graph_new(void);
void graph_destroy(struct graph *g, bool shallow);
struct node *graph_add_node(struct graph *g, void *data);
void graph_remove_node(struct graph *g, struct node *node);
struct node *graph_get_node(struct graph *g, unsigned int id);
struct edge *graph_add_edge(struct graph *g, struct node *local,
			    struct node *remote);
void graph_remove_edge(struct graph *g, struct edge *edge);
void graph_compute_minimal_edges(struct graph *g);
void graph_compute_all_neighbors(struct graph *g);
struct graph *graph_clone(struct graph *g);
void graph_dijkstra(struct graph *g, struct node *src, struct dres *res);
void graph_dijkstra_free(struct dres *res);
int graph_prune(struct graph *g, bool (*prune)(struct edge *e, void *arg),
		void *_arg);

static inline void graph_finalize(struct graph *g)
{
	graph_compute_minimal_edges(g);
	graph_compute_all_neighbors(g);
}

#endif
