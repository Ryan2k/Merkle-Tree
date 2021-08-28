#include "RBTree.h"

/*Constructor for a Red Black tree object. Only two private data members. Root is the root of the tree which needs to be initialized to NULL before
any input, and bits is a vector of boolBits I am using to keep track of whether a node is red or black. The reason for this is that it takes 8 bits (one byte)
to declare a new boolean, however, a boolean only needs one bit to be stored.*/
RBTree::RBTree() {
	this->size = 0;
	root = NULL;
}

/*x is the node we are trying to rotate. In a left rotation, there are three nodes we have to worry about:
x (the node we are going to rotate), y (x's right child), and z(y's left child). This rotation will always consist of
y becoming the parent and since we know x is less than y and z is both less than y and greater than x, we set z as y's
new left child and x as z's new right child. Then we return y which is the new root of this subtree*/
Node* RBTree::leftRotation(Node* x) {
	Node* y = x->right;
	Node* z = NULL;
	Node* grandParent = getParent(x->val, root);

	if (y->left != NULL) {
		z = y->left;
		y->left = x;
		x->right = z;
	}
	else {
		y->left = x;
		x->right = NULL;
	}

	
	if (grandParent == NULL) {
		root = y;
	}
	else {
		grandParent->right = y;
	}

	return y;
}

/*A right rotation is similar to the left had except y and z are defined as x's left child and y's right child respectively.
This turn is opposite to the left rotation so we always set x to be y's new right child and z to be x's new left.*/
Node* RBTree::rightRotation(Node* x) {
	Node* y = x->left;
	Node* z = NULL;
	Node* grandParent = getParent(x->val, root);

	if (y->right != NULL) {
		z = y->right;
		y->right = x;
		x->left = z;
	}
	else {
		y->right = x;
		x->left = NULL;
	}

	if (grandParent == NULL) {
		root = y;
	}
	else {
		grandParent->left = y;
	}

	return y;
}

/*Added an extra function to rotate left via the parent passed in as x. Needed to make a seperate function as a rotation on the parent 
requires the grandparents pointers to re attach to a new node. Called in left right case*/
Node* RBTree::leftParentRotation(Node* x) {
	Node* y = x->right;
	x->right = y->left;
	Node* grandParent = getParent(x->val, root);

	if (grandParent == NULL) {
		root = y;
	}
	else if (x->val = grandParent->left->val) {
		grandParent->left = y;
	}
	else {
		grandParent->right = y;
	}

	y->left = x;
	return y;
	


}

/*Same as function above however for right rotation. Called in right left case*/
Node* RBTree::rightParentRotation(Node* x) {
	Node* y = x->left;
	x->left = y->right;
	Node* grandParent = getParent(x->val, root);

	if (grandParent == NULL) {
		root = y;
	}
	else if (grandParent->left != NULL && x->val == grandParent->left->val) {
		grandParent->left = y;
	}
	else {
		grandParent->right = y;
	}

	y->right = x;
	return y;
}

/*getParent takes in newNode which is the node we are trying tho extract the parent from, as well as the root of the tree.
Simply iterating through the tree until either the value of the left child of a node we are at, or the right child of the node
we are at matches the value of the input node. That means we have reached its parent and return it.*/
Node* RBTree::getParent(int childVal, Node* root) {
	if (root == NULL || childVal == root->val) {
		return NULL;
	}

	Node* parent = NULL;

	while (root != NULL) {
		if (root->val < childVal) {
			parent = root;
			root = root->right;
		}
		else if (root->val > childVal) {
			parent = root;
			root = root->left;
		}
		else {
			break;
		}
	}

	return parent;
}

/*This is the same insert we would use for a regular binary search tree which i am going to use as the first step in the RBTree insert
once we reach the leaf note (null) we return the newNode and if there is a node, we set its right or left child respectively using the
rules of a bst to recursively change the child until we have inserted the leaf using BST Rules (Rules are comment by each move and z is newNode)*/
Node* RBTree::insertBST(Node* newNode, Node* root) {
	if (root == NULL) {
		return newNode;
	}

	if (newNode->val > root->val) {
		root->right = insertBST(newNode, root->right);
	}

	if (newNode->val < root->val) {
		root->left = insertBST(newNode, root->left);
	}

	return root;
}

