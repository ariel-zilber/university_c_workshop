/**
* @file TreeAnalyzer.c
* @author  Ariel Zilbershtein
* @version 1.0
*
* @brief Solution to exercise 2
 *
*
* @section DESCRIPTION
*/

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "queue.h"
// -------------------------- const definitions -------------------------

/**
* @def VALID_ARG_COUNT 4
* @brief Represents the amount of expected arguments
*/
#define VALID_ARG_COUNT 4

/**
* @def FILE_PATH_INDEX 1
* @brief Represents the expected index of the file path given by the user
*/
#define FILE_PATH_INDEX 1

/**
* @def FIRST_VERTEX_INDEX 2
* @brief Represents the expected index of the first vertex to compute the shortest path.It is  given by the user
*/
#define FIRST_VERTEX_INDEX 2

/**
* @def SECOND_VERTEX_INDEX 3
* @brief Represents the expected index of the second vertex to compute the shortest path.It is  given by the user
*/
#define SECOND_VERTEX_INDEX 3

/**
* @def MAX_ROW_LENGTH 1024
* @brief  the maximal expected length of row in the vertex file
*/
#define MAX_ROW_LENGTH 1024

/**
* @def IS_LEAF "-"
* @brief  representing a leaf in the input text file
*/
#define  IS_LEAF "-"

/**
* @def INVALID_USAGE_MSG "Usage: TreeAnalyzer <Graph File Path> <First Vertex> <Second Vertex>\n"
* @brief Error mesasge printed when the input parameters given by the user are not correct
*/
#define INVALID_USAGE_MSG "Usage: TreeAnalyzer <Graph File Path> <First Vertex> <Second Vertex>\n"

/**
* @def INVALID_INPUT_MSG "Invalid input\n"
* @brief  Message printed when the txt file representing the graph contains illegal information
*/
#define INVALID_INPUT_MSG "Invalid input\n"


/**
* @def GRAPH_NOT_TREE_MSG "The given graph is not a tree\n"
* @brief  Error message printed when given graph is not a tree
*/
#define GRAPH_NOT_TREE_MSG "The given graph is not a tree\n"

/**
* @def ROOT_VERTEX_MSG "Root Vertex: "
* @brief  printed to the user as part of the TreeAnalzyer output
*/
#define  ROOT_VERTEX_MSG "Root Vertex: "

/**
* @def VERTICES_COUNT_MSG "Vertices Count: "
* @brief  printed to the user as part of the TreeAnalzyer output
*/
#define  VERTICES_COUNT_MSG "Vertices Count: "

/**
* @def EDGES_COUNT_MSG "Edges Count: "
* @brief  printed to the user as part of the TreeAnalzyer output
*/
#define  EDGES_COUNT_MSG "Edges Count: "

/**
* @def MINIMAL_BRACH_LENGTH_MSG "Length of Minimal Branch: "
* @brief  printed to the user as part of the TreeAnalzyer output
*/
#define  MINIMAL_BRACH_LENGTH_MSG "Length of Minimal Branch: "

/**
* @def MAXIMAL_BRACH_LENGTH_MSG "Length of Maximal Branch: "
* @brief  printed to the user as part of the TreeAnalzyer output
*/
#define  MAXIMAL_BRACH_LENGTH_MSG "Length of Maximal Branch: "

/**
* @def DIAMETER_LENGTH_MSG "Diameter Length: "
* @brief  printed to the user as part of the TreeAnalzyer output
*/
#define  DIAMETER_LENGTH_MSG "Diameter Length: "


// ------------------------------ Structures -----------------------------

/**
 * @brief represents A vertex
 * */
typedef struct Vertex
{
    struct Vertex *next; /* representing all the connected with distance of 1 vertexs*/
    int vertexKey; /* the numerical id of the vertex **/

} Vertex;

/**
 * @brief represents a graph
 **/
