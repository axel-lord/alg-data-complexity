#ifndef LAB_4_DEPTH_FIRST_SEARCH_H
#define LAB_4_DEPTH_FIRST_SEARCH_H

#include "graph.h"
#include "sized_uint_array.h"

typedef struct DFSOutput_s
{
	unsigned int discover;
	unsigned int finished;
	unsigned int root;
} DFSOutput;

typedef struct SccOutput_s
{
	unsigned int size;
	sized_uint_array ** componentArray;
} SccOutput;

/* Run the dfs algorithm on a graph
 * Parameters:
 * graph, Graph to run dfs on
 * output, an array of the same 
 *   size as the number of vertices in graph
 * Returns:
 * 0, if the agorithm fails
 * 1, otherwise
 */
int depthFirstSearch(Graph * graph, DFSOutput * output);

/* print sccs in graph */
int findStronglyConnectedComponents(Graph * graph, SccOutput * output);

/* free SccOutput content but not sccOut */
void resetSccOutput(SccOutput * sccOut);

#endif // LAB_4_DEPTH_FIRST_SEARCH_H
