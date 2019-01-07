----------------------
Assignment 6 - Part 2
----------------------
Author: Nikolai Alexander
Published: July 27, 2018
Course: CSCI2270-300


----------------------
DESCRIPTION
----------------------

This program simulates a network of cities in the United States during a zombie
apocolypse. Zombies have knocked over communication towers and taken control of 
roads and bridges. The program reads a text file determined in the line argument
containing an adjacency matrix, and builds a graph, connecting each city. If the
weight between two cities is -1, then there is no direct path. However, a path
can be found by navigating through available cities. In part 2 of the project,
we use a Depth First Seach algorithm to separate the cities into districts by 
their available paths - two cities with no path to eachother are in different
districts. We then use a Breath First Search algorithm to find the shortest
path between cities from one city to another, and Dijkstra's algorithm to find
the shortest weighted path between two cities.



----------------------
FILE DESCRIPTION
----------------------

Graph.hpp - The header file containing the vertex and adjVertex structures, and the
			  Graph class. It also initializes all of the function prototypes in 
			  the Graph class


GraphPart2.cpp - This is the file building all of the functions in the Graph class.

	Functions:
	* void addEdge(string, string, int) - Builds adjecency between two vectors 
                                          in the graph with their assigned weight

    * void addVertex(string) - Adds a vertex to the graph (does not care about 
                               adjacent vertices)

    * void displayEdges() - Prints each vertex and their adjacent verticies

    * void shortestPath(string, string) - Uses a Breath First Search algorithm to
    									  find the shortest unweighted path from 
    									  the starting point to the ending point. 
    									  If two cities are not in the same district 
    									  or are not in the graph the function will 
    									  return a message. If there is a path, the 
    									  function will print the number of steps in 
    									  the past, along with the path.

    * void shortestWeightedPath(string, string) - Uses Dijkstra's algorithm to find
    											  the shortest weighted path between 
    											  the starting and ending cities. If 
    											  two cities are not in the same 
    											  district or are not in the graph the 
    											  function will return a message. If 
    											  there is a path, the function will 
    											  print the number of steps in the 
    											  past, along with the path.

	* vertex* findVertex(string) - Find the vertex you are looking for among the graph.
								   If the city does not exist in the fraph, the function
								   will return a NULL

	* void DFSLabel(string, int) - Uses a depth-first search to assign district IDs to 
								   the city called in assignDistrict function, and all 
								   the city's adjacent cities and all of adjacent cities' 
								   adjacent cities

Part2.cpp - The main file of the program. Constructs the data structure, and displays a
		  main menu showing options to select any of the functions written in Graph.cpp.

	Functions:
	* int main(int, char*) - The main function of the file. Constructs the data structure, and uses a main menu to call functions written in Graph.cpp

	* void buildGraph(Graph*, char*) - Reads the text file determined in the line argument and 
                                       populates the graph with all vertexes and edges through
                                       the addVertex() and addEdge() functions in the Graph
                                       class



----------------------
BUILD INSTRUCTIONS
----------------------

To build and run the file, type the following commands in your terminal:

	g++ GraphPart2.cpp Part2.cpp -o Part2

	./Part2 filename

	* Where filename is the name of the txt file containing the movie data you would like to 
	  populate the database with.