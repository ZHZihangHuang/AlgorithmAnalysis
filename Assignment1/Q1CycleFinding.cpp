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
	int GraphEdge;

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
		}
	}

	// This function generate a graph "int userInput" is the vertex number user want to generate.
	void graphGenerator(int userInput){
		// Clear all previous structure as test automation will generate multiple graph for testing
		adjList.clear();
		Discovered.clear();
		AdjListInProgress.clear();
		Parent.clear();
		Cycle.clear();
		terminate_dfs = false;
		GraphEdge = 0;
		int GraphVertex = userInput + 1;

		while (adjList.size() < GraphVertex) {
			addEdge(rand()%GraphVertex,rand()%GraphVertex);
			GraphEdge += 1;
		}
		cout << "edge number: " << GraphEdge << endl;
	}

	// This function print out graph generated from graphGenerator
	void printAdjacencyList(){
		cout << "printAdjacencyList:" << endl;
		for(auto i:adjList){
			cout<<i.first<<"->";
			for(T entry:i.second){
				cout<<entry<<",";
			}
			cout << endl;
		}
		cout << endl;
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
			cout << "Cycle: " << endl;
			for(auto i:Cycle){
				cout << i << "-";
			}
			cout << "end" << endl;
		}
	}

	// The functions below are all for testing purpose

	// Test a small given graph with cycle so that cycle detection can be varified
	void cycle_exist_test_code(){
		// Given a graph with 1 cycle and see if algorithm can detect the existing cycle: 1-7-8-1-end
		addEdge(0,1);
		addEdge(0,2);
		addEdge(2,3);
		addEdge(2,6);
		addEdge(6,4);
		addEdge(6,5);
		addEdge(1,7);
		addEdge(1,8); // Cycle edge
		addEdge(8,9); // Cycle edge
		addEdge(7,8); // Cycle edge
		dfs();
		printAdjacencyList();
	}

	// Test a small given graph with no cycle so that cycle detection can be varified
	void cycle_not_exist_test_code(){
		// Given a graph with no cycle and see if algorithm can tell there is no cycle exists
		addEdge(0,1);
		addEdge(0,2);
		addEdge(2,3);
		addEdge(2,6);
		addEdge(6,4);
		addEdge(6,5);
		addEdge(1,7);
		addEdge(1,8);
		addEdge(8,9);
		dfs();
		printAdjacencyList();
	}

	// Test a small given graph (disconnected) with cycle so that cycle detection can be varified
	void disconnected_graph_with_a_cycle_test_code(){
		// Given a graph with 1 cycle and see if algorithm can detect the existing cycle: 1-7-8-1-end
		addEdge(0,1);
		addEdge(0,2);
		addEdge(3,3); // node 3 has no edge
		addEdge(2,6);
		addEdge(6,4);
		addEdge(6,5);
		addEdge(1,7);
		addEdge(1,8); // Cycle edge
		addEdge(8,9); // Cycle edge
		addEdge(7,8); // Cycle edge
		dfs();
		printAdjacencyList();
	}

	// Test a small given graph (disconnected) with no cycle so that cycle detection can be varified
	void disconnected_graph_with_no_cycle_test_code(){
		// Given a graph with 1 cycle and see if algorithm can detect the existing cycle: 1-7-8-1-end
		addEdge(0,1);
		addEdge(0,2);
		addEdge(2,3);
		addEdge(2,6);
		addEdge(3,3); // node 3 has no edge
		addEdge(6,4);
		addEdge(6,5);
		addEdge(1,7);
		addEdge(1,8);
		addEdge(8,9);
		dfs();
		printAdjacencyList();
	}

	// This function test a customized graph created according to user input.
	// "int vertices_number" is the number of vertices of the graph being created
	// DFS will be run against the customized input graph
	int graph_with_custom_vertices_test_code(int vertices_number){
		graphGenerator(vertices_number);
		auto nanoseconds_before = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
		dfs();
		auto nanoseconds_after = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
		cout << "dfs run time in nanoseconds: " << nanoseconds_after - nanoseconds_before << endl;
		return nanoseconds_after - nanoseconds_before;
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
		cout << "average running time: " << totalRunningTime/100 << endl;
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

//int main(){
//
//	Graph<int> g;
//	srand((unsigned int)time(NULL));
//	// uncomment lines 273-275 to to generate a random undirected graph.
//	// 100000 is the number of vertices of the graph. It can be changed to any integer.
//	// algorithm will find if a cycle exist. It will print the graph at the end
//	g.graphGenerator(1000);
//	g.dfs();
//	g.printAdjacencyList();
//	// uncomment next line to test if algorithm may find the cycle in the given connected graph with cycle
////	g.cycle_exist_test_code();
//	// uncomment next line to test if algorithm may figure out there is no cycle in the given connected graph
////	g.cycle_not_exist_test_code();
//	// uncomment next line to test if algorithm may find the cycle in the given disconnected graph with cycle
////	g.disconnected_graph_with_a_cycle_test_code();
//	// uncomment next line to test if algorithm may figure out there is no the cycle in the given disconnected graph
////	g.disconnected_graph_with_no_cycle_test_code();
//	// uncomment next line to test a random generated graph 1000 times to get average running time of dfs. 100 is the number of vertices
////	g.increasing_graph_size_test_code(1000);
//	return 0;
//}
