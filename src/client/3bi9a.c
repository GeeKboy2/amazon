#include "common_player.h"
#include "graph.h"
#include "move.h"
#include "player.h"

char const *get_player_name() {
  return "3bi9a";
}


struct move_t play(struct move_t previous_move) {
  struct move_t next_move = {UINT_MAX, UINT_MAX, UINT_MAX};

  update(
	  previous_move, loc_player_id, loc_num_queens, loc_graph, loc_queens, 0);

  shuffle(loc_queens[loc_player_id], loc_num_queens);
  for (unsigned int i = 0; i < loc_num_queens; i++) {
	  unsigned int moving_queen = loc_queens[loc_player_id][i];

	  free_spots_a(moving_queen, loc_graph, loc_queens, loc_num_queens, all_moves_pos);
	  unsigned int free_pos_length = length(all_moves_pos);
	  if (!free_pos_length) {
		  continue; // If there is no free spots, choose another queen.
	  }
	  unsigned int next_pos = all_moves_pos[rand() % free_pos_length];
	  loc_queens[loc_player_id][i] =
		  next_pos; // Updates the moving queen position.
	  free_spots_a(next_pos, loc_graph, loc_queens, loc_num_queens, all_moves_pos);
	  unsigned int next_arrow_pos =
		  all_moves_pos[rand() % length(all_moves_pos)];

	  next_move.queen_src = moving_queen;
	  next_move.queen_dst = next_pos;
	  next_move.arrow_dst = next_arrow_pos;
	  update(
		  next_move, next(loc_player_id), loc_num_queens, loc_graph,
		  loc_queens, 1);
	  break;
  }
  return next_move;
}

