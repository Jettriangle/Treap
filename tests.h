#ifndef tests_h
#define tests_h

#include "treap_interface.h"
#include "treap.h"
#include "stupid.h"

enum EnumTestType {
    INSERT,
    ASSIGN,
    SUBSEGMENT_SUM,
    NEXT_PERMUTATION
};

class Test {

private:
    struct test {
        EnumTestType testType;
        ui left, right;
        int value;
        test(EnumTestType testType, ui left, ui right, int value): testType(testType), left(left), right(right), value(value) {}
    };

    stupidStruct stupid;
    treap treap;
    
    vector <test> generateTests(ui amount, ui left, ui right) {
        vector <test> result;
        result.push_back(test(INSERT, 0, 0, generateRandom(left, right)));
        ui size = 1;
        ui leftIndex, rightIndex, index;
        for (int i = 1; i < amount; ++i) {
            ui diceRoll = generateRandom(0, 3);
            switch (diceRoll) {
                case INSERT:
                    index = generateRandom(0, size);
                    result.push_back(test(INSERT, index, index, generateRandom(left, right)));
                    size++;
                    break;
                case ASSIGN:
                    index = generateRandom(0, size - 1);
                    result.push_back(test(ASSIGN, index, index, generateRandomValue()));
                    break;
                case SUBSEGMENT_SUM:
                    leftIndex = generateRandom(0, size);
                    rightIndex = generateRandom(0, size);
                    if (leftIndex > rightIndex) {
                        swap(leftIndex, rightIndex);
                    }
                    if (leftIndex != rightIndex)
                        result.push_back(test(SUBSEGMENT_SUM, leftIndex, rightIndex, 0));
                    break;
                case NEXT_PERMUTATION:
                    leftIndex = generateRandom(0, size);
                    rightIndex = generateRandom(0, size);
                    if (leftIndex > rightIndex) {
                        swap(leftIndex, rightIndex);
                    }
                    if (leftIndex != rightIndex)
                        result.push_back(test(NEXT_PERMUTATION, leftIndex, rightIndex, 0));
                    break;
                default:
                    break;
            }
        }
        return result;
    }
    
    vector <ll> resultTest(INextPermutationStruct &realisation, vector <test> &tests) {
        vector <ll> result;
        for (int i = 0; i < tests.size(); ++i) {
            switch (tests[i].testType) {
                case INSERT:
                    realisation.insert(tests[i].value, tests[i].left);
                    break;
                case ASSIGN:
                    realisation.assign(tests[i].value, tests[i].left);
                    break;
                case SUBSEGMENT_SUM:
                    result.push_back(realisation.subSegmentSum(tests[i].left, tests[i].right));
                    break;
                case NEXT_PERMUTATION:
                    result.push_back(realisation.nextPermutation(tests[i].left, tests[i].right));
                    break;
            }
        }
        return result;
    }
    
public:
    bool Check(size_t count, size_t left, size_t right) {
        vector <test> tests = generateTests(count, left, right);
        vector <ll> resultStupid = resultTest(stupid, tests);
        vector <ll> resultTreap = resultTest(treap, tests);
        for (int i = 0; i < resultStupid.size(); ++i) {
            if (resultStupid[i] != resultTreap[i]) {
                cout << "Error: " << i << " " << resultStupid[i] << " " << resultTreap[i] << "\n";
            }
        }
        return resultStupid == resultTreap;
    }
};

#endif//tests_h