typedef struct Graph
{
    Vertex **listOfAdjacent; /** represents infomration of adjecnt connected vertex for each vertex in the graph**/
    Vertex **listOfAncestors; /** represents  for every vertex from which other vertex they were attached**/
    int *visited; /** Represents for each vertex whenever it was visted.Used in bfs**/
    int verticesCount; /** represents the total amount of vertices in the graph **/
    int edgesCount; /** represents the total edge count**/
    int root; /** represents the index of the root**/
    bool hasCycle; /** represents whenver the graph contains a cycle.Used in the bfs algorithm**/
} Graph;

/**
 * @brief an array represents for a given vertex v the distance to every other vertex in a given graph.
 * Each index in the array represnts the index of the vertex.
 **/
typedef int *DistanceFromNode;

// ------------------------------ function prototype --------------------

int validateVertexAmountLine(char *str);

bool validateVertexEdgeLine(char *line, int rowIndex, int numOfVertices);

bool parseGraphFile(char ***vertexAdjContent, int *numOfVertices, char *file);

int findLeafIndex(Graph *graph);

void setRootVertexKey(Graph *graph);

int getVertexDeg(Graph *graph, int vertexKey);

void attachEdges(Graph *graph, char **contentToAttach);

void addEdge(Graph *graph, int uVertexIndex, int vVertexIndex);

bool isTree(Graph *graph);

bool isConnected(Graph *graph);

void bfs(Graph *graph, int startVertexKey, DistanceFromNode distFromVertex);

Graph *initGraph(int verticesCount);

Vertex *createVertex(int vertexKey);

int findGraphDiameter(Graph *graph, int rootVertex);

int findMaxBranchLength(Graph *graph, DistanceFromNode distanceFromNode);

int findMinBranchLength(Graph *graph, DistanceFromNode distanceFromNode, int root);

void printTreeDistanceInfo(Graph *graph);

void printShortestPath(Graph *graph, int uVertexKey, int vVertexKey);

void printTreeInfo(Graph *graph, int uVertexKey, int vVertexKey);

void freeContent(char **content, int length);

void deleteAllEdges(Graph **graphPtr);

void freeGraph(Graph **graphPtr);

int nonNumerical(char *str);

bool containsNumber(char *str);

int getMaxValueFromString(char *str);

bool onlyDigitsAndSpaces(char *str);

// ------------------------------ functions -----------------------------

/**
 * @brief Analzing a txt file representng a graph.Expect 3 arguments <Graph File Path> <First Vertex> <Second Vertex>
 * @param argc
 * @param argv
 * */
int main(int argc, char *argv[])
{

    // graph related information
    Graph *graph;
    int firstVertex, secondVertex;
    int graphSize = 0;

    // the output of the given file parsing
    char **vertexAdjContent;

    // check the total amount of given arguments
    if (argc != VALID_ARG_COUNT)
    {
        fprintf(stderr, "%s", INVALID_USAGE_MSG);
        return EXIT_FAILURE;
    }

    // check whenver the given number represents valid integers
    if ((nonNumerical(argv[FIRST_VERTEX_INDEX]) != 0) ||
        (nonNumerical(argv[SECOND_VERTEX_INDEX]) != 0))
    {
        fprintf(stderr, "%s", INVALID_INPUT_MSG);
        return EXIT_FAILURE;
    }

    // save the vertex user input as integers
    firstVertex = (int) strtod(argv[FIRST_VERTEX_INDEX], NULL);
    secondVertex = (int) strtod(argv[SECOND_VERTEX_INDEX], NULL);

    /* parse the given file .exit the program case something went wrong*/
    if (!parseGraphFile(&vertexAdjContent, &graphSize, argv[FILE_PATH_INDEX]))
    {
        freeContent(vertexAdjContent, graphSize);
        return EXIT_FAILURE;
    }

    // Create a graph
    graph = initGraph(graphSize);

    /* check whenever the given vertex values are legal comapred to the graph size.Exit eitherwise*/
    if (firstVertex >= graphSize || secondVertex >= graphSize)
    {
        fprintf(stderr, "%s", INVALID_INPUT_MSG);
        freeContent(vertexAdjContent, graphSize);
        freeGraph(&graph);
        return EXIT_FAILURE;
    }

    // Attached the edges for each vertex
    attachEdges(graph, vertexAdjContent);

    // check whenever a tree was provided
    if (!isTree(graph))
    {
        fprintf(stderr, "%s", GRAPH_NOT_TREE_MSG);
        freeContent(vertexAdjContent, graphSize);
        freeGraph(&graph);
        return EXIT_FAILURE;
    }

    // set the root of the tree
    setRootVertexKey(graph);
    printTreeInfo(graph, firstVertex, secondVertex);

    // delete the graph
    freeGraph(&graph);

    // delete the text file data
    freeContent(vertexAdjContent, graphSize);

    return EXIT_SUCCESS;

}

