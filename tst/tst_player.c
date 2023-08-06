#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include "common_player.h"
#include "player.h"
#include "dir.h"
#include "server.h"
#include "graph.h"

#define TEST_SENTINEL UINT_MAX

unsigned int *free_pos;
extern unsigned int *arrows;
void initialize(unsigned int player_id, struct graph_t *graph,
				unsigned int num_queens, unsigned int *queens[NUM_PLAYERS]);

int is_in_general(unsigned int t[], unsigned int t2[], int len) {
	for (int i = 0; i<len; i++) {
		if (!is_in(t[i], t2, len))
			return 0;
	}
	return 1;
}
void print_tab(unsigned int t[], unsigned int length)
{
	for(unsigned int i=0; i<length; i++)
		printf("%u ", t[i]);
	printf("\n");
}

void test_is_in() {
	printf("%s", __func__);
	unsigned int t[] = { 45, 213, 8, 56, SENTINEL };
	unsigned int t2[] = { 45, 213, 8, 56, SENTINEL };
	unsigned int notin[] = { 44, 872, 651, 2, 564, 846, SENTINEL };
	for (int i = 0; t2[i] != SENTINEL; i++)
		assert(is_in(t2[i], t, 4));
	for (int i = 0; notin[i] != SENTINEL; i++)
		assert(!is_in(notin[i], t, 4));
	printf("\tOK\n");
}

void test_is_in_queens() {
	printf("%s", __func__);
	unsigned int noirs[] = { 1, 23, 45, 78 };
	unsigned int blancs[] = { 2, 54, 87, 99 };
	unsigned int noirs2[] = { 1, 23, 45, 78 };
	unsigned int blancs2[] = { 2, 54, 87, 99 };
	unsigned int notin[] = { 3, 15, 55, 80 };
	unsigned int *queens[] = { noirs, blancs };
	unsigned int length = 4;
	for (int i = 0; i < 4; i++) {
		assert(is_in_queens(noirs2[i], queens, length));
		assert(is_in_queens(blancs2[i], queens, length));
	}
	for (int i = 0; i < 4; i++)
		assert(!is_in_queens(notin[i], queens, length));
	printf("\tOK\n");
}



void test_free_spots_dir_a() {
	printf("%s", __func__);
	int position;
	enum dir_t dir;

	(void)position;
	(void)dir;
	// create graph
	/*
	int *free_spots_1 = free_spots_dir_a(0, graph);
	int t[] = {};
	assert(is_in_general(t, t2, ));*/
	printf("\tOK\n");
}

/*

*********
**Q******
*********
***###Q**
***###***
***###***
*Q*****Q*
*****Q***
Q********

*/ 
void test_free_spots_a() {
	printf("%s", __func__);

    struct graph_t *square = make_donut_graph_a(9, 1);
	free_pos = malloc((square->num_vertices+1)*sizeof(unsigned int));
	unsigned int num_queens = get_num_queens(square->num_vertices);

    unsigned int queens_1[num_queens];
    unsigned int queens_2[num_queens];
	unsigned int fill1[] = {11, 33, 55};
	unsigned int fill2[] = {61, 68, 72};
	for (unsigned int i=0; i<3; i++)
	{
		queens_1[i] = fill1[i];
		queens_2[i] = fill2[i];
	}
    for (unsigned int i = 3; i<num_queens; i++)
      {
        queens_1[i] = UINT_MAX;
        queens_2[i] = UINT_MAX;
      }
    unsigned int *queens[] = {queens_1, queens_2};

    free_spots_a(11, square, queens, num_queens, free_pos);
    unsigned int actual_free_spots_1[] = {2, 9, 10, 12, 13, 14, 15, 16, 17, 20, 29, 38, 47, 56, 65, 74, TEST_SENTINEL};
	assert(is_in_general(free_pos, actual_free_spots_1, 16));
	assert(is_in_general(actual_free_spots_1, free_pos, 16));

    free_spots_a(33, square, queens, num_queens, free_pos);
    unsigned int actual_free_spots_2[] = {6, 15, 24, 34, 35, 42, 51, 60, 69, 78, TEST_SENTINEL};
	assert(is_in_general(free_pos, actual_free_spots_2, 10));
	assert(is_in_general(actual_free_spots_2, free_pos, 10));

    free_spots_a(55, square, queens, num_queens, free_pos);
    unsigned int actual_free_spots_3[] = {1, 10, 19, 28, 37, 46, 54, 56, 57, 58, 59, 60, 64, 73,TEST_SENTINEL};
	assert(is_in_general(free_pos, actual_free_spots_3, 14));
	assert(is_in_general(actual_free_spots_3, free_pos, 14));

    free_spots_a(61, square, queens, num_queens, free_pos);
    unsigned int actual_free_spots_4[] = {7, 16, 25, 34, 43, 52, 56, 57, 58, 59, 60, 62, 70, 79, TEST_SENTINEL};
	assert(is_in_general(free_pos, actual_free_spots_4, 14));
	assert(is_in_general(actual_free_spots_4, free_pos, 14));

    free_spots_a(68, square, queens, num_queens, free_pos);
    unsigned int actual_free_spots_5[] = {59, 63, 64, 65, 66, 67, 69, 70, 71, 77, TEST_SENTINEL};
	assert(is_in_general(free_pos, actual_free_spots_5, 10));
	assert(is_in_general(actual_free_spots_5, free_pos, 10));

    free_spots_a(72, square, queens, num_queens, free_pos);
    unsigned int actual_free_spots_6[] = {0, 9, 18, 27, 36, 45, 54, 63, 73, 74, 75, 76, 77, 78, 79, 80, TEST_SENTINEL};
	assert(is_in_general(free_pos, actual_free_spots_6, 16));
	assert(is_in_general(actual_free_spots_6, free_pos, 16));

	free(free_pos);
    free_graph(square);
    printf("\tOK\n");
}

void test_length() {
	printf("%s", __func__);
	unsigned int t[] = {1, 3, 7, 23, 432, SENTINEL};
	unsigned int t2[] = {SENTINEL};
	unsigned int t3[] = {781923782, SENTINEL};

    unsigned int *list_t[] = {t, t2, t3};
    unsigned int lengths[] = {5, 0, 1};
    for (int i=0; i<3; i++)
        assert(length(list_t[i]) == lengths[i]);
    printf("\tOK\n");
}

void test_update() {
	printf("%s", __func__);
	// create graph
	;
	printf("\tOK\n");
}



void test_add_arrow_client()
{
	printf("%s\n", __func__);

    struct move_t move = {.queen_src=-1, .queen_dst=-1, .arrow_dst=-1}; //À enlever si display_board est enlevé.
	struct graph_t * graph = make_square_graph_a(5, 1);
	arrows = malloc(25*sizeof(unsigned int));
    unsigned int **queens =
	    malloc(sizeof(unsigned int *) * 2);
	unsigned int *queen_0 =
	    malloc(sizeof(unsigned int) * 1);
	unsigned int *queen_1 =
	    malloc(sizeof(unsigned int) * 1);

	queen_0[0] = 0;

	queen_1[0] = 3;

	queens[0] = queen_0;
    	queens[1] = queen_1;
	display_board(graph, 5, queens, 1, move);
	graph = add_arrow_client(graph, 7);

	display_board(graph, 5, queens, 1, move);

	free(queen_0);
	free(queen_1);
	free(queens);
	free_graph(graph);
	free(arrows);
}
