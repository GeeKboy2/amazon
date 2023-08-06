#include "common_player.h"
#include "graph.h"
#include "move.h"
#include "player.h"
#include "server.h"

#define MAX_DEPTH 0 // Maximum depth of the search tree

extern unsigned int
	loc_player_id; // the id of the player, between 0 and NUM_PLAYERS-1
extern struct graph_t *loc_graph;
extern struct graph_t *origin_graph;
extern unsigned int loc_num_queens; // the number of queens per player
extern unsigned int
	*loc_queens[NUM_PLAYERS]; // an array containing, for each player, an array
							  // of length `num_queens`, giving the ids of the
							  // positions of the queens

// Structure to represent the game state
struct game_state_t {
	struct graph_t *graph;	 // the game board
	unsigned int num_queens; // number of queens per player
	unsigned int player_id;	 // current player ID
	unsigned int **queens;	 // queens for all players
};

char const *get_player_name() { return "The_Watcher_depth"; }

/*
// Creates a deep copy of a game state
struct game_state_t *copy_state_a(struct game_state_t *state) {
	struct game_state_t *copy = malloc(sizeof(struct game_state_t));
	copy->graph = copy_graph_client(state->graph, state->graph->num_vertices);
	copy->num_queens = state->num_queens;
	copy->player_id = state->player_id;
	unsigned int *init_quee[] = {NULL, NULL};
	copy->queens = init_quee;
	copy->queens[0] = malloc(sizeof(unsigned int) * state->num_queens);
	copy->queens[1] = malloc(sizeof(unsigned int) * state->num_queens);
	for (int num_players = 0; num_players < NUM_PLAYERS; num_players++) {
		for (unsigned int i = 0; i < state->num_queens; i++) {
			copy->queens[num_players][i] = state->queens[num_players][i];
		}
	}
	return copy;
}
*/
void free_state(struct game_state_t *state) {
	free(state->queens[0]);
	free(state->queens[1]);
	free(state);
}

int max(int a, int b) { return a > b ? a : b; }

int min(int a, int b) { return a < b ? a : b; }

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

// This functions computes a score that is equal to the difference of number of
// possible moves between the player and the ennemi
int score(struct game_state_t *state) {
	struct all_moves_t all_moves = false_get_possible_moves_a(
		state->graph, state->queens, state->num_queens);

	int counts[NUM_PLAYERS] = {0};
	for (unsigned int player = 0; player < NUM_PLAYERS; player++) {
		for (unsigned int queens_index = 0; queens_index < state->num_queens;
			 queens_index++) // For all queens.
		{
			unsigned int i = state->queens[player][queens_index];
			for (unsigned int k = all_moves.dsts_i[i];
				 k < all_moves.dsts_i[i + 1]; k++) {
				unsigned int j = all_moves.dsts[k];
				counts[player] +=
					all_moves.dsts_i[j + 1] - all_moves.dsts_i[j] + 1;
			}
		}
	}
	free_false_possible_moves(all_moves);
	return counts[state->player_id] - counts[1 - state->player_id];
}

int minimax(
	struct game_state_t *state, unsigned int player_id, int depth, int alpha,
	int beta, int step) {
	// printf("minmax calling depth :%d\n", depth);
	//  Base case: reached maximum depth or no more moves
	if (!depth) { // not forget if there are no possible moves
		return score(state);
	}

	int best_score = player_id ? INT_MAX : INT_MIN;
	struct all_moves_t all_moves = false_get_possible_moves_a(
		state->graph, state->queens, state->num_queens);
	for (unsigned int queens_index = 0; queens_index < state->num_queens;
		 queens_index += step) // For all queens.
	{
		unsigned int i = state->queens[player_id][queens_index];
		for (unsigned int k = all_moves.dsts_i[i]; k < all_moves.dsts_i[i + 1];
			 k += step) // Iterates over the possible destinations from i.
		{
			unsigned int j = all_moves.dsts[k]; // A possible destination.
			struct move_t move_and_block = {
				.queen_src = i,
				.queen_dst = j,
				.arrow_dst = i};
			// Don't forget that a queen can move and shoot at her source.

			apply_move(state, move_and_block, player_id);
			int score = minimax(state, 1, depth - 1, alpha, beta, 1);
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
			for (unsigned int l = all_moves.dsts_i[j];
				 l < all_moves.dsts_i[j + 1]; l++)
			// Iterates over the possible destinations from the new position of the queen.
			{
				unsigned int m =
					all_moves.dsts[l]; // Gets a position for the arrow.
				struct move_t move = {
					.queen_src = i,
					.queen_dst = j,
					.arrow_dst = m};
				apply_move(state, move, player_id);
				int score = minimax(state, 1, depth - 1, alpha, beta, 1);
				cancel_move(state, move, player_id);
				best_score = max(best_score, score);

				if (player_id) {
					beta = min(beta, best_score);
				} else {
					alpha = max(alpha, best_score);
				}

				// free_state(new_state);
				if (alpha >= beta) {
					break;
				}
			}
		}
	}

	free_false_possible_moves(all_moves);
	return best_score;
}

