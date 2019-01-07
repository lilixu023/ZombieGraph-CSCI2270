// Nikolai Alexander
// Assignment 6 - Part 2
// Christopher Godley

/*

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

*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <sstream>
#include "Graph.hpp"

using namespace std;

void buildGraph(Graph* cities, char* txtFile);

int main(int argc, char* argv[]){
	Graph zombieCities;
	int option = 0;

	// Builds the graph from the text file specified in the line argument
	buildGraph(&zombieCities, argv[1]);

	while (1){
		// Main Menu
		cout << "======Main Menu======" << endl; 
		cout << "1. Print vertices" << endl;
		cout << "2. Find Districts" << endl;
		cout << "3. Find Shorest Path" << endl;
		cout << "4. Find Shortest Weighted Path" << endl;
		cout << "5. Quit" << endl;

		cin >> option;

		// Print vertices
		if(option == 1){
			zombieCities.displayEdges();
		}
		// Find Districts
		else if(option == 2){
			zombieCities.assignDistricts();
		}
		// Find Shortest Path
		else if(option == 3){
			string startingCity;
			string endingCity;


			cout << "Enter a starting city:" << endl;
			cin >> ws;
			getline(cin, startingCity);

			cout << "Enter an ending city:" << endl;
			cin >> ws;
			getline(cin, endingCity);

			zombieCities.shortestPath(startingCity, endingCity);
		}
		// Find Shortest Weighted Path
		else if (option == 4){
			string startingCity;
			string endingCity;


			cout << "Enter a starting city:" << endl;
			cin >> ws;
			getline(cin, startingCity);

			cout << "Enter an ending city:" << endl;
			cin >> ws;
			getline(cin, endingCity);

			zombieCities.shortestWeightedPath(startingCity, endingCity);
		}
		// Quit
		else if(option == 5){
			break;
		}
		// Invalid Entry
		else{
			cout << "Invalid entry. Please try again." << endl;

			// Prevent from infinite looping if a non-int input is entered
			// Source: https://stackoverflow.com/questions/10828937/how-to-make-cin-take-only-numbers/10829026#10829026
			cin.clear();
    		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}

	cout << "Goodbye!"  << endl;

	return 0;
}

// Build the graph initialized in the main function. It opens the text file containing the 
// adjacency matrix and builds each vertex out of the headers. After the vertexes are assigned,
// the function then goes through and assigns adjecency between vectors that do not have a
// weight of 0 or -1.
//		- Graph* cities - a pointer to the graph we initialized in the main function
//		- char* txtFile - the txtFile determined in the line argument
void buildGraph(Graph* cities, char* txtFile){
	string header;
	string city;

	// Create a dynamically allocated array to store the names of the city for later use
	const int cityNamesLength = 100;
	string* cityNames = new string[cityNamesLength];

	ifstream adjMatrix(txtFile);
	if(!adjMatrix){
		cout << "Unable to locate " << txtFile << endl;
		return;
	}

	// Skip the first word of the first line, as "cities" is not apart of our
	// network of cities we are adding to the graph
	getline(adjMatrix, header, ',');

	// Pull the rest of the header line from the file and use istringstream so we
	// can split the line up further into seperate cities
	getline(adjMatrix,header);
	istringstream vertexHeaders(header);

	// Split each city up into it's own string and insert it into the vector	
	while(getline(vertexHeaders, city, ',')){
		// Build verticies for each city name
		cities->addVertex(city);

		// Store city names in a dynamically allocated array for later use
		for(int i = 0; i < cityNamesLength; i++){
			if(cityNames[i] == ""){
				cityNames[i] = city;
				break;
			}
		}
	}

	string cityEdges;

	string strEdge;
	int edge = 0;

	while(!adjMatrix.eof()){
		getline(adjMatrix, cityEdges);
		istringstream vertexEdges(cityEdges);

		// Pull the city we are observing from the first column of the text file
		getline(vertexEdges, city, ',');

		// Iterate through the stored city names in the dynamically allocated array, and build
		// the adjacent vectors using the weights from the text file
		for(int i = 0; i < cityNamesLength && cityNames[i] != ""; i++){
			// Pulls the edge for the relationship between the two cities
			getline(vertexEdges, strEdge, ',');
			edge = stoi(strEdge);

			if(edge != -1 && edge != 0){
				cities->addEdge(city, cityNames[i], edge);
			}
		}
	}

	// Delete the array of city names to free up memory
	delete[] cityNames;

	// Close the file to free up memory
	adjMatrix.close();

}