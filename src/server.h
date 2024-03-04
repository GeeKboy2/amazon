#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "dir.h"
#include "graph.h"
#include "move.h"
#include "player.h"

void display_2D_array(struct graph_t graph, size_t n);

void display_board(
	struct graph_t *graph, size_t n, unsigned int **queens,
	unsigned int nb_queens, struct move_t last);

unsigned int next_player(unsigned int player_id);

unsigned int previous_player(unsigned int player_id);

struct graph_t *add_arrow(struct graph_t *graph, unsigned int i);

int is_in(unsigned int x, unsigned int t[], unsigned int length);

unsigned int is_end_game(
	struct move_t previous_move, struct move_t last_move);

unsigned int get_num_queens_server(unsigned int nb_vertices);

void update_server(
	struct move_t previous_move, struct graph_t *graph, unsigned int player_id,
	unsigned int **queens);

struct graph_t * copy_graph(struct graph_t * graph, size_t m);

int is_valid_move(
	struct move_t *move, struct graph_t *graph, unsigned int **queens);

