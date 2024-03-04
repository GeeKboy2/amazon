#include <stdio.h>
#include <assert.h>

#include "tst.h"



int main(void)
{
	//Test graph and diplay
	test__display_2D_array();
	test_make_square_graph_grid_disp();
	test_make_square_graph_disp();
	test_make_donut_graph_grid_disp();
	test_make_donut_graph_disp();
    test__oak_trapdoor_a();
	test_make_8_graph_grid_disp();
    test_make_8_graph_disp();

	//tst_player.c
	test_is_in();
	test_is_in_queens();
	test_free_spots_dir_a();
	test_free_spots_a();
	test_length();
	test_update();
	test_add_arrow_client();
    printf("\tOK\n");
    return 0;
}
