#include "common_player.h"
#include "graph.h"
#include "move.h"
#include "player.h"
#include "server.h"

#define MAX_DEPTH 0 // Maximum depth of the search tree

char const *get_player_name() {
	return "The_Watcher";
}

// This functions computes a score that is equal to the difference of number of
// possible moves between the player and the ennemi
int score(struct game_state_t *state) {
	false_get_possible_moves_a(state->graph, state->queens, state->num_queens, all_moves2);

	int counts[NUM_PLAYERS] = {0};
	for (unsigned int player=0; player<NUM_PLAYERS; player++) {
		for (unsigned int queens_index = 0; queens_index < state->num_queens; queens_index++) // For all queens.
		{
			unsigned int i = state->queens[player][queens_index];
			for (unsigned int k = all_moves2.dsts_i[i]; k < all_moves2.dsts_i[i + 1]; k++)
			{
				unsigned int j = all_moves2.dsts[k];
				counts[player] += all_moves2.dsts_i[j + 1] - all_moves2.dsts_i[j] + 1;
			}
		}
	}
	return counts[state->player_id] - counts[1-state->player_id];
}


// Takes the last move and returns the next move.
struct move_t play(struct move_t previous_move) {
	update(previous_move, loc_player_id, loc_num_queens, loc_graph, loc_queens, 0);

	struct move_t best_move = {-1, -1, -1};
	int best_score = INT_MIN;
	false_get_possible_moves_a(
		state.graph, state.queens, state.num_queens, all_moves);

	for (unsigned int queens_index = 0; queens_index < state.num_queens;
		 queens_index++) // For all queens.
	{
		unsigned int i = state.queens[loc_player_id][queens_index];
		for (unsigned int k = all_moves.dsts_i[i]; k < all_moves.dsts_i[i + 1]; k++)
          // Iterates over the possible destinations from i.
		{
			unsigned int j = all_moves.dsts[k]; // A possible destination.
			struct move_t move_and_block = {.queen_src = i, .queen_dst = j, .arrow_dst = i}; 
            // Don't forget that a queen can move and shoot at her source.
			apply_move(&state, move_and_block, loc_player_id);
			int cur_score = minimax(&state, loc_player_id, MAX_DEPTH, INT_MIN, INT_MAX, score);
			cancel_move(&state, move_and_block, loc_player_id);
			if (cur_score > best_score) {
				best_score = cur_score;
				best_move = move_and_block;
			}
			for (unsigned int l=all_moves.dsts_i[j]; l<all_moves.dsts_i[j + 1]; l++) // Iterates over the possible destinations from the new
					  // position of the queen.
			{
				unsigned int m = all_moves.dsts[l]; // Gets a position for the arrow.
				struct move_t move = {.queen_src = i, .queen_dst = j, .arrow_dst = m};

				apply_move(&state, move, loc_player_id);
				int cur_score = minimax(&state, loc_player_id, MAX_DEPTH, INT_MIN, INT_MAX, score);
				cancel_move(&state, move, loc_player_id);
				if (cur_score > best_score) {
					best_score = cur_score;
					best_move = move;
				}
			}
		}
	}

    update(best_move, 1-loc_player_id, loc_num_queens, loc_graph, loc_queens, 0);
	return best_move;
}
