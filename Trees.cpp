//This is a code that has two trees that are outputted in pre-order
#include<iostream>
#include<stdexcept>
#include<algorithm>
#include<fstream>
#include<iomanip>
using namespace std;
class PrecondViolationExcep : public logic_error //inheritance
{
public:
	PrecondViolationExcep(const string& message = "");
};
PrecondViolationExcep::PrecondViolationExcep(const string& message) : logic_error("Precondition Violated Exception: " + message)
{
}
template<class T>
struct BNode{
	T item;
	BNode<T>* lChild;
	BNode<T>* rChild;
	BNode<T>() : lChild(nullptr), rChild(nullptr) {}
	BNode<T>(const T& i) : lChild(nullptr), rChild(nullptr), item(i) {}
	BNode<T>(const T& i, BNode<T>* left, BNode<T>* right) : lChild(left), rChild(right), item(i) {}
	bool isLeaf() const { return lChild == nullptr && rChild == nullptr; }
};
template<class T>
class Tree {
	BNode<T>* root;
	int gHeiHlp(BNode<T>* subtree) const;
	int gNumOfNodesHlp(BNode<T> subtree) const;
	BNode<T>* balancedAdd(BNode<T>* subtree, BNode<T>* nNode);
	void preOrder(BNode<T>* tr, int id) const;
	BNode<T>* cpyTree(const BNode<T>* oldRt);
	void destroyTree(BNode<T>*& subtree);
	//BST
	BNode<T>* bstAdd(BNode<T>*& subree, BNode<T>* nNode);
	//BNode<T>* bstFind(BNode<T>*& subtree, const T& tgt) const; //search
	BNode<T>* bstRmvVal(BNode<T>*& subtree, const T& tgt, bool& success);
	BNode<T>* bstRmvNode(BNode<T>* ndPtr);
	/*void printTree(const BNode<T>* x, int& id);*/
	BNode<T>* bstRmvLftmstNode(BNode<T>*& subtree, T& inSuc);
	BNode<T>* lftTrav(BNode<T>* subtree);
	BNode<T>* rtTrav(BNode<T>* subtree); 
public:
	Tree() : root(nullptr) {}
	Tree(const T& rtItem);
	Tree(const T& rtItem, const BNode<T>* lTr, const BNode<T>* rTr);
	Tree(const Tree<T>& rhs);
	~Tree() { destroyTree(root); }
	/*Tree<T>& operator= (const Tree<T>& rhs);*/
	bool isEmpty() const { return root == nullptr; }
	int gHei() const{
		return gHeiHlp(root);
	}
	int gNumOfNodes() const;
	T gRtData() const throw (PrecondViolationExcep);
	void sRtData(const T& nData);
	bool add(const T& nData);
	void preTrav(int id) const;
	/*void printTr(int& id);*/
	bool remove(const T& tgt);
	
};
template <class T>
Tree<T> ::Tree(const T& rtItem){
	root = new BNode<T>(rtItem);
}
template<class T>
BNode<T>* Tree<T>:: lftTrav(BNode<T>* subtree){
	return subtree->lChild;
}
template<class T>
BNode<T>* Tree<T>::rtTrav(BNode<T>* subtree){
	return subtree->rChild;
}
template<class T>
Tree<T>:: Tree(const T& rtItem, const BNode<T>* lTr, const BNode<T>* rTr){
	root = new BNode<T>(rtItem, cpyTree(lTr), cpyTree(rTr));
}
template<class T>
Tree<T>::Tree(const Tree<T>& rhs){
	root = cpyTree(rhs.root);
}
//template<class T>
//Tree<T>& Tree<T>:: operator= (const Tree<T>& rhs){
//	//needs implementation
//}
template<class T>
BNode<T>* Tree<T>::balancedAdd(BNode<T>* subtree, BNode<T>* nNode){
	if (subtree == nullptr)
		return nNode;
	else
	{
		BNode<T>* leftPtr = subtree->lChild;
		BNode<T>* rightPtr = subtree->rChild;
		if (gHeiHlp(leftPtr) > gHeiHlp(rightPtr))
		{
			rightPtr = balancedAdd(rightPtr, nNode);
			subtree-> rChild = rightPtr;
		}
		else
		{
			leftPtr = balancedAdd(leftPtr, nNode);
			subtree->lChild = leftPtr;
			return subtree;
		}
	}
}
template<class T>
bool Tree<T>::add(const T& nData)
{
	BNode<T>* nNode = new BNode<T>(nData);
	//root = balancedAdd(root, nNode);
	root = bstAdd(root, nNode);
	return true;
}
template<class T>
int Tree<T>:: gHeiHlp(BNode<T>* subtree) const
{
	if (subtree == nullptr)
		return 0;
	else
		return 1 + max(gHeiHlp(subtree->lChild),gHeiHlp(subtree->rChild));
}
template<class T>
int Tree<T>::gNumOfNodesHlp(BNode<T> subtree) const{
	if (subtree == nullptr)
		return 0;
	else
		return 1 + gNumOfNodesHlp(subtree->lChild) + gNumOfNodesHlp(subtree->rChild);
}
template <class T>
void Tree<T>::preOrder(BNode<T>* tr, int id) const{
	if (tr != nullptr) {
		T it = tr->item;
		cout << setw(id) << it;
		cout << endl;
		if (tr->lChild)
			cout << setw(id) << "/" << endl;
		if (tr->rChild)
			cout << setw(id) <<  "\\" << endl;
		preOrder(tr-> lChild, id += 2);
		preOrder(tr->rChild, id+= 2);
	}
}
template<class T>
T Tree<T> ::gRtData() const throw (PrecondViolationExcep){
	if (isEmpty())
		throw PrecondViolationExcep("the tree is empty");
	return root->item;
}
template <class T>
void Tree<T>::sRtData(const T& nData){
	if (isEmpty())
		root = new BNode<T>(nData);
	else
		root->item = nData;
}
template<class T>
void Tree<T>:: preTrav(int id) const{
	preOrder(root, id);
}
void print(int& x){
	cout << x << " ";
}
template<class T>
int Tree<T> ::gNumOfNodes() const{
	return gNumOfNodesHlp(root);
}
template<class T>
BNode<T>* Tree<T>:: cpyTree(const BNode<T>* oldRt){
	BNode<T>* nTree = nullptr;
	if (oldRt != nullptr){
		nTree = new BNode<T>(oldRt->item, nullptr, nullptr);
		nTree->lChild = cpyTree(oldRt->lChild);
		nTree->rChild = cpyTree(oldRt->rChild);
	}
	return nTree;
}
template<class T>
void Tree<T>:: destroyTree(BNode<T>*& subtree){
	if (subtree != nullptr){
		destroyTree(subtree->lChild);
		destroyTree(subtree->rChild);
		delete subtree;
		subtree = nullptr;
	}
}
//BST
template<class T>
BNode<T>* Tree<T>:: bstAdd(BNode<T>*& subtree, BNode<T>* nNode){
	if (subtree == nullptr)
		return nNode;
	else {
		if (subtree->item == nNode->item)
			return subtree;
		else if (subtree->item > nNode->item){
			BNode<T>* temp = bstAdd(subtree->lChild, nNode);
			subtree->lChild = temp;
		}
		else{
			BNode<T>* temp = bstAdd(subtree->lChild, nNode);
			subtree->rChild = temp;
		}
	}
	return subtree;
}
//template<class T>
//BNode<T>* Tree<T>:: bstFind(BNode<T>*& subtree, const T& tgt) const{
//	//needs implementation
//}
template<class T>
BNode<T>* Tree<T>:: bstRmvVal(BNode<T>*& subtree, const T& tgt, bool& success){
	BNode<T>* tempPtr;
	if (subtree == nullptr)
	{
		success = false;
		return nullptr;
	}
	else if (subtree->item == tgt)
	{
		subtree = bstRmvNode(BNode<T>* ndPtr); // Remove the item
		success = true;
		return subtree;
	}
	else if (subtree-> item > tgt)
	{
		tempPtr = bstRmvVal(subtree->lChild, tgt, success);
		subtree->lChild = tempPtr;
		return subtree;
	}
	else
	{
		tempPtr = bstRmvVal(subtree->rChild, tgt, success);
		subtree->rChild = tempPtr;
		return subtree;
	}
}
template<class T>
BNode<T>* Tree<T>:: bstRmvNode(BNode<T>* ndPtr){
	if (ndPtr->isLeaf())
	{
		// Remove leaf from the tree
		delete ndPtr;
		ndPtr = nullptr;
		return ndPtr;
	}
	else if (ndPtr -> lChild != nullptr && !( ndPtr -> rChild != nullptr))
	{
		// C replaces N as the child of N’s parent
		if (ndPtr->lChild != nullptr)
			BNode<T>* nodeToConnectPtr = ndPtr->lChild;
		else
			BNode<T>* nodeToConnectPtr = ndPtr->rChild;
		delete ndPtr;
		ndPtr = nullptr;
		return nodeToConnectPtr;
	}
	else // N has two children
	{
		T newNodeValue;
		ndPtr -> rChild = rmvLeftmstNd(ndPtr->rChild, newNodeValue);
		ndPtr->item = newNodeValue;// Put replacement value in node N
		return ndPtr;
	}
}
template<class T>
bool Tree<T>::remove(const T& tgt) {
	bool isSuccess = false;
	root = bstRmvVal(root, tgt, isSuccess);
	return isSuccess;
}
//template<class T>
//void Tree<T> ::printTree(const BNode<T>* x, int& id){
//	if (x != NULL)
//	{
//		cout << setw(id) << x->item;
//		cout << "\nHi";
//		printTree(x->lChild, id);
//		printTree(x->rChild, id);
//		id += 5;
//	}
//}
//template<class T>
//void Tree<T> ::printTr(int & id){
//	printTree(root, id);
//}
template<class T>
BNode<T>* Tree<T>:: bstRmvLftmstNode(BNode<T>*& subtree, T& inSuc){
	if (subtree->lChild == nullptr) {
		inSuc = subtree->item;
		return bstRmvNode(subtree);
	}
	else {
		subtree->lChild = bstRmvLftmstNode(subtree->lChild, inSuc);
		return subtree;
	}
}
int main(){
	Tree<int> test;
	ifstream inFile;
	int id = 0;
	int i = 0;
	int first, mid, last;
	char twod[50][50];
	int arr[100];
	inFile.open("Nums.txt");
	if (!inFile){
		cout << "Error";
		return 0;
	}
		while (!inFile.eof()){
			inFile >> arr[i];
		}
		for (int j = 0; j < 13; j++){
			test.add(arr[j]);
		}
		for (int i = 0; i < 50; i++){
			for (int j = 0; j < 50; j++){
				first = i;
				mid = i / 2;
				last = 50;
				if (i == 0)
					twod[mid][j] = test.gRtData();
				else{
					
				}
			}
		}
	
	cout << "Pre Order: \n";
	test.preTrav(id);
}
/*
OUTPUT:
Pre Order: 1 2 4 7 6 10 3 5 9 8
Pre Order: 25 6 1 11 29 8 14 7 9 15 13
*/
/*
Remove a node from a BST. 3 cases
1) a leaf node. delete it
2) a node w/ exacty one child. can be bypassed easily. remove 3 bypass 2-> 5
3) a node w/ two children is "removed" by replacing its value w/ the smallest in its right subtree and delete
	the corresponding node (that has the smallest value)
*/
