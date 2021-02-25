#include "sized_uint_array.h"
#include "graph.h"
#include "merge_sort_uint.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define COL_RESET "\033[0m"
#define COL_YELLOW "\033[1;93m"
#define COL_RED "\033[1;91m"
#define COL_GREEN "\033[92m"
#define COL_BOLD "\033[1m"


sized_uint_array * edgeListToSizedArray(Edge * edgeList, unsigned int vertex)
{
	sized_uint_array * output = malloc(sizeof(sized_uint_array));
	if (output == NULL)
		return NULL;

	Edge * iter = edgeList;
	unsigned int counter = 0;
	while (iter != NULL)
	{
		++counter;
		iter = iter->next;
	}

	if (counter == 0)
	{
		output->size = 0;
		output->array = NULL;
		return output;
	}

	unsigned int * arr = calloc(counter, sizeof(unsigned int));
	if (arr == NULL)
	{
		free(output);
		return NULL;
	}
	
	iter = edgeList; unsigned int i = 0;
	while (iter != NULL)
	{
		arr[i] = (iter->from == vertex) ? iter->to : iter->from;
		++i;
		iter = iter->next;
	}

	output->size = counter;
	output->array = arr;
	return output;
}

void freePointersInSizedArrayArray(sized_uint_array * array, int size)
{
	for (int i = 0; i < size; ++i)
	{
		free(array[i].array);
	}
}

void print_s_arr(sized_uint_array * array)
{
	for (unsigned int i = 0; i < array->size; ++i)
		printf("%d ", array->array[i]);
	printf("\n");
}

