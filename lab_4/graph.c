#include "graph.h"
#include "sized_uint_array.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static unsigned int closestPowerOf2(unsigned int number)
{
	return (unsigned int) pow(2, ceil(log(number) / log(2)));
}

const char * addEdgeFailureMessage = NULL;
const char * EDGE_FAILURE_MALLOC = "Memory Allocation Failed!";
const char * EDGE_FAILURE_EDGE_EXIST = "Edge Already Exists!";
const char * EDGE_FAILURE_VERTEX_NOT_EXIST = "Vertex does not exist!";

Graph * createGraph(unsigned int numVertices)
{
	unsigned int arraySize = closestPowerOf2(numVertices);
	Edge ** array = calloc(arraySize, sizeof(Edge*));
	if (array == NULL)
		return NULL;

	Edge ** array2 = calloc(arraySize, sizeof(Edge*));
	if (array2 == NULL)
	{
		free(array);
		return NULL;
	}

	Graph * graph = malloc(sizeof(Graph));
	if (graph == NULL)
	{
		free(array);
		free(array2);
		return NULL;
	}

	for (unsigned int i = 0; i < numVertices; ++i)
	{
		array[i] = NULL;
		array2[i] = NULL;
	}

	graph->inEdges = array;
	graph->outEdges = array2;
	graph->arraySize = arraySize;
	graph->vertexCount = numVertices;
	graph->edgeCount = 0;
	return graph;
}

void freeEdgeList(Edge * edge)
{
	while (edge != NULL)
	{
		Edge * remEdge = edge;
		edge = edge->next;
		free(remEdge);
	}
}

void freeGraph(Graph * graph)
{
	unsigned int vertexCount = getNumVertices(graph);
	for (unsigned int i = 0; i < vertexCount; ++i)
	{
		freeEdgeList(graph->outEdges[i]);
		freeEdgeList(graph->inEdges[i]);
	}
	free(graph);
}

unsigned int getNumVertices(Graph * graph)
{
	return graph->vertexCount;
}

unsigned int getNumEdges(Graph * graph)
{
	return graph->edgeCount;
}

static sized_uint_array * getMatchNeighbors(
	Graph * graph, unsigned int vertex, 
	int (*matchFunc)(Edge * edge, unsigned int vertex, unsigned int index, int * array)
)
{
	if (vertex >= getNumVertices(graph))
		return NULL;
	sized_uint_array * output = malloc(sizeof(sized_uint_array));
	if (output == NULL)
		return NULL;

	int array [getNumVertices(graph)];

	unsigned int numVertices = getNumVertices(graph);
	unsigned int k = 0;
	Edge * edge = graph->edges[vertex];
	while (edge != NULL && k < numVertices)
	{
		k += (*matchFunc)(edge, vertex, k, array);
		edge = edge->next;
	}

	if (k > 0)
	{
		output->array = calloc(k, sizeof(unsigned int));
		if (output->array == NULL)
		{
			free(output);
			return NULL;
		}
		memcpy(output->array, array, sizeof(int) * k); // NOLINT
	}
	else
		output->array = NULL;
	output->size = k;
	
	return output;

}

static int anyNeighbor(Edge * edge, unsigned int vertex, unsigned int index, int * array)
{
	if (edge->from == vertex)
		array[index] = edge->to;
	else
		array[index] = edge->from;
	return 1;
}

sized_uint_array * getNeighbors(Graph * graph, unsigned int vertex)
{
	return getMatchNeighbors(graph, vertex, &anyNeighbor);
}

static int inNeighbor(Edge * edge, unsigned int vertex, unsigned int index, int * array)
{
	if(!edge->isDirected)
	{
		array[index] = edge->to;
		return 1;
	}
	if (edge->to == vertex)
	{
		array[index] = edge->from;
		return 1;
	}
	return 0;
}

sized_uint_array * getInNeighbors(Graph * graph, unsigned int vertex)
{
	return getMatchNeighbors(graph, vertex, &inNeighbor);
}

static int outNeighbor(Edge * edge, unsigned int vertex, unsigned int index, int * array)
{
	if (edge->from == vertex) // undirected edges always has a from matching vertex
	{
		array[index] = edge->to;
		return 1;
	}
	return 0;
}

sized_uint_array * getOutNeighbors(Graph * graph, unsigned int vertex)
{
	return getMatchNeighbors(graph, vertex, &outNeighbor);
}

int addDirectedEdge(Graph * graph, unsigned int vertexFrom, unsigned int vertexTo)
{
	return addDirectedWeightedEdge(graph, vertexFrom, vertexTo, 0);
}