/**
 * @brief Checks whenever the a line in the text file representing a vertex edge relation is valid
 * @param str
 * */
int validateVertexAmountLine(char *str)
{
    char *nextNumber = str;

    // should contain number
    if (!containsNumber(str))
    {
        return false;
    }


    strtol(nextNumber, &nextNumber, 10);

    // should only contain a single number
    if (strlen(nextNumber) != 0)
    {
        return false;
    }

    return true;
}

/**
 * @brief Checks whenver the first line representing graph size in the supplied text file is valid
 * @param numOfVertices
 * @param line
 * @param rowIndex
 * */
bool validateVertexEdgeLine(char *line, int rowIndex, int numOfVertices)
{

    if (rowIndex >= numOfVertices)
    {
        fprintf(stderr, "%s", INVALID_INPUT_MSG);

        return false;
    }

    if ((!onlyDigitsAndSpaces(line)) && (strcmp(IS_LEAF, line) != 0))
    {
        fprintf(stderr, "%s", INVALID_INPUT_MSG);
        return false;
    }

    if (numOfVertices <= getMaxValueFromString(line))
    {
        fprintf(stderr, "%s", INVALID_INPUT_MSG);
        return false;
    }

    return true;
}


/**
 * @brief Parses a txt file representing a graph in the given foramt that was provided as input ar
 * @param graph
 * @param numOfVertices
 * @param file
 * */
bool parseGraphFile(char ***vertexAdjContent, int *numOfVertices, char *file)
{
    FILE *fp;
    char line[MAX_ROW_LENGTH + 1];
    int rowIndex = 0;

    fp = fopen(file, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "%s", INVALID_USAGE_MSG);
        return false;
    }

    // read the first line and whenever an empty file was given:
    if (fgets(line, sizeof(line), fp) == NULL)
    {
        fprintf(stderr, "%s", INVALID_USAGE_MSG);
        fclose(fp);
        return false;
    }

    // remove line terminators
    sscanf(line, "%[^\r]", line);
    sscanf(line, "%[^\n]", line);

    // check whenever the first line displays the expected number of rows
    if ((!validateVertexAmountLine(line)))
    {
        fprintf(stderr, "%s", INVALID_INPUT_MSG);
        fclose(fp);
        return false;
    }

    // get the number of vertices the graph should have
    *numOfVertices = (int) strtod(line, NULL);

    //
    *vertexAdjContent = malloc((*numOfVertices) * (sizeof(char *)));


    // get the the list of adjacent vertices
    while (fgets(line, sizeof(line), fp) != NULL)
    {

        // remove line terminators
        sscanf(line, "%[^\r]", line);
        sscanf(line, "%[^\n]", line);

        if (!validateVertexEdgeLine(line, rowIndex, *numOfVertices))
        {

            fclose(fp);
            return false;
        }

        // add the row
        (*(vertexAdjContent))[rowIndex] = calloc(strlen(line) + 1, sizeof(char));
        strcpy((*(vertexAdjContent))[rowIndex], line);
        rowIndex++;
    }

    // check the number of rows
    if (rowIndex < *numOfVertices)
    {
        fprintf(stderr, "%s", INVALID_INPUT_MSG);
        fclose(fp);
        return false;
    }

    fclose(fp);
    return true;

}