/*There are 6 different rules to follow when inserting into a Red Black tree. Each of these rules is stated in comments throughout the method with both
the case and the solution. This is not where I actually insert, rather I preform a regular bst insertion, then call this method to fix the violation
to make the tree again follow the rules of a red black tree.*/
void RBTree::applyRules(Node* root, Node* newNode) {
	Node* parent = getParent(newNode->val, root);
	Node* grandParent = NULL;

	while (newNode != NULL && parent != NULL && parent->color == RED && newNode->color == RED) {
		parent = getParent(newNode->val, root);
		grandParent = getParent(parent->val, root);

		//Rule Set 1) Parent of the new node is the left child of its grandparent
		if (grandParent->left != NULL && grandParent->left->val == parent->val) {
			Node* Uncle = grandParent->right;

			//First case: If the uncle is exists and is red, recolor grandparent, parent, and uncle
			if (Uncle != NULL && Uncle->color == RED) {
				grandParent->color = RED;
				parent->color = BLACK;
				Uncle->color = BLACK;
				Node* p = getParent(newNode->val, root);
				newNode = getParent(p->val, root);
			}
			//Second case: the new node is its parents right child, rotate its parent left
			else if (parent->right != NULL && parent->right->val == newNode->val) {
				leftParentRotation(parent);
				newNode = parent;
				
			}
			//Third case: the new node is its parents left child, rotate grandparent right & Recolor both the grandparent and parent
			else {
				rightRotation(grandParent);
				reColor(parent);
				reColor(grandParent);
				Node* p = getParent(newNode->val, root);
				newNode = getParent(p->val, root);
			}
		}
		//Rule Set 2) Parent of the new node is the right child of its grandparent
		else {
			Node* Uncle = grandParent->left;

			//First case: exactly the same, if the uncle exists and is red, just recolor the grandparent, parent, and uncle
			if (Uncle != NULL && Uncle->color == RED) {
				reColor(grandParent);
				reColor(parent);
				reColor(Uncle);
				Node* p = getParent(newNode->val, root);
				newNode = getParent(p->val, root);
			}
			//Second case: if the new node is the left child of its parent forming a triangle, rotate parent right to make newnode the parent
			else if (parent-> left != NULL && parent->left->val == newNode->val) {
				rightParentRotation(parent);
				newNode = parent;
			}
			//Third case: if the new node is the right child of its parent, rotate the grandparent left and recolor both the parent and grandparent
			else {
				leftRotation(grandParent);
				reColor(parent);
				reColor(grandParent);
				Node* p = getParent(newNode->val, root);
				newNode = getParent(p->val, root);
			}
		}

		//make sure that the root is still black as if it is the grandparent it might turn red due to one of these rules
		this->root->color = BLACK;
		if (newNode != NULL) {
			parent = getParent(newNode->val, root);
		}
	}
	/*Rule 1) if z is the root, paint in black and return
	if (root == NULL) {
		newNode->color = BLACK;
		this->root = newNode;
		return;
	}

	//The Remaining 3 cases involve identifying the color of z's unlce and potentially rotating the parent and grandparent
	Node* Parent = getParent(newNode, root);
	Node* grandParent = getParent(Parent, root);
	
	if (grandParent == NULL) {
		return;
	}

	Node* Uncle = NULL;
	if (grandParent->right->val == Parent->val) {
		Uncle = grandParent->left;
	}
	else {
		Uncle = grandParent->right;
	}

	//If uncle is NULL, then it follows same as case 4 because it is linear
	if (Uncle == NULL) {
		if (grandParent->val == this->root->val) {

		}
		else if (grandParent->color == BLACK) {
			grandParent->color = RED;
		}
		else {
			grandParent->color = BLACK;
		}
		if (Parent->color == BLACK) {
			Parent->color = RED;
		}
		else {
			Parent->color = BLACK;
		}

		if (Parent->right->val == newNode->val) {
			leftRotation(grandParent);
		}
		else {
			rightRotation(grandParent);
		}
		root->color = BLACK;

		return;
	}

	//Rule 2) If z's unclue is red, recolor z's parent, grandparent, and uncle
	if (Uncle->color == RED) {
		Uncle->color = BLACK;
		if (grandParent->color == BLACK) {
			grandParent->color = RED;
		}
		else {
			grandParent->color = BLACK;
		}
		if (Parent->color == BLACK) {
			Parent->color = RED;
		}
		else {
			Parent->color = BLACK;
		}
		root->color = BLACK;

		return;
	}

	//Rule 3) If z's Unlce is Black and they form a Triangle, rotate its parent in the opposite direction of z so z becomes the parent and its parent is now its child
	//Case 1: If the parent is the right child of the grandparent and newNode is the parents left child, rotate parent right
	if (grandParent->right->val == Parent->val && Parent->left->val == newNode->val) {
		rightRotation(Parent);
		return;
	}
	//Case 2: If the parent is the left child of the grandparent and newNode is the parents right child, rotate parent left
	if (grandParent->left->val == Parent->val && Parent->right->val == newNode->val) {
		rightRotation(Parent);
		return;
	}

	//Rule 4) If z's Uncle is Black and the grandparent, parent, and child preform a line, rotate the grandparent in the opposite direction of z so the parent takes the spot of the grandparent
	//the grandparent and the parent must also be re colored so I will do that first.
	if (grandParent->val == this->root->val) {

	}
	if (grandParent->color == BLACK) {
		grandParent->color = RED;
	}
	else {
		grandParent->color = BLACK;
	}
	if (Parent->color == BLACK) {
		Parent->color = RED;
	}
	else {
		Parent->color = BLACK;
	}

	//if z is right child rotate grandparent left
	if (Parent->right->val == newNode->val) {
		leftRotation(grandParent);
	}
	else {
		rightRotation(grandParent);
	}
	root->color = BLACK;

	//not sure if all this happens in this order or if i need more if statements*/
}

