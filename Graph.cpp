// Nikolai Alexander
// Assignment 6 - Part 2
// Christopher Godley

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits> // Needed to find the maximum system integer in shortestWeightedPath()
#include <queue>
#include <stack>
#include <list>
#include "Graph.hpp"

using namespace std;

/* ------PUBLIC FUNCTIONS------ */

// Constructor
Graph::Graph(){}
// Deconstructor
Graph::~Graph(){
	for(int i = vertices.size()-1; i > 0; i--){
		delete &vertices[i];
	}

	delete &vertices[0];
}

// Builds adjecency between two vectors in the graph with their assigned weight
// 		- string v1 - the first city we are comparing
//		- string v2 - the second city we are comparing
//		- int weight - the weight of the adjecency between the two cities
void Graph::addEdge(string v1, string v2, int weight){
	// Iterate through vertices until we find the vertex we are building the edge
	// for
	for(int i = 0; i < vertices.size(); i++){
		// If we find the first vector, iterate through the vertices again, until 
		// we find the vertex we are relating our first vertex to
		if(vertices[i].name == v1){
			for(int j = 0; j < vertices.size(); j++){
				// If we find the second vertex, assign the adjecent relation
				// between the two vertexes and the weight between the two.
				if(vertices[j].name == v2 && i != j){
					adjVertex av;
					av.v = &vertices[j];
					av.weight = weight;
					vertices[i].adj.push_back(av);
				}
			}
		}
	}
}

// Adds a vertex to the graph - does not care about adjacent vertices
//		- string name - the name of the city we would like to add
void Graph::addVertex(string name){
	bool found = false;
	int id = 0;

	// Search through the vertices to see if the city is already in the graph
	while(id < vertices.size()){
		if(vertices[id].name == name){
			found = true;
			break;
		}
		id++;
	}

	// If the city is not in the graph, insert at the end and initialize
	// all struct variables
	if(found == false){
		vertex city;
		city.ID = id;
		city.parent = NULL;
		city.name = name;
		city.districtID = -1;
		city.visited = false;
		city.unweightedDistance = -1;
		city.weightedDistance = -1;
		vertices.push_back(city);
	}
}


// Prints each vertex and their adjacent verticies
void Graph::displayEdges(){
	for(int i = 0; i < vertices.size(); i++){
		// Print district and vertex
		cout << vertices[i].districtID << ":" << vertices[i].name << "-->";
		// Print adjacent vertices
		for(int j = 0; j < vertices[i].adj.size(); j++){
			cout << vertices[i].adj[j].v->name;
			if(j != vertices[i].adj.size()-1){
				cout << "***";
			}
		}
		cout << endl;
	}
}

// Iterates through the vertices and calls the DFSLabel function on all 
// cities that do not have a district ID
void Graph::assignDistricts(){
	// For loop to increase the district ID by 1 every time we are adding
	// a new district ID for a set of cities
	for(int distID = 1; distID < vertices.size(); distID++){
		// Iterate through the vertices, until we reach a city with a
		// district ID of -1
		for(int i = 0; i < vertices.size(); i++){
			// Once we reach a city without a district ID, call the
			// DFSLabel function to assign a district ID to the city and
			// all of its adjacent cities
			if(vertices[i].districtID == -1){
				DFSLabel(vertices[i].name, distID);
				break;
			}
		}
	}
}

// Uses a Breath First Search algorithm to find the shortest unweighted path from the starting
// point to the ending point. If two cities are not in the same district or are not in the graph
// the function will return a message. If there is a path, the function will print the number of
// steps in the past, along with the path.
//		- string startingCity - the city our path is starting at
//		- string endingCity - the city we are trying to get to
void Graph::shortestPath(string startingCity, string endingCity){
	vertex* start = findVertex(startingCity);
	vertex* end = findVertex(endingCity);

	// Check to see if both cities exist
	if(start == NULL || end == NULL){
		cout << "One or more cities doesn't exist" << endl;
		return; 
	}
	
	// Check to see if district IDs are identified and match
	if(start->districtID == -1 && end->districtID == -1){
		cout << "Please identify the districts before checking distances" << endl;
		return;
	}
	else if(start->districtID != end->districtID){
		cout << "No safe path between cities" << endl;
		return;
	}

	// Initialize all "visited" to false and get rid of all parents before moving through the 
	// algorithm
	for(int i = 0; i < vertices.size(); i++){
		vertices[i].visited = false;
		vertices[i].parent = NULL;
	}

	start->visited = true;
	start->unweightedDistance = 0;

	// Build a queue to place the path
	queue<vertex*> pathQueue;

	// Enqueue the first vertex
	pathQueue.push(start);

	while(!pathQueue.empty()){
		// Set the queued vertex to our temporary vertex and dequeue it
		vertex* tmp = pathQueue.front();
		pathQueue.pop();

		// Iterate through the temporary vertex's adjacent vertexes
		for(int i = 0; i < tmp->adj.size(); i ++){
			// If the vertex has been visited, move to the next. If not, increase the distance
			// from the starting vertex by 1, and set the parent to the previous vertex
			if(tmp->adj[i].v->visited == false){
				tmp->adj[i].v->unweightedDistance = tmp->unweightedDistance + 1;
				tmp->adj[i].v->parent = tmp;

				// If we reach our ending city, print the shortest distance from the previous vertex and the path between the two cities
				if(tmp->adj[i].v->name == endingCity){
					cout << tmp->adj[i].v->unweightedDistance;

					// Build a stack so we can print the path in the correct order
					stack<string> printStack;
					vertex* tmpPrint = tmp->adj[i].v;

					// Push the city onto the stack and move onto its parent
					while(tmpPrint != NULL){
						printStack.push(tmpPrint->name);
						tmpPrint = tmpPrint->parent;
					}

					// Print the city at the top of the stack and pop it
					while(!printStack.empty()){
						cout << "," << printStack.top();
						printStack.pop();
					}
					cout << endl;
					return;
				}
				// If we don't reach our ending city, enqueue the vertex into the queue
				else{
					tmp->adj[i].v->visited = true;
					pathQueue.push(tmp->adj[i].v);
				}
			}
		}
	}


}

