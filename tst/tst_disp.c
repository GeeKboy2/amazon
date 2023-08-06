#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "dir.h"
#include "server.h"
#include "move.h"

#include "tst.h"

void test__display_2D_array()
{
//Test affichage diagonales sur un plateau 2 * 2:
	
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
	

    struct graph_t *graph10 = malloc(sizeof(struct graph_t));
    gsl_spmatrix_uint  * tmp = gsl_spmatrix_uint_alloc(2*2, 2*2);
    graph10->num_vertices = 2*2;

    gsl_spmatrix_uint_set(tmp, 0, 1, DIR_EAST);
    gsl_spmatrix_uint_set(tmp, 2, 3, DIR_EAST);
    gsl_spmatrix_uint_set(tmp, 1, 0, DIR_WEST);
    gsl_spmatrix_uint_set(tmp, 3, 2, DIR_WEST);
    gsl_spmatrix_uint_set(tmp, 0, 2, DIR_SOUTH);
    gsl_spmatrix_uint_set(tmp, 1, 3, DIR_SOUTH);
    gsl_spmatrix_uint_set(tmp, 2, 0, DIR_NORTH);
    gsl_spmatrix_uint_set(tmp, 3, 1, DIR_NORTH);
	gsl_spmatrix_uint_set(tmp, 0, 3, DIR_SE);
	gsl_spmatrix_uint_set(tmp, 1, 2, DIR_SW);

	graph10->t = gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
    gsl_spmatrix_uint_free(tmp);

	printf("\n");

    struct move_t move = {.queen_src=-1, .queen_dst=-1, .arrow_dst=-1};
	display_board(graph10, 2, queens, 1, move);

    gsl_spmatrix_uint_free(graph10->t);
    free(graph10);
    free(queens[0]);
    free(queens[1]);
    free(queens);

}
