#ifndef treap_h
#define treap_h

#include <iostream>
#include <cstdlib>
#include <vector>
#include "treap_interface.h"

using std::vector;
using std::swap;
using std::cout;

class treap: public INextPermutationStruct {
    
    struct Node {
        int y, value, left_value, right_value;
        ll sum;
        ui size, decreasing_end_length, increasing_begin_length;
        bool needReverse;
        Node *left, *right;
        
        Node(int y, int value): y(y), value(value), sum(value), size(1), left_value(value), right_value(value), decreasing_end_length(1), needReverse(false), left(NULL), right(NULL) {}

        explicit Node(int value)
		{
			(*this) = Node(rand(), value);
		}
    };
    
    struct threeNode {
        Node *firstNode, *secondNode, *thirdNode;
        threeNode(): firstNode(NULL), secondNode(NULL), thirdNode(NULL) {}
        threeNode(Node *firstNode, Node *secondNode, Node *thirdNode): firstNode(firstNode), secondNode(secondNode), thirdNode(thirdNode) {}
    };
    
    struct twoNode {
        Node *firstNode, *secondNode;
        twoNode(): firstNode(NULL), secondNode(NULL) {}
        twoNode(Node *firstNode, Node *secondNode): firstNode(firstNode), secondNode(secondNode) {}
    };
    
	int getRightValue(Node *node) {
        return node->needReverse? node->left_value : node->right_value;
    }

    int getLeftValue(Node *node) {
        return node->needReverse? node->right_value : node->left_value;
    }

	ui nodeSize(Node *node) {
        return node? node->size : 0;
    }
    
    ll nodeSum(Node *node) {
        return node? node->sum : 0;
    }
    
    ui getDecreasingEndLength(Node *node) {
        return node->needReverse? node->increasing_begin_length : node->decreasing_end_length;
    }
    
    ui getIncreasingBeginLength(Node *node) {
        return node->needReverse? node->decreasing_end_length : node->increasing_begin_length;
    }
    
    void changeReverse(Node *node) {
        if (node)
            node->needReverse ^= 1;
    }
    
    void relax(Node *node) {
        if (!node)
            return;
        if (node->needReverse) {
            swap(node->left, node->right);
            swap(node->decreasing_end_length, node->increasing_begin_length);
            swap(node->left_value, node->right_value);
            changeReverse(node->left);
            changeReverse(node->right);
            node->needReverse = false;
        }
    }
    
    void updateLeftAndRightValue(Node *node) {
        node->left_value = node->left? getLeftValue(node->left) : node->value;
        node->right_value = node->right? getRightValue(node->right) : node->value;
    }
    
    void updateSize(Node *node) {
        node->size = 1 + nodeSize(node->left) + nodeSize(node->right);
    }
    
    void updateSum(Node *node) {
        node->sum = node->value + nodeSum(node->left) + nodeSum(node->right);
    }
    
    void updatePrefixAndSuffixLength(Node *node) {
        if (!node)
            return;
        if (!node->left && !node->right) {
            node->decreasing_end_length = node->increasing_begin_length = 1;
        } 
		else 
			if (!node->left) {
				node->increasing_begin_length = 1;
				node->decreasing_end_length = getDecreasingEndLength(node->right);
				if (getLeftValue(node->right) >= node->value) {
					node->increasing_begin_length += getIncreasingBeginLength(node->right);
				}
				if (nodeSize(node->right) == getDecreasingEndLength(node->right) && node->value >= getLeftValue(node->right)) {
					node->decreasing_end_length++;
				}
			} 
			else 
				if (!node->right) {
				node->increasing_begin_length = getIncreasingBeginLength(node->left);
				node->decreasing_end_length = 1;
					if (getRightValue(node->left) >= node->value) {
					node->decreasing_end_length += getDecreasingEndLength(node->left);
					}
					if (nodeSize(node->left) == getIncreasingBeginLength(node->left) && node->value >= getRightValue(node->left)) {
					node->increasing_begin_length++;
					}
				} 
			else {
				if (getDecreasingEndLength(node->right) < nodeSize(node->right)) {
					node->decreasing_end_length = getDecreasingEndLength(node->right);
				} 
				else {
					if (node->value >= getLeftValue(node->right)) {
						node->decreasing_end_length = nodeSize(node->right) + 1;
						if (node->value <= getRightValue(node->left)) {
							node->decreasing_end_length += getDecreasingEndLength(node->left);
						}
					} 
					else {
                    node->decreasing_end_length = nodeSize(node->right);
					}
				}
				if (getIncreasingBeginLength(node->left) < nodeSize(node->left)) {
					node->increasing_begin_length = getIncreasingBeginLength(node->left);
				} 
				else {
					if (node->value >= getRightValue(node->left)) {
						node->increasing_begin_length = nodeSize(node->left) + 1;
						if (node->value <= getLeftValue(node->right)) {
							node->increasing_begin_length += getIncreasingBeginLength(node->right);
						}
					} 
					else {
                    node->increasing_begin_length = nodeSize(node->left);
					}
				}     
			}
    }
    
    void update(Node *node) {
        if (!node) 
			return;
        relax(node);
        updateLeftAndRightValue(node);
        updateSize(node);
        updateSum(node);
        updatePrefixAndSuffixLength(node);
    }
    
