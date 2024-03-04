#include "common_player.h"

unsigned int loc_player_id; //the id of the player, between 0 and NUM_PLAYERS-1
struct graph_t *loc_graph;
struct graph_t *origin_graph;
unsigned int loc_num_queens; //the number of queens per player
unsigned int *loc_queens[NUM_PLAYERS]; //an array containing, for each player, an array of length `num_queens`, giving the ids of the positions of the quee
unsigned int *arrows;
struct all_moves_t all_moves;
unsigned int *all_moves_pos;
unsigned int *indexes;
struct all_moves_t all_moves2;
unsigned int *all_moves_pos2;
unsigned int *indexes2;
struct game_state_t state;

// Copy values to local variables.
void initialize(
	unsigned int player_id, struct graph_t *graph, unsigned int num_queens,
	unsigned int *queens[NUM_PLAYERS]) {

	loc_player_id = player_id;
	loc_graph = graph;
	loc_num_queens = num_queens;
	for (int i = 0; i < NUM_PLAYERS; i++)
		loc_queens[i] = queens[i];
	origin_graph = copy_graph_client(graph, graph->num_vertices/2); // Division par 2 car m<=sqrt(num_vertices)
	arrows = malloc(graph->num_vertices*sizeof(unsigned int));
	for (unsigned int i=0; i<graph->num_vertices; i++)
		arrows[i] = 0; //There is no arrow at the start.

	all_moves_pos = malloc(graph->num_vertices*graph->num_vertices*sizeof(unsigned int));
	indexes = malloc((graph->num_vertices+1)*sizeof(unsigned int));
	all_moves.dsts = all_moves_pos;
	all_moves.dsts_i = indexes;

	all_moves_pos2 = malloc(graph->num_vertices*graph->num_vertices*sizeof(unsigned int));
	indexes2 = malloc((graph->num_vertices+1)*sizeof(unsigned int));
	all_moves2.dsts = all_moves_pos2;
	all_moves2.dsts_i = indexes2;

	state.graph = loc_graph;
	state.num_queens = loc_num_queens;
	state.player_id = loc_player_id;
	state.queens = loc_queens;
}


// Display the end message and free all the heap-allocated variables.
void finalize() {
  printf("Game ended.\n");
  for (int i = 0; i < NUM_PLAYERS; i++)
  {
	  free(loc_queens[i]);
  }
  free_graph(loc_graph);
  free_graph(origin_graph);
  free(arrows);
  free(all_moves_pos);
  free(indexes);
  free(all_moves_pos2);
  free(indexes2);
}

unsigned int next(unsigned int player_id)
{
  return (player_id+1)%NUM_PLAYERS;
}

unsigned int previous(unsigned int player_id)
{
  return (player_id+NUM_PLAYERS-1)%NUM_PLAYERS;
}

int max(int a, int b) { return a > b ? a : b; }

int min(int a, int b) { return a < b ? a : b; }

unsigned int get_num_queens(unsigned int nb_vertices)
{
  return 4*((unsigned int)(sqrt(nb_vertices))/10+1);
}

struct graph_t *add_arrow_client(struct graph_t *graph, unsigned int i)
{
  arrows[i] = 1;
  for (int k = graph->t->p[i]; k < graph->t->p[i+1]; k++)
  {
    unsigned int j = graph->t->data[k];
    int column = graph->t->i[k];

    unsigned int direction_op = (j + 4)%8;
    direction_op = direction_op+8*(int)(!direction_op);

    for (int b = graph->t->p[column]; b < graph->t->p[column + 1]; b++)
      {
        if (graph->t->data[b] == direction_op)
        {
          graph->t->data[b] = 0;
          break;
        }
      }

    graph->t->data[k] = 0;
  }
  return graph;
}

struct graph_t *copy_graph_client(struct graph_t * graph, size_t m)
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

int is_in_client(unsigned int x, unsigned int t[], unsigned int length)
{
  for (unsigned int i=0; i<length; i++)
    {
      if (t[i]==x)
	      return 1;
    }
  return 0;
}

