/*
 * Q2MST.cpp
 *
 *  Created on: Oct 5, 2021
 *      Author: zhhua
 */

#include<iostream>
#include<stdlib.h>
#include <ctime>
#include<map>
#include<list>
#include <algorithm>
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


template<typename T, typename K>
class Graph{

	map<T,list<std::pair<T,int>>> adjList;
	map<T,list<std::pair<T,int>>> MST;
	map<T,list<std::pair<T,int>>>* addEdgeGraph;
	map<T,bool> Discovered;
	map<T,bool> AdjListInProgress;
	map<T,std::pair<T,int>> Parent;
	map<std::pair<T,T>, int> Cycle;
	map<std::pair<T,T>, int> SortedEdges;
	bool terminate_dfs = false;
	int GraphEdge;
	int GraphVertex;
	int MSTVertexCount = 0;
	int MSTEdgeCount = 0;

public:
	Graph(){

	}
	// This function add edge to the existing random generated graph
	void addEdge(T u, std::pair<T, int> v,bool undirected=true, bool isMST=false){
		if (isMST == true)
		{
			addEdgeGraph = &MST;
		}
		else{
			addEdgeGraph = &adjList;
		}

		bool edge_exist = false;
		// Check if edge already exists
		for(std::pair<T,int> neighbour: (*addEdgeGraph)[u])
		{
			if(neighbour.first == v.first)
			{
				edge_exist = true;
			}
		}

		// If edge doesn't exists and the edge is not connecting itself, add edge to graph
		if (edge_exist == false && u != v.first)
		{
			(*addEdgeGraph)[u].push_back(v);
			if(undirected){
				int EdgeCost = v.second;
				T node_v = v.first;
				std::pair<T,T> node_u = {u, EdgeCost};
				(*addEdgeGraph)[node_v].push_back(node_u);
			}
			if (isMST == true)
			{
				MSTEdgeCount += 1;
			}
			else{
				GraphEdge += 1;
			}
		}
	}

	// This function remove edge from the existing MST
	void removeMSTEdge(T node1, T node2){
		for(std::pair<T,int> neighbour: adjList[node1]){
			if (node2 == neighbour.first){
				MST[node1].remove(neighbour);
				break;
			}
		}
		for(std::pair<T,int> neighbour: adjList[node2]){
			if (node1 == neighbour.first){
				MST[node2].remove(neighbour);
				break;
			}
		}
		MSTEdgeCount -= 1;
	}

	// This function generate a graph "int userInput" is the vertex number user want to generate.
	// High probability graphGenerator will get a disconnected graph. Therefore "bool connected"
	// is created forcing graphGenerator to generate a connected graph
	void graphGenerator(int userInput, bool connected=false){
		// Clear all previous structure as test automation will generate multiple graph for testing
		adjList.clear();
		MST.clear();
		Discovered.clear();
		AdjListInProgress.clear();
		Parent.clear();
		Cycle.clear();
		SortedEdges.clear();
		terminate_dfs = false;
		GraphEdge = 0;
		GraphVertex = userInput;
		MSTVertexCount = 0;
		MSTEdgeCount = 0;
		int node1;
		int node2;
		int previousNode = -1;

		cout << "MSTEdgeCount------0.1: " << MSTEdgeCount << endl;
		if (connected){
			for (int createEdge=0; createEdge<GraphVertex-1; createEdge++){
				addEdge(createEdge,{createEdge+1, rand()%50});
			}
			while (GraphEdge < GraphVertex + 8) {
				int nodeToAddEdge;
				nodeToAddEdge = rand()%GraphVertex;
				cout << "nodeToAddEdge: " << nodeToAddEdge << endl;
				for(auto i:adjList){
					for(std::pair<T,int> entry:i.second){
						int EdgeCost = entry.second;
						if ( SortedEdges.find({entry.first,i.first}) == SortedEdges.end() ) {
							SortedEdges[{i.first, entry.first}] = EdgeCost;
						}
					}
				}
				if ( adjList.find(nodeToAddEdge) != adjList.end() ) {
					addEdge(nodeToAddEdge,{rand()%GraphVertex, rand()%50});
				}
			}
		}
		else{
			while (adjList.size() < GraphVertex && GraphEdge < GraphVertex + 8) {
				addEdge(rand()%GraphVertex,{rand()%GraphVertex, rand()%50});
			}
		}
		cout << "MSTEdgeCount------0.1: " << MSTEdgeCount << endl;

		cout << "vertices number: " << GraphVertex << endl;
		cout << "edge number: " << GraphEdge << endl;
	}

