///*
// * CycleFinding.cpp
// *
// *  Created on: Sep 27, 2021
// *      Author: Zihang Huang
// */
//
//#include<iostream>
//#include<stdlib.h>
//#include <ctime>
//#include<map>
//#include<list>
//#include <algorithm>
//#include <typeinfo>
//using namespace std;
//
//// Packages for timing dfs run on increasing graph size
//#include <chrono>
//using std::cout; using std::endl;
//using std::chrono::duration_cast;
//using std::chrono::milliseconds;
//using std::chrono::nanoseconds;
//using std::chrono::seconds;
//using std::chrono::system_clock;
//
//
//template<typename T>
//class Graph{
//
//	map<T,list<T> > adjList;
//	map<T,bool> Discovered;
//	map<T,bool> AdjListInProgress;
//	map<T,T> Parent;
//	list<T> Cycle;
//	bool terminate_dfs = false;
//	int GraphEdge;
//
//public:
//	Graph(){
//
//	}
//	void addEdge(T u, T v,bool undirected=true){
//
//		bool edge_exist = false;
//		// Check if edge already exists
//		for(T neighbour: adjList[u])
//		{
//			if(neighbour == v)
//			{
////				cout << "edge (" << u << ", " << v << ") already exist in adjList\n";
//				edge_exist = true;
//			}
//		}
//
//		// Check if edge doesn't exists and the edge is not connecting itself, add edge to graph
//		if (edge_exist == false && u != v)
//		{
//			adjList[u].push_back(v);
//			if(undirected){
//				adjList[v].push_back(u);
//			}
//		}
//	}
//
//	void graphGenerator(int userInput){
//		// add 1 as 0 is counted
//		int GraphVertex = userInput + 1;
//		adjList.clear();
//		Discovered.clear();
//		AdjListInProgress.clear();
//		Parent.clear();
//		Cycle.clear();
//		terminate_dfs = false;
//		GraphEdge = 0;
////		adjList[NOEdge].push;
////		for(T neighbour: adjList[node]){
////							//Two things can happen
////							//The current node is not visited but its further branch is leading to a cycle
////							if((!visited[neighbour] && isCyclicHelper(neighbour,visited,inStack))||inStack[neighbour]){
////								return true;
////							}
////
////					}
//
//		cout << "rand()%GraphVertex: " << rand()%GraphVertex << endl;
//		while (adjList.size() < GraphVertex) {
//			addEdge(rand()%GraphVertex,rand()%GraphVertex);
//			GraphEdge += 1;
//		}
//		cout << "edge number: " << GraphEdge << endl;
////		addEdge(0,1);
////		addEdge(0,1);
////		addEdge(0,1);
////		addEdge(0,1);
////		addEdge(1,2);
////		addEdge(0,4);
////		addEdge(2,4);
////		addEdge(2,3);
////		addEdge(3,4);
////		addEdge(3,5);
//	}
//
//	void printAdjacencyList(){
//
//		cout << "printAdjacencyList:" << endl;
//		//Iterate over the map
//		for(auto i:adjList){
//			cout<<i.first<<"->";
//			//i.second is LL
//
//			for(T entry:i.second){
//				cout<<entry<<",";
//			}
//			cout << endl;
//		}
//		cout << endl;
//	}
//
//	void GetCycle(T Ancestor, T Descendant){
//		Cycle.push_back(Ancestor);
//		Cycle.push_back(Descendant);
////		cout << "Descendant: " << Descendant << endl;
////		cout << "Parent[Descendant]: " << Parent[Descendant] << endl;
////		cout << "Ancestor: " << Ancestor << endl;
//		while (Parent[Descendant] != Ancestor){
//			Descendant = Parent[Descendant];
//			Cycle.push_back(Descendant);
////			cout << "Parent[Descendant]: " << Parent[Descendant] << endl;
////			cout << "Ancestor: " << Ancestor << endl;
//		}
//
//		Cycle.push_back(Parent[Descendant]);
//	}
//
//	void dfsDiscover(T node){
//		//Mark the first seen node as discovered
//		Discovered[node] = true;
//		AdjListInProgress[node] = true;
//		//Try to find out a node which is neigbour of current node and not yet visited
////		cout << "current node: " << node << endl;
//		for(T neighbour: adjList[node]){
////			cout << "Parent[node]: " << Parent[node] << endl;
////			cout << "neighbour: " << neighbour << endl;
////			cout << "AdjListInProgress[neighbour]: " << AdjListInProgress[neighbour] << endl;
////			cout << "Parent[neighbour]: " << Parent[neighbour] << endl;
//			if(AdjListInProgress[neighbour] && Parent[node] != neighbour){
////				cout << "debug ---- 1" << endl;
////				cout << "Cycle exist." << endl;
////				for(auto i:Parent){
////					cout << "Parent i: "<< i.first <<  "Parent[i]: " << i.second << endl;
////				}
//
//				terminate_dfs = true;
//				GetCycle(neighbour, node);
//				break;
//			}
//			else if (!Discovered[neighbour]){
//				Parent[neighbour] = node;
//				dfsDiscover(neighbour);
//				if (terminate_dfs){
//					break;
//				}
//			}
//		}
//		AdjListInProgress[node] = false;
//	}
//
//	void dfs(){
//
//		//Go through all nodes (vertices ) in graph G
//		for(auto i:adjList){
//			T node = i.first;
//			if(!Discovered[node]){
//				Parent[node] = NULL;
//				dfsDiscover(node);
//			}
//			if (terminate_dfs){
//				break;
//			}
//		}
//		if (!terminate_dfs){
//			cout << "Cycle doesn't exist." << endl;
//		}
//		else{
//			cout << "Cycle: " << endl;
//			for(auto i:Cycle){
//				cout << i << "-";
//			}
//			cout << "end" << endl;
//		}
//	}
//	void cycle_exist_test_code(){
//		// Given a graph with 1 cycle and see if algorithm can detect the existing cycle: 1-7-8-1-end
//		addEdge(0,1);
//		addEdge(0,2);
//		addEdge(2,3);
//		addEdge(2,6);
//		addEdge(6,4);
//		addEdge(6,5);
//		addEdge(1,7);
//		addEdge(1,8); // Cycle edge
//		addEdge(8,9); // Cycle edge
//		addEdge(7,8); // Cycle edge
//		dfs();
//		printAdjacencyList();
//	}
//
//	void cycle_not_exist_test_code(){
//		// Given a graph with no cycle and see if algorithm can tell there is no cycle exists
//		addEdge(0,1);
//		addEdge(0,2);
//		addEdge(2,3);
//		addEdge(2,6);
//		addEdge(6,4);
//		addEdge(6,5);
//		addEdge(1,7);
//		addEdge(1,8);
//		addEdge(8,9);
//		dfs();
//		printAdjacencyList();
//	}
//
//	int graph_with_100_vertices_test_code(){
//		graphGenerator(100);
////		printAdjacencyList();
//		// time dfs
//		auto nanoseconds_since_epoch = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
//		auto sec_since_epoch = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
//		auto millisec_before = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
//		auto nanoseconds_before = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
//		dfs();
//		auto millisec_after = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
//		auto sec_since_epoch_after = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
//		auto nanoseconds_after = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
//
//		cout << "dfs run time in nanoseconds: " << nanoseconds_after - nanoseconds_before << endl;
////		printAdjacencyList();
//		return nanoseconds_after - nanoseconds_before;
//	}
//	int graph_with_1000_vertices_test_code(){
//		graphGenerator(1000);
////		printAdjacencyList();
////		cout << "edge number: " << GraphEdge << endl;
//		// time dfs
//		auto nanoseconds_before = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
//		dfs();
//		auto nanoseconds_after = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
//		cout << "dfs run time in nanoseconds: " << nanoseconds_after - nanoseconds_before << endl;
////		printAdjacencyList();
//		return nanoseconds_after - nanoseconds_before;
//	}
//	int graph_with_10000_vertices_test_code(){
//			graphGenerator(10000);
////			printAdjacencyList();
//			// time dfs
//			auto nanoseconds_before = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
//			dfs();
//			auto nanoseconds_after = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
//			cout << "dfs run time in nanoseconds: " << nanoseconds_after - nanoseconds_before << endl;
//	//		printAdjacencyList();
//			return nanoseconds_after - nanoseconds_before;
//		}
//	int graph_with_custom_vertices_test_code(int vertices_number){
//				graphGenerator(vertices_number);
//	//			printAdjacencyList();
//				// time dfs
//				auto nanoseconds_before = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
//				dfs();
//				auto nanoseconds_after = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
//				cout << "dfs run time in nanoseconds: " << nanoseconds_after - nanoseconds_before << endl;
//		//		printAdjacencyList();
//				return nanoseconds_after - nanoseconds_before;
//			}
//	void test_1000_times_take_avg_running_time(int graph_vertices_number){
//		int totalRunningTime = 0;
//		int totalGraphEdge = 0;
//		for (int i = 0; i<1000; i++){
//			totalRunningTime += graph_with_custom_vertices_test_code(graph_vertices_number);
//			totalGraphEdge += GraphEdge;
//		}
////		totalRunningTime += graph_with_1000_vertices_test_code();
////		totalRunningTime += graph_with_1000_vertices_test_code();
////		totalRunningTime += g.graph_with_10000_vertices_test_code();
////		totalRunningTime += g.graph_with_10000_vertices_test_code();
//	//	cout << "running time: " << g.graph_with_10000_vertices_test_code() << endl;
//		cout << "total running time: " << totalRunningTime << endl;
//		cout << "average running time: " << totalRunningTime/100 << endl;
//		cout << "vertices: 1000" << endl;
//		cout << "average edges: " << totalGraphEdge/100 << endl;
//
//	}
//	void increasing_graph_size_test_code(){
//		// Graph with 500 vertices
////		test_1000_times_take_avg_running_time(500);
//		// Graph with 1500 vertices
////		test_1000_times_take_avg_running_time(1500);
//		// Graph with 2500 vertices
////		test_1000_times_take_avg_running_time(2500);
//		// Graph with 3500 vertices
////		test_1000_times_take_avg_running_time(3500);
//		// Graph with 4000 vertices
////		test_1000_times_take_avg_running_time(4000);
//		// Graph with 5500 vertices
////		test_1000_times_take_avg_running_time(5500);
//		// Graph with 6000 vertices
////		test_1000_times_take_avg_running_time(6000);
//		// Graph with 7500 vertices
////		test_1000_times_take_avg_running_time(7500);
//		// Graph with 9000 vertices
//		test_1000_times_take_avg_running_time(9000);
//	}
//};
//#include<tuple>
//
//int main(){
//
//	Graph<int> g;
//	tuple<T,int> v = {1,2};
//	int EdgeCost = std::get<2>(v);
////	g.dfs(0);
////	int n;
////	n= 7 + rand()%6;
////	g.graphGenerator(9);
////	g.test_code1();
//	srand((unsigned int)time(NULL));
//	g.increasing_graph_size_test_code();
//
//	// to do, write for loop and get avg of the running time.
////	cout << 1000/100 << endl;
//
//
//
//return 0;
//}
