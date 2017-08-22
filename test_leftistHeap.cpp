#include <iostream>
#include "leftistHeap.h"
#include <vector>

using namespace std;




int main()
{
	LeftistHeap<int> h, h1, h2;

	int numItems = 10000;


	cout<<endl<<"-------------------LeftistHeap test-------------------"<<endl;


	vector<int> v;
	for(int i = 37; i != 0; i = (i + 37)%numItems)
	{
		v.push_back(i);
		if(i % 2 == 0)
			h1.insert(i);
		else
			h.insert(i);
	}
	
	LeftistHeap<int> h3 = h;

	h.merge(h1);
	h2 = h;

	
	for(int i= 1; i<numItems; i++)
	{
		int x;
		h.deleteMin(x);
		if(x != i)
			cout<<"error at: "<<i<<endl;
	}
	
	for(int i= 1; i<numItems; i++)
	{
		int x;
		h2.deleteMin(x);
		if(x != i)
			cout<<"error at: "<<i<<endl;
	}

	if(!h1.isEmpty())
		cout<<"error: h1 should have been empty"<<endl;

	cout<<"------------end------------------"<<endl;
}



