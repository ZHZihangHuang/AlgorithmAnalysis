#include <cstdlib>
#include <iostream>

#include "List.h"

using namespace std;

int main (int argc, char** argv){
	const clock_t begin_time = clock();
	int input_size_n = 100;
	int current_woman_partner [input_size_n];
	// use -1 as null since it is not easy to store different data type in c++
	for (int i = 0; i < input_size_n; i++){
		current_woman_partner[i] = -1;
	}
	int woman_preference_ranking[input_size_n][input_size_n];
	for (int woman_index = 0; woman_index < input_size_n; woman_index++){
		for (int preference_index = 0; preference_index < input_size_n; preference_index++){
			woman_preference_ranking[woman_index][preference_index] = rand() % input_size_n + 1;
			//std::cout << "woman_index: " << woman_index << endl;
			//std::cout << "preference_index: " << preference_index << endl;
			//std::cout << woman_preference_ranking[woman_index][preference_index] << endl;
		}
		//std::cout << "outside nest for loop woman_index: " << woman_index << endl;
		//std::cout << endl;
	}
	int next_propose_target_array[input_size_n];
	List free_man;
	for (int i = 0; i < input_size_n; i++){
		free_man.AddNode(i);
	}
	List free_woman;
	for (int i = 0; i < input_size_n; i++){
		free_woman.AddNode(i);
	}
	//free_man.PrintList();
	for (int woman_index = 0; woman_index < input_size_n; woman_index++){
		for (int preference_index = 0; preference_index < input_size_n; preference_index++){
			std::cout << woman_preference_ranking[woman_index][preference_index] << " ";
		}
		std::cout << std::endl;
		//std::cout << rand() % input_size_n + 1 << std::endl;
		//std::cout << woman_preference_ranking[print_woman_index][print_woman_index] << endl;
	}
	std::cout << "Task takes " << float(clock() - begin_time)/1000 << "s to complete." << std::endl; 
	return 0;
}