/*This is the actual Red Black Tree insert function. We take in an integer which the newest leaf is going to hold and insert it into the BST
Then we call the applyRules function which is going to apply whichever of the 4 rules needs to be fixed (see comment above applyRules)*/
void RBTree::insert(int value) {
	this->size++;
	Node* newNode = new Node;
	newNode->val = value;
	newNode->color = RED;
	if (root == NULL) {
		newNode->color = BLACK;
		root = newNode;
		return;
	}
	insertBST(newNode, root);
	applyRules(root, newNode);
	//cout << "value: " << newNode->val << ", height: " << height(root) - height(newNode) << ", color: " << newNode->color << endl;
}

/*This method takes in a node we are trying to find the height of in a tree. This height is the height from the bottom.
Base case is if the node is null then return zero, otherwise return the max of the left and right height plus one recursing through*/
int RBTree::height(Node* root) {
	if (root == NULL) {
		return 0;
	}

	return std::max(height(root->left), height(root->right)) + 1;
}

/*Take in a node as input and simply changes the current color to the only other possible color
if it is currently black it turns red, and visa versa*/
void RBTree::reColor(Node* node) {
	if (node == NULL) {
		return;
	}
	if (node->color == BLACK) {
		node->color = RED;
	}
	else {
		node->color = BLACK;
	}
}

/*Prints every node in the tree using the cout statement from the inorder traversal. Uses the root node which is kept as a private data member*/
void RBTree::printTree() {
	inorderTraversal(root);
}

/*Standard inorder traversal. Takes in a node and returns at the base case where the node is null. Otherwise, it recurses all the way to the left
most node, prints the value, then recurses right. In a BST this would print out the tree in ascending order*/
void RBTree::inorderTraversal(Node* node) {
	if (node == NULL) {
		return;
	}

	inorderTraversal(node->left);
	cout << "value: " << node->val << ", height: " << height(node) << ", color: " << node->color << endl;
	inorderTraversal(node->right);
}

/*Destructor, calls deleteTree on root which will delete the memory for each node in tree, then sets root back to null*/
RBTree::~RBTree() {
	deleteTree(root);
	this->size = 0;
}

/*Helper for destructor. Takes in a node (we drop the actual root in the destructor) then recurses down the tree deleting every
pointer to a node in the tree. Base case is if there are no more children, then we return.*/
void RBTree::deleteTree(Node* root) {
	if (root == NULL) {
		return;
	}

	deleteTree(root->right);
	deleteTree(root->left);
	delete root;
	root = NULL;
}

/*getter for the root node used in the insert method of the merkletree*/
Node* RBTree::getRoot() {
	return this->root;
}

/*returns node with given value as parameter, if it doestn exist, returns null. Does this by calling helper starting at root*/
Node* RBTree::search(int target) {
	return searchHelper(target, root);
}

/*Simple search method used for a BST. Takes in a target value for a node and trickles down the tree until we have reached null. If the 
target doesnt exist, null is returned. Called in search function with root as initial node passed through.*/
Node* RBTree::searchHelper(int target, Node* node) {
	if (node == NULL || node->val == target) {
		return node;
	}

	if (node->val < target) {
		return searchHelper(target, node->right);
	}

	return searchHelper(target, node->left);
}