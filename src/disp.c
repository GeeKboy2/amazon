#include <stdlib.h>
#include <stdio.h>

#include "dir.h"
#include "graph.h"
#include "server.h"


void purple(){printf("\033[0;35m");} //Couleur case départ.
void green(){printf("\033[0;32m");} //Couleur case arrivée.
void cyan(){printf("\033[0;36m");} //Couleur case arrivée flèche.
void reset(){printf("\033[0m");}

//Display a 2D Array (size n)
void display_2D_array(struct graph_t graph, size_t n)
{
  printf("[");
  for (size_t i = 0; i < (n*n); i++)
    {
      if (i == 0)
        printf("[");
      else
        printf(" [");
      for (size_t j = 0; j < (n*n); j++)
        {
          printf("%d", gsl_spmatrix_uint_get(graph.t, i, j));
          if (j < (n*n)-1)
            printf(" ");
          else
            printf("]");
        }
      if (i < (n*n) - 1)
        printf("\n");
      else
        printf("]");
    }
  printf("\n");
}


//Display the board game of size n*n
//Empty vertices are represented by 0, queens by black and white queen characters and unconnected vertices by blanks
void display_board(struct graph_t * graph, size_t n,unsigned int**queens, unsigned int nb_queens, struct move_t last)
{
  for (size_t i = 0; i < (n*n); i++)
    {
      if (i==last.queen_src)
        purple();
      if (i==last.queen_dst)
        green();
      if (i==last.arrow_dst)
        cyan();
      //if the vertices is connected, prints what is in it and his edges. Otherwise print a blank.
      if (graph->t->p[i] != graph->t->p[i+1])
        {
          //Print queens or empty vertices
          if(is_in(i,queens[0],nb_queens)){
            printf("♕");
          }else if(is_in(i,queens[1],nb_queens)){
            printf("♛");
          }else{
            printf("□");
          }
          reset();
          int a = -1;
          for (int j = graph->t->p[i]; j < graph->t->p[i+1]; j++)
            {
              if (graph->t->data[j] == DIR_EAST)
                {
                  a = 1;
                }
            }
          //Print East edges if existing
          if (a == 1)
            printf(" - ");
          else
            printf("   ");
        }
      //Print blank
      else
        printf("    ");
        
      //Print South directed edges if existing
      if ((i+1)%n == 0)
        {
            printf("\n");
            for (size_t  k = i-n+1; k < i+1; k++)
            {
                if (graph->t->p[k] != graph->t->p[k+1])
                {
                    int a = -1;
                    int b = -1;
                    int c = -1;
                    for (int j = graph->t->p[k]; j < graph->t->p[k+1]; j++)
                    {
                        if (graph->t->data[j] == DIR_SOUTH)
                        {
                            a = 1;
                        }
                        if (graph->t->data[j] == DIR_SE)
                        {
                            b = 1;
                        }
                    }
                    if (k != n*n-1)
                    {
                        for (int j = graph->t->p[k+1]; j < graph->t->p[k+2]; j++)
                        {
                            if (graph->t->data[j] == DIR_SW)
                            {
                                c = 1;
                            }
                        }
                    }
                    if (a == 1)
                        printf("| ");
                    else
                        printf("  ");
                    if ((b == 1) && (c == 1))
                        printf("X ");
                    else
                    {
                        if (b == 1)
                            printf("\\ ");
                        else
                            if (c == 1)
                                printf("/ ");
                            else
                                printf("  ");
                    }
                }
                else
                    printf("    ");            
            }
            printf("\n");
        }
    }
}
