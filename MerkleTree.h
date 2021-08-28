#pragma once
#include "RBTree.h"
#include<queue>
#include <cstdlib>
#include <ctime>
#include "cryptlib.h"
#include "sha.h"
#include <unordered_map>

using namespace std;
using namespace CryptoPP;

class MerkleTree{
	private:
		RBTree Tree;
		unordered_map<unsigned int, byte*> map;//faster than regular map which keeps in a bst anyways
		SHA256 hash;//just to create new hash's

	public:
		MerkleTree();
		~MerkleTree();
		void Insert(const byte digest[], const unsigned int key);
		bool Verify(const byte digest[], const unsigned int key);
		void createHash(int key);
		void reHashAncestors(int key);
		byte* calculateProperHash(int key);
		void print();
};