int addDirectedWeightedEdge(Graph * graph, unsigned int vertexFrom, unsigned int vertexTo, int weight)
{
	if (vertexFrom == vertexTo) // in case of edge from vertex to self
		return addUndirectedWeightedEdge(graph, vertexFrom, vertexTo, weight);

	if (
		vertexFrom >= getNumVertices(graph) || // edge not possible
		vertexTo >= getNumVertices(graph) // edge not possible
	) {
		addEdgeFailureMessage = EDGE_FAILURE_VERTEX_NOT_EXIST;
		return 0;
	}

	if (hasEdge(graph, vertexFrom, vertexTo)) // edge exists
	{
		addEdgeFailureMessage = EDGE_FAILURE_EDGE_EXIST;
		return 0;
	}
	
	// create new edges
	Edge * edgeFrom = malloc(sizeof(Edge));
	if (edgeFrom == NULL)
	{
		addEdgeFailureMessage = EDGE_FAILURE_MALLOC;
		return 0;
	}

	Edge * edgeTo = malloc(sizeof(Edge));
	if (edgeTo == NULL)
	{
		addEdgeFailureMessage = EDGE_FAILURE_MALLOC;
		free(edgeFrom);
		return 0;
	}
	
	// set data of edges
	edgeFrom->from = vertexFrom;
	edgeFrom->to = vertexTo;
	edgeFrom->isDirected = 1;
	edgeFrom->weight = weight;

	memcpy(edgeTo, edgeFrom, sizeof(Edge)); // NOLINT
	edgeFrom->next = graph->edges[vertexFrom];
	edgeTo->next = graph->edges[vertexTo];

	graph->edges[vertexFrom] = edgeFrom;
	graph->edges[vertexTo] = edgeTo;
	graph->edgeCount += 1;

	return 1;
}

int addUndirectedEdge(Graph * graph, unsigned int vertex1, int unsigned vertex2)
{
	return addUndirectedWeightedEdge(graph, vertex1, vertex2, 0);
}

int addUndirectedWeightedEdge(Graph * graph, unsigned int vertex1, unsigned int vertex2, int weight)
{
	if (
		vertex1 >= getNumVertices(graph) || // edge not possible
		vertex2 >= getNumVertices(graph) // edge not possible
	) {
		addEdgeFailureMessage = EDGE_FAILURE_VERTEX_NOT_EXIST;
		return 0;
	}

	if (
		hasEdge(graph, vertex1, vertex2) || // edge exists in any direction
		hasEdge(graph, vertex2, vertex1) // edge exists in any direction
	) {
		addEdgeFailureMessage = EDGE_FAILURE_EDGE_EXIST;
		return 0;
	}
	
	// create new edges
	Edge * edgeFrom = malloc(sizeof(Edge));
	if (edgeFrom == NULL)
	{
		addEdgeFailureMessage = EDGE_FAILURE_MALLOC;
		return 0;
	}

	if (vertex1 != vertex2) // if the edge goes to self
	{
		Edge * edgeTo = malloc(sizeof(Edge));
		if (edgeTo == NULL)
		{
			addEdgeFailureMessage = EDGE_FAILURE_MALLOC;
			free(edgeFrom);
			return 0;
		}
		
		// set data of edge belonging to vertex2
		edgeTo->from = vertex2;
		edgeTo->to = vertex1;
		edgeTo->isDirected = 0;
		edgeTo->weight = weight;

		edgeTo->next = graph->edges[vertex2];
		graph->edges[vertex2] = edgeTo;
	}
	
	// set data of edge belonging to vertex1
	edgeFrom->from = vertex1;
	edgeFrom->to = vertex2;
	edgeFrom->isDirected = 0;
	edgeFrom->weight = weight;

	edgeFrom->next = graph->edges[vertex1];
	graph->edges[vertex1] = edgeFrom;
	graph->edgeCount += 1;
	return 1;
}

int hasEdge(Graph * graph, unsigned int vertexFrom, unsigned int vertexTo)
{
	if (
		vertexFrom >= getNumVertices(graph) || 
		vertexTo >= getNumVertices(graph)
	) return 0; // edge does not exist if and involved vertex does not

	Edge * edge = graph->edges[vertexFrom];
	while (edge != NULL)
	{
		if (edge->isDirected)
		{
			if (edge->from == vertexFrom && edge->to == vertexTo)
				return 1;
		}
		else if (edge->to == vertexTo) // from of all undirected edges is same as vertex
			return 1;
		edge = edge->next;
	}

	return 0;
}

int getWeight(Graph * graph, unsigned int vertexFrom, unsigned int vertexTo, int * weight)
{
	if (
		vertexFrom >= getNumVertices(graph) || 
		vertexTo >= getNumVertices(graph)
	) return 0; // edge does not exist if and involved vertex does not

	Edge * edge = graph->edges[vertexFrom];
	while (edge != NULL)
	{
		if (edge->isDirected)
		{
			if (edge->from == vertexFrom && edge->to == vertexTo)
			{
				*weight = edge->weight;
				return 1;
			}
		}
		else if (edge->to == vertexTo) // from of all undirected edges is same as vertex
		{
			*weight = edge->weight;
			return 1;
		}
		edge = edge->next;
	}

	return 0;
}

int getAllEdges(Graph * graph, Edge ** list)
{
	Edge * outputList = NULL;
	for (unsigned int v = 0; v < graph->vertexCount; ++v)
	{
		Edge * edge = graph->edges[v];
		while (edge != NULL)
		{
			if (
				(edge->isDirected && edge->from == v) ||
				(!edge->isDirected && edge->from <= edge->to)
			) {
				Edge * newEdge = malloc(sizeof(Edge));
				if (newEdge == NULL)
				{
					freeEdgeList(outputList);
					return 0;
				}
				memcpy(newEdge, edge, sizeof(Edge)); // NOLINT
				newEdge->next = outputList;
				outputList = newEdge;
			}
			edge = edge->next;
		}
	}
	*list = outputList;
	return 1;
}