/**
 * @brief Find some leaf in the graph
 * @param graph
 * */
int findLeafIndex(Graph *graph)
{
    int length = graph->verticesCount;
    int i = 0;
    int leafIndex = 0;

    for (i = 0; i < length; i++)
    {
        leafIndex = i;
        if (graph->listOfAdjacent[i] == NULL)
        {
            continue;
        }

        // check whenever the vertex is a leaf
        if (graph->listOfAdjacent[i]->next == NULL)
        {
            break;
        }

    }
    return leafIndex;

}


/**
 * @brief Find out the root of the graph
 * @param graph
 * @param vertexKey
 * */
void setRootVertexKey(Graph *graph)
{
    int leafIndex = findLeafIndex(graph);
    int rootIndex = leafIndex;
    Vertex *currentVertex;

    while (graph->listOfAncestors[rootIndex] != NULL)
    {
        currentVertex = graph->listOfAncestors[rootIndex];
        rootIndex = currentVertex->vertexKey;
    }
    graph->root = rootIndex;
}

/**
 * @brief Get the degree of a given vertex in a graph
 * @param graph
 * @param vertexKey
 * */
int getVertexDeg(Graph *graph, int vertexKey)
{

    Vertex *vertexPtr = graph->listOfAdjacent[vertexKey];
    int deg = 0;

    while (vertexPtr)
    {
        deg++;

        vertexPtr = vertexPtr->next;
    }
    return deg;
}

/**
 * @brief Add edges to the graph
 * @param graph
 * @param contentToAttach an array of string in the format of the txt input
 * */
void attachEdges(Graph *graph, char **contentToAttach)
{

    int u;
    long int v;

    int verticesCount = graph->verticesCount;
    char *nextNumber;

    // attach the edges
    for (u = 0; u < verticesCount; u++)
    {
        nextNumber = contentToAttach[u];

        // check leaf
        if (strcmp(IS_LEAF, nextNumber) == 0)
        {
            continue;
        }

        // iterate over each index of vertice and add it
        while (*nextNumber)
        {
            v = strtol(nextNumber, &nextNumber, 10);
            addEdge(graph, u, v);

            // no more numbers
            if (strlen(nextNumber) == 0)
            {
                break;
            }

            nextNumber++;
        }

    }

}

/**
 * @brief Add edge between 2 vertices in a given graph
 * @param graph
 * @param uVertexIndex
 * @param vVertexIndex
 * */
void addEdge(Graph *graph, int uVertexIndex, int vVertexIndex)
{

    // add the edge from u to v
    Vertex *newVertex = createVertex(vVertexIndex);
    newVertex->next = graph->listOfAdjacent[uVertexIndex];
    graph->listOfAdjacent[uVertexIndex] = newVertex;

    // add the edge from v to u
    newVertex = createVertex(uVertexIndex);
    newVertex->next = graph->listOfAdjacent[vVertexIndex];
    graph->listOfAdjacent[vVertexIndex] = newVertex;

    // save  the ancestors
    graph->listOfAncestors[vVertexIndex] = newVertex;

    // increase the edge count
    graph->edgesCount = graph->edgesCount + 1;

};


/**
 * @brief Find out whenver a graph is a tree
 * @param graph
 * */
bool isTree(Graph *graph)
{

    return ((isConnected(graph)) && (!graph->hasCycle));
}

/**
 * @brief Find out whenver a graph is connected
 * @param graph
 * */
bool isConnected(Graph *graph)
{
    DistanceFromNode distArray = calloc(graph->verticesCount, sizeof(int));
    int i;
    bool isConnected = true;

    // fill the distance array
    bfs(graph, graph->verticesCount - 1, distArray);

    // count how whenever there is more then 1 connected component
    for (i = 0; i < graph->verticesCount; i++)
    {
        if (!graph->visited[i])
        {
            isConnected = false;
        }
    }

    // delete the distance array
    free(distArray);
    return isConnected;
}

