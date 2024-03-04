#include "dir.h"
#include "graph.h"

// This function makes edges to have a square graph with m*m vertices and size m.
// PRECOND : m >= 5
//grid bool to make graph with diagonals (grid = 0) or without diagonals (grid != 0)
struct graph_t *make_square_graph_a(size_t m, int grid)
{
    struct graph_t *graph = malloc(sizeof(struct graph_t));
    gsl_spmatrix_uint *tmp = gsl_spmatrix_uint_alloc(m*m, m*m);
    graph->num_vertices = m*m;
    for (size_t i = 0; i < m*m; i++){
        if(i%m != m-1)
        {
            gsl_spmatrix_uint_set(tmp, i, i+1, DIR_EAST);
            gsl_spmatrix_uint_set(tmp, i+1, i, DIR_WEST);
            if(!grid && i < m*(m-1))
            {
                gsl_spmatrix_uint_set(tmp, i, i+m+1, DIR_SE);
                gsl_spmatrix_uint_set(tmp, i+m+1, i, DIR_NW);
            }
        }
        if(i < m*(m-1))
        {
            gsl_spmatrix_uint_set(tmp, i, i+m, DIR_SOUTH);
            gsl_spmatrix_uint_set(tmp, i+m, i, DIR_NORTH);
            if(!grid && i%m)
            {
                gsl_spmatrix_uint_set(tmp, i, i+m-1, DIR_SW);
                gsl_spmatrix_uint_set(tmp, i+m-1, i, DIR_NE);
            }
        }
    }
    
    graph->t = gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
    gsl_spmatrix_uint_free(tmp);
    return graph;
}

// This function makes edges to have a donut graph with m*m vertices and size m. 
// PRECOND : m >= 5; m%3 = 0
//grid bool to make graph with diagonals (grid = 0) or without diagonals (grid != 0)
struct graph_t *make_donut_graph_a(size_t m, int grid)
{
    struct graph_t *graph = malloc(sizeof(struct graph_t));
    gsl_spmatrix_uint *tmp = gsl_spmatrix_uint_alloc(m*m, m*m);
    graph->num_vertices = m*m; 
    if(m%3 != 0)
    {
        printf("Wrong size but I got you bro\n");
    }
    for ( size_t i = 0; i < m*m; i++)
    {
        if (m%3 == 0)
        {
            if (i/m < m/3 || i/m >= 2*m/3 || i%m < m/3 || i%m >= 2*m/3)
            {
               if(i%m != m-1 && (i%m != m/3-1 || i/m < m/3 || i/m >= 2*m/3))
                {
                    gsl_spmatrix_uint_set(tmp, i, i+1, DIR_EAST);
                    gsl_spmatrix_uint_set(tmp, i+1, i, DIR_WEST);
                    if(!grid && i < m*(m-1) && (i/m != m/3-1 || i%m < m/3-1 || i%m >= 2*m/3))
                    {
                        gsl_spmatrix_uint_set(tmp, i, i+m+1, DIR_SE);
                        gsl_spmatrix_uint_set(tmp, i+m+1, i, DIR_NW);
                    }
                }
                if(i < m*(m-1) && (i/m != m/3-1 || i%m < m/3 || i%m >= 2*m/3))
                {
                    gsl_spmatrix_uint_set(tmp, i, i+m, DIR_SOUTH);
                    gsl_spmatrix_uint_set(tmp, i+m, i, DIR_NORTH);
                    if(!grid && i%m != 0 && (i%m != 2*m/3 || i/m < m/3-1 || i/m >= 2*m/3))
                    {
                        gsl_spmatrix_uint_set(tmp, i, i+m-1, DIR_SW);
                        gsl_spmatrix_uint_set(tmp, i+m-1, i, DIR_NE);
                    }
                } 
            }
        }
        if (m%3 == 1)
        {
            if (i/m < m/3 || i/m > 2*m/3 || i%m < m/3 || i%m > 2*m/3)
            {
                if(i%m != m-1 && (i%m != m/3-1 || i/m < m/3 || i/m > 2*m/3))
                {
                    gsl_spmatrix_uint_set(tmp, i, i+1, DIR_EAST);
                    gsl_spmatrix_uint_set(tmp, i+1, i, DIR_WEST);
                }
                if(i < m*(m-1) && (i/m != m/3-1 || i%m < m/3 || i%m > 2*m/3))
                {
                    gsl_spmatrix_uint_set(tmp, i, i+m, DIR_SOUTH);
                    gsl_spmatrix_uint_set(tmp, i+m, i, DIR_NORTH);
                }
            }
        }
        if (m%3 == 2)
        {
            if(i/m < m/3 || i/m > 2*m/3 || i%m < m/3 || i%m > 2*m/3)
            {
                if(i%m != m-1 && (i%m != m/3-1 || i/m < m/3 || i/m > 2*m/3) )
                {
                    gsl_spmatrix_uint_set(tmp, i, i+1, DIR_EAST);
                    gsl_spmatrix_uint_set(tmp, i+1, i, DIR_WEST);
                }
                if(i < m*(m-1) && (i/m != m/3-1 || i%m < m/3 || i%m > 2*m/3))
                {
                    gsl_spmatrix_uint_set(tmp, i, i+m, DIR_SOUTH);
                    gsl_spmatrix_uint_set(tmp, i+m, i, DIR_NORTH);
                }
            }
        }
    }
    graph->t = gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
    gsl_spmatrix_uint_free(tmp);
    return graph;
}

