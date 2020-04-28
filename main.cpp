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

	{
		LOG_DURATION("std::map");
		map<uint64_t, uint64_t> storage;
		for(size_t i = 0; i < TEST_SIZE; i++){
			storage[test[i]] = test[i];
		}
	}
	{
		LOG_DURATION("tree");
		Tree<uint64_t, uint64_t> storage;
		for(size_t i = 0; i < TEST_SIZE; i++){
			storage.Insert(test[i], test[i]);
		}
	}
	return 0;
}