	// This function print out graph generated from graphGenerator
	void printAdjacencyList(){
		cout << "printOriginalGraph:" << endl;
		for(auto i:adjList){
			cout<<i.first<<"->";
			for(std::pair<T,int> entry:i.second){
				cout << "(" << entry.first << "," << entry.second << ")" <<",";
			}
			cout << endl;
		}
		cout << endl;
	}

	// This function print out the minimum spanning tree
	void printMST(){
		cout << "printMST:" << endl;
		for(auto i:MST){
			cout<<i.first<<"->";
			MSTVertexCount += 1;
			for(std::pair<T,int> entry:i.second){
				cout << "(" << entry.first << "," << entry.second << ")" <<",";
//				MSTEdgeCount += 1;
			}
			cout << endl;
		}
		cout << endl;
	}

	void checkIfMSTExist(){
		cout << "MSTEdgeCount/2: " << MSTEdgeCount/2 << endl;
		cout <<  "MSTVertexCount - 1: " << MSTVertexCount - 1 << endl;
		cout <<  "GraphVertex - 1: " << GraphVertex - 1 << endl;
		cout <<  "GraphEdge: " << GraphEdge << endl;
		if (MSTEdgeCount == GraphVertex - 1){
			cout << "MST exists, input graph is connected" << endl;
		}
		else{
			cout << "MST doesn't exist, input graph is disconnected" << endl;
		}
	}

	// This function capture all the edges of the cycle being found (with edge's cost)
	void GetCycleEdges(std::pair<T, int> Ancestor, T Descendant){
		Cycle.clear();
		T AncestorNode = Ancestor.first;
		int EdgeCost = Ancestor.second;
		Cycle[{AncestorNode,Descendant}] = EdgeCost;
		while (Parent[Descendant].first != AncestorNode){
			EdgeCost = Parent[Descendant].second;
			Cycle[{Descendant,Parent[Descendant].first}] = EdgeCost;
			Descendant = Parent[Descendant].first;
		}

		EdgeCost = Parent[Descendant].second;
		Cycle[{Descendant,Parent[Descendant].first}] = EdgeCost;
	}

	// This is the actual function which does the graph traversal
	void dfsDiscover(T node){
		//Mark the first seen node as discovered
		Discovered[node] = true;
		AdjListInProgress[node] = true;
		//Try to find out a node which is neigbour of current node and not yet visited
		for(std::pair<T,int> neighbour: MST[node]){
			if(AdjListInProgress[neighbour.first] && Parent[node].first != neighbour.first){
				GetCycleEdges(neighbour, node);
				terminate_dfs = true;
			}
			else if (!Discovered[neighbour.first]){
				Parent[neighbour.first] = {node,neighbour.second};
				dfsDiscover(neighbour.first);
			}
			if (terminate_dfs){
				break;
			}
		}
		AdjListInProgress[node] = false;
	}

	// This function loop through all nodes in given graph,
	// if the node has not been discovered, the node will be used as starting node for DFS finding cycle.
	// function DFS will still go through all disjoint sets until it finds a cycle or given graph vertices exhausted.
	void dfs(){
		//Go through all nodes (vertices ) in graph G
		for(auto i:MST){
			T node = i.first;
			if(!Discovered[node]){
				Parent[node] = {NULL,NULL};
				dfsDiscover(node);
			}
			if (terminate_dfs){
				break;
			}
		}
	}

	// This function get all the edges in graph for merge sort
	void getEdgesToSort(){
		for(auto i:adjList){
			for(std::pair<T,int> entry:i.second){
				int EdgeCost = entry.second;
				if ( SortedEdges.find({entry.first,i.first}) == SortedEdges.end() ) {
//					cout << "i.first: " << i.first << "entry.first" << entry.first << endl;
					SortedEdges[{i.first, entry.first}] = EdgeCost;
				}
			}
		}
	}