// Uses Dijkstra's algorithm to find the shortest weighted path between the
// starting and ending cities. If two cities are not in the same district 
// or are not in the graph the function will return a message. If there is 
// a path, the function will print the number of steps in the past, along
// with the path.
//		- string startingCity - the city our path is starting at
//		- string endingCity - the city we are trying to get to
void Graph::shortestWeightedPath(string startingCity, string endingCity){
	vertex* start = findVertex(startingCity);
	vertex* end = findVertex(endingCity);

	// Check to see if both cities exist
	if(start == NULL || end == NULL){
		cout << "One or more cities doesn't exist" << endl;
		return; 
	}
	
	// Check to see if district IDs are identified and match
	if(start->districtID == -1 && end->districtID == -1){
		cout << "Please identify the districts before checking distances" << endl;
		return;
	}
	else if(start->districtID != end->districtID){
		cout << "No safe path between cities" << endl;
		return;
	}

	// Initialize all "visited" to false and get rid of all parents before moving through the 
	// algorithm
	for(int i = 0; i < vertices.size(); i++){
		vertices[i].visited = false;
		vertices[i].parent = NULL;
	}

	// Set start to true and weighted Distance to false so the algorithm
	// does not continue to try and "solve" the starting city - the
	// distance from the starting city to itself is always 0 (go figure)
	start->visited = true;
	start->weightedDistance = 0;

	// Create a list to store the solved cities - Once a city is solved,
	// we no longer need to look at it.
	list<vertex*> solvedList;
	solvedList.push_back(start);

	// Continue to solve each vertex until we reach our target city
	while(end->visited == false){
		int minDistance = INT_MAX; // Maximum value pulled from <climits> library
		vertex* solvedCity = NULL;
		vertex* parent = NULL;

		// Iterate through the list of solved cities and look at their
		// adjacent cities
		for(list<vertex*>::iterator i = solvedList.begin(); i != solvedList.end(); i++){
			vertex* tmpCity = *i;

			// Iterate through all adjacent cities and try and solve them
			// if they have not been solved already
			for(int j = 0; j < tmpCity->adj.size(); j++){
				if(tmpCity->adj[j].v->visited == false){
					int distance = tmpCity->weightedDistance + tmpCity->adj[j].weight;

					// Find the smallest distance between each adjacent
					// city and solved that city
					if(distance < minDistance){
						solvedCity = tmpCity->adj[j].v;
						minDistance = distance;
						parent = tmpCity;
					}
				}
			}
		}

		// Store the city with the smallest distance between itself
		// and it's adjacent city and push it to the end of the list
		solvedCity->weightedDistance = minDistance;
		solvedCity->parent = parent;
		solvedCity->visited = true;
		solvedList.push_back(solvedCity);

		// When we reach our target city, print the shortest distance
		// between itself and the starting city, and print the path.
		if(solvedCity->name == endingCity){
			cout << solvedCity->weightedDistance;

			stack<string> printStack;
			vertex* tmpPrint = solvedCity;

			while(tmpPrint != NULL){
				printStack.push(tmpPrint->name);
				tmpPrint = tmpPrint->parent;
			}

			while(!printStack.empty()){
				cout << "," << printStack.top();
				printStack.pop();
			}
			cout << endl;
			return;
		}
	}
}


/* ------PRIVATE FUNCTIONS------ */
// Find the vertex you are looking for among the graph. If the city does not exist in the
// fraph, the function will return a NULL
//		- string name - The name of the city we are looking for in the graph
vertex* Graph::findVertex(string name){
	for(int i = 0; i < vertices.size(); i++){
		if(vertices[i].name == name){
			return &vertices[i];
		}
	}

	return NULL;
}

// Uses a depth-first search to assign district IDs to the city called in 
// assignDistrict function, and all the city's adjacent cities and all of
// adjacent cities' adjacent cities
//		- string startingCity - the city we are starting the assigning at
//		- int distID - The district ID we are assigning to the cities
void Graph::DFSLabel(string startingCity, int distID){
	// Find the vertex with our city name
	vertex* city = findVertex(startingCity);

	city->districtID = distID;

	// Iterate through all adjacent cities to your city
	for(int i = 0; i < city->adj.size(); i++){
		// Set condition to only assign district ID's to cities without one
		if(	city->adj[i].v->districtID == -1){
			city->adj[i].v->districtID = distID;
			// Recall the function to look at the adjacent cities of the
			// adjacent city (not all cities in the district are adjacent
			// to eachother)
			DFSLabel(city->adj[i].v->name, distID);
		}
	}
}