// This function makes edges to have a 8 graph with m*m vertices and size m. 
// PRECOND : m >= 5; m%4 = 0
//grid bool to make graph with diagonals (grid = 0) or without diagonals (grid != 0)
struct graph_t * make_8_graph_a(size_t m, int grid)
{
    struct graph_t *graph = malloc(sizeof(struct graph_t));
    gsl_spmatrix_uint *tmp;
    tmp = gsl_spmatrix_uint_alloc(m*m, m*m);
    graph->num_vertices = m*m;
    if(m%4 != 0)
    {
        printf("Wrong size\n");
    }
    for ( size_t i = 0; i < m*m; i++)
    {
        if(i/m < m/4 || i/m >= 3*m/4 || i%m < m/4 || i%m >= 3*m/4 || (i/m < m/2 && i%m < m/2) || (i/m >= m/2 && i%m >= m/2))
        {
            if(i%m != m-1 && (i/m < m/4 || i/m >= m/2 || i%m != m/2-1) && (i/m < m/2 || i/m >= 3*m/4 || i%m != m/4-1))
            {
                gsl_spmatrix_uint_set(tmp, i, i+1, DIR_EAST);
                gsl_spmatrix_uint_set(tmp, i+1, i, DIR_WEST);
                if (!grid && i < m*(m-1) && (i/m != m/4-1 || i%m < m/2-1 || i%m >= 3*m/4) && (i/m != m/2-1 || i%m < m/4-1 || i%m >= m/2))
                {
                    gsl_spmatrix_uint_set(tmp, i, i+m+1, DIR_SE);
                    gsl_spmatrix_uint_set(tmp, i+m+1, i, DIR_NW);
                }
            }
            if(i < m*(m-1) && (i%m < m/2 || i%m >= 3*m/4 || i/m != m/4-1) && (i%m < m/4 || i%m >= m/2 || i/m != m/2 -1))
            {
                gsl_spmatrix_uint_set(tmp, i, i+m, DIR_SOUTH);
                gsl_spmatrix_uint_set(tmp, i+m, i, DIR_NORTH);
                if(!grid && i%m != 0 && (i%m != 3*m/4 || i/m < m/4-1 || i/m >= m/2) && (i%m != m/2 || i/m < m/2 || i/m >= 3*m/4))
                {
                    gsl_spmatrix_uint_set(tmp, i, i+m-1, DIR_SW);
                    gsl_spmatrix_uint_set(tmp, i+m-1, i, DIR_NE);
                }
            }
        }
    }
    gsl_spmatrix_uint_set(tmp, m*(m/2-1) + (m/2-1) , m*(m/2) + (m/2), DIR_SE);
    gsl_spmatrix_uint_set(tmp,  m*(m/2) + (m/2), m*(m/2-1) + (m/2-1), DIR_NW);
    graph->t = gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
    gsl_spmatrix_uint_free(tmp);
    return graph;
}

