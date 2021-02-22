#ifndef LAB_4_BREADTH_FIRST_SEARCH_H
#define LAB_4_BREADTH_FIRST_SEARCH_H

#include "graph.h"

/* Calculates the shortest path to each vertex
 * and their lengths, then writes it to the given
 * arrays
 * Parameters:
 * graph, graph to search in
 * vertex, vertex to search from
 * pathLength, distance to vertex on each index,
 *   distance -1 means not reachable, 
 *   expected to have a size of getNumVertices(graph)
 * pathNext, next vertex on the pat to given source certex if the 
 *   value is the same as index the vertex is not reachable 
 *   or the vertex is the source, 
 *   expected to have a size of getNumVertices(graph)
 * Returns:
 * 0, in case of failure
 * 1, otherwise
 */
int breadthFirstSearch(Graph * graph, unsigned int vertex, int * pathLength, unsigned int * pathNext);

#endif // LAB_4_BREADTH_FIRST_SEARCH_H
