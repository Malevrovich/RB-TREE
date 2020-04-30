#include "tree.h"
#include "profile.h"
#include <iostream>
#include <map>
#include <random>
#include <vector>

using namespace std;

#define TEST_SIZE 1000000

int main(){
	vector<uint64_t> test(TEST_SIZE);

	for(size_t i = 0; i < TEST_SIZE; i++){
		test[i] = random() % (TEST_SIZE);
	}

	map<uint64_t, uint64_t> map_storage;
	Tree<uint64_t, uint64_t> tree_storage;

	{
		LOG_DURATION("std::map");
		for(size_t i = 0; i < TEST_SIZE; i++){
			map_storage[test[i]] = test[i];
		}
	}
	{
		LOG_DURATION("tree");
		for(size_t i = 0; i < TEST_SIZE; i++){
			tree_storage.Insert(test[i], test[i]);
		}
	}
	return 0;
}