int is_in_queens(unsigned int x, unsigned int *queens[NUM_PLAYERS], unsigned int length)
{
  for (int i=0; i<NUM_PLAYERS; i++)
    {
      if (is_in_client(x, queens[i], length))
	return 1;
    }
  return 0;
}

void free_spots_dir_a(unsigned int position, enum dir_t dir, struct graph_t *graph, unsigned int *queens[],  unsigned int num_queens, unsigned int *free_positions)
{
  unsigned int filled = 0;
  gsl_spmatrix_uint *csr = graph->t;
  unsigned int current = position;
  unsigned int old = position;
  for (unsigned int i = 0; i<graph->num_vertices; i++)
    {
      for (int k = csr->p[current]; k < csr->p[current+1]; k++) { //going through the neighbours of current position in one direction
        unsigned int j = csr->i[k];
        old = current;
        if (csr->data[k]==dir)
          {
            if (is_in_queens(j, queens, num_queens))
              break; //Il est impossible de continuer à travers une autre pièce.
            current = j;
			free_positions[filled] = current;
            filled++;
            break; //On change de current.
          }

      }
      if (old==current) //Si le current ne possède pas de voisin valide dans la direction.
        break;
    }
	free_positions[filled] = SENTINEL;
}

void free_spots_a(unsigned int position, struct graph_t *graph, unsigned int *queens[], unsigned int num_queens, unsigned int *free_positions)
{
  int to_fill = 0;
  for (int dir = FIRST_DIR; dir<LAST_DIR+1; dir++) // go through all directions
    {
      free_spots_dir_a(position, dir, graph, queens, num_queens, &free_positions[to_fill]);
      for (; free_positions[to_fill]!=SENTINEL; to_fill++)
        {
          ;
        }
    }
}

void copy(unsigned int src[], unsigned int dst[], unsigned int length)
{
  for (unsigned int i=0; i<length; i++)
    dst[i] = src[i];
}

void shuffle(unsigned int t[], unsigned int length)
{
  for (unsigned int i=0; i<length; i++)
    {
      unsigned int index = rand()%length;
      unsigned int index_2 = rand()%length;
      while (index==index_2)
        index_2 = rand()%length;
      unsigned int tmp = t[index];
      t[index] = t[index_2];
      t[index_2] = tmp;
    }
}

//Get the length of position array
unsigned int length(unsigned int pos[])
{
  unsigned int l = 0;
  while (pos[l] != SENTINEL)
    l++;
  return l;
}

void false_get_possible_moves_a(struct graph_t *graph, unsigned int *queens[], unsigned int num_queens, struct all_moves_t moves)
{
  unsigned int to_fill = 0;
  moves.dsts_i[0] = 0;
  for (unsigned int i=0; i<graph->num_vertices; i++)
  {
      free_spots_a(i, graph, queens, num_queens, &moves.dsts[to_fill]);

	  unsigned int length_pos = length(&moves.dsts[to_fill]);
	  to_fill += length_pos;
	  moves.dsts_i[i+1] = to_fill;
  }
}

unsigned int nb_dir_pos(struct graph_t *graph, unsigned int *queens[], unsigned int num_queens, unsigned int pos)
{
  unsigned int nb_dirs = 0;
  gsl_spmatrix_uint *csr = graph->t;
  for (int k = csr->p[pos]; k < csr->p[pos+1]; k++) { //going through the neighbours of pos position
	  unsigned int j = csr->i[k];
	  if (csr->data[k] && !is_in_queens(j, queens, num_queens))
	  {
		nb_dirs++;
	  }
  }
  return nb_dirs;
}

void update(struct move_t previous_move, unsigned int player_id, unsigned int num_queens, struct graph_t *graph, unsigned int *queens[], unsigned int self)
{
  if (!self)
  {
      unsigned int other = previous(player_id);
      for (unsigned int i = 0; i < num_queens; i++) {
        if (queens[other][i] == previous_move.queen_src) {
          queens[other][i] = previous_move.queen_dst;
          break;
        }
      }
    }
  if (previous_move.arrow_dst != UINT_MAX) {
	  add_arrow_client(graph, previous_move.arrow_dst);
	  arrows[previous_move.arrow_dst] = 1;
	}
}