void bfs(Graph *graph, int startVertexKey, DistanceFromNode distFromVertex)
{
    Queue *q = allocQueue();
    int adjVertex;
    int dist = 0;
    distFromVertex[startVertexKey] = dist;
    enqueue(q, startVertexKey);
    int i;
    int currentVertex;
    Vertex *temp;
    int *traverseSource = malloc((graph->verticesCount) * sizeof(int));
    //
    for (i = 0; i < graph->verticesCount; i++)
    {
        graph->visited[i] = 0;
        traverseSource[i] = -1;
    }

    graph->visited[startVertexKey] = 1;


    while (!queueIsEmpty(q))
    {
        currentVertex = (int) dequeue(q);
        temp = graph->listOfAdjacent[currentVertex];
        while (temp)
        {
            adjVertex = temp->vertexKey;

            if (graph->visited[adjVertex] == 0)
            {
                graph->visited[adjVertex] = 1;
                distFromVertex[adjVertex] = distFromVertex[currentVertex] + 1;
                enqueue(q, adjVertex);
                traverseSource[adjVertex] = currentVertex;
            }
                // cycle check
            else if ((graph->visited[adjVertex] == 1) &&
                     (traverseSource[currentVertex] != adjVertex))
            {
                graph->hasCycle = true;
            }

            temp = temp->next;
        }
        dist++;
    }

    freeQueue(&q);
    free(traverseSource);
}

/**
 * @brief Initialize a Graph  struct with a given size
 * @param verticesCount
 * */
Graph *initGraph(int verticesCount)
{

    Graph *graph = malloc(sizeof(Graph));
    int i;

    graph->verticesCount = verticesCount;
    graph->listOfAdjacent = malloc(verticesCount * sizeof(Vertex *));
    graph->listOfAncestors = malloc(verticesCount * sizeof(Vertex *));
    graph->visited = malloc(verticesCount * sizeof(int *));
    graph->edgesCount = 0;
    graph->root = 0;
    graph->hasCycle = false;


    for (i = 0; i < verticesCount; i++)
    {
        graph->listOfAdjacent[i] = NULL;
        graph->listOfAncestors[i] = NULL;
        graph->visited[i] = 0;
    }

    return graph;
}

/**
 * @brief Initialize a vertex struct with a given key value
 * @param vertexKey
 * */
Vertex *createVertex(int vertexKey)
{

    Vertex *newVertex = malloc(sizeof(Vertex));
    newVertex->vertexKey = vertexKey;
    newVertex->next = NULL;

    return newVertex;
};


/**
 * @brief Finds the diameter of a given graph
 * @param graph
 * @param rootVertex the index of the root of the graph
 * */
int findGraphDiameter(Graph *graph, int rootVertex)
{
    int farthestNodeFromRoot = rootVertex;
    DistanceFromNode distFromNode = calloc(graph->verticesCount, sizeof(int));

    int diameter = 0;
    int vertexIndex;
    int maxDistance = 0;

    // performs bfs to get the distances from the root
    bfs(graph, rootVertex, distFromNode);

    // find leaft with a maximal length from root
    for (vertexIndex = 0; vertexIndex < graph->verticesCount; vertexIndex++)
    {

        if (distFromNode[vertexIndex] > maxDistance)
        {
            maxDistance = distFromNode[vertexIndex];
            farthestNodeFromRoot = vertexIndex;
        }
    }

    // performs bfs to get the distances from the found leaft
    bfs(graph, farthestNodeFromRoot, distFromNode);

    // find the vertex with the maximal length from the found vertex
    for (vertexIndex = 0; vertexIndex < graph->verticesCount; vertexIndex++)
    {
        if (distFromNode[vertexIndex] > diameter)
        {
            diameter = distFromNode[vertexIndex];
        }
    }

    // delete the distance array
    free(distFromNode);

    return diameter;
}

