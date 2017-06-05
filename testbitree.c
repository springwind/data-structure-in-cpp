#include <iostream>
#include <iomanip>

#include "bitree.h"

using namespace std;

class VisitFun
{

public:
	bool visit(char c) const
	{
		cout<<setw(2)<<c;
		return true;
	}
};

int main()
{
	BiTree<char>  bt;

	vector<char> v = {'A', 'B', 'C', ' ', ' ', 'D', 'E', ' ', 'G', ' ', ' ', 'F', ' ', ' ', ' '};
	bt.createBiTree(v, ' ');
	

	cout<<"preorder traverse:"<<endl;
	cout<<"recursively:\t";
	bt.preorderTraverse_r(VisitFun());
	cout<<endl;
	
	cout<<"not recursively:\t";
	bt.preorderTraverse(VisitFun());
	cout<<endl;

	cout<<"inorder traverse:"<<endl;
	cout<<"recursively:\t";
	bt.inorderTraverse_r(VisitFun());
	cout<<endl;

	cout<<"not recursively:\t";
	bt.inorderTraverse(VisitFun());
	cout<<endl;

	cout<<"post order traverse:"<<endl;
	cout<<"recursively:\t";
	bt.postorderTraverse_r(VisitFun());
	cout<<endl;

	cout<<"not recursively:\t";
	bt.postorderTraverse(VisitFun());
	cout<<endl;

}
