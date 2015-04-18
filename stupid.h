#ifndef stupid_h
#define stupid_h

#include <vector>
#include "treap_interface.h"
#include <cstdlib>
#include <algorithm>
#include <vector>

using std::vector;

class stupidStruct: public INextPermutationStruct {
  
private:

    vector <int> data;
    
public:
    
    void assign(int value, size_t index) {
        data[index] = value;
    }

	 void insert(int value, size_t index) {
        data.insert(data.begin() + index, value);
    }
    
    ll subSegmentSum(size_t left, size_t right) {
        ll result = 0;
        for (size_t i = left; i < right; ++i) {
            result += data[i];
        }
        return result;
    }
    
    bool nextPermutation(size_t left, size_t right) {
        return next_permutation(data.begin() + left, data.begin() + right);
    }
    
    vector <int> getData() {
        return data;
    }
    
    void print() {
        for (int i = 0; i < data.size(); ++i) {
            std::cout << data[i] << " ";
        }
    }
    
    ui getSize() {
        return data.size();
    }
};

#endif//stupid_h