/**
 * @brief Finds the max branch length for a given graph
 * @param graph
 * @param distanceFromNode an reach where value in a given index represents distance from the root
 * @param root
 * */
int findMaxBranchLength(Graph *graph, DistanceFromNode distanceFromNode)
{
    int maxBranchLength = 0;
    int vertexIndex;

    // goes over each leaf and find the one with the minimal distance from the root
    for (vertexIndex = 0; vertexIndex < graph->verticesCount; vertexIndex++)
    {

        if ((distanceFromNode[vertexIndex] > maxBranchLength) &&
            (getVertexDeg(graph, vertexIndex) == 1))
        {
            maxBranchLength = distanceFromNode[vertexIndex];
        }
    }

    return maxBranchLength;
}


/**
 * @brief Finds the min branch length for a given graph
 * @param graph
 * @param distanceFromNode an reach where value in a given index represents distance from the root
 * @param root
 * */
int findMinBranchLength(Graph *graph, DistanceFromNode distanceFromNode, int root)
{
    int minBranchLength = graph->verticesCount;
    int vertexIndex;

    // case there is only vertex
    if (graph->verticesCount == 1)
    {
        return 0;
    }

    // goes over each leaf and find the one with the minimal distance from the root
    for (vertexIndex = 0; vertexIndex < graph->verticesCount; vertexIndex++)
    {

        if ((distanceFromNode[vertexIndex] < minBranchLength) &&
            (getVertexDeg(graph, vertexIndex) == 1) && vertexIndex != root)
        {
            minBranchLength = distanceFromNode[vertexIndex];
        }
    }

    return minBranchLength;
}


/**
 * @brief Prints a given graph graph info regarding diameter,max branch length and min  branch length
 * @param graph
 * */
void printTreeDistanceInfo(Graph *graph)
{

    DistanceFromNode distFromNode = calloc(graph->verticesCount, sizeof(int));

    int root = graph->root;
    int minBranchLength, maxBranchLength;
    int diameter = 0;

    // find the distance
    bfs(graph, root, distFromNode);

    // find out the min/max branches length
    maxBranchLength = findMaxBranchLength(graph, distFromNode);
    minBranchLength = findMinBranchLength(graph, distFromNode, root);

    // find out the diameter of the graph
    diameter = findGraphDiameter(graph, root);

    // print the results to the terminal
    printf("%s%d\n", MINIMAL_BRACH_LENGTH_MSG, minBranchLength);
    printf("%s%d\n", MAXIMAL_BRACH_LENGTH_MSG, maxBranchLength);
    printf("%s%d\n", DIAMETER_LENGTH_MSG, diameter);

    // delete the distance array
    free(distFromNode);
}

/**
 * @brief Prints the shortest path between two vertices in graph
 * @param uVertexKey  the path start vertex
 * @param vVertexKey the  path end vertex
 * */
void printShortestPath(Graph *graph, int uVertexKey, int vVertexKey)
{

    // represents the distance from node v
    DistanceFromNode distArray = calloc(graph->verticesCount, sizeof(int));

    int currentVertexKey = uVertexKey;
    int currentDist = graph->verticesCount;
    Vertex *adjVertexPtr;

    // perform bfs to fill the distance array
    bfs(graph, vVertexKey, distArray);

    // print the path to the terminal
    printf("Shortest Path Between %d and %d:", uVertexKey, vVertexKey);

    // print each vertex index
    while (currentVertexKey != vVertexKey)
    {

        printf(" %d", currentVertexKey);
        adjVertexPtr = graph->listOfAdjacent[currentVertexKey];

        // navigate to the next vertex ehich decrease the length of the path
        while (adjVertexPtr)
        {
            if (currentDist > distArray[adjVertexPtr->vertexKey])
            {
                currentDist = distArray[adjVertexPtr->vertexKey];
                currentVertexKey = adjVertexPtr->vertexKey;
            }
            adjVertexPtr = adjVertexPtr->next;
        }
    }

    //  end the row
    printf(" %d\n", vVertexKey);

    // delete the distance array
    free(distArray);
};

