#include "graph.h"
#include "single_source_shortest_path.h"
#include <stdio.h>

#define ADD_EDGE(g, a, b, w) if (!addDirectedWeightedEdge(g, a, b, w)) {\
	printf("Failed to add edge: (%u, %u)! %s\n", a, b, addEdgeFailureMessage);\
	freeGraph(g);\
	return 2;\
}

int testBellmanFord(Graph * graph)
{
	unsigned int numVertices = graph->vertexCount;

	double vertexDistance [numVertices];
	unsigned int prevVertex [numVertices];
	
	unsigned int source = 0;
	if (!bellmanFord(graph, source, vertexDistance, prevVertex))
	{
		return 0;
	}

	for (unsigned int i = 0; i < numVertices; ++i)
	{
		printf("Vertex: %u, Prev: ", i);

		if (prevVertex[i] >= numVertices || i == source)
			printf("None");
		else
			printf("%4u", prevVertex[i]);

		printf(", Distance: ");

		if (prevVertex[i] < numVertices)
			printf("%2.1lf", vertexDistance[i]);
		else
			printf("inf");

		printf("\n");
	}

	return 1;
}

int main()
{
	Graph * graph = createGraph(5);
	if (!graph) return 3;

	ADD_EDGE(graph, 0, 1, 5);
	ADD_EDGE(graph, 0, 2, 3);
	ADD_EDGE(graph, 0, 3, 4);
	ADD_EDGE(graph, 1, 4, -2);
	ADD_EDGE(graph, 2, 1, -1);
	ADD_EDGE(graph, 2, 4, 4);
	ADD_EDGE(graph, 3, 2, -2);
	ADD_EDGE(graph, 3, 4, 2);

	if (testBellmanFord(graph))
		printf("Test Success!\n\n");
	else
		printf("Test Failure!\n\n");

	freeGraph(graph);
	graph = createGraph(8);
	if (!graph) return 4;

	ADD_EDGE(graph, 7, 4, 1);
	ADD_EDGE(graph, 6, 7, 1.5);
	ADD_EDGE(graph, 5, 2, 2);
	ADD_EDGE(graph, 4, 5, 2.5);
	ADD_EDGE(graph, 4, 3, 3);
	ADD_EDGE(graph, 3, 6, 3.5);
	ADD_EDGE(graph, 2, 4, 4);
	ADD_EDGE(graph, 1, 0, 4.5);
	ADD_EDGE(graph, 0, 3, 5);
	ADD_EDGE(graph, 0, 1, 5.5);

	if (testBellmanFord(graph))
		printf("Test Success!\n");
	else
		printf("Test Failure!\n");
	
	freeGraph(graph);
	return 0;
}