	// This function sort input array elements in ascending order
	void mergeEdges(std::pair<std::pair<int,int>, int> SortedEdges[], int const left, int const mid, int const right)
	{
	    // Split input array into two sub arrays.
		// Two sub arrays have sorted in ascending from previous recursive mergeEdges calls
		// Copy those 2 subarray elements into 2 temp arrays.
		auto const subSortedEdgesOne = mid - left + 1;
	    auto const subSortedEdgesTwo = right - mid;

	    auto *SortedEdgesLeft = new std::pair<std::pair<int,int>, int>[subSortedEdgesOne];
	    auto *SortedEdgesRight = new std::pair<std::pair<int,int>, int>[subSortedEdgesTwo];

	    for (auto i = 0; i < subSortedEdgesOne; i++){
	        SortedEdgesLeft[i] = SortedEdges[left + i];
	    }
	    for (auto j = 0; j < subSortedEdgesTwo; j++){
	        SortedEdgesRight[j] = SortedEdges[mid + 1 + j];
	    }

	    auto indexOfsubSortedEdgesOne = 0;
	    auto indexOfsubSortedEdgesTwo = 0;
	    int indexOfMergedArray = left;

	    // While loop check two sorted sub array and merge elements into one array
	    while (indexOfsubSortedEdgesOne < subSortedEdgesOne && indexOfsubSortedEdgesTwo < subSortedEdgesTwo) {
	        if (SortedEdgesLeft[indexOfsubSortedEdgesOne].second <= SortedEdgesRight[indexOfsubSortedEdgesTwo].second) {
	        	SortedEdges[indexOfMergedArray] = SortedEdgesLeft[indexOfsubSortedEdgesOne];
	            indexOfsubSortedEdgesOne++;
	        }
	        else {
	        	SortedEdges[indexOfMergedArray] = SortedEdgesRight[indexOfsubSortedEdgesTwo];
	            indexOfsubSortedEdgesTwo++;
	        }
	        indexOfMergedArray++;
	    }
	    // While loop check leftover elements in first sub array and put them all in merged array
	    while (indexOfsubSortedEdgesOne < subSortedEdgesOne) {
	    	SortedEdges[indexOfMergedArray] = SortedEdgesLeft[indexOfsubSortedEdgesOne];
	        indexOfsubSortedEdgesOne++;
	        indexOfMergedArray++;
	    }
	    // While loop check leftover elements in second sub array and put them all in merged array
	    while (indexOfsubSortedEdgesTwo < subSortedEdgesTwo) {
	    	SortedEdges[indexOfMergedArray] = SortedEdgesRight[indexOfsubSortedEdgesTwo];
	        indexOfsubSortedEdgesTwo++;
	        indexOfMergedArray++;
	    }
	}


	// This function recursively calls itself to split input array into two sub array.
	// In each recursive call, it then sort each sub array's elements in ascending order
	// and merges two sub array back into one
	void mergeSortEdges(std::pair<std::pair<int,int>, int> SortedEdges[], int const begin, int const end)
	{
		// Returns when element number has reduced down to 1.
		if (begin >= end)
	        return;

	    auto mid = begin + (end - begin) / 2;
	    // Recursively call itself until element number reduce down to one
		mergeSortEdges(SortedEdges, begin, mid);
		mergeSortEdges(SortedEdges, mid + 1, end);
		// Merge two sorted sub array into one
	    mergeEdges(SortedEdges, begin, mid, end);
	}

