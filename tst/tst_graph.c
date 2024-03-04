#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "dir.h"
#include "server.h"
#include "tst.h"
#include "move.h"

#include "server.h"
#include "graph.h"

#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_spmatrix_uint.h>
#include <gsl/gsl_spblas.h>

struct move_t move = {.queen_src=-1, .queen_dst=-1, .arrow_dst=-1};

void test__oak_trapdoor_a(void)
{
  printf("%s", __func__);
    
  unsigned int **queens = malloc(sizeof(unsigned int *) * 2);
  unsigned int *queen_0 = malloc(sizeof(unsigned int) * 1);
  unsigned int *queen_1 = malloc(sizeof(unsigned int) * 1);

  queen_0[0] = 0;

  queen_1[0] = 3;

  queens[0] = queen_0;
  queens[1] = queen_1;
    
    
  struct graph_t * graph = make_oak_trapdoor_graph_a(15, 0);
  struct graph_t * graph2 = make_oak_trapdoor_graph_a(15, 1);
  printf("\n Matrice graphe taille 15 forme oak trapdoor sans diagonale : \n \n");
  display_board(graph, 15, queens, 1, move);
  printf("\n Matrice graphe taille 15 forme oak trapdoor avec diagonale : \n \n");
  display_board(graph2, 15, queens, 1, move);
    
  free_graph(graph); 
  free_graph(graph2);
  free(queens[0]);
  free(queens[1]);
  free(queens);

  printf("\tOK\n");
}


void test_make_square_graph_grid_disp(void)
{
  printf("%s", __func__);
  struct move_t move = {.queen_src=-1, .queen_dst=-1, .arrow_dst=-1};
  unsigned int width = 9;
  unsigned int num_queens = width;
  unsigned int **queens = malloc(sizeof(unsigned int *) * 2);
  unsigned int *queen_0 = malloc(sizeof(unsigned int) * num_queens);
  unsigned int *queen_1 = malloc(sizeof(unsigned int) * num_queens);

  for (unsigned int i = 0; i < num_queens; i++) {
    queen_0[i] = width * i;
    queen_1[i] = width * i + width - 1;
  }

  queens[0] = queen_0;
  queens[1] = queen_1;

  struct graph_t *graph = make_square_graph_a(9, 1);
  printf("\n Matrice graphe taille 9 forme carré sans diagonale : \n \n");
  display_board(graph, 9, queens, num_queens, move);


  free(queens[0]);
  free(queens[1]);
  free(queens);
  free_graph(graph);
  printf("\tOK\n");
}

void test_make_square_graph_disp(void)
{
  printf("%s", __func__);
  unsigned int width = 9;
  unsigned int num_queens = width;
  unsigned int **queens = malloc(sizeof(unsigned int *) * 2);
  unsigned int *queen_0 = malloc(sizeof(unsigned int) * num_queens);
  unsigned int *queen_1 = malloc(sizeof(unsigned int) * num_queens);

  for (unsigned int i = 0; i < num_queens; i++) {
    queen_0[i] = width * i;
    queen_1[i] = width * i + width - 1;
  }

  queens[0] = queen_0;
  queens[1] = queen_1;

  struct graph_t *graph = make_square_graph_a(9, 0);
  printf("\n Matrice graphe taille 9 forme carré avec diagonale : \n \n");
  display_board(graph, 9, queens, num_queens, move);


  free(queens[0]);
  free(queens[1]);
  free(queens);
  free_graph(graph);
  printf("\tOK\n");
}

void test_make_donut_graph_grid_disp(void)
{
  printf("%s", __func__);
  unsigned int width = 9;
  unsigned int num_queens = width;
  unsigned int **queens = malloc(sizeof(unsigned int *) * 2);
  unsigned int *queen_0 = malloc(sizeof(unsigned int) * num_queens);
  unsigned int *queen_1 = malloc(sizeof(unsigned int) * num_queens);

  for (unsigned int i = 0; i < num_queens; i++) {
    queen_0[i] = width * i;
    queen_1[i] = width * i + width - 1;
  }

  queens[0] = queen_0;
  queens[1] = queen_1;

  struct graph_t *graph = make_donut_graph_a(9, 1);
  printf("\n Matrice graphe taille 9 forme donuts sans diagonales  : \n \n");
  display_board(graph, 9, queens, num_queens, move);


  free(queens[0]);
  free(queens[1]);
  free(queens);
  free_graph(graph);
  printf("\tOK\n");
}

void test_make_donut_graph_disp(void)
{
  printf("%s", __func__);
  unsigned int width = 9;
  unsigned int num_queens = width;
  unsigned int **queens = malloc(sizeof(unsigned int *) * 2);
  unsigned int *queen_0 = malloc(sizeof(unsigned int) * num_queens);
  unsigned int *queen_1 = malloc(sizeof(unsigned int) * num_queens);

  for (unsigned int i = 0; i < num_queens; i++) {
    queen_0[i] = width * i;
    queen_1[i] = width * i + width - 1;
  }

  queens[0] = queen_0;
  queens[1] = queen_1;

  struct graph_t *graph = make_donut_graph_a(9, 0);
  printf("\n Matrice graphe taille 9 forme donuts avec diagonales  : \n \n");
  display_board(graph, 9, queens, num_queens, move);


  free(queens[0]);
  free(queens[1]);
  free(queens);
  free_graph(graph);
  printf("\tOK\n");
}


void test_make_8_graph_grid_disp(void)
{
  printf("%s", __func__);
  unsigned int width = 12;
  unsigned int num_queens = width;
  unsigned int **queens = malloc(sizeof(unsigned int *) * 2);
  unsigned int *queen_0 = malloc(sizeof(unsigned int) * num_queens);
  unsigned int *queen_1 = malloc(sizeof(unsigned int) * num_queens);

  for (unsigned int i = 0; i < num_queens; i++) {
    queen_0[i] = width * i;
    queen_1[i] = width * i + width - 1;
  }

  queens[0] = queen_0;
  queens[1] = queen_1;

  struct graph_t *graph = make_8_graph_a(12, 1);
  printf("\n Matrice graphe taille 12 forme 8  : \n \n");
  display_board(graph, 12, queens, num_queens, move);


  free(queens[0]);
  free(queens[1]);
  free(queens);
  free_graph(graph);
  printf("\tOK\n");
}

void test_make_8_graph_disp(void)
{
  printf("%s", __func__);
  unsigned int width = 12;
  unsigned int num_queens = width;
  unsigned int **queens = malloc(sizeof(unsigned int *) * 2);
  unsigned int *queen_0 = malloc(sizeof(unsigned int) * num_queens);
  unsigned int *queen_1 = malloc(sizeof(unsigned int) * num_queens);

  for (unsigned int i = 0; i < num_queens; i++) {
    queen_0[i] = width * i;
    queen_1[i] = width * i + width - 1;
  }

  queens[0] = queen_0;
  queens[1] = queen_1;

  struct graph_t *graph = make_8_graph_a(12, 0);
  printf("\n Matrice graphe taille 12 forme 8  : \n \n");
  display_board(graph, 12, queens, num_queens, move);


  free(queens[0]);
  free(queens[1]);
  free(queens);
  free_graph(graph);
  printf("\tOK\n");
}
