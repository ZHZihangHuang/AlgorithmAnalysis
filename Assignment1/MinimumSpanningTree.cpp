/*
 * MinimumSpanningTree.cpp
 *
 *  Created on: Oct 3, 2021
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




//using MapIterator = typename std::map<K,V>::const_iterator;
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

public:
	Graph(){

	}
	void addEdge(T u, std::pair<T, int> v,bool undirected=true, bool isMST=false){
//		cout << "debug------------------5" << endl;
		if (isMST == true)
		{
//			cout << "debug------------------6" << endl;
//			addEdgeGraph.clear();
			addEdgeGraph = &MST;
		}
		else{
//			cout << "debug------------------7" << endl;
//			addEdgeGraph.clear();
			addEdgeGraph = &adjList;
		}

		bool edge_exist = false;
		// Check if edge already exists
		for(std::pair<T,int> neighbour: (*addEdgeGraph)[u])
		{
//			cout << "neighbour.first: " << neighbour.first << endl;
//			cout << "neighbour.second: " << neighbour.second << endl;
//			cout << "v.first: " << v.first << endl;
//			cout << "v.second: " << v.second << endl;
			if(neighbour.first == v.first)
			{
//				cout << "edge (" << u << ", (" << v.first << "," << v.second << ")) already exist in adjList\n";
				edge_exist = true;
			}
		}

		// Check if edge doesn't exists and the edge is not connecting itself, add edge to graph
//		cout << "debug------------------8" << endl;
//		cout << "edge_exist: " << edge_exist << endl;
//		cout << "v.first: " << v.first << endl;
//		cout << "v.second: " << v.second << endl;
		if (edge_exist == false && u != v.first)
		{
			(*addEdgeGraph)[u].push_back(v);
			if(undirected){
				int EdgeCost = v.second;
				T node_v = v.first;
				std::pair<T,T> node_u = {u, EdgeCost};
				(*addEdgeGraph)[node_v].push_back(node_u);
			}
		}
	}
	void rempveEdge(T node1, T node2){
//		cout << "node1: " << node1 << endl;
//		cout << "node2: " << node2 << endl;

		for(std::pair<T,int> neighbour: adjList[node1]){
//			cout << "debug-----------3.4.1" << endl;
//			cout << "node2: " << node2 << endl;
//			cout << "neighbour.first: " << neighbour.first << endl;
			if (node2 == neighbour.first){
				MST[node1].remove(neighbour);
//				cout << neighbour.first << " has been removed from " << node1 << endl;
//				cout << "debug-----------3.4.2" << endl;
//				debug();
				break;
			}
		}
		for(std::pair<T,int> neighbour: adjList[node2]){
			if (node1 == neighbour.first){
				MST[node2].remove(neighbour);
//				cout << "debug-----------3.4.4" << endl;
//				cout << neighbour.first << " has been removed from " << node2 << endl;
//				debug();
				break;
			}
		}

	}

	void graphGenerator(int userInput){
		// add 1 as 0 is counted
		int GraphVertex = userInput + 1;
		adjList.clear();
		Discovered.clear();
		AdjListInProgress.clear();
		Parent.clear();
		Cycle.clear();
		terminate_dfs = false;
		GraphEdge = 0;

//		cout << "rand()%GraphVertex: " << rand()%GraphVertex << endl;
		while (adjList.size() < GraphVertex && GraphEdge < GraphVertex + 8) {
			addEdge(rand()%GraphVertex,{rand()%GraphVertex, rand()%50});
			GraphEdge += 1;
		}
		cout << "edge number: " << GraphEdge << endl;
	}

	void printAdjacencyList(){

		cout << "printAdjacencyList:" << endl;
		//Iterate over the map
		for(auto i:adjList){
			cout<<i.first<<"->";
			//i.second is LL

			for(std::pair<T,int> entry:i.second){
				cout << "(" << entry.first << "," << entry.second << ")" <<",";
			}
			cout << endl;
		}
		cout << endl;
	}

	void printNewAdjacencyList(){

		cout << "printNewAdjacencyList:" << endl;
		//Iterate over the map
		for(auto i:MST){
			cout<<i.first<<"->";
			//i.second is LL

			for(std::pair<T,int> entry:i.second){
				cout << "(" << entry.first << "," << entry.second << ")" <<",";
			}
			cout << endl;
		}
		cout << endl;
	}

	void GetCycleEdges(std::pair<T, int> Ancestor, T Descendant){
		Cycle.clear();
		T AncestorNode = Ancestor.first;
		int EdgeCost = Ancestor.second;
		Cycle[{AncestorNode,Descendant}] = EdgeCost;
//		cout << "Descendant: " << Descendant << endl;
//		cout << "Parent[Descendant]: " << Parent[Descendant] << endl;
//		cout << "Ancestor: " << Ancestor << endl;
		while (Parent[Descendant].first != AncestorNode){
			EdgeCost = Parent[Descendant].second;
			Cycle[{Descendant,Parent[Descendant].first}] = EdgeCost;
			Descendant = Parent[Descendant].first;
//			cout << "Parent[Descendant]: " << Parent[Descendant] << endl;
//			cout << "Ancestor: " << Ancestor << endl;
		}

		EdgeCost = Parent[Descendant].second;
		Cycle[{Descendant,Parent[Descendant].first}] = EdgeCost;
	}

	void dfsDiscover(T node){
		//Mark the first seen node as discovered
		Discovered[node] = true;
		AdjListInProgress[node] = true;
		//Try to find out a node which is neigbour of current node and not yet visited
		for(std::pair<T,int> neighbour: MST[node]){
//			cout << "current node: " << node << endl;
//			cout << "Parent[node].first: " << Parent[node].first << endl;
//			cout << "neighbour.first: " << neighbour.first << endl;
//			cout << "AdjListInProgress[neighbour.first]: " << AdjListInProgress[neighbour.first] << endl;
//			cout << "Parent[neighbour].first: " << Parent[neighbour].first << endl;
//			debug();

			if(AdjListInProgress[neighbour.first] && Parent[node].first != neighbour.first){
//				cout << "debug ---- 1" << endl;
//				cout << "Cycle exist." << endl;
//				for(auto i:Parent){
//					cout << "Parent i: "<< i.first <<  "Parent[i]: " << i.second << endl;
//				}

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
//			cout << "debug-----------3.6" << endl;
		}
		AdjListInProgress[node] = false;
	}

	void RemoveMostExpensiveEdge(){
		int MaxCost = 0;
		std::pair<T, T> EdgeToRemove = {NULL, NULL};
		std::map<std::pair<int, int>, int>::iterator MapIterator;
		for (MapIterator = Cycle.begin(); MapIterator != Cycle.end(); MapIterator++){
			std::pair<T, T> edge = MapIterator->first;
			if (MaxCost < Cycle[edge]){
				MaxCost = Cycle[edge];
				EdgeToRemove = edge;
			}
		}
		cout << "debug-----------3.4" << endl;
		T node1 = EdgeToRemove.first;
		T node2 = EdgeToRemove.second;
		cout << "node1: " << node1 << endl;
		cout << "node2: " << node2 << endl;
//		int count1 = 0;
//		int count2 = 0;
//		printAdjacencyList();
		for(std::pair<T,int> neighbour: adjList[node1]){
			cout << "debug-----------3.4.1" << endl;
			cout << "node2: " << node2 << endl;
			cout << "neighbour.first: " << neighbour.first << endl;
			if (node2 == neighbour.first){
				MST[node1].remove(neighbour);
				cout << neighbour.first << " has been removed from " << node1 << endl;
				cout << "debug-----------3.4.2" << endl;
				debug();
				break;
			}
//			count1 += 1;
//			if (count1 > 25){
//				break;
//			}
		}
//		printAdjacencyList();
		for(std::pair<T,int> neighbour: adjList[node2]){
			if (node1 == neighbour.first){
				MST[node2].remove(neighbour);
				cout << "debug-----------3.4.4" << endl;
				cout << neighbour.first << " has been removed from " << node2 << endl;
				debug();
				break;
			}
//			count2 += 1;
//			if (count2 > 25){
//				break;
//			}
		}
		cout << "debug-----------3.5" << endl;
		GraphEdge -= 1;
	}

	void dfs(){

		//Go through all nodes (vertices ) in graph G
//		cout << "debug-----------3.1" << endl;
		for(auto i:MST){
//			cout << "debug-----------3.2" << endl;
			T node = i.first;
			if(!Discovered[node]){
				Parent[node] = {NULL,NULL};
//				cout << "dfs node: " << node << endl;
				dfsDiscover(node);
			}
			if (terminate_dfs){
//				cout << "debug-----------3.8 Cycle exists" << endl;
				break;
			}
//			cout << "debug-----------3.7" << endl;
		}
	}


	void getSortedEdges(){
		for(auto i:adjList){
//			cout<<i.first<<"->";
			//i.second is LL

			for(std::pair<T,int> entry:i.second){
//				cout << "(" << entry.first << "," << entry.second << ")" <<",";
				int EdgeCost = entry.second;
				if ( SortedEdges.find({entry.first,i.first}) == SortedEdges.end() ) {
					SortedEdges[{i.first, entry.first}] = EdgeCost;
				}
			}
//			cout << endl;
		}
//		cout << endl;
	}

	void mergeEdges(std::pair<std::pair<int,int>, int> SortedEdges[], int const left, int const mid, int const right)
	{
	    auto const subSortedEdgesOne = mid - left + 1;
	    auto const subSortedEdgesTwo = right - mid;
//	    cout << "subSortedEdgesOne: " << subSortedEdgesOne << endl;
//	    cout << "subSortedEdgesTwo: " << subSortedEdgesTwo << endl;
//	    cout << "left array:";

	    auto *SortedEdgesLeft = new std::pair<std::pair<int,int>, int>[subSortedEdgesOne];
	    auto *SortedEdgesRight = new std::pair<std::pair<int,int>, int>[subSortedEdgesTwo];

	    for (auto i = 0; i < subSortedEdgesOne; i++){
	        SortedEdgesLeft[i] = SortedEdges[left + i];
	//    	cout << SortedEdgesLeft[i] << " ";
	    }
//	    cout << endl << "right array:";
	    for (auto j = 0; j < subSortedEdgesTwo; j++){
	        SortedEdgesRight[j] = SortedEdges[mid + 1 + j];
	//        cout << SortedEdgesRight[j] << " ";
	    }
//	    cout << endl;



	    auto indexOfsubSortedEdgesOne = 0;
	    auto indexOfsubSortedEdgesTwo = 0;
	    int indexOfMergedArray = left;

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
	    while (indexOfsubSortedEdgesOne < subSortedEdgesOne) {
	    	SortedEdges[indexOfMergedArray] = SortedEdgesLeft[indexOfsubSortedEdgesOne];
	        indexOfsubSortedEdgesOne++;
	        indexOfMergedArray++;
	    }
	    while (indexOfsubSortedEdgesTwo < subSortedEdgesTwo) {
	    	SortedEdges[indexOfMergedArray] = SortedEdgesRight[indexOfsubSortedEdgesTwo];
	        indexOfsubSortedEdgesTwo++;
	        indexOfMergedArray++;
	    }
	}


	void mergeSortEdges(std::pair<std::pair<int,int>, int> SortedEdges[], int const begin, int const end)
	{
	    if (begin >= end)
	        return; // Returns recursivly

	    auto mid = begin + (end - begin) / 2;
	//    cout << "First mergeSort started.\n";
	//    cout << "array: ";
	//    for (auto i = 0; i < end + 1; i++)
	//            cout << array[i] << " ";
//	    cout << "first begin, mid, end:\n";
//		cout << begin << "," << mid << "," << end << endl;
		mergeSortEdges(SortedEdges, begin, mid);
	//    cout << "Second mergeSort started.\n";
	//	cout << "array: ";
	//	for (auto i = 0; i < end + 1; i++)
	//			cout << array[i] << " ";
//	    cout << "second begin, mid, end:\n";
//		cout << begin << "," << mid << "," << end << endl;
		mergeSortEdges(SortedEdges, mid + 1, end);
//	    cout << "third begin, mid, end:\n";
//	    cout << begin << "," << mid << "," << end << endl;
	    mergeEdges(SortedEdges, begin, mid, end);
	}
	void printSortedEdges(std::pair<std::pair<int,int>, int> SortedEdges[], int size)
	{
		for (int i = 0; i < size; i++){
			cout << "((" << SortedEdges[i].first.first << "," << SortedEdges[i].first.second << ")," << SortedEdges[i].second << ")";
		}
	}

	void MSTAlgorithm(){
//		cout << "debug--------------------6" << endl;
		getSortedEdges();
//		cout << "debug--------------------7" << endl;
//		if ( SortedEdges.find({0,7}) == SortedEdges.end() ) {
//			cout << "!SortedEdges[{0,7}] not exists" << endl;
//		} else {
//			cout << "SortedEdges[{0,7}]: " << SortedEdges[{0,7}] << endl;
//		}
//		cout << "debug--------------------8" << endl;
		std::map<std::pair<int, int>, int>::iterator MapIterator2;
		std::pair<std::pair<int,int>, int> SortedEdges2[SortedEdges.size()];
		int arrayIndex = 0;
		for (MapIterator2 = SortedEdges.begin(); MapIterator2 != SortedEdges.end(); MapIterator2++){
			SortedEdges2[arrayIndex] = {MapIterator2->first, MapIterator2->second};
			arrayIndex += 1;
//			cout << "debug--------------------8.1" << endl;
		}
//		cout << "debug--------------------9" << endl;
//		cout << endl;
		auto edge_size = sizeof(SortedEdges2) / sizeof(SortedEdges2[0]);
//	    cout << "Given array is \n";
//	    printSortedEdges(SortedEdges2, edge_size);
//	    cout << "\n";

	    mergeSortEdges(SortedEdges2, 0, edge_size - 1);

//	    cout << "\nSorted array is \n";
//	    printSortedEdges(SortedEdges2, edge_size);

	    for (int i = 0; i < edge_size; i++){
//	    	cout << "debug--------------------10" << endl;
	    	addEdge(SortedEdges2[i].first.first, {SortedEdges2[i].first.second, SortedEdges2[i].second}, true, true);
//	    	adjList.clear();
			Discovered.clear();
			AdjListInProgress.clear();
//			Parent.clear();
//			Cycle.clear();
			terminate_dfs = false;
//			GraphEdge = 0;
	    	dfs();
	    	if (terminate_dfs){
	    		rempveEdge(SortedEdges2[i].first.first, SortedEdges2[i].first.second);
//	    		cout << "debug-----------11 Cycle exists" << endl;
			}
//	    	else{
//	    		cout << "debug-----------12 Cycle doesn't exists" << endl;
//	    	}
//	    	printAdjacencyList();
//	    	printNewAdjacencyList();
		}
	}


	void cycle_exist_test_code(){
		// Given a graph with 1 cycle and see if algorithm can return the minimum spanning tree
		if ( SortedEdges.find({0,7}) == SortedEdges.end() ) {
			cout << "!SortedEdges[{0,7}] not exists" << endl;
		} else {
			cout << "SortedEdges[{0,7}]: " << SortedEdges[{0,7}] << endl;
		}

		addEdge(0,{7,41});
		if ( SortedEdges.find({0,7}) == SortedEdges.end() ) {
			cout << "!SortedEdges[{0,7}] not exists" << endl;
		} else {
			cout << "SortedEdges[{0,7}]: " << SortedEdges[{0,7}] << endl;
		}
		addEdge(1,{2,39});
		addEdge(1,{9,41});
		addEdge(2,{8,49}); // most expensive edge in cycle 1
		addEdge(2,{6,22});
		addEdge(2,{3,19});
		addEdge(2,{5,14});
		addEdge(3,{10,49});
		addEdge(4,{9,44});
		addEdge(4,{8,18});
		addEdge(5,{8,5});
		addEdge(5,{6,5});
		addEdge(7,{8,19});

		// run minimum spanning tree Algorithm
		MSTAlgorithm();
	}


	void debug(){
		printNewAdjacencyList();
		std::map<int, bool>::iterator MapIterator;
		std::map<std::pair<int, int>, int>::iterator MapIterator2;
		for (MapIterator = Discovered.begin(); MapIterator != Discovered.end(); MapIterator++){
			T node = MapIterator->first;
			cout << "node: " << node << endl;
			cout << "Discovered[node]: " << Discovered[node] << endl;
		}
		for (MapIterator = AdjListInProgress.begin(); MapIterator != AdjListInProgress.end(); MapIterator++){
			T node = MapIterator->first;
			cout << "node: " << node << endl;
			cout << "AdjListInProgress[node]: " << AdjListInProgress[node] << endl;
		}
		cout << "Cycle: " << endl;
		for (MapIterator2 = Cycle.begin(); MapIterator2 != Cycle.end(); MapIterator2++){
			std::pair<T, T> edge = MapIterator2->first;
			cout << "(" << edge.first << "," << edge.second << ")-";
		}
		cout << "SortedEdges size: " << SortedEdges.size() << endl;
		for (MapIterator2 = SortedEdges.begin(); MapIterator2 != SortedEdges.end(); MapIterator2++){
			std::pair<T, T> edge = MapIterator2->first;
			int EdgeCost = MapIterator2->second;
			cout << "((" << edge.first << "," << edge.second << ")," << EdgeCost << ")-";
		}
		cout << endl;
	}


	void cycle_not_exist_test_code(){
		// Given a not connected graph with 0 cycle and see if algorithm can detect no cycle exist
		addEdge(0,{10,22});
		addEdge(0,{9,25});
		addEdge(0,{4,49});
		addEdge(1,{5,9});
		addEdge(2,{6,20});
		addEdge(3,{7,1});
		addEdge(3,{4,49});
		addEdge(4,{8,48});
		addEdge(5,{2,15});
		addEdge(5,{9,6});
		MSTAlgorithm();
		printAdjacencyList();
		printNewAdjacencyList();
	}

	int graph_with_100_vertices_test_code(){
		graphGenerator(100);
//		printAdjacencyList();
		// time dfs
		auto nanoseconds_since_epoch = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
		auto sec_since_epoch = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
		auto millisec_before = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		auto nanoseconds_before = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
		dfs();
		auto millisec_after = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		auto sec_since_epoch_after = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
		auto nanoseconds_after = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

		cout << "dfs run time in nanoseconds: " << nanoseconds_after - nanoseconds_before << endl;
//		printAdjacencyList();
		return nanoseconds_after - nanoseconds_before;
	}
	int graph_with_1000_vertices_test_code(){
		graphGenerator(1000);
//		printAdjacencyList();
//		cout << "edge number: " << GraphEdge << endl;
		// time dfs
		auto nanoseconds_before = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
		dfs();
		auto nanoseconds_after = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
		cout << "dfs run time in nanoseconds: " << nanoseconds_after - nanoseconds_before << endl;
//		printAdjacencyList();
		return nanoseconds_after - nanoseconds_before;
	}
	int graph_with_10000_vertices_test_code(){
			graphGenerator(10000);
//			printAdjacencyList();
			// time dfs
			auto nanoseconds_before = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
			dfs();
			auto nanoseconds_after = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
			cout << "dfs run time in nanoseconds: " << nanoseconds_after - nanoseconds_before << endl;
	//		printAdjacencyList();
			return nanoseconds_after - nanoseconds_before;
		}
	int graph_with_custom_vertices_test_code(int vertices_number){
				graphGenerator(vertices_number);
	//			printAdjacencyList();
				// time dfs
//				auto nanoseconds_before = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
//				auto seconds_before = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
				auto millisec_before = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
				MSTAlgorithm();
//				auto nanoseconds_after = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
//				auto seconds_after = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
				auto millisec_after = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
//				cout << "dfs run time in nanoseconds: " << nanoseconds_after - nanoseconds_before << endl;
				cout << "dfs run time in millisec: " << millisec_after - millisec_before << endl;
		//		printAdjacencyList();
//				return nanoseconds_after - nanoseconds_before;
				return millisec_after - millisec_before;
			}
	void test_1000_times_take_avg_running_time(int graph_vertices_number){
		int totalRunningTime = 0;
		int totalGraphEdge = 0;
		for (int i = 0; i<1000; i++){
			totalRunningTime += graph_with_custom_vertices_test_code(graph_vertices_number);
			cout << '------------------------GraphEdge: ' << GraphEdge << endl;
			totalGraphEdge += GraphEdge;
		}
//		totalRunningTime += graph_with_1000_vertices_test_code();
//		totalRunningTime += graph_with_1000_vertices_test_code();
//		totalRunningTime += g.graph_with_10000_vertices_test_code();
//		totalRunningTime += g.graph_with_10000_vertices_test_code();
	//	cout << "running time: " << g.graph_with_10000_vertices_test_code() << endl;
		cout << "total running time: " << totalRunningTime << endl;
		cout << "average running time: " << totalRunningTime/100 << endl;
		cout << "vertices: " << graph_vertices_number << endl;
		cout << "average edges: " << totalGraphEdge/100 << endl;

	}
	void increasing_graph_size_test_code(){
		// Graph with 500 vertices
		test_1000_times_take_avg_running_time(70);
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
	}
};

//int main(){
//
//	Graph<int, std::pair<int,int>> g;
//	srand((unsigned int)time(NULL));
//	g.increasing_graph_size_test_code();
////	int a = 10339892963094;
////	int b = a*10;
////	unsigned long int c = 10339892963094;
////	unsigned long int d = a*10;
////	cout << a << endl;
////	cout << b << endl;
////	cout << c << endl;
////	cout << d << endl;
//
////	g.graphGenerator(10);
////	g.MSTAlgorithm();
////	g.printAdjacencyList();
////	g.printNewAdjacencyList();
//
////	// to do, write for loop and get avg of the running time.
////	g.printAdjacencyList();
////	g.printNewAdjacencyList();
////	g.cycle_not_exist_test_code();
//	return 0;
//}
//

