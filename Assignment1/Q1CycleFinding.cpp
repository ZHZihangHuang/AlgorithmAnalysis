/*
 * CycleFinding.cpp
 *
 *  Created on: Sep 27, 2021
 *      Author: Zihang Huang
 */

#include<iostream>
#include<stdlib.h>
#include <ctime>
#include<map>
#include<list>
#include <typeinfo>
using namespace std;

// Packages for timing dfs run on increasing graph size
#include <chrono>
using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;
using std::chrono::seconds;
using std::chrono::system_clock;


template<typename T>
class Graph{

	map<T,list<T> > adjList;
	map<T,bool> Discovered;
	map<T,bool> AdjListInProgress;
	map<T,T> Parent;
	list<T> Cycle;
	bool terminate_dfs = false;
	int GraphEdge = 0;

public:
	Graph(){

	}
	// This function add edge to the existing random generated graph
	void addEdge(T u, T v,bool undirected=true){

		bool edge_exist = false;
		// Check if edge already exists
		for(T neighbour: adjList[u])
		{
			if(neighbour == v)
			{
				edge_exist = true;
			}
		}

		// Check if edge doesn't exists and the edge is not connecting itself, add edge to graph
		if (edge_exist == false && u != v)
		{
			adjList[u].push_back(v);
			if(undirected){
				adjList[v].push_back(u);
			}
			GraphEdge += 1;
		}
	}

	// This function generate a graph "int userInput" is the vertex number user want to generate.
	void graphGenerator(int userInput, bool testing=false, bool connected=false, bool atLeastOneCycle=false){
		// Clear all previous structure as test automation will generate multiple graph for testing
		adjList.clear();
		Discovered.clear();
		AdjListInProgress.clear();
		Parent.clear();
		Cycle.clear();
		terminate_dfs = false;
		GraphEdge = 0;
		int GraphVertex = userInput;
		int GraphVertexLimit;

		cout << "Generating random graph... " << endl;
		if (testing){
			if (connected){
				for (int createEdge=0; createEdge<GraphVertex-1; createEdge++){
					addEdge(createEdge,createEdge+1);
				}
				if (atLeastOneCycle){
					addEdge(0,adjList.size());
					if (GraphVertex < 20){
						GraphVertexLimit = GraphVertex;
					}
					else{
						GraphVertexLimit = GraphVertex + rand()%20;
					}
					while (GraphEdge < GraphVertexLimit) {
						int nodeToAddEdge;
						nodeToAddEdge = rand()%GraphVertex;
						if ( adjList.find(nodeToAddEdge) != adjList.end() ) {
							addEdge(nodeToAddEdge,rand()%GraphVertex);
						}
					}
				}
			}
			else{

				// Disconnect vertex 0 from the rest of the vertices
				addEdge(0,0);
				for (int createEdge=1; createEdge<GraphVertex-2; createEdge++){
					addEdge(createEdge,createEdge+1);
				}
				if (atLeastOneCycle){
					addEdge(1,adjList.size());
					if (GraphVertex < 20){
						GraphVertexLimit = GraphVertex;
					}
					else{
						GraphVertexLimit = GraphVertex + rand()%20;
					}
					while (GraphEdge < GraphVertexLimit) {
						int nodeToAddEdge;
						nodeToAddEdge = rand()%GraphVertex+1;
						if ( adjList.find(nodeToAddEdge) != adjList.end() ) {
							addEdge(nodeToAddEdge,rand()%GraphVertex+1);
						}
					}
				}
			}
		}
		else{
			while (adjList.size() < GraphVertex) {
				addEdge(rand()%GraphVertex,rand()%GraphVertex);
			}
		}
		cout << "Graph has been generated. " << endl;
		cout << "Vertex number: " << GraphVertex << endl;
		cout << "Edge number: " << GraphEdge << endl;
	}

	// This function print out graph generated from graphGenerator
	void printAdjacencyList(){
		cout << "printOriginalGraph:" << endl;
		for(auto i:adjList){
			cout<<i.first<<"->";
			for(T entry:i.second){
				cout<<entry<<",";
			}
			cout << endl;
		}
		cout << endl;
		cout << "The graph printed above is the original given graph." << endl;
	}

	// This function capture all the vertices of the cycle being found
	void GetCycle(T Ancestor, T Descendant){
		Cycle.push_back(Ancestor);
		Cycle.push_back(Descendant);
		while (Parent[Descendant] != Ancestor){
			Descendant = Parent[Descendant];
			Cycle.push_back(Descendant);
		}

		Cycle.push_back(Parent[Descendant]);
	}

