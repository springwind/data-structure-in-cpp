#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "redblackTree.h"

using namespace std;


class VisitFun
{
public:
	void operator() (int i)
	{
		//cout<<i<<endl;
		cout<<setw(8)<<i;
		cout.flush();
	}
};

int main()
{
	int NEG_INF = -9999;
	int NUMS = 150;//000;
	int GAP = 37;
	int i;

	RedBlackTree<int> t1(NEG_INF);

	for(i = GAP; i != 0; i = (i+GAP) %NUMS)
	{
		t1.insert(i);
	}

	t1.checkReds();
//	t1.checkBlacks();


	RedBlackTree<int> t2(t1);
	RedBlackTree<int> t3 = t2;

	if(NUMS < 50)
	{
		cout<<"original tree: ";
		t1.printTree(VisitFun());
		cout<<endl;
	}

	cout<<endl<<"-------------test t1:----------------"<<endl;

	if(1 != t1.findMin() || NUMS - 1 != t1.findMax())
		cout<<"findMin or findMax error"<<endl;

	
	for(i = 1; i<NUMS; i++)
		if(!t1.contains(i))
			cout<<"find error: "<<i<<endl;
	if(t1.contains(0))
		cout<<"error: contains 0"<<endl;

	int cnt = NUMS;
	for(i = GAP; i!=0; i = (i+GAP)%NUMS)
	{		
		t1.remove(i);
		if(t1.contains(i))
			cout<<"error: "<<i<<" still present";
		if(--cnt < 20)
		{
			cout<<"---after remove "<<i<<": "<<endl;
			t1.printTree(VisitFun());
			cout<<endl;
		}
	}
	if(!t1.isEmpty())
	{
		cout<<"not empty: ";
	}
	t1.printTree(VisitFun());
	cout<<endl;

	cout<<endl<<"-------------test t2 :----------------"<<endl;

	if(1 != t2.findMin() || NUMS - 1 != t2.findMax())
		cout<<"findMin or findMax error"<<endl;
	
	for(i = 1; i<NUMS; i++)
		if(!t2.contains(i))
			cout<<"find error: "<<i<<endl;
	if(t2.contains(0))
		cout<<"error: contains 0"<<endl;

	cnt = NUMS;
	for(i = 1; i<NUMS; i++)
	{		
		t2.remove(i);
		if(t2.contains(i))
			cout<<"error: "<<i<<" still present";
		if(--cnt < 20)
		{
			cout<<"---after remove "<<i<<": "<<endl;
			t2.printTree(VisitFun());
			cout<<endl;
		}
	}
	if(!t2.isEmpty())
	{
		cout<<"not empty: ";
	}
	t2.printTree(VisitFun());
	cout<<endl;

	cout<<endl<<"-------------test t3 :----------------"<<endl;
	if(1 != t3.findMin() || NUMS - 1 != t3.findMax())
		cout<<"findMin or findMax error"<<endl;

	for(i = 1; i<NUMS; i++)
		if(!t3.contains(i))
			cout<<"find error: "<<i<<endl;
	if(t3.contains(0))
		cout<<"error: contains 0"<<endl;
	cnt = NUMS;
	for(i = NUMS -1; i >0; i--)
	{		
		t3.remove(i);
		if(t3.contains(i))
			cout<<"error: "<<i<<" still present";
		if(--cnt < 20)
		{
			cout<<"---after remove "<<i<<": "<<endl;
			t3.printTree(VisitFun());
			cout<<endl;
		}
	}
	if(!t3.isEmpty())
	{
		cout<<"not empty: ";
	}
	t3.printTree(VisitFun());
	cout<<endl;

	cout<<endl<<"end of test"<<endl;
}
