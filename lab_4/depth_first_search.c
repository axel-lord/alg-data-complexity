#include "sized_uint_array.h"
#include "graph.h"
#include "depth_first_search.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum vColor_s {white, gray, black} vColor;

// really unsafe stack as remembering arr size is up to user
typedef struct vertexStack_s
{
	unsigned int head;
	unsigned int * array;
} vertexStack;

static int vertexPush(vertexStack * vStack, unsigned int vertex)
{
	vStack->array[vStack->head] = vertex;
	vStack->head += 1;
	return 1;
}

static int vertexPop(vertexStack * vStack, unsigned int * vertex)
{
	if (vStack->head == 0)
		return 0;
	vStack->head -= 1;
	*vertex = vStack->array[vStack->head];
	return 1;
}

static int dfsVisit(
	Graph * graph, 
	unsigned int vertex, 
	vColor * vertexColors, 
	DFSOutput * outputBuffer, 
	unsigned int * time,
	sized_uint_array * (*neighborFunction)(Graph *, unsigned int),
	vertexStack * vStack
) {
	*time += 1;
	outputBuffer[vertex].discover = *time;
	vertexColors[vertex] = gray;

	sized_uint_array * adjecent = (*neighborFunction)(graph, vertex);
	if (adjecent == NULL)
		return 0;

	for (unsigned int i = 0; i < adjecent->size; ++i)
	{
		unsigned int v = adjecent->array[i];
		if (vertexColors[v] == white)
		{
			outputBuffer[v].root = v;
			if (!dfsVisit(graph, v, vertexColors, outputBuffer, time, neighborFunction, vStack))
				return 0;
		}
	}

	vertexColors[vertex] = black;
	*time += 1;
	outputBuffer[vertex].finished = *time;
	if (vStack != NULL)
		vertexPush(vStack, vertex);

	return 1;
}

static int depthFirstSearchFlexible(
	Graph * graph, DFSOutput * outputBuffer, 
	unsigned int vertexCount,
	vertexStack * vStack, 
	sized_uint_array * (*neighborFunction)(Graph *, unsigned int)
) {
	vColor vertexColors [vertexCount];

	for (unsigned int v = 0; v < vertexCount; ++v)
	{
		vertexColors[v] = white;
		outputBuffer[v].root = v;
	}

	unsigned int time = 0;
	for (unsigned int v = 0; v < vertexCount; ++v)
		if (vertexColors[v] == white)
			if (!dfsVisit(graph, v, vertexColors, outputBuffer, &time, neighborFunction, vStack))
				return 0;
	return 1;
}

int depthFirstSearch(Graph * graph, DFSOutput * output) 
{
	unsigned int numVertices = getNumVertices(graph);
	DFSOutput outputBuffer [numVertices];
	
	if (!depthFirstSearchFlexible(graph, outputBuffer, numVertices, NULL, &getOutNeighbors))
		return 0;

	memcpy(output, outputBuffer, numVertices * sizeof(DFSOutput)); // NOLINT
	return 1;
}

static void freeSizedJaggedArrayContent(sized_uint_array ** arr, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i)
		FREE_SIZED_ARRAY(arr[i]);
}

int findStronglyConnectedComponents(Graph * graph, SccOutput * output)
{
	unsigned int numVertices = getNumVertices(graph);
	DFSOutput outputBuffer [numVertices];
	unsigned int stackArray [numVertices];

	vertexStack orderStack = {.head=0, .array=stackArray};
	output->size = 0;
	output->componentArray = NULL;

	if (!depthFirstSearchFlexible(graph, outputBuffer, numVertices, &orderStack, &getOutNeighbors))
		return 0;
	
	// dfs in order
	vColor vertexColors [numVertices];
	for (unsigned int v = 0; v < numVertices; ++v)
	{
		vertexColors[v] = white;
		outputBuffer[v].root = v;
	}

	unsigned int time = 0;

	unsigned int stackArray2 [numVertices];
	vertexStack vStack = {.head=0, .array=stackArray2};

	sized_uint_array * sccArray [numVertices];
	unsigned int sccArraySize = 0;

	unsigned int v;
	while (vertexPop(&orderStack, &v))
		if (vertexColors[v] == white)
		{
			// getInNeighbors same as transpose
			if (!dfsVisit(graph, v, vertexColors, outputBuffer, &time, &getInNeighbors, &vStack))
			{
				freeSizedJaggedArrayContent(sccArray, sccArraySize);
				return 0;
			}
			
			// pop contents of stack should be one component
			unsigned int scc [numVertices];
			unsigned int sccSize = 0;
			unsigned int u;
			while (vertexPop(&vStack, &u))
			{
				printf("%u ", u);
				scc[sccSize] = u;
				++sccSize;
			}
			printf("\n");
			
			// prepare to return output
			if (sccSize > 0)
			{
				sized_uint_array * sArr = malloc(sizeof(sized_uint_array));
				if (sArr == NULL)
				{
					freeSizedJaggedArrayContent(sccArray, sccArraySize);
					return 0;
				}


				sArr->array = calloc(sccSize, sizeof(unsigned int));
				if (sArr->array == NULL)
				{
					freeSizedJaggedArrayContent(sccArray, sccArraySize+1);
					return 0;
				}

				memcpy(sArr->array, scc, sizeof(unsigned int) * sccSize); // NOLINT
				sArr->size = sccSize;

				sccArray[sccArraySize] = sArr;
				sccArraySize += 1;
			}
		}
	
	if (sccArraySize > 0)
	{
		output->componentArray = calloc(sccArraySize, sizeof(sized_uint_array*));
		if (output->componentArray == NULL)
		{
			freeSizedJaggedArrayContent(sccArray, sccArraySize);
			return 0;
		}
		memcpy(output->componentArray, sccArray, sizeof(sized_uint_array*) * sccArraySize); // NOLINT
	}

	return 1;
}

void resetSccOutput(SccOutput * sccOut)
{
	if (sccOut == NULL) return;

	for (unsigned int i = 0; i < sccOut->size; ++i)
		FREE_SIZED_ARRAY(sccOut->componentArray[i]);
	free(sccOut->componentArray);
	sccOut->componentArray = NULL;
	sccOut->size = 0;
}
