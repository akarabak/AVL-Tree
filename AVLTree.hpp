#ifndef AVLTree_hpp
#define AVLTree_hpp

#include <iostream>
#include <string>

template<class K, class V>
struct AVLNode
{
	K key;
	V value;
	int height;
	AVLNode<K, V>* left;
	AVLNode<K, V>* right;
};

template<class K, class V>
class AVLTree
{
public:
	AVLTree();
	AVLTree(V value);
	~AVLTree();
	V operator[] (const K& key); //same as lookup
	V lookup(const K& key);
	void insert(const K& key, const V& value);
	void print();
	void remove(const K& key);
	void check(); //used to check if AVL is balanced and sorted properly

private:
	AVLNode<K, V>*& lookup(const K& key, AVLNode<K, V>*& node);
	AVLNode<K, V>* insert(const K& key, const V& value, AVLNode<K, V>* node);
	void print(AVLNode<K, V>* leaf);
	
	int balanceFactor(AVLNode<K, V>* node); //calculates balance factor on node
	void rotate(AVLNode<K, V>*& node); //rotates node using rightRight or leftLeft
	void fixBalance(AVLNode<K, V>*& node); //rotates tree recursively
	AVLNode<K, V>* rightRight(AVLNode<K, V>* node);  //right right rotation
	AVLNode<K, V>* leftLeft(AVLNode<K, V>* node); //left left rotation
	void fixHeight(AVLNode<K, V>*& node); //fixes height on nodes recursively
	int max(AVLNode<K, V>* node); //return biggest height of node's children

	void memClean(AVLNode<K, V>* node); //used with destructor

	void checkBalance(AVLNode<K, V>* node); //checks balance factors of the entire tree
	void sortCheck(AVLNode<K, V>* node); //checks that tree is properly balanced

	//variables
	AVLNode<K, V>* head;
	V defaultValue;
};

template<class K, class V>
AVLTree<K, V>::AVLTree() : defaultValue()
{
	head = nullptr;
}

template<class K, class V>
AVLTree<K, V>::AVLTree(V value)
{
	head = nullptr;
	defaultValue = value;
}

template<class K, class V>
void AVLTree<K, V>::memClean(AVLNode<K, V>* node)
{
	if (node->left != nullptr)
		memClean(node->left);
	if (node->right != nullptr)
		memClean(node->right);
	delete node;
}

template<class K, class V>
AVLTree<K, V>::~AVLTree()
{
	if (head != nullptr)
		memClean(head);
}

template<class K, class V>
void AVLTree<K, V>::insert(const K& key, const V& value)
{
	head = insert(key, value, head);
	head->height = max(head) + 1;
}


template<class K, class V>
AVLNode<K, V>* AVLTree<K, V>::insert(const K& key, const V& value, AVLNode<K, V>* node)
{
	//cout << "In insert" << endl;
	if (node == nullptr)
		return new AVLNode<K, V>{key, value, 0};
	if (node->key > key)
	{
		node->left = insert(key, value, node->left);
		node->height = max(node) + 1;
	}
	else if (node->key < key)
	{
		node->right = insert(key, value, node->right);
		node->height = max(node) + 1;
	}
	else
		node->value = value;

	rotate(node);
	return node;
}

template<class K, class V>
V AVLTree<K, V>::operator[](const K& key)
{
	return lookup(key);
}

template<class K, class V>
V AVLTree<K, V>::lookup(const K& key)
{
	AVLNode<K, V>* temp = lookup(key, head);
	if (temp == nullptr)
		return defaultValue;
	return temp->value;
}

template<class K, class V>
AVLNode<K, V>*& AVLTree<K, V>::lookup(const K& key, AVLNode<K, V>*& node)
{
	if (node == nullptr)
		return node;
	else if (key < node->key)
		return lookup(key, node->left);
	else if (key > node->key)
		return lookup(key, node->right);
	else
		return node;
}

template<class K, class V>
int AVLTree<K, V>::max(AVLNode<K, V>* node)
{
	if (node->right != nullptr && node->left != nullptr)
		if (node->right->height > node->left->height)
			return node->right->height;
		else
			return node->left->height;
	else if (node->right != nullptr)
		return node->right->height;
	else if (node->left != nullptr)
		return node->left->height;
	else
		return 0;
}

template<class K, class V>
void AVLTree<K, V>::fixHeight(AVLNode<K, V>*& node)
{
	if (node == nullptr)
		return;
	fixHeight(node->right);
	fixHeight(node->left);
	if (node->left == nullptr && node->right == nullptr)
		node->height = 0;
	else
		node->height = max(node) + 1;
}

template<class K, class V>
int AVLTree<K, V>::balanceFactor(AVLNode<K, V>* node)
{
	if (node->left == nullptr && node->right == nullptr)
		return 0;
	else if (node->right == nullptr)
		return node->left->height + 1;
	else if (node->left == nullptr)
		return 0 - (node->right->height + 1);
	else
		return node->left->height - node->right->height;
}