/**
 * @brief Prints the full analysis for a graph
 * @param uVertexKey  the path start vertex
 * @param vVertexKey the  path end vertex
 * */
void printTreeInfo(Graph *graph, int uVertexKey, int vVertexKey)
{

    // prints info of the root the graph
    printf("%s%d\n", ROOT_VERTEX_MSG, graph->root);

    // Displays the total vertices number
    printf("%s%d\n", VERTICES_COUNT_MSG, graph->verticesCount);

    // Displays the total edges number
    printf("%s%d\n", EDGES_COUNT_MSG, graph->edgesCount);

    // prints info regarding the distance of the tree
    printTreeDistanceInfo(graph);

    // print the shortest path between the given vertex
    printShortestPath(graph, uVertexKey, vVertexKey);
};

/**
 * @brief Release the of a given array of strings
 * @param content a given array of strings
 * @param length the length of content
 * */
void freeContent(char **content, int length)
{
    int i;

    // delete each item in the array
    for (i = 0; i < length; i++)
    {
        free(content[i]);
    }

    // delete the array
    free(content);
}


/**
 * @brief Release the memory allocated to edges on a given graph
 * @param graphPtr
 * */
void deleteAllEdges(Graph **graphPtr)
{
    Graph *graph = *graphPtr;
    int verticesCount = graph->verticesCount;
    Vertex *currentNode;
    Vertex *temp;
    int i;

    // delete all edges
    for (i = 0; i < verticesCount; i++)
    {
        currentNode = graph->listOfAdjacent[i];

        while (currentNode != NULL)
        {
            temp = currentNode->next;
            free(currentNode);
            currentNode = temp;
        }
    }


}

/**
 * @brief Release the memory allocated to a given graph
 * @param graphPtr
 * */
void freeGraph(Graph **graphPtr)
{

    // deletes all the edges
    deleteAllEdges(graphPtr);

    // deletes the list of vertices
    free((*graphPtr)->listOfAncestors);
    free((*graphPtr)->listOfAdjacent);

    // deletes the visited list of every vertex
    free((*graphPtr)->visited);
    free((*graphPtr));

}


/**
 * @brief Find whenever a number is contained in a given string
 * @param str A given string
 * */
bool containsNumber(char *str)
{
    return nonNumerical(str) != ((int) strlen(str));
}

/**
 * @brief Find the total non numerical characters in a given string
 * @param str A given string
 * @return total non numerical values
 * */
int nonNumerical(char *str)
{
    int i;
    int length = (int) strlen(str);
    int nonNumericalTotal = 0;

    for (i = 0; i < length; i++)
    {
        if (!isdigit(str[i]))
        {
            nonNumericalTotal++;
        }

    }

    return nonNumericalTotal;
}

/**
 * @brief Find the maximal number in a give given string
 * @param str A given string
 * @return maximal number in the string
 * */
int getMaxValueFromString(char *str)
{
    char *end = str;
    int currentIntegerValue;
    int base = 10;
    int maxIntegerValue = 0;

    // iterate over each number and compares it to the current number found
    while (containsNumber(end))
    {

        currentIntegerValue = (int) strtol(end, &end, base);
        if (maxIntegerValue < currentIntegerValue)
        {
            maxIntegerValue = currentIntegerValue;
        }

        end++;
    }

    return maxIntegerValue;
}

/**
 * @brief checks whenever an array contains only digits and spaces
 * @param str A given string
 * @return
 * */
bool onlyDigitsAndSpaces(char *str)
{
    char *c = str;

    // iterates over each character
    while (*c)
    {
        if (!(isdigit(*c) || *c == ' '))
        {
            return false;
        }
        c++;
    }

    return true;
}