	// This is the actual function which does the graph traversal
	void dfsDiscover(T node){
		// Mark the first seen node as discovered
		Discovered[node] = true;
		AdjListInProgress[node] = true;
		// Try to discover current node's neigbour which have not yet been visited
		for(T neighbour: adjList[node]){
			if(AdjListInProgress[neighbour] && Parent[node] != neighbour){
				terminate_dfs = true;
				GetCycle(neighbour, node);
				break;
			}
			else if (!Discovered[neighbour]){
				Parent[neighbour] = node;
				dfsDiscover(neighbour);
				if (terminate_dfs){
					break;
				}
			}
		}
		AdjListInProgress[node] = false;
	}

	// This function loop through all nodes in given graph,
	// if the node has not been discovered, the node will be used as starting node for DFS finding cycle.
	// function DFS will still go through all disjoint sets until it finds a cycle or given graph vertices exhausted.
	void dfs(){
		//Go through all nodes (vertices) in graph G in case there are disjoint sets in graph G
		for(auto i:adjList){
			T node = i.first;
			if(!Discovered[node]){
				Parent[node] = NULL;
				dfsDiscover(node);
			}
			// If cycle is found, break the for loop
			if (terminate_dfs){
				break;
			}
		}
		//  If cycle exists, show found cycle
		if (!terminate_dfs){
			cout << "Cycle doesn't exist." << endl;
		}
		else{
			cout << "Cycle exists." << endl;
			cout << "Printing out cycle: " << endl;
			for(auto i:Cycle){
				cout << i << "-";
			}
			cout << "end" << endl;
		}
	}

	void printMenu(){
		cout << "For running DFS enter argument '1'" << endl;
		cout << "For testing DFS given a connected graph with a cycle enter argument '2'" << endl;
		cout << "For testing DFS given a connected graph without a cycle enter argument '3'" << endl;
		cout << "For testing DFS given a disconnected graph with a cycle enter argument '4'" << endl;
		cout << "For testing DFS given a disconnected graph without a cycle enter argument '5'" << endl;
	}

	// The functions below are all for testing purpose

	// This function test if algorithm can detect a cycle given a connected graph with cycle(s).
	void cycle_exist_test_code(int userInput){
		// graphGenerator arguments:
		// first argument "int userInput" control how many vertices the given graph has
		// second argument "bool testing=false" if program in test mode (default is not in test mode)
		// third argument "bool connected=false" if the given graph is connected (default is not connected)
		// fourth argument "bool atLeastOneCycle=false" if the given graph has at least one cycle (default graph doesn't have to have cycle)
		graphGenerator(userInput,true,true,true);
		dfs();
		printAdjacencyList();
	}

	// This function test if algorithm can tell there is no cycle exists given a connected graph with no cycle.
	void cycle_not_exist_test_code(int userInput){
		// graphGenerator arguments:
		// first argument "int userInput" control how many vertices the given graph has
		// second argument "bool testing=false" if program in test mode (default is not in test mode)
		// third argument "bool connected=false" if the given graph is connected (default is not connected)
		// fourth argument "bool atLeastOneCycle=false" if the given graph has at least one cycle (default graph doesn't have to have cycle)
		graphGenerator(userInput,true,true,false);
		dfs();
		printAdjacencyList();
	}

	// This function test if algorithm can detect a cycle given a disconnected graph with cycle(s).
	void disconnected_graph_with_a_cycle_test_code(int userInput){
		// graphGenerator arguments:
		// first argument "int userInput" control how many vertices the given graph has
		// second argument "bool testing=false" if program in test mode (default is not in test mode)
		// third argument "bool connected=false" if the given graph is connected (default is not connected)
		// fourth argument "bool atLeastOneCycle=false" if the given graph has at least one cycle (default graph doesn't have to have cycle)
		graphGenerator(userInput,true,false,true);
		dfs();
		printAdjacencyList();
	}

	// This function test if algorithm can tell there is no cycle exists given a disconnected graph with no cycle.
	void disconnected_graph_with_no_cycle_test_code(int userInput){
		// graphGenerator arguments:
		// first argument "int userInput" control how many vertices the given graph has
		// second argument "bool testing=false" if program in test mode (default is not in test mode)
		// third argument "bool connected=false" if the given graph is connected (default is not connected)
		// fourth argument "bool atLeastOneCycle=false" if the given graph has at least one cycle (default graph doesn't have to have cycle)
		graphGenerator(userInput,true,false,false);
		dfs();
		printAdjacencyList();
	}