void free_graph(struct graph_t* graph)
{
    gsl_spmatrix_uint_free(graph->t);
    free(graph);
}

// This function makes edges to have a donut graph with m*m vertices and size m. 
// PRECOND : m >= 5 ; m = 0 mod 5
//grid bool to make graph with diagonals (grid = 1) or without diagonals (grid != 1)
struct graph_t * make_oak_trapdoor_graph_a(size_t m, int grid)
{
    struct graph_t *graph = malloc(sizeof(struct graph_t));
    gsl_spmatrix_uint *tmp;
    tmp = gsl_spmatrix_uint_alloc(m*m, m*m);
    graph->num_vertices = m*m; 

    for (size_t k = 0; k < m*m; k++)
    {
        size_t i = k/m;
        size_t j = k%m;
        if (!((((j >= m/5) && (j < 2*m/5)) && (((i >= m/5) && (i < 2*m/5)) || ((i >= 3*m/5) && (i < 4*m/5)))) || (((j >= 3*m/5) && (j < 4*m/5)) && (((i >= m/5) && (i < 2*m/5)) || ((i >= 3*m/5) && (i < 4*m/5))))))
        {
            if (!((((j == m/5-1) || (j == 3*m/5-1)) && (((i >= m/5) && i < 2*m/5) || ((i >= 3*m/5) && (i < 4*m/5))))))
                if (j != m-1)
                {
                    gsl_spmatrix_uint_set(tmp, k, k+1, DIR_EAST);
                    gsl_spmatrix_uint_set(tmp, k+1, k, DIR_WEST);
                    if (!grid && (i != m-1) && !((i == m/5-1) && (((j > m/5-2) && (j < 2*m/5)) || ((j > 3*m/5-2) && (j < 4*m/5)))) && !((i == 3*m/5-1) && (((j > m/5-2) && (j < 2*m/5)) || ((j > 3*m/5-2) && (j < 4*m/5)))))
                    {
                        gsl_spmatrix_uint_set(tmp, k, k+m+1, DIR_SE);
                        gsl_spmatrix_uint_set(tmp, k+m+1, k, DIR_NW);
                    }
                }
            
            if (!((i == m/5-1) && (((j > m/5-1) && (j < 2*m/5)) || ((j > 3*m/5-1) && (j < 4*m/5)))) && !((i == 3*m/5-1)&& (((j > m/5-1) && (j < 2*m/5)) || ((j > 3*m/5-1) && (j < 4*m/5)))))
                if (!(i == m-1))
            {
                gsl_spmatrix_uint_set(tmp, k, k+m, DIR_SOUTH);
                gsl_spmatrix_uint_set(tmp, k+m, k, DIR_NORTH);
                if (!grid && (j != 0) && !((j == 2*m/5) && (((i > m/5-2) && (i < 2*m/5)) || ((i > 3*m/5-2) && (i < 4*m/5)))) && !((j == 4*m/5) && (((i > m/5-2) && (i < 2*m/5)) || ((i > 3*m/5-2) && (i < 4*m/5)))))
                {
                    gsl_spmatrix_uint_set(tmp, k, k+m-1, DIR_SW);
                    gsl_spmatrix_uint_set(tmp, k+m-1, k, DIR_NE);
                }
            }
        }
    }

    graph->t = gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
    gsl_spmatrix_uint_free(tmp);
    return graph;
}
