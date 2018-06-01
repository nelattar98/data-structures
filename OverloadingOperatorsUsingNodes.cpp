#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
using namespace std;
//atoi (ascii to int)
struct nNode{
	nNode* next;
	int data;
	nNode(){
		data = 0;
		next = nullptr;
	}
	nNode(int d){
		data = d;
		next = nullptr;
	}
	nNode(nNode* addon, int d){
		next = addon;
		data = d;
	}
};
class lnkdBN{
	int ic;
	nNode* head;
	string num;
public:
	lnkdBN(){
		cout << "Default ctor " << endl;
		ic = 0;
		head = nullptr;
	}
	lnkdBN(const string& s);
	lnkdBN(const lnkdBN& bigNum);
	~lnkdBN();
	lnkdBN& operator =(const lnkdBN & rhs);
	const lnkdBN operator +(const lnkdBN & rhs);
	void addnode(int d);
	bool remove(int pos);
	friend ostream& operator << (ostream& out, const lnkdBN& bn);
	void reverse();
};
void lnkdBN::reverse() {
	nNode *curr, *prev, *n;
	curr = head;
	prev = nullptr;
	while (curr != nullptr) {
		n = curr->next;
		curr->next = prev;
		prev = curr;
		curr = n;
	}
	head = prev;
}

bool lnkdBN:: remove(int pos) {
	nNode* prev = nullptr;
	nNode* curr = head;
	bool ret = false;
	while (curr && pos--) {
		prev = curr;
		curr - curr->next;
	}
	if (curr) {
		if (curr == head)
			head = head->next;
		else
			prev->next - curr->next;
		delete curr;
		ret = true;
	}
	return ret;
}
void lnkdBN::addnode(int d){
	nNode* curr = head;
	if (head == nullptr){
		head = new nNode(d);
		return;
	}
	curr = new nNode(d);
	curr->next = head;
	head = curr;
}
lnkdBN::lnkdBN(const string& s){
	cout << "Conversion ctor" << endl;
	num = s;
	string tmp;
	int str;
	nNode* curr = NULL;
	int len = s.size();
	int leftover = len % 4;
	int it = len / 4;
	if (leftover)
		it++;
	for (int i = 0; i < it; i++)
	{
		if (i == 0) {
			if (leftover > 0)
				tmp = s.substr(0, leftover);
			else
				tmp = s.substr(0, 4);
		}
		else {
			if (leftover > 0)
				tmp = s.substr(leftover + (i - 1) * 4, 4);
			else
				tmp = s.substr(i * 4, 4);
		}
		str = atoi(tmp.c_str());
		if (curr == NULL)  
			curr = head = new nNode(atoi(tmp.c_str()));
		else {
			curr->next = new nNode(atoi(tmp.c_str()));
			curr = curr->next;
		}
	}
}
lnkdBN:: ~lnkdBN(){
	cout << "Destructor called " << endl;
	nNode* curr = head;
	nNode* temp;
	while (curr != nullptr)
	{
		temp = curr->next;
		delete curr;
		curr = temp;
	}
	head = nullptr;
}
lnkdBN::lnkdBN(const lnkdBN &bigNum){
	cout << "Cpy ctor" << endl;
	ic = bigNum.ic;
	nNode* curr = bigNum.head;

	if (curr == nullptr)
		head = nullptr;
	else {
		head = new nNode(curr->data);
		nNode* nChain = head;
		curr = curr->next;
		while (curr != nullptr) {
			nNode* node = new nNode(curr->data);
			nChain->next = node;
			nChain = nChain->next;
			curr = curr->next;
		}
		nChain->next = nullptr;
	}
}
lnkdBN& lnkdBN :: operator =(const lnkdBN & rhs){
	nNode * curr = rhs.head;
		if (this->head == curr)
			return *this;
		nNode * t = this->head;
		nNode * temp;
		while (t != nullptr){
			temp = t->next;
			delete t;
			t = temp;
		}
		this->head = nullptr;
		this->head = new nNode(curr->data);
		nNode* nChain = this->head;
		curr = curr->next;
		while (curr != nullptr){
			nNode* node = new nNode(curr->data);
			nChain->next = node;
			nChain = nChain->next;
			curr = curr->next;
		}
		nChain->next = nullptr;
		return *this;
}
const lnkdBN lnkdBN:: operator +(const lnkdBN & rhs){
	int carry = 0;
	int total;
	nNode* lftHand = this->head;
	nNode* rtHand = rhs.head;
	lnkdBN nNum;
	nNode* temp = nNum.head;
	if (rtHand == nullptr)
		return *this;
	else{
		while (lftHand != nullptr || rtHand != nullptr){
			total = (rtHand ? rtHand->data : 0) + (lftHand ? lftHand->data + carry : 0);
			if(lftHand)
				lftHand = lftHand->next;
			if(rtHand)
				rtHand = rtHand->next;
			if (total >= 10000) {
				carry = total / 10000;
				total = total % 10000;
				nNum.addnode(total);
			}
			else
				nNum.addnode(total);
			
		}
		if (carry != 0)
		{
			nNum.addnode(carry);
		}
	}
	return nNum;
}
ostream& operator << (ostream& out, const lnkdBN& bn){
	nNode* curr = bn.head;
	while (curr != nullptr){
		out << curr->data;
		curr = curr->next;
	}
	return out;
}
int main(){
	ifstream inFile;
	lnkdBN n3, n4;
	string num1, num2;
	inFile.open("testdata.txt");
	if (!inFile)
	{
		cout << "Error in opening file";
		return 1;
	}
	while (!inFile.eof()){
		inFile >> num1;
		inFile >> num2;
		lnkdBN n1(num1);
		lnkdBN n2(num2);
		n1.reverse();
		n2.reverse();
		n3 = n1 + n2;
		n1.reverse();
		n2.reverse();
		cout << n1 << " + " << n2 << " = " << n3 << endl;
		n4 = n1*n2;
		cout << n1 << " * " << n2 << " = " << n4 << endl;
	}
	return 0;
}
