#include "MerkleTree.h"

using namespace std;

MerkleTree::MerkleTree() {

}

/*This function inserts into the merkle tree taking in the hash has an array of byts and the key as an unsinged int.
If the root is non-existent or is a leaf node, it simply inserts into the red black tree and assings the key which is mapped to a hash
to the hash passed through. Otherwise, it will do the same thing but also insert a key that is 50 less than the input key along with a new 
hash value for it and recalculate the hash for there parents until we get to the root.*/
void MerkleTree::Insert(const byte digest[], const unsigned int key) {
	bool noExtraInsertion = false;//no need for extra insert when inserting to an empty tree
	if (Tree.getRoot() == NULL) {
		noExtraInsertion = true;
	}
	
	byte* newHash = new byte[CryptoPP::SHA256::DIGESTSIZE];
	for (int i = 0; i < CryptoPP::SHA256::DIGESTSIZE; i++) {
		newHash[i] = digest[i];
	}

	Tree.insert(key);
	map.insert(pair<unsigned int, byte*>(key, newHash));//cannot just insert an array into a map as we dont know the size so pass in empty and copy in digest
	
	if (noExtraInsertion == true) {
		return;
	}
	else {
		Tree.insert(key - 50);
		//map.insert(key - 50, {});
		//set the hash value for this key-50 to be the concatenation of the hash's of its two children, then call the hash function on it.

		createHash(key - 50);
		//rhash all of the parents to make it follow the rules
	}
}

/*This method takes in a data node and the key you are querying to see if it exists in the tree.  It will then identify all necessary nodes to retrieve the hashes from, acquire the hashes,
and verify that they match the hash.*/
bool MerkleTree::Verify(const byte digest[], const unsigned int key) {
	for (auto i : map) {
		if (i.first % 100 == 0) {
			reHashAncestors(i.first);
		}
	}

	Node* leaf = Tree.search(key);
	if (leaf == NULL) {
		return false;
	}

	for (int i = 0; i < CryptoPP::SHA256::DIGESTSIZE; i++) {
		if (map[key][i] != digest[i]) {
			return false;
		}
	}

	Node* curr = Tree.getParent(key, Tree.getRoot());

	while (curr != NULL) {
		byte* expectedHash = calculateProperHash(curr->val);
		byte* currsMapping = map[curr->val];
		for (int i = 0; i < CryptoPP::SHA256::DIGESTSIZE; i++) {
			if (map[curr->val][i] != expectedHash[i]) {
				return false;
			}
		}

		curr = Tree.getParent(curr->val, Tree.getRoot());
	}


	return true;
}

/*Takes in the key of the node that needs to have a hash created (called in insert). It then concatenates the byte array from its left child and its 
right child. After the concatenation, it calls the hash function on this new array which will be the hash value for this parent key.
Finally it adds the hash value to the map for the given key. */
void MerkleTree::createHash(int key) {
	byte* digest = calculateProperHash(key);
	byte* mapping = new byte[CryptoPP::SHA256::DIGESTSIZE];
	if (map.find(key) == map.end()) {//the key does not currently have a hash paired with it
		map[key] = mapping;
		for (int i = 0; i < CryptoPP::SHA256::DIGESTSIZE; i++) {
			map[key][i] = digest[i];
		}
	}
	else {//key already has a pair and it is being rehashed
		for (int i = 0; i < CryptoPP::SHA256::DIGESTSIZE; i++) {
			map[key][i] = digest[i];
		}
	}
}

/*This method is used when creating a hash for a newly inserted node as well as to assign a new hash value to an item that has to be rehashed.
Takes in an integer that is the value of the node and creates a pointer to the node holding that value using the search function.
It then creates a new byte array twice the size of a hash because it needs to concatonate its childrens hashes, then concatonates the
two hashes into this 64 length hash. It then passes the new hash, along with an empty 32 size byte array into the calculate digest function
which hashes the concatonated byte array and stores it inside of the empty 32 size array.*/
byte* MerkleTree::calculateProperHash(int key) {
	Node* node = Tree.search(key);

	if (node->left == NULL || node->right == NULL) {
		return map[key];
	}

	byte* newHash = new byte[CryptoPP::SHA256::DIGESTSIZE * 2];//create a byte array that is twice the length of size

	for (int i = 0; i < CryptoPP::SHA256::DIGESTSIZE; i++) {
		newHash[i] = map[node->left->val][i];
		newHash[i + CryptoPP::SHA256::DIGESTSIZE] = map[node->right->val][i];
	}

	byte* digest = new byte[CryptoPP::SHA256::DIGESTSIZE];//remember: every time we use new, it returns a pointer so use * not []
	hash.CalculateDigest(digest, newHash, CryptoPP::SHA256::DIGESTSIZE * 2);

	delete[] newHash;

	return digest;
}

/*This is called when there is a change in the tree that could change parents in the tree meaning the tree would need to rehash some of the nodes.
Takes in a key (going to be called with a leaf node) and trickles up the tree rehashing all of the parents who need new hashes.*/
void MerkleTree::reHashAncestors(int key) {
	if (Tree.search(key) == NULL) {
		return;
	}
	createHash(key);
	Node* parent = Tree.getParent(key, Tree.getRoot());
	if (Tree.getParent(key, Tree.getRoot()) != NULL) {
		reHashAncestors(parent->val);
	}
}

/*Simply gives the merkle tree class the same ability to print as the red black tree*/
void MerkleTree::print() {
	Tree.printTree();
}

MerkleTree::~MerkleTree() {
	for (auto i : map) {
		delete[] i.second;
	}
}
