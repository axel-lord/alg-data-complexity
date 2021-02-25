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

int getNeighbors(Graph * graph, unsigned int vertex, Edge ** edgeListOutput)
{
	Edge * edgeList = NULL;
	Edge * iterEdge = graph->outEdges[vertex];
	while (iterEdge != NULL)
	{
		Edge * newEdge = malloc(sizeof(Edge));
		if (newEdge == NULL)
		{
			freeEdgeList(edgeList);
			return 0;
		}

		memcpy(newEdge, iterEdge, sizeof(Edge)); // NOLINT
		newEdge->next = edgeList;
		edgeList = newEdge;

		iterEdge = iterEdge->next;
	}

	iterEdge = graph->inEdges[vertex];
	while(iterEdge != NULL)
	{
		if (iterEdge->isDirected && !hasEdge(graph, vertex, iterEdge->from)) 
		{
			Edge * newEdge = malloc(sizeof(Edge));
			if (newEdge == NULL)
			{
				freeEdgeList(edgeList);
				return 0;
			}

			memcpy(newEdge, iterEdge, sizeof(Edge)); // NOLINT
			newEdge->next = edgeList;
			edgeList = newEdge;
		}
		iterEdge = iterEdge->next;
	}

	*edgeListOutput = edgeList;
	return 1;
}

Edge * getInNeighbors(Graph * graph, unsigned int vertex)
{
	return graph->inEdges[vertex];
}

Edge * getOutNeighbors(Graph * graph, unsigned int vertex)
{
	return graph->outEdges[vertex];
}

int addDirectedEdge(Graph * graph, unsigned int vertexFrom, unsigned int vertexTo)
{
	return addDirectedWeightedEdge(graph, vertexFrom, vertexTo, 0);
}

int addDirectedWeightedEdge(Graph * graph, unsigned int vertexFrom, unsigned int vertexTo, double weight)
{
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
	edgeFrom->isDirected = vertexFrom != vertexTo; // edge is undirected if it goes to self
	edgeFrom->weight = weight;

	memcpy(edgeTo, edgeFrom, sizeof(Edge)); // NOLINT
	edgeFrom->next = graph->outEdges[vertexFrom];
	edgeTo->next = graph->inEdges[vertexTo];

	graph->outEdges[vertexFrom] = edgeFrom;
	graph->inEdges[vertexTo] = edgeTo;
	graph->edgeCount += 1;

	return 1;
}

int addUndirectedEdge(Graph * graph, unsigned int vertexFrom, int unsigned vertexTo)
{
	return addUndirectedWeightedEdge(graph, vertexFrom, vertexTo, 0);
}

int addUndirectedWeightedEdge(Graph * graph, unsigned int vertexFrom, unsigned int vertexTo, double weight)
{
	if (!addDirectedWeightedEdge(graph, vertexTo, vertexFrom, weight))
		return 0;

	if (vertexFrom != vertexTo) // in case of edge from vertex to self
	{
		if (!addDirectedWeightedEdge(graph, vertexFrom, vertexTo, weight))
		{
			// cleanup
			Edge * rEdge1 = graph->outEdges[vertexTo];
			Edge * rEdge2 = graph->inEdges[vertexFrom];

			graph->outEdges[vertexTo] = rEdge1->next;
			graph->inEdges[vertexFrom] = rEdge2->next;

			free(rEdge1);
			free(rEdge2);

			addEdgeFailureMessage = EDGE_FAILURE_MALLOC;
			return 0;
		}
		// set newly added edge's directed status to false
		graph->outEdges[vertexFrom]->isDirected = 0;
		graph->inEdges[vertexTo]->isDirected = 0;
	}
	
	// set edge's isDirected to false
	graph->outEdges[vertexTo]->isDirected = 0;
	graph->inEdges[vertexFrom]->isDirected = 0;

	// decrement edgeCount since only "one" edge was added
	if (vertexFrom != vertexTo)
		--graph->edgeCount;

	return 1;
}

int hasEdge(Graph * graph, unsigned int vertexFrom, unsigned int vertexTo)
{
	if (
		vertexFrom >= getNumVertices(graph) || 
		vertexTo >= getNumVertices(graph)
	) return 0; // edge does not exist if and involved vertex does not

	Edge * edge = graph->outEdges[vertexFrom];
	while (edge != NULL)
	{
		if (edge->to == vertexTo) // from of all undirected edges is same as vertex
			return 1;
		edge = edge->next;
	}

	return 0;
}

int getWeight(Graph * graph, unsigned int vertexFrom, unsigned int vertexTo, double * weight)
{
	if (
		vertexFrom >= getNumVertices(graph) || 
		vertexTo >= getNumVertices(graph)
	) return 0; // edge does not exist if and involved vertex does not

	Edge * edge = graph->outEdges[vertexFrom];
	while (edge != NULL)
	{
		if (edge->to == vertexTo) // from of all undirected edges is same as vertex
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
		Edge * iterEdge = graph->outEdges[v];
		while (iterEdge != NULL)
		{
			Edge * newEdge = malloc(sizeof(Edge));
			if (newEdge == NULL)
			{
				freeEdgeList(outputList);
				return 0;
			}
			memcpy(newEdge, iterEdge, sizeof(Edge)); // NOLINT
			newEdge->next = outputList;
			outputList = newEdge;

			iterEdge = iterEdge->next;
		}
	}
	*list = outputList;
	return 1;
}

