#include "bellman_ford.h"
#include "graph.h"
#include "sized_uint_array.h"
#include <stdlib.h>

static void initSingleSource(
	Graph * graph,
	unsigned int vertex,
	int * vertexDistance,
	unsigned int * prevVertex
) {
	unsigned int numVertices = getNumVertices(graph);
	for (unsigned int i = 0; i < numVertices; ++i)
		prevVertex[i] = numVertices; // vertex does not exist since vertices count from 0
	vertexDistance[vertex] = 0;
}

static void relax(
	Graph * graph, 
	Edge * edge,
	int * distance,
	unsigned int * prevVertex
) {
	unsigned int numVertices = getNumVertices(graph);
	unsigned int vertexTo = edge->to;
	unsigned int vertexFrom = edge->from;
	if (
		prevVertex[vertexTo] >= numVertices || // no distance set
		distance[vertexTo] > (distance[vertexFrom] + edge->weight)
	) {
		distance[vertexTo] = distance[vertexFrom] + edge->weight;
		prevVertex[vertexTo] = vertexFrom;
	}
}

int bellmanFord(
	Graph * graph, 
	unsigned int vertex, 
	int * vertexDistance, 
	unsigned int * prevVertex
) {
	initSingleSource(graph, vertex, vertexDistance, prevVertex);
	unsigned int numVertices = getNumVertices(graph);

	Edge * edgeList;
	if (!getAllEdges(graph, &edgeList))
		return 0;
	for (unsigned int i = 1; i < numVertices; ++i)
	{
		Edge * edge = edgeList;
		while(edge != NULL)
		{
			relax(graph, edge, vertexDistance, prevVertex);
			edge = edge->next;
		}
	}

	Edge * edge = edgeList;
	while (edge != NULL)
	{
		if (vertexDistance[edge->to] > (vertexDistance[edge->from] + edge->weight))
		{
			freeEdgeList(edgeList);
			return 0;
		}
		edge = edge->next;
	}
	freeEdgeList(edgeList);
	return 1;
}