// Takes the last move and returns the next move.
struct move_t play(struct move_t previous_move) {
	update(
		previous_move, loc_player_id, loc_num_queens, loc_graph, loc_queens, 0);
	// previous state

	struct move_t best_move = {-1, -1, -1};

	struct game_state_t previous_state = {
		.graph = loc_graph,
		.num_queens = loc_num_queens,
		.player_id = loc_player_id,
		.queens = loc_queens};

	struct all_moves_t all_moves = false_get_possible_moves_a(
		previous_state.graph, previous_state.queens, previous_state.num_queens);
	int counts = 0;

	for (unsigned int queens_index = 0; queens_index < loc_num_queens;
		 queens_index++) // For all queens.
	{
		unsigned int i = loc_queens[loc_player_id][queens_index];
		for (unsigned int k = all_moves.dsts_i[i]; k < all_moves.dsts_i[i + 1];
			 k++) {
			unsigned int j = all_moves.dsts[k];
			counts += all_moves.dsts_i[j + 1] - all_moves.dsts_i[j] + 1;
		}
	}

	free_false_possible_moves(all_moves);
	printf("count of moves : %d\n", counts);

	int best_score = INT_MIN;
	all_moves = false_get_possible_moves_a(
		previous_state.graph, previous_state.queens, previous_state.num_queens);

	for (unsigned int queens_index = 0;
		 queens_index < previous_state.num_queens;
		 queens_index++) // For all queens.
	{
		unsigned int i = previous_state.queens[loc_player_id][queens_index];
		for (unsigned int k = all_moves.dsts_i[i]; k < all_moves.dsts_i[i + 1];
			 k++)
		// Iterates over the possible destinations from i.
		{
			unsigned int j = all_moves.dsts[k]; // A possible destination.
			struct move_t move_and_block = {
				.queen_src = i,
				.queen_dst = j,
				.arrow_dst = i};
			// Don't forget that a queen can move and shoot at her source.
			// struct game_state_t *new_state = copy_state_a(&previous_state);
			apply_move(&previous_state, move_and_block, loc_player_id);
			int score = 0;
			if (counts > 100) {
				score = minimax(
					&previous_state, loc_player_id, MAX_DEPTH, INT_MIN, INT_MAX,
					5);
			} else {
				score = minimax(
					&previous_state, loc_player_id, MAX_DEPTH + 1, INT_MIN,
					INT_MAX, 2);
			}
			cancel_move(&previous_state, move_and_block, loc_player_id);
			if (score > best_score) {
				best_score = score;
				best_move = move_and_block;
			}
			for (unsigned int l = all_moves.dsts_i[j];
				 l < all_moves.dsts_i[j + 1];
				 l++) // Iterates over the possible destinations from the new
			// position of the queen.
			{
				unsigned int m =
					all_moves.dsts[l]; // Gets a position for the arrow.
				struct move_t move = {
					.queen_src = i,
					.queen_dst = j,
					.arrow_dst = m};

				apply_move(&previous_state, move, loc_player_id);
				if (counts > 200) {
					score = minimax(
					&previous_state, loc_player_id, MAX_DEPTH, INT_MIN, INT_MAX,
					1);
				} else if(counts >90){
					score = minimax(
					&previous_state, loc_player_id, MAX_DEPTH+1, INT_MIN, INT_MAX,
					1);
				}else{
                    score = minimax(
					&previous_state, loc_player_id, MAX_DEPTH+2, INT_MIN, INT_MAX,
					1);
                }
				
				cancel_move(&previous_state, move, loc_player_id);
				if (score > best_score) {
					best_score = score;
					best_move = move;
				}
			}
		}
	}

	update(
		best_move, 1 - loc_player_id, loc_num_queens, loc_graph, loc_queens, 0);
	free_false_possible_moves(all_moves);

	return best_move;
}