	// This function sorts all the edges in ascending order with respect to their weight.
	// will iterate through all edges starting from the smallest edge adding the edge to a spanning tree set (MST).
	// If the edge forms a cycle with the spanning tree, discard the edge, otherwise include the edge.
	// Algorithm stops when all edges exhaust.
	void MSTAlgorithm(){
//		cout << "GraphEdge------1.1: " << GraphEdge << endl;
		getEdgesToSort();
//		cout << "GraphEdge------1.2: " << GraphEdge << endl;
		std::map<std::pair<int, int>, int>::iterator MapIterator2;
		std::pair<std::pair<int,int>, int> SortedEdges2[SortedEdges.size()];

		// Sort all the edges in ascending order with respect to their weight.
		int arrayIndex = 0;
		for (MapIterator2 = SortedEdges.begin(); MapIterator2 != SortedEdges.end(); MapIterator2++){
//			cout << "arrayIndex: " << arrayIndex << endl;
//			cout << "MapIterator2->first.first: " << MapIterator2->first.first << endl;
//			cout << "MapIterator2->first.second: " << MapIterator2->first.second << endl;
//			cout << "MapIterator2->second: " << MapIterator2->second << endl;
			SortedEdges2[arrayIndex] = {MapIterator2->first, MapIterator2->second};
			arrayIndex += 1;
		}
		auto edge_size = sizeof(SortedEdges2) / sizeof(SortedEdges2[0]);
	    mergeSortEdges(SortedEdges2, 0, edge_size - 1);
//		cout << "GraphEdge------1.3: " << GraphEdge << endl;
//		int removeCount = 0;

//	    cout << "SortedEdges.size(): " << SortedEdges.size() << endl;
		// Iterate through all edges starting from the smallest edge adding the edge to a spanning tree set (MST).
	    for (int i = 0; i < edge_size; i++){
	    	addEdge(SortedEdges2[i].first.first, {SortedEdges2[i].first.second, SortedEdges2[i].second}, true, true);
			Discovered.clear();
			AdjListInProgress.clear();
			terminate_dfs = false;
	    	dfs();
	    	// If the edge forms a cycle with the spanning tree, discard the edge, otherwise include the edge.
	    	if (terminate_dfs){
	    		removeMSTEdge(SortedEdges2[i].first.first, SortedEdges2[i].first.second);
//	    		removeCount += 1;
//	    		cout << "Cycle: " << endl;
//				for (MapIterator2 = Cycle.begin(); MapIterator2 != Cycle.end(); MapIterator2++){
//					std::pair<T, T> edge = MapIterator2->first;
//					cout << "(" << edge.first << "," << edge.second << ")-";
//				}
//	    		cout << "removed: (" << SortedEdges2[i].first.first << "," << SortedEdges2[i].first.second << ")";
			}
		}
//		cout << "GraphEdge------1.4: " << GraphEdge << endl;
//		cout << "removeCount------1.5: " << removeCount << endl;
	}

	void findMST(int graphVertexNum, bool connected=true){
		graphGenerator(graphVertexNum, connected);
		auto millisec_before = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		MSTAlgorithm();
		auto millisec_after = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		printAdjacencyList();
		printMST();
		checkIfMSTExist();
		cout << "MST running time: " << millisec_after - millisec_before << " milliseconds";
	}

	// Test a given connected graph which was generated by the graphGenerator
	void connected_graph_test_code(int graphVertexNum){
		// Given a not connected graph and see if algorithm can return the minimum spanning tree
		bool connected = true;
		graphGenerator(graphVertexNum, connected);
		cout << "MSTEdgeCount------1: " << MSTEdgeCount << endl;
		MSTAlgorithm();
		cout << "MSTEdgeCount------2: " << MSTEdgeCount << endl;
//		printAdjacencyList();
//		printMST();
		checkIfMSTExist();
		cout << "MSTEdgeCount------3: " << MSTEdgeCount << endl;
	}

