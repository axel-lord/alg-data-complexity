#include "breadth_first_search.h"
#include "graph.h"
#include "sized_uint_array.h"
#include <stdlib.h>
#include <string.h>

typedef enum color_e {white, gray, black} color;

typedef struct vertexQueue_s
{
	unsigned int head;
	unsigned int tail;
	unsigned int * arr;
} vertexQueue;

static int vertexEnqueue(vertexQueue * vQueue, unsigned int v)
{
	vQueue->arr[vQueue->tail] = v;
	vQueue->tail++;
	return 1;
}

static int vertexDequeue(vertexQueue * vQueue, unsigned int * v)
{
	if (vQueue->tail <= vQueue->head)
		return 0;

	*v = vQueue->arr[vQueue->head];
	vQueue->head++;
	return 1;
}

int breadthFirstSearch(Graph * graph, unsigned int vertex, int * pathLength, unsigned int * pathNext)
{
	unsigned int numVertices = getNumVertices(graph);
	color vertexColor [numVertices];
	
	// by not writing to given array until the last step the function could be seen as a bit safer
	int pathLengthBuffer [numVertices];
	unsigned int pathNextBuffer [numVertices];

	// init arrays
	for (unsigned int v = 0; v < numVertices; ++v)
	{
		pathLengthBuffer[v] = -1;
		pathNextBuffer[v] = v;
		vertexColor[v] = white;
	}

	pathLengthBuffer[vertex] = 0;
	vertexColor[vertex] = gray;
	
	// queue init
	unsigned int queueArr [numVertices]; //every vertex will be enqueued once so this size is enough
	vertexQueue queue = {.head = 0, .tail = 0, .arr = queueArr};
	vertexEnqueue(&queue, vertex);

	unsigned int vertexCurrent;
	while (vertexDequeue(&queue, &vertexCurrent))
	{
		sized_uint_array * adjecent = getOutNeighbors(graph, vertexCurrent);
		if (adjecent == NULL)
			return 0;
		
		for (unsigned int i = 0; i < adjecent->size; ++i)
		{
			unsigned int v = adjecent->array[i];
			if (vertexColor[v] == white)
			{
				vertexColor[v] = gray;
				pathLengthBuffer[v] = pathLengthBuffer[vertexCurrent] + 1;
				pathNextBuffer[v] = vertexCurrent;
				vertexEnqueue(&queue, v);
			}
		}

		free(adjecent->array);
		free(adjecent);

		vertexColor[vertexCurrent] = black;
	}
	
	// data is copied to given arrays
	memcpy(pathLength, pathLengthBuffer, sizeof(int) * numVertices); // NOLINT
	memcpy(pathNext, pathNextBuffer, sizeof(unsigned int) * numVertices); // NOLINT

	return 1;
}