	// This function test a customized graph created according to user input.
	// "int vertices_number" is the number of vertices of the graph being created
	// DFS will be run against the customized input graph
	int graph_with_custom_vertices_test_code(int vertices_number){
		graphGenerator(vertices_number);
//		auto nanoseconds_before = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
		auto millisec_before = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		dfs();
//		auto nanoseconds_after = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
		auto millisec_after = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		cout << "dfs run time in millisecond: " << millisec_after - millisec_before << endl;
		return millisec_after - millisec_before;
	}

	// This function test a customized graph created according to user input.
	// it will test it 1000 times and take the average for plotting (time complexity analysis)
	// MSTAlgorithm will be run against the customized input graph
	void test_1000_times_take_avg_running_time(int graph_vertices_number){
		int totalRunningTime = 0;
		int totalGraphEdge = 0;
		for (int i = 0; i<1000; i++){
			totalRunningTime += graph_with_custom_vertices_test_code(graph_vertices_number);
			totalGraphEdge += GraphEdge;
		}
		cout << "total running time: " << totalRunningTime << endl;
		cout << "average running time: " << totalRunningTime/1000 << endl;
		cout << "vertices: " << graph_vertices_number << endl;
		cout << "average edges: " << totalGraphEdge/100 << endl;
	}

	// This function test how the running time grows when graph size increase
	void increasing_graph_size_test_code(int graph_vertices_number){
		// Graph with 500 vertices
//		test_1000_times_take_avg_running_time(500);
		// Graph with 1500 vertices
//		test_1000_times_take_avg_running_time(1500);
		// Graph with 2500 vertices
//		test_1000_times_take_avg_running_time(2500);
		// Graph with 3500 vertices
//		test_1000_times_take_avg_running_time(3500);
		// Graph with 4000 vertices
//		test_1000_times_take_avg_running_time(4000);
		// Graph with 5500 vertices
//		test_1000_times_take_avg_running_time(5500);
		// Graph with 6000 vertices
//		test_1000_times_take_avg_running_time(6000);
		// Graph with 7500 vertices
//		test_1000_times_take_avg_running_time(7500);
		// Graph with 9000 vertices
//		test_1000_times_take_avg_running_time(9000);
		// Graph with customized vertices
		test_1000_times_take_avg_running_time(graph_vertices_number);
	}
};

int main(int argc, char* argv[]){
	char *userMenuInput = argv[1];
	Graph<int> g;
	srand((unsigned int)time(NULL));

	if (argc == 1){
		g.printMenu();
	}
	else if (*userMenuInput == '1'){
		int userInput;
		cout << "You will be running the DFS algorithm to find if a cycle exist in a random generated graph." << endl;
		cout << "How many vertices do you want the input graph to have?" << endl;
		cout << "Please enter an integer..." << endl;
		cin >> userInput;
		g.graphGenerator(userInput);
		g.dfs();
		g.printAdjacencyList();

	}
	else if (*userMenuInput == '2'){
		int userInput;
		cout << "You will be testing the DFS algorithm with a connected graph which has at least 1 cycle." << endl;
		cout << "How many vertices do you want the connected graph to have?" << endl;
		cout << "Please enter an integer..." << endl;
		cin >> userInput;
		g.cycle_exist_test_code(userInput);
	}
	else if (*userMenuInput == '3'){
		int userInput;
		cout << "You will be testing the DFS algorithm with a connected graph which has no cycle." << endl;
		cout << "How many vertices do you want the connected graph to have?" << endl;
		cout << "Please enter an integer..." << endl;
		cin >> userInput;
		g.cycle_not_exist_test_code(userInput);
	}
	else if (*userMenuInput == '4'){
		int userInput;
		cout << "You will be testing the DFS algorithm with a disconnected graph which at least 1 cycle." << endl;
		cout << "How many vertices do you want the disconnected graph to have?" << endl;
		cout << "Please enter an integer..." << endl;
		cin >> userInput;
		g.disconnected_graph_with_a_cycle_test_code(userInput);
	}
	else if (*userMenuInput == '5'){
		int userInput;
		cout << "You will be testing the DFS algorithm with a disconnected graph which has no cycle." << endl;
		cout << "How many vertices do you want the disconnected graph to have?" << endl;
		cout << "Please enter an integer..." << endl;
		cin >> userInput;
		g.disconnected_graph_with_no_cycle_test_code(userInput);
	}
	else{
		g.printMenu();
	}
	// uncomment next line to test a random generated graph 1000 times to get average running time of dfs. 100 is the number of vertices
//	g.increasing_graph_size_test_code(1000);
	return 0;
}