	// Test a small given graph with no cycle so that cycle detection can be varified
	void disconnected_graph_test_code(int graphVertexNum){
		// Given a not connected graph with 0 cycle and see if algorithm can detect MST doesn't exist
		graphGenerator(graphVertexNum,true);
		for (auto item :SortedEdges){
			cout << "(" << item.first.first << "," << item.first.second << ")";
		}
		// remove all edges of the first node to make sure the graph is disconnected
		int count = 0;
		list<std::pair<T,int>> nodeToRemove;
//		printAdjacencyList();
		for (std::pair<T,int> zeroNeighbour :adjList[0]){
//			for(std::pair<T,int> neighbour: adjList[zeroNeighbour.first]){
//				cout << "neighbour.first: " << neighbour.first << endl;
//				if (0 == neighbour.first){
//					adjList[zeroNeighbour.first].remove(neighbour);
//					break;
//				}
//				count += 1;
//			}
//			cout << "zeroNeighbour: " << zeroNeighbour.first << endl;
			nodeToRemove.push_back(zeroNeighbour);
//			adjList[0].remove(zeroNeighbour);
		}
		std::map<std::pair<int,int>, int>::iterator it;
		for (std::pair<T,int> zeroNeighbour :nodeToRemove){
			cout << "zeroNeighbour: (" << zeroNeighbour.first << "," << zeroNeighbour.second << ")" << endl;
			adjList[0].remove(zeroNeighbour);
			for(std::pair<T,int> neighbour: adjList[zeroNeighbour.first]){
				cout << "neighbour.first: " << neighbour.first << endl;
				if (0 == neighbour.first){
					adjList[zeroNeighbour.first].remove(neighbour);
					SortedEdges.erase({zeroNeighbour.first,0});
					SortedEdges.erase({0,zeroNeighbour.first});
					break;
				}
			}
		}
		for (auto item :SortedEdges){
			cout << "(" << item.first.first << "," << item.first.second << ")";
		}
		MSTAlgorithm();
//		printAdjacencyList();
		printMST();
		checkIfMSTExist();
	}

	// This function test a customized graph created according to user input.
	// "int vertices_number" is the number of vertices of the graph being created
	// MSTAlgorithm will be run against the customized input graph
	int graph_with_custom_vertices_test_code(int vertices_number){
		graphGenerator(vertices_number);
		// Record the time when the algorithm start running
		auto millisec_before = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		MSTAlgorithm();
		// Record the time when the algorithm stop running
		auto millisec_after = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		// Return running time
		return millisec_after - millisec_before;
	}

	// This function test a customized graph created according to user input.
	// it will test it 10 times and take the average for plotting (time complexity analysis)
	// MSTAlgorithm will be run against the customized input graph
	void test_10_times_take_avg_running_time(int graph_vertices_number){
		int testCount = 1;
		int totalRunningTime = 0;
		int runningTime = 0;
		int totalGraphEdge = 0;
		// Run the test 10 times
		for (int i = 0; i<10; i++){
			runningTime += graph_with_custom_vertices_test_code(graph_vertices_number);
			totalRunningTime += runningTime;
			totalGraphEdge += GraphEdge;
			cout << "TestCount: " << testCount << endl;
			cout << "runningTime: " << runningTime << endl;
			testCount += 1;
		}

		// Print average running time result
		cout << "total running time: " << totalRunningTime << endl;
		cout << "average running time: " << totalRunningTime/10 << endl;
		cout << "vertices: " << graph_vertices_number << endl;
		cout << "average edges: " << totalGraphEdge/10 << endl;

	}

	// This function test how the running time grows when graph size increase
	void increasing_graph_size_test_code(int graph_vertices_number){
		// Graph with 500 vertices
//		test_10_times_take_avg_running_time(500);
		// Graph with 1500 vertices
//		test_10_times_take_avg_running_time(1500);
		// Graph with 2500 vertices
//		test_10_times_take_avg_running_time(2500);
		// Graph with 3500 vertices
//		test_10_times_take_avg_running_time(3500);
		// Graph with 4000 vertices
//		test_10_times_take_avg_running_time(4000);
		test_10_times_take_avg_running_time(graph_vertices_number);
	}
};

int main(){
	Graph<int, std::pair<int,int>> g;
	srand((unsigned int)time(NULL));
	// uncomment line  to to generate a random undirected graph.
	// 100 is the vertices number of the graph. It can be changed to any integer.
	// algorithm will find the MTS for the graph if exist. It will print the MST at the end
	g.findMST(100);
	// uncomment next line to test if algorithm may return MST for the given connected graph. The number 1000 is the number of vertices of the graph.
//	g.connected_graph_test_code(1000);
	// uncomment next line to test if algorithm may return MST for the given disconnected graph. The number 1000 is the number of vertices of the graph.
//	g.disconnected_graph_test_code(1000);
	// uncomment next line to test a random generated graph 1000 times to get average running time of dfs. 100 is the number of vertices
//	g.increasing_graph_size_test_code(4000);
	return 0;
}