int testDirected(int vertexCount)
{
	static char * padding = "   ";

	Graph * testGraph = createGraph(vertexCount); // a number of vertices between 3 and 10
	int edgeArraySize = getNumVertices(testGraph);
	sized_uint_array edgeArrays [edgeArraySize];
	int edgeCount = 0;

	// init edge arrays
	for (int i = 0; i < edgeArraySize; ++i)
	{
		edgeArrays[i].array = NULL;
		edgeArrays[i].size = 0;
	}
	
	// create random edges
	printf(COL_YELLOW "Building graph!" COL_RESET "\n");
	for (int i = 0; i < edgeArraySize; ++i)
	{
		int array1 [edgeArraySize];
		int k = 0;
		
		// see wich vertices to make edge to
		for (int j = 0; j < edgeArraySize; ++j)
		{
			//if (j == i) continue;

			if (rand() % 2)
				continue; // go to beginning of loop, saves indentation

			if (!addDirectedEdge(testGraph, i, j))
			{
				printf(
					COL_RED 
					"Failure adding edge (%d, %d)!" 
					COL_RESET COL_BOLD
					" %s"
					COL_RESET
					" Continuing..." 
					"\n", 
					i, j, addEdgeFailureMessage
				);
				continue;
			}

			array1[k] = j;
			++edgeCount;
			++k;
		}
		
		// save created edges for later comparison
		if (k > 0)
		{
			edgeArrays[i].array = calloc(k, sizeof(unsigned int));
			if (edgeArrays[i].array == NULL)
			{
				freeGraph(testGraph);
				freePointersInSizedArrayArray(edgeArrays, edgeArraySize);
				return 3;
			}
			memcpy(edgeArrays[i].array, array1, k * sizeof(int)); // NOLINT
			edgeArrays[i].size = k;
		}

		if (i % 10 == 0)
		{
			printf("Vertex: %d\r", i);
			fflush(stdout);
		}
	}
	printf(COL_GREEN "Graph built!" COL_RESET "\n\n");

	int testFailure = 0;
	
	// test graph
	printf(COL_YELLOW "Testing if edges were added correctly!" COL_RESET "\n");
	for (int i = 0; i < edgeArraySize; ++i)
	{
		sized_uint_array * neighbors = edgeListToSizedArray(getOutNeighbors(testGraph, i), i);
		if (neighbors == NULL)
		{
			printf("Vertex: %d ", i);
			printf(COL_RED "Problem Getting neighbors!" COL_RESET "\n----\n");
			continue;
		}

		mergesort_array(neighbors);

		if (memcmp( // NOLINT
			neighbors->array, 
			(edgeArrays+i)->array, 
			neighbors->size * sizeof(int)
		)) 
		{
			printf("Vertex: %d ", i);
			printf(COL_RED "Problem" COL_RESET "\n");

			printf("Saved: ");
			print_s_arr(edgeArrays+i);

			printf("Graph: ");
			print_s_arr(neighbors);
			printf("----\n");
			testFailure = 1;
		}

		FREE_SIZED_ARRAY(neighbors);

		if (i % 100 == 0)
			printf("Vertex: %d\r", i);
	}

	if (testFailure)
		printf(COL_RED "Edge test failed somewhere!" COL_RESET "\n\n");
	else
		printf(COL_GREEN "Edge test success!" COL_RESET "\n\n");
	
	// test getNumVertices
	printf(COL_YELLOW "Testing if getNumVertices returns the right amount of edges!" COL_RESET "\n");
	int numVertices = getNumVertices(testGraph);
	if (vertexCount != numVertices)
		printf(COL_RED "Vertex count failed!" COL_RESET "\n");
	else
		printf(COL_GREEN "Vertex count success!" COL_RESET "\n");
	printf("%sExpected: %d, Result: %d" "\n\n", padding, vertexCount, numVertices);
	
	// test getNumEdges
	printf(COL_YELLOW "Testing if getNumEdges returns the right amount of edges!" COL_RESET "\n");
	int numEdges = getNumEdges(testGraph);
	if (edgeCount != numEdges)
		printf(COL_RED "Edge count failed!" COL_RESET "\n");
	else
		printf(COL_GREEN "Edge count success!" COL_RESET "\n");
	printf("%sExpected: %d, Result: %d" "\n\n", padding, edgeCount, numEdges);
	
	// test getInNeighbors, hasEdge
	printf(COL_YELLOW "Testing if getInNeighbors and hasEdge works!" COL_RESET "\n");
	testFailure = 0;
	for (int vertex = 0; vertex < vertexCount; ++vertex)
	{
		sized_uint_array * inNeighbors = edgeListToSizedArray(
			getInNeighbors(testGraph, vertex), vertex
		);
		if (inNeighbors == NULL)
		{
			printf(COL_RED "getInNeighbors returned NULL for vertex %d!" COL_RESET "\n", vertex);
			continue;
		}
		for (unsigned int i = 0; i < inNeighbors->size; ++i)
		{
			if (!hasEdge(testGraph, inNeighbors->array[i], vertex))
			{
				printf(
					COL_RED 
					"Edge that should exists (%d, %d) does not!" 
					COL_RESET "\n", inNeighbors->array[i], vertex
				);
				testFailure = 1;
			}
		}
		free(inNeighbors->array);
		free(inNeighbors);
		
		if (vertex % 10 == 0)
		{
			printf("Vertex: %d\r", vertex);
			fflush(stdout);
		}
	}
	if (testFailure)
		printf(COL_RED "Test Failed!" COL_RESET "\n\n");
	else
		printf(COL_GREEN "Test Success!" COL_RESET "\n\n");

	// test getOutNeighbors, hasEdge
	printf(COL_YELLOW "Testing if getOutNeighbors and hasEdge works!" COL_RESET "\n");
	testFailure = 0;
	for (int vertex = 0; vertex < vertexCount; ++vertex)
	{
		sized_uint_array * outNeighbors = edgeListToSizedArray(
			getOutNeighbors(testGraph, vertex), vertex
		);
		if (outNeighbors == NULL)
		{
			printf(COL_RED "getOutNeighbors returned NULL for vertex %d!" COL_RESET "\n", vertex);
			continue;
		}
		for (unsigned int i = 0; i < outNeighbors->size; ++i)
		{
			if (!hasEdge(testGraph, vertex, outNeighbors->array[i]))
			{
				printf(
					COL_RED 
					"Edge that should exists (%d, %d) does not!" 
					COL_RESET "\n", vertex, outNeighbors->array[i]
				);
				testFailure = 1;
			}
		}
		free(outNeighbors->array);
		free(outNeighbors);
		
		if (vertex % 10 == 0)
		{
			printf("Vertex: %d\r", vertex);
			fflush(stdout);
		}
	}
	if (testFailure)
		printf(COL_RED "Test Failed!" COL_RESET "\n\n");
	else
		printf(COL_GREEN "Test Success!" COL_RESET "\n\n");

	// test getNeighbors, hasEdge
	printf(COL_YELLOW "Testing if getNeighbors and hasEdge works!" COL_RESET "\n");
	testFailure = 0;
	for (int vertex = 0; vertex < vertexCount; ++vertex)
	{

		Edge * neighborEdgeList;
		if (!getNeighbors(testGraph, vertex, &neighborEdgeList))
		{
			printf(COL_RED "getNeighbors returned NULL for vertex %d!" COL_RESET "\n", vertex);
			continue;
		}

		sized_uint_array * neighbors = edgeListToSizedArray(neighborEdgeList, vertex);
		freeEdgeList(neighborEdgeList); // freed when no longer needed
		if (neighbors == NULL)
		{
			printf(COL_RED "Trouble converting edgeList to array!" COL_RESET "\n");
			continue;
		}
		

		for (unsigned int i = 0; i < neighbors->size; ++i)
		{
			if (
				!hasEdge(testGraph, vertex, neighbors->array[i]) &&
				!hasEdge(testGraph, neighbors->array[i], vertex)
			) {
				printf(
					COL_RED 
					"Neither edge (%d, %d) nor (%d, %d) exists, at least one of them should!" 
					COL_RESET "\n", vertex, neighbors->array[i], neighbors->array[i], vertex
				);
				testFailure = 1;
			}
		}
		free(neighbors->array);
		free(neighbors);
		
		if (vertex % 10 == 0)
		{
			printf("Vertex: %d\r", vertex);
			fflush(stdout);
		}
	}
	if (testFailure)
		printf(COL_RED "Test Failed!" COL_RESET "\n\n");
	else
		printf(COL_GREEN "Test Success!" COL_RESET "\n\n");

	freePointersInSizedArrayArray(edgeArrays, edgeArraySize);
	freeGraph(testGraph);
	return 0;
}

