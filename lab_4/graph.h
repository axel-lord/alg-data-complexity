#ifndef LAB_4_GRAPH_H
#define LAB_4_GRAPH_H

#include "sized_uint_array.h"

typedef struct edge_s
{
	/* Pointer to next edge in edge list
	 * NULL if last edge in list;
	 */
	struct edge_s * next;
	/* Index of vertex this edge points to
	 */
	unsigned int to;
	/* Index of vertex that points to this edge
	 */
	unsigned int from;
	/* Set to 1 if the graph is directed
	 * 0 otherwise 
	 */
	int isDirected;

	/* Weight of the edge */
	int weight;
} Edge;

typedef struct graph_s
{
	/* Array of pointers to first edge in edge list
	 * NULL if vertex has no connected edges */
	Edge ** outEdges;
	Edge ** inEdges;

	/* Amount of vertices contained in graph */
	unsigned int vertexCount;
	/* Amount of edges contained in graph */
	unsigned int edgeCount;
	/* Size of the array containing edge lists */
	unsigned int arraySize;
} Graph;

/* Create a new graph with numVertices vertices
 * Parameters:
 * numVertices, amount of vertices in graph
 * Returns:
 * NULL, if graph creation fails
 * A pointer to a new Graph otherwise
 */
Graph * createGraph(unsigned int numVertices);

/* Free a linked list of edges */
void freeEdgeList(Edge * edge);

/* Free a graph and all edges */
void freeGraph(Graph * graph);

/* Get number of vertices in a graph
 * Parameters:
 * graph, Graph to count vertices of
 * Returns:
 * Number of vertices in graph
 */
unsigned int getNumVertices(Graph * graph);

/* Get number of edges in a graph
 * Parameters:
 * graph, Graph to count edges of
 * Returns:
 * number of edges in graph, undirected edges counts as 1
 */
unsigned int getNumEdges(Graph * graph);

/* Get a list of all vertices connected to vertex in any way
 * Parameters:
 * graph, Graph containing vertices
 * vertex, index of vertex
 * Returns:
 * NULL, if no Neighbors exist 
 * an edge pointer/list otherwise (should be manually freed)
 */
Edge * getNeighbors(Graph * graph, unsigned int vertex);

/* Get a list of all vertices connected to vertex 
 * with an edge from them to vertex
 * Parameters:
 * graph, Graph containing vertices
 * vertex, index of vertex
 * Returns:
 * NULL, if no Neighbors exist 
 * an edge pointer/list otherwise (should not be manually freed)
 */
Edge * getInNeighbors(Graph * graph, unsigned int vertex);

/* Get a list of all vertices connected to vertex 
 * with an edge from vertex to them
 * Parameters:
 * graph, Graph containing vertices
 * vertex, index of vertex
 * Returns:
 * NULL, if no Neighbors exist 
 * an edge pointer/list otherwise (should not be manually freed)
 */
Edge * getOutNeighbors(Graph * graph, unsigned int vertex);

/* get a linked list of all edges
 * belonging to graph
 * Parameters:
 * graph, graph to get edges from
 * list, an edge double pointer to bind edge list to 
 *   set to null if graph has no edges
 * Returns:
 * 0, on failure
 * 1, otherwise
 */
int getAllEdges(Graph * graph, Edge ** list);

/* Add a directed edge to the graph
 * Parameters:
 * graph, Graph to add edge to
 * vertexFrom, vertex the edge leads from
 * vertexTo, vertex the edge leads to
 * Returns:
 * 0, if edge creation is a failure, addEdgeFailureMessage
 *   is pointed to a message explaining the failure
 * 1, otherwise
 */
int addDirectedEdge(Graph * graph, unsigned int vertexFrom, unsigned int vertexTo);

/* Add an undirected edge to the graph
 * Parameters:
 * graph, Graph to add edge to
 * vertex1, a vertex in the edge
 * vertex2, another vertex in the edge
 * Returns:
 * 0, if edge creation is a failure, addEdgeFailureMessage 
 *   is pointed to a message explaining the failure
 * 1, otherwise
 */
int addUndirectedEdge(Graph * graph, unsigned int vertex1, unsigned int vertex2);

/* Add a directed edge to the graph
 * with a weight
 * Parameters:
 * graph, Graph to add edge to
 * vertexFrom, vertex the edge leads from
 * vertexTo, vertex the edge leads to
 * weight, weight of the edge
 * Returns:
 * 0, if edge creation is a failure, addEdgeFailureMessage
 *   is pointed to a message explaining the failure
 * 1, otherwise
 */
int addDirectedWeightedEdge(Graph * graph, unsigned int vertexFrom, unsigned int vertexTo, int weight);

/* Add an undirected edge to the graph
 * with a weight
 * Parameters:
 * graph, Graph to add edge to
 * vertex1, a vertex in the edge
 * vertex2, another vertex in the edge
 * weight, weight of the edge
 * Returns:
 * 0, if edge creation is a failure, addEdgeFailureMessage 
 *   is pointed to a message explaining the failure
 * 1, otherwise
 */
int addUndirectedWeightedEdge(Graph * graph, unsigned int vertex1, unsigned int vertex2, int weight);

/* points to error message from edge adding functions */
extern const char * addEdgeFailureMessage;
extern const char * EDGE_FAILURE_MALLOC;
extern const char * EDGE_FAILURE_EDGE_EXIST;
extern const char * EDGE_FAILURE_VERTEX_NOT_EXIST;

/* see if an edge exists in graph
 * Parameters:
 * graph, Graph to check for edge in
 * vertexFrom, vertex from wich the edge leads
 * vertexTo, vertex the edge leads to
 * Returns:
 * 1, if edge exist
 * 0, otherwise
 */
int hasEdge(Graph * graph, unsigned int vertexFrom, unsigned int vertexTo);

/* see if an edge exists and if so
 * get weight of the edge it is then
 * written to weight
 * Parameters:
 * graph, Graph to seach in
 * vertexFrom, first vertex in edge
 * vertexTo, second vertex in edge
 * weight, pointer to write weight to
 * Returns:
 * 1, if edge exists
 * 0, otherwise
 */
int getWeight(Graph * graph, unsigned int vertexFrom, unsigned int vertexTo, int * weight);

#endif //LAB_4_GRAPH_H
