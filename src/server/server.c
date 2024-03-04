#include "server.h"
#include <dlfcn.h>
#include <getopt.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define UNUSED(x) (void)(x)

typedef char const *(*get_player_name_fct)(void);
typedef void *(*initialize_t)(
	unsigned int player_id, struct graph_t *graph, unsigned int num_queens,
	unsigned int *queens[NUM_PLAYERS]);
typedef struct move_t (*get_move)(struct move_t previous_move);

typedef void (*finalize_func)(void);

int main(int argc, char *argv[]) {
	srand(time(NULL));
	int opt;				  // Pour le getopt.
	unsigned int width = 10;  // La largeur du plateau.
    struct graph_t *(*graph_creator)(size_t m, int grid) = make_square_graph_a;
	//char shape = 'c';		  // La forme de la grille.
	unsigned int display = 0; // Activation de l'affichage
	while ((opt = getopt(argc, argv, "m:t:d")) != -1) {
		switch (opt) {
		case 'm':
			width = (unsigned int)atoi(optarg);
			break;
		case 't':
			if (optarg[0] == 'c') // Si la grille carrée est choisie.
			{
              graph_creator = make_square_graph_a;
				break;
			}
			if (optarg[0] == 'd') // Si la grille donut est choisie.
			{
              graph_creator = make_donut_graph_a;
				break;
			}
			if (optarg[0] == 't') // Si la grille trèfle est choisie.
			{
              graph_creator = make_oak_trapdoor_graph_a;
				break;
			}
			if (optarg[0] == '8') // Si la grille en 8 est choisie.
			{
              graph_creator = make_8_graph_a;
				break;
			}
			break;
		case 'd':
			display = 1;
			break;
		case '?':
			printf("unknown option: %c\n", optopt);
			break;
		default:;
		}
	}

	void *handle_p1 = dlopen(argv[argc-2], RTLD_LAZY);
	if (handle_p1 == NULL) {
		fprintf(stderr, "%s\n", dlerror());
		return 0;
	}

	void *handle_p2 = dlopen(argv[argc-1], RTLD_LAZY);
	if (handle_p2 == NULL) {
		fprintf(stderr, "%s\n", dlerror());
		return 0;
	}

	dlerror();

	get_player_name_fct player_name_1 =
		(get_player_name_fct)dlsym(handle_p1, "get_player_name");

	get_player_name_fct player_name_2 =
		(get_player_name_fct)dlsym(handle_p2, "get_player_name");

	unsigned int player_id = 0;
	unsigned int num_queens = 4 * (width / 10 + 1);
	unsigned int *queens[2];
	unsigned int *queen_0 = malloc(sizeof(unsigned int) * num_queens);
	unsigned int *queen_1 = malloc(sizeof(unsigned int) * num_queens);

	for (unsigned int i = 0; i < num_queens; i++) {
		queen_0[i] = width * i;
		queen_1[i] = width * i + width - 1;
	}

	queens[0] = queen_0;
	queens[1] = queen_1;

	struct graph_t *graph = graph_creator(width, 0);

	/* WORLD IS SET UP */

	initialize_t initialize_p1 = (initialize_t)dlsym(handle_p1, "initialize");
	initialize_t initialize_p2 = (initialize_t)dlsym(handle_p2, "initialize");

	struct graph_t *graph1 = copy_graph(graph, graph->num_vertices);
	struct graph_t *graph2 = copy_graph(graph, graph->num_vertices);

	unsigned int *queens1[NUM_PLAYERS];
	unsigned int *queens2[NUM_PLAYERS];
	for (int player = 0; player < NUM_PLAYERS; player++) {
		queens1[player] = malloc(num_queens * sizeof(unsigned int));
		queens2[player] = malloc(num_queens * sizeof(unsigned int));
		for (unsigned int i = 0; i < num_queens; i++) {
			queens1[player][i] = queens[player][i];
			queens2[player][i] = queens[player][i];
		}
	}

	initialize_p1(player_id, graph1, num_queens, queens1);
	player_id = next_player(player_id);
	initialize_p2(player_id, graph2, num_queens, queens2);
	player_id = next_player(player_id);

	fprintf(stderr, "%s\n", player_name_1());
	fprintf(stderr, "%s\n", player_name_2());

	get_move play_p1 = (get_move)dlsym(handle_p1, "play");
	get_move play_p2 = (get_move)dlsym(handle_p2, "play");

	struct move_t last_move = {UINT_MAX, UINT_MAX, UINT_MAX};
	struct move_t previous_move = {0, UINT_MAX, UINT_MAX};
	int i = 0;
    int validity = 0;
	while (!is_end_game(previous_move, last_move)) {
		if (display)
			printf("== Turn : %3d ==\n", i + 1);

		previous_move = last_move;
		player_id = next_player(player_id);
		if (player_id == 0) {
			last_move = play_p1(last_move);
		} else {
			last_move = play_p2(last_move);
		}


		if (display) {
          validity = is_valid_move(&last_move, graph, queens);
		}

		update_server(last_move, graph, player_id, queens);
		if (display) {
          printf(
                 "%s's queen at position %d moves to %d and shoots an arrow to "
                 "%d.\n",
                 player_id ? player_name_2() : player_name_1(),
                 last_move.queen_src, last_move.queen_dst, last_move.arrow_dst);
          printf("Move is %s.\n", validity ? "valid": "invalid");
          display_board(graph, width, queens, num_queens, last_move);
		}
        if (!validity) {
          printf("=======================================================\n");
          printf(
                 "\n\n\n %s is a WINNER\n\n\n",
                 1 - player_id ? player_name_2() : player_name_1());
          printf("=======================================================\n");
          break;
        }
		i++;
	}

	finalize_func finalize_p1 = (finalize_func)dlsym(handle_p1, "finalize");
	finalize_func finalize_p2 = (finalize_func)dlsym(handle_p2, "finalize");
	finalize_p1();
	finalize_p2();
	dlclose(handle_p1);
	dlclose(handle_p2);

	free(queens[0]);
	free(queens[1]);
	free_graph(graph);


	return 0;
}
