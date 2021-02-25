#ifndef LAB_4_SINGLE_SOURCE_SHORTEST_PATH_H
#define LAB_4_SINGLE_SOURCE_SHORTEST_PATH_H
#include "graph.h"

/* find the shortest path to each vertext
 * going from given vertex
 * results are written to two given
 * arrays, arrays should have a size matching the
 * number of vertices contained in the given graph
 * Parameters:
 * graph, graph to search in
 * vertex, vertex to search from
 * vertexDistance, array to write distance to each vertex to
 * prevVertex, array to write next inline towards source vertex to
 * Returns:
 * 0, on failure
 * 1, othherwise
 */
int bellmanFord(Graph * graph, unsigned int vertex, double * vertexDistance, unsigned int * prevVertex);

#endif // LAB_4_SINGLE_SOURCE_SHORTEST_PATH_H
