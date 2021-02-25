#include "graph.h"
#include "depth_first_search.h"
#include <stdio.h>

#define ADD_EDGE(g, a, b) if (!addDirectedEdge(g, a, b)) {freeGraph(g); return 2;} 

int main()
{
	Graph * graph = createGraph(8);
	if (!graph) return 2;
	/* 0 -> 1, 3
	 * 1 -> 0
	 * 2 -> 4
	 * 3 -> 6
	 * 4 -> 3
	 * 4 -> 5
	 * 5 -> 2
	 * 6 -> 7
	 * 7 -> 4
	 */
	ADD_EDGE(graph, 7, 4);
	ADD_EDGE(graph, 6, 7);
	ADD_EDGE(graph, 5, 2);
	ADD_EDGE(graph, 4, 5);
	ADD_EDGE(graph, 4, 3);
	ADD_EDGE(graph, 3, 6);
	ADD_EDGE(graph, 2, 4);
	ADD_EDGE(graph, 1, 0);
	ADD_EDGE(graph, 0, 3);
	ADD_EDGE(graph, 0, 1);

	DFSOutput dfsOut [graph->vertexCount];
	if(!depthFirstSearch(graph, dfsOut))
	{
		freeGraph(graph);
		return 1;
	}

	printf("DFS result:\n");
	for (unsigned int i = 0; i < graph->vertexCount; ++i)
		printf("vertex %u: %u/%u" "\n", i, dfsOut[i].discover, dfsOut[i].finished);

	printf("\nStrongly Connected Components:\n");
	
	SccOutput output;
	if (!findStronglyConnectedComponents(graph, &output))
	{
		freeGraph(graph);
		return 1;
	}
	printf("\n");
	
	resetSccOutput(&output);
	freeGraph(graph);
	return 0;
}
