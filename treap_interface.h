#ifndef treap_interface_h
#define treap_interface_h

#include <cstdlib>
#include <random>
#include <vector>

typedef long long ll;
typedef unsigned int ui;

using std::vector;

std::mt19937 generator;

ui generateRandom(ui minx, ui maxx) {
    std::uniform_int_distribution<ui> distribution(minx, maxx);
    return distribution(generator);
}

int generateRandomValue() {
    std::uniform_int_distribution<int> distribution(INT_MIN, INT_MAX);
    return distribution(generator);
}

class INextPermutationStruct {

public:
    virtual void insert(int value, size_t index) = 0;
    virtual void assign(int value, size_t index) = 0;
    virtual ll subSegmentSum(size_t left, size_t right) = 0;
    virtual bool nextPermutation(size_t left, size_t right) = 0;
	virtual ui getSize() = 0;
    virtual vector <int> getData() = 0;
};

#endif//treap_interface_h