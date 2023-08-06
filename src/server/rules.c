#include "player.h"
#include "server.h"

unsigned int next_player(unsigned int player_id) {
	return (player_id + 1) % NUM_PLAYERS;
}

unsigned int previous_player(unsigned int player_id) {
	return (player_id + NUM_PLAYERS - 1) % NUM_PLAYERS;
}

// Adds an arrow at position i on the graph of size n*n.
struct graph_t *add_arrow(struct graph_t *graph, unsigned int i) {
	for (int k = graph->t->p[i]; k < graph->t->p[i + 1]; k++) {
		int j = graph->t->data[k];
		int column = graph->t->i[k];

		switch (j) {
		case 1: {
			for (int b = graph->t->p[column]; b < graph->t->p[column + 1];
				 b++) {
				if (graph->t->data[b] == 5)
					graph->t->data[b] = 0;
			}
			break;
		}
		case 2: {
			for (int b = graph->t->p[column]; b < graph->t->p[column + 1];
				 b++) {
				if (graph->t->data[b] == 6)
					graph->t->data[b] = 0;
			}
			break;
		}
		case 3: {
			for (int b = graph->t->p[column]; b < graph->t->p[column + 1];
				 b++) {
				if (graph->t->data[b] == 7)
					graph->t->data[b] = 0;
			}
			break;
		}
		case 4: {
			for (int b = graph->t->p[column]; b < graph->t->p[column + 1];
				 b++) {
				if (graph->t->data[b] == 8)
					graph->t->data[b] = 0;
			}
			break;
		}
		case 5: {
			for (int b = graph->t->p[column]; b < graph->t->p[column + 1];
				 b++) {
				if (graph->t->data[b] == 1)
					graph->t->data[b] = 0;
			}
			break;
		}
		case 6: {
			for (int b = graph->t->p[column]; b < graph->t->p[column + 1];
				 b++) {
				if (graph->t->data[b] == 2)
					graph->t->data[b] = 0;
			}
			break;
		}
		case 7: {
			for (int b = graph->t->p[column]; b < graph->t->p[column + 1];
				 b++) {
				if (graph->t->data[b] == 3)
					graph->t->data[b] = 0;
			}
			break;
		}
		case 8: {
			for (int b = graph->t->p[column]; b < graph->t->p[column + 1];
				 b++) {
				if (graph->t->data[b] == 4)
					graph->t->data[b] = 0;
			}
			break;
		}
		}
		graph->t->data[k] = 0;
	}
	return graph;
}
/*
struct graph_t*apply_move(struct graph_t *graph, struct move_t move){}
*/

int is_in(unsigned int x, unsigned int t[], unsigned int length) {
	for (unsigned int i = 0; i < length; i++) {
		if (t[i] == x)
			return 1;
	}
	return 0;
}

// TODO detects the end of the game
unsigned int is_end_game(
	struct move_t previous_move, struct move_t last_move){
	if (previous_move.arrow_dst == last_move.arrow_dst) {
		if (previous_move.queen_dst == last_move.queen_dst) {
			if (previous_move.queen_src == last_move.queen_src) {
				return 1;
			}
		}
	}

	return 0;
}

unsigned int get_num_queens_server(unsigned int nb_vertices) {
	return 4 * ((unsigned int)(sqrt(nb_vertices) + 0.1) / 10 + 1);
}
void update_server(
	struct move_t previous_move, struct graph_t *graph, unsigned int player_id,
	unsigned int **queens) {
	for (unsigned int i = 0; i < get_num_queens_server(graph->num_vertices);
		 i++) {
		if (queens[player_id][i] == previous_move.queen_src) {
			queens[player_id][i] = previous_move.queen_dst;
			break;
		}
	}

	if (previous_move.arrow_dst != UINT_MAX)
		add_arrow(graph, previous_move.arrow_dst);
}

struct graph_t *copy_graph(struct graph_t * graph, size_t m)
{
    struct graph_t * copy = malloc(sizeof(struct graph_t));
    copy->num_vertices = graph->num_vertices;
    gsl_spmatrix_uint * tmp = gsl_spmatrix_uint_alloc(m*m, m*m);

    for (size_t i = 0; i < graph->t->nz; i++)
    {
        tmp->data[i] = graph->t->data[i];
        tmp->i[i] = graph->t->i[i];
    } 
    for (size_t i = 0; i < graph->t->size1+1; i++)
    {
        tmp->p[i] = graph->t->p[i];
    }
    tmp->nz = graph->t->nz;
    tmp->size1 = graph->t->size1;
    tmp->node_size = graph->t->node_size;
    tmp->nzmax = graph->t->nzmax;
    tmp->size2 = graph->t->size2;

    copy->t = tmp;

    return copy;
} 

int is_in_queens_server(
	unsigned int x, unsigned int *queens[NUM_PLAYERS], unsigned int length) {
	for (int i = 0; i < NUM_PLAYERS; i++) {
		if (is_in(x, queens[i], length))
			return 1;
	}
	return 0;
}

int is_valid_move(
	struct move_t *move, struct graph_t *graph, unsigned int **queens) {
	unsigned int num_queens = get_num_queens_server(graph->num_vertices);
	if (is_in_queens_server(move->queen_dst, queens, num_queens)) {
		printf("move on queen\n");
		return 0;
	}
	if (is_in_queens_server(move->arrow_dst, queens, num_queens) && move->arrow_dst!=move->queen_src) {
		printf("shoot queen\n");
		return 0;
	}
	
	gsl_spmatrix_uint *csr = graph->t;
	for (unsigned int i = 0; i < graph->num_vertices; i++) {
		for (int k = csr->p[i]; k < csr->p[i + 1]; k++) {
			unsigned int j = csr->i[k];
			if(move->queen_dst == i || move->queen_dst == j){
				if(csr->data[k] != 0){
					return 1;
				}
			}
			if ((j == move->arrow_dst) || (i == move->arrow_dst)) {
				if (csr->data[k] != 0) {
					return 1;
				}
			}
		}
	}
	printf("shoot arrow or move on arrow\n");
	return 0;
}