template<class K, class V>
void AVLTree<K, V>::rotate(AVLNode<K, V>*& node)
{
	while (balanceFactor(node) < -1 || balanceFactor(node) > 1)
	{
		if (balanceFactor(node) < -1)
		{
			if (balanceFactor(node->right) > 0)
			{
				//std::cout << "rightLeft(leftLeft followed by rightRight) case on node " << node->right->key << std::endl;
				node->right = leftLeft(node->right);
			}
			//std::cout << "rightRight case on node " << node->key << std::endl;
			node = rightRight(node);

		}
		if (balanceFactor(node) > 1)
		{
			if (balanceFactor(node->left) < 0)
			{
				//std::cout << "leftRight(rightRight followed by LeftLeft) case on node " << node->left->key << std::endl;
				node->left = rightRight(node->left);
			}
			//std::cout << "leftLeft case on node " << node->key << std::endl;
			node = leftLeft(node);
		}
	}
}

template<class K, class V>
AVLNode<K, V>* AVLTree<K, V>::rightRight(AVLNode<K, V>* node)
{
	AVLNode<K, V>* newParent = node->right;
	node->right = newParent->left;
	newParent->left = node;

	//fixing height
	fixHeight(newParent);
	return newParent;
}

template<class K, class V>
AVLNode<K, V>* AVLTree<K, V>::leftLeft(AVLNode<K, V>* node)
{
	AVLNode<K, V>* newParent = node->left;
	node->left = newParent->right;
	newParent->right = node;

	//fixing height
	fixHeight(newParent);
	return newParent;
}

template<class K, class V>
void AVLTree<K, V>::print(AVLNode<K, V>* node)
{
	//in order
	if (node == nullptr)
		return;
	print(node->left);
	std::cout << node->key << ", " << node->value << std::endl;
	print(node->right);
}

template<class K, class V>
void AVLTree<K, V>::print()
{
	print(head);
}

template<class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
	AVLNode<K, V>*& nodeToReplace = lookup(key, head);
	AVLNode<K, V>* replacement, *temp;
	if (nodeToReplace == nullptr)
		return; //return if key not found
	else if (nodeToReplace->left != nullptr)
	{
		if (nodeToReplace->left->right == nullptr)
		{
			nodeToReplace->left->right = nodeToReplace->right;
			temp = nodeToReplace->left;
			delete nodeToReplace;
			nodeToReplace = temp;
		}
		else
		{
			replacement = nodeToReplace->left;
			while (replacement->right->right != nullptr)
				replacement = replacement->right;
			replacement->right->right = nodeToReplace->right;			
			temp = replacement->right->left;
			replacement->right->left = nodeToReplace->left;
			delete nodeToReplace;
			nodeToReplace = replacement->right;
			replacement->right = temp;
		}
	}
	else if (nodeToReplace->right != nullptr)
	{
		if (nodeToReplace->right->left == nullptr)
		{
			nodeToReplace->right->left = nodeToReplace->left;
			temp = nodeToReplace->right;
			delete nodeToReplace;
			nodeToReplace = temp;
		}
		else
		{
			replacement = nodeToReplace->right;
			while (replacement->left->left != nullptr)
				replacement = replacement->left;
			temp = replacement->left->right;
			replacement->left->right = nodeToReplace->right;
			delete nodeToReplace;
			nodeToReplace = replacement->left;
			replacement->left = temp;
		}
	}
	else //node is a leaf
	{
		delete nodeToReplace;
		nodeToReplace = nullptr;
	}
	fixHeight(head);
	fixBalance(head);
}

template<class K, class V>
void AVLTree<K, V>::fixBalance(AVLNode<K, V>*& node)
{
	if (node == nullptr)
		return;
	fixBalance(node->left);
	fixBalance(node->right);
	rotate(node);
}

template<class K, class V>
void AVLTree<K, V>::check()
{
	checkBalance(head);
	sortCheck(head);
}

template<class K, class V>
void AVLTree<K, V>::sortCheck(AVLNode<K, V>* node)
{
	if(node == nullptr)
		return;
	if (node->left != nullptr && node->left->key > node->key)
		std::cout << "sort error" << std::endl;
	sortCheck(node->left);
	if (node->right != nullptr && node->right->key < node->key)
		std::cout << "sort error" << std::endl;
	sortCheck(node->right);
}

template<class K, class V>
void AVLTree<K, V>::checkBalance(AVLNode<K, V>* node)
{
	if (node == nullptr)
		return;
	checkBalance(node->left);
	checkBalance(node->right);
	if (balanceFactor(node) > 1 || balanceFactor(node) < -1)
		std::cout << "Balance factor on " << node->key << " is " << balanceFactor(node) << std::endl;
}

#endif