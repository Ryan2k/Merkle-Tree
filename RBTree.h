#pragma once
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

enum color {RED, BLACK};//red shows as false and black shows as true (red == 0, black == 1)

struct boolBit {
	unsigned char b0 : 1, b1: 1, b2: 1, b3: 1, b4
		: 1, b5: 1, b6: 1, b7: 1;

	friend ostream& operator<<(ostream& os, const boolBit& s) {
		os << (unsigned short)s.b7
		<< ","
		<< (unsigned short)s.b6
		<< ","
		<< (unsigned short)s.b5
		<< ","
		<< (unsigned short)s.b4
		<< ","
		<< (unsigned short)s.b3
		<< ","
		<< (unsigned short)s.b2
		<< ","
		<< (unsigned short)s.b1
		<< ","
		<< (unsigned short)s.b0;
		return os;
	}
};

struct Node {
	int val;
	bool color;
	Node* left = NULL;
	Node* right = NULL;//Have to initialize pointer otherwise we dont know the difference between an initialized and uninitialized pointer
};

class RBTree {
	private:
		Node* root;
		vector<boolBit> bits;
		Node* nullNode = new Node;
		int size;

	public:
		RBTree();
		~RBTree();
		Node* leftRotation(Node* x);  
		Node* rightRotation(Node* x);
		Node* leftParentRotation(Node* x);
		Node* rightParentRotation(Node* x);
		void insert(int value);
		Node* insertBST(Node* newNode, Node* root);
		void applyRules(Node* root, Node* newNode);
		int height(Node* root);
		Node* getParent(int newNode, Node* root);
		void printTree();
		void inorderTraversal(Node* node);
		void reColor(Node* node);
		void deleteTree(Node* root);
		Node* getRoot();
		Node* search(int target);
		Node* searchHelper(int target, Node* node);
};