    Node* merge(Node *leftTreap, Node *rightTreap) {
        update(leftTreap);
        update(rightTreap);
        if (!leftTreap || !rightTreap)
            return leftTreap? leftTreap : rightTreap;
        if (leftTreap->y > rightTreap->y) {
            leftTreap->right = merge(leftTreap->right, rightTreap);
            update(leftTreap);
            return leftTreap;
        } else {
            rightTreap->left = merge(leftTreap, rightTreap->left);
            update(rightTreap);
            return rightTreap;
        }
    }

    Node* twoMerges(threeNode &nodes) {
        nodes.firstNode = merge(nodes.firstNode, nodes.secondNode);
        nodes.firstNode = merge(nodes.firstNode, nodes.thirdNode);
        return nodes.firstNode;
    }

    twoNode split(Node *root, ui sizeLeft) {
        update(root);
        if (!sizeLeft)
            return twoNode(NULL, root);
        if (root->size == sizeLeft)
            return twoNode(root, NULL);
        if (nodeSize(root->left) >= sizeLeft) {
            twoNode splitResult = split(root->left, sizeLeft);
            root->left = splitResult.secondNode;
            update(root);
            return twoNode(splitResult.firstNode, root);
        } else {
            twoNode splitResult = split(root->right, sizeLeft - nodeSize(root->left) - 1);
            root->right = splitResult.firstNode;
            update(root);
            return twoNode(root, splitResult.secondNode);
        }
    }
    
    threeNode splitByValueInSuffix(Node *root, int value) {
        if (!root)
            return threeNode();
        if (root->size == 1) {
            if (root->value > value) {
                return threeNode(NULL, root, NULL);
            } else {
                return threeNode(NULL, NULL, root);
            }
        }
        if (root->value > value) {
            threeNode rightPart = splitByValueInSuffix(root->right, value);
            if (rightPart.secondNode) {
                root->right = rightPart.firstNode;
                update(root);
                return threeNode(root, rightPart.secondNode, rightPart.thirdNode);
            } else {
                return twoSplits(root, nodeSize(root->left), 1);
            }
        } else {
            threeNode leftPart = splitByValueInSuffix(root->left, value);
            root->left = leftPart.thirdNode;
            update(root);
            return threeNode(leftPart.firstNode, leftPart.secondNode, root);
        }
    }
    
    threeNode twoSplits(Node *root, ui sizeLeft, ui sizeMiddle) {
        twoNode firstSplitResult = split(root, sizeLeft);
        twoNode secondSplitResult = split(firstSplitResult.secondNode, sizeMiddle);
        return threeNode(firstSplitResult.firstNode, secondSplitResult.firstNode, secondSplitResult.secondNode);
    }
     
    int getValue(Node *root, ui index) {
        threeNode separareParts = twoSplits(root, index, 1);
        int result = separareParts.secondNode->value;
        root = twoMerges(separareParts);
        return result;
    }
    
    void print(Node *root) {
        update(root);
        if (root) {
            print(root->left);
            cout << root->value << " ";
            print(root->right);
        }
    }
    
    bool nextPermutation(Node *root) {
        if (!root)
            return true;
        if (root->decreasing_end_length == root->size) {
            root->needReverse ^= 1;
            update(root);
            return false;
        }
        threeNode separateParts = twoSplits(root, root->size - root->decreasing_end_length - 1, 1);
        threeNode splittedSuffix = splitByValueInSuffix(separateParts.thirdNode, separateParts.secondNode->value);
        swap(separateParts.secondNode, splittedSuffix.secondNode);
        separateParts.thirdNode = twoMerges(splittedSuffix);
        separateParts.thirdNode->needReverse ^= 1;
        root = twoMerges(separateParts);
        return true;
    }
    
    vector <int> getData(Node *root) {
        update(root);
        if (root) {
            getData(root->left);
            data.push_back(root->value);
            getData(root->right);
        }
        return data;
    }
    
    Node *root;
    vector <int> data;
    
public:
    
	treap()
	{ 
		root = NULL;
	}

    void reverse(size_t left, size_t right) {
        threeNode separateParts = twoSplits(root, left, right - left);
        separateParts.secondNode->needReverse ^= 1;
        root = twoMerges(separateParts);
    }
    
    void insert(int value, size_t index) {
        Node *newElement = new Node(value);
        twoNode previousTreap = split(root, index);
        threeNode newTreap = threeNode(previousTreap.firstNode, newElement, previousTreap.secondNode);
        root = twoMerges(newTreap);
    }
    
    void assign(int value, size_t index) {
        Node *newElement = new Node(value);
        threeNode previousTreap = twoSplits(root, index, 1);
        threeNode newTreap = threeNode(previousTreap.firstNode, newElement, previousTreap.thirdNode);
        root = twoMerges(newTreap);
    }
    
    ll subSegmentSum(size_t left, size_t right) {
        threeNode separateParts = twoSplits(root, left, right - left);
        ll result = separateParts.secondNode->sum;
        root = twoMerges(separateParts);
        return result;
    }
    
    bool nextPermutation(size_t left, size_t right) {
        threeNode separateParts = twoSplits(root, left, right - left);
        bool result = nextPermutation(separateParts.secondNode);
        root = twoMerges(separateParts);
        return result;
    }
    
	ui getSize() {
        return root->size;
    }

	int getValue(size_t index) {
		return getValue(root, index);
    }
  
    vector <int> getData() {
        data.clear();
        return getData(root);
    }
};

#endif//treap_h