void remove_arrow_client(struct graph_t *graph_origin, struct graph_t *graph_current, unsigned int arrow)
{
	arrows[arrow] = 0;
  for (int k = graph_current->t->p[arrow]; k < graph_current->t->p[arrow+1]; k++)
    {
      unsigned int dir = graph_origin->t->data[k];
      int column = graph_origin->t->i[k];

      unsigned int  direction_op = (dir + 4)%8;
      direction_op = direction_op+8*(int)(!direction_op);

      if (!arrows[column])
        {
          graph_current->t->data[k] = dir;
          for (int b = graph_origin->t->p[column]; b < graph_origin->t->p[column + 1]; b++)
            {
              if (graph_origin->t->data[b] == direction_op)
                {
                  graph_current->t->data[b] = direction_op;
                  break;
                }
            }
        }
    }
}

void apply_move(
	struct game_state_t *state, struct move_t move, unsigned int player_id) {
	state->graph = add_arrow_client(state->graph, move.arrow_dst);

	for (unsigned int i = 0; i < state->num_queens; i++) {
		if (state->queens[player_id][i] == move.queen_src) {
			state->queens[player_id][i] = move.queen_dst;
			break;
		}
	}
}

void cancel_move(
	struct game_state_t *state, struct move_t move, unsigned int player_id) {
	remove_arrow_client(origin_graph, state->graph, move.arrow_dst);

	for (unsigned int i = 0; i < state->num_queens; i++) {
		if (state->queens[player_id][i] == move.queen_dst) {
			state->queens[player_id][i] = move.queen_src;
			break;
		}
	}
}

int minimax(
	struct game_state_t *state, unsigned int player_id, int depth, int alpha,
	int beta, int (*score_func)(struct game_state_t *state)) {
	//  Base case: reached maximum depth or no more moves
	if (!depth) { // not forget if there are no possible moves
		return score_func(state);
	}

	int best_score = player_id ? INT_MAX: INT_MIN;
	false_get_possible_moves_a(
		state->graph, state->queens, state->num_queens, all_moves2);
	for (unsigned int queens_index = 0; queens_index < state->num_queens; queens_index++) // For all queens.
      {
		unsigned int i = state->queens[player_id][queens_index];
		for (unsigned int k = all_moves2.dsts_i[i]; k < all_moves2.dsts_i[i + 1]; k++) // Iterates over the possible destinations from i.
          {
			unsigned int j = all_moves2.dsts[k]; // A possible destination.
			struct move_t move_and_block = {.queen_src = i, .queen_dst = j, .arrow_dst = i}; 
            // Don't forget that a queen can move and shoot at her source.

			apply_move(state, move_and_block, player_id);
			int score = minimax(state, 1, depth - 1, alpha, beta, score_func);
			cancel_move(state, move_and_block, player_id);
			best_score = max(best_score, score);
			if (player_id) {
              beta = min(beta, best_score);
			} else {
              alpha = max(alpha, best_score);
			}

			if (alpha >= beta) {
              break;
			}
			for (unsigned int l=all_moves2.dsts_i[j]; l<all_moves2.dsts_i[j + 1]; l++) 
              // Iterates over the possible destinations from the new position of the queen.
              {
				unsigned int m = all_moves2.dsts[l]; // Gets a position for the arrow.
				struct move_t move = {.queen_src = i, .queen_dst = j, .arrow_dst = m};
				apply_move(state, move, player_id);
				int score = minimax(state, 1, depth - 1, alpha, beta, score_func);
				cancel_move(state, move, player_id);
				best_score = max(best_score, score);

				if (player_id) {
                  beta = min(beta, best_score);
				} else {
                  alpha = max(alpha, best_score);
				}

				if (alpha >= beta) {
					break;
				}
			}
		}
	}
	return best_score;
}