void freeSizedArray(sized_uint_array * sArray)
{
	if (sArray != NULL)
	{
		free(sArray->array);
		free(sArray);
	}
}

int testUndirected(int vertexCount)
{
	static char * padding = "   ";

	Graph * testGraph = createGraph(vertexCount);
	int edgeCount = 0;
	Edge * edges = NULL;

	printf(COL_YELLOW "Building undirected graph!" COL_RESET "\n");
	for (int vertex = 0; vertex < vertexCount; ++vertex)
	{
		for (int vertexTo = vertex; vertexTo < vertexCount; ++vertexTo)
		{
			if (rand() % 2) // skip half of the time
				continue;

			if (!addUndirectedEdge(testGraph, vertex, vertexTo))
			{
				printf(
					COL_RED 
					"Failed to create edge (%d, %d)!" 
					COL_RESET COL_BOLD 
					" %s" 
					COL_RESET "\n",
					vertex, vertexTo, addEdgeFailureMessage
				);
				continue;
			}
			
			Edge * edge = malloc(sizeof(Edge));
			if (edge == NULL)
			{
				printf(COL_RED "Problem occured building edge check list!" COL_RESET "\n");
				freeEdgeList(edges);
				return 1;
			}
			edge->from = vertex;
			edge->to = vertexTo;
			edge->next = edges;
			edges = edge;

			edgeCount++;
		}
		if (vertex % 10 == 0)
		{
			printf("Vertex: %d\r", vertex);
			fflush(stdout);
		}
	}
	printf(COL_GREEN "Undirected graph built!" COL_RESET "\n\n");

	// test getNumVertices
	printf(COL_YELLOW "Testing if getNumVertices returns the right amount of edges!" COL_RESET "\n");
	int numVertices = getNumVertices(testGraph);
	if (vertexCount != numVertices)
		printf(COL_RED "Vertex count failed!" COL_RESET "\n");
	else
		printf(COL_GREEN "Vertex count success!" COL_RESET "\n");
	printf("%sExpected: %d, Result: %d" "\n\n", padding, vertexCount, numVertices);
	
	// test getNumEdges
	printf(COL_YELLOW "Testing if getNumEdges returns the right amount of edges!" COL_RESET "\n");
	int numEdges = getNumEdges(testGraph);
	if (edgeCount != numEdges)
		printf(COL_RED "Edge count failed!" COL_RESET "\n");
	else
		printf(COL_GREEN "Edge count success!" COL_RESET "\n");
	printf("%sExpected: %d, Result: %d" "\n\n", padding, edgeCount, numEdges);

	printf(COL_YELLOW "Test that hasEdge returns true for all added edges" COL_RESET "\n");
	int testFailure = 0;
	Edge * edge = edges;
	while (edge != NULL)
	{
		if (!hasEdge(testGraph, edge->from, edge->to))
		{
			printf(
				COL_RED 
				"Edge (%d, %d) does not exist, it should!" 
				COL_RESET "\n",
				edge->from, edge->to
			);
			testFailure = 1;
		}

		edge = edge->next;
	}
	if (testFailure)
		printf(COL_RED "Test Failed!" COL_RESET "\n\n");
	else
		printf(COL_GREEN "Test Success!" COL_RESET "\n\n");

	printf(
		COL_YELLOW 
		"Testing getNeighbors, getOutNeighbors and getInNeighbors"

		"\n%s" COL_RESET
		"If they give the same result (they should)!"
		"\n%s"
		"Also testing hasEdge on edges if above succeeds!"
		"\n%s"
		"And if edges that should not exist do!"
		"\n", padding, padding, padding
	);
	testFailure = 0;
	for (int v = 0; v < vertexCount; ++v)
	{
		
		Edge * neighborList;
		if (!getNeighbors(testGraph, v, &neighborList))
		{
			printf(COL_RED "Function failed! Stopping test..." COL_RESET "\n");
			testFailure = -1;
			break;
		}

		sized_uint_array * neighbors = edgeListToSizedArray(neighborList, v);
		freeEdgeList(neighborList);

		sized_uint_array * outNeighbors = edgeListToSizedArray(getOutNeighbors(testGraph, v), v);
		sized_uint_array * inNeighbors = edgeListToSizedArray(getInNeighbors(testGraph, v), v);
		if (outNeighbors == NULL || inNeighbors == NULL || neighbors == NULL)
		{
			printf(COL_RED "Function failed! Stopping test..." COL_RESET "\n");
			freeSizedArray(outNeighbors);
			freeSizedArray(inNeighbors);
			freeSizedArray(neighbors);
			testFailure = -1;
			break;
		}
		if ( // sort to make sure edges are in same order
			!mergesort_array(outNeighbors) || 
			!mergesort_array(inNeighbors) || 
			!mergesort_array(neighbors)
		) {
			printf(COL_RED "Sorting of edge lists failed! Stopping test..." COL_RESET "\n");
			testFailure = -1;
			break;
		}
		if (
			memcmp(outNeighbors->array, inNeighbors->array, neighbors->size * sizeof(int)) ||
			memcmp(outNeighbors->array, neighbors->array, neighbors->size * sizeof(int))
		) {
			printf(COL_RED "Test failure for vertex %d!" COL_RESET "\n", v);
			printf("All: "); print_s_arr(neighbors);
			printf("Out: "); print_s_arr(outNeighbors);
			printf(" In: "); print_s_arr(inNeighbors);

			testFailure = 1;
			continue;
		}
		unsigned int lastExistingEdge = 0;
		for (unsigned int u = 0; u < neighbors->size; ++u)
		{
			if (!hasEdge(testGraph, v, neighbors->array[u]))
			{
				printf(
					COL_RED 
					"Edge (%d, %d) built from getNeighbors "
					"has hasEdge return false!" 
					COL_RESET "\n", v, neighbors->array[u]
				);
				testFailure = 1;
				continue;
			}
			// test for edges that sould not exist
			for (unsigned int i = lastExistingEdge+1; i < neighbors->array[u]; ++i)
			{
				if (hasEdge(testGraph, v, i))
				{
					printf(
						COL_RED 
						"Edge that should not exist (%d, %d) exists!" 
						COL_RESET "\n",
						v, i
					);
				}
			}
			lastExistingEdge = neighbors->array[u];
		}

		freeSizedArray(outNeighbors);
		freeSizedArray(inNeighbors);
		freeSizedArray(neighbors);
		
		if (v % 10 == 0)
		{
			printf("Vertex: %d\r", v);
			fflush(stdout);
		}
	}
	if (testFailure > 0)
		printf(COL_RED "Test Failed!" COL_RESET "\n\n");
	else if (!testFailure)
		printf(COL_GREEN "Test Success!" COL_RESET "\n\n");
	
	freeEdgeList(edges);
	free(testGraph);
	return 0;
}

int main(int argc, char * argv [])
{
	int vertexCount;
	if (argc < 2)
		vertexCount = 16;
	else
		vertexCount = atoi(argv[1]);
	
	// random init
	time_t t;
	srand((unsigned) time(&t));
	
	int directedResult = testDirected(vertexCount);
	if (directedResult)
		return directedResult;
	
	printf("-----" "\n");

	int undirectedResult = testUndirected(vertexCount);
	if (undirectedResult)
		return undirectedResult;
	
	return 0;
}
