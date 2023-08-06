#ifndef _COMMON_PLAYER_
#define _COMMON_PLAYER_

#include "dir.h"
#include "graph.h"
#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <math.h>
//#include "game.h"

extern unsigned int loc_player_id; //the id of the player, between 0 and NUM_PLAYERS-1
extern struct graph_t *loc_graph;
extern unsigned int loc_num_queens; //the number of queens per player
extern unsigned int *loc_queens[NUM_PLAYERS]; //an array containing, for each player, an array of length `num_queens`, giving the ids of the positions of the queens
extern struct graph_t *origin_graph;
extern unsigned int *arrows;
extern struct all_moves_t all_moves;
extern unsigned int *all_moves_pos;
extern unsigned int *indexes;
extern struct all_moves_t all_moves2;
extern unsigned int *all_moves_pos2;
extern unsigned int *indexes2;
extern struct game_state_t state;

#define SENTINEL UINT_MAX

struct all_moves_t {
  unsigned int *dsts_i; //Nombre de positions (dsts_i[i+1]-dsts_i[i])
  unsigned int *dsts; //Les positions de la reine i entre dsts[dsts_i[i]] et dsts[dsts_i[i+1]]
};

// Structure to represent the game state
struct game_state_t {
  struct graph_t *graph;	 // the game board
  unsigned int num_queens; // number of queens per player
  unsigned int player_id;	 // current player ID
  unsigned int **queens;	 // queens for all players
};

unsigned int next(unsigned int player_id);

unsigned int previous(unsigned int player_id);

int max(int a, int b);

int min(int a, int b);

unsigned int get_num_queens(unsigned int);

//Adds an arrow at position i on the graph of size n*n.
struct graph_t *add_arrow_client(struct graph_t *graph, unsigned int i);

int is_in_client(unsigned int x, unsigned int t[], unsigned int length);

int is_in_queens(unsigned int x, unsigned int *queens[NUM_PLAYERS], unsigned int length);

void free_spots_dir_a(unsigned int position, enum dir_t dir, struct graph_t *graph, unsigned int *queens[],  unsigned int num_queens, unsigned int *free_positions);

void free_spots_a(unsigned int position, struct graph_t *graph, unsigned int *queens[], unsigned int num_queens, unsigned int *free_positions);

void false_get_possible_moves_a(struct graph_t *graph, unsigned int *queens[], unsigned int num_queens, struct all_moves_t moves);

unsigned int nb_dir_pos(struct graph_t *graph, unsigned int *queens[], unsigned int num_queens, unsigned int pos);

//Get the length of position array
unsigned int length(unsigned int pos[]);

void copy(unsigned int src[], unsigned int dst[], unsigned int length);

struct graph_t *copy_graph_client(struct graph_t * graph, size_t m);

void shuffle(unsigned int t[], unsigned int length);

void update(struct move_t previous_move, unsigned int player_id, unsigned int num_queens, struct graph_t *graph, unsigned int *queens[], unsigned int self);

void remove_arrow_client(struct graph_t * graph_origin, struct graph_t * graph_current, unsigned int arrow);

//void initialize(unsigned int player_id, struct graph_t *graph, unsigned int num_queens, unsigned int *queens[NUM_PLAYERS]);

void finalize();

void apply_move(struct game_state_t *state, struct move_t move, unsigned int player_id);

void cancel_move(struct game_state_t *state, struct move_t move, unsigned int player_id);

int minimax(struct game_state_t *state, unsigned int player_id, int depth, int alpha, int beta, int (*score)(struct game_state_t *state););

#endif
