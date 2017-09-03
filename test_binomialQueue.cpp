#include <iostream>
#include "binomialQueue.h"

using namespace std;


int main()
{
	BinomialQueue<int> h, h1, h2;
	int numItems = 10000;
	int i;


	cout<<endl<<"begin test..."<<endl;
	for(i = 37; i != 0; i = (i+37)%numItems)
	{
		if(i%2 == 0)
			h.insert(i);
		else
			h1.insert(i);
	}

	h.merge(h1);
	h2 = h;

	
	for(i=1; i<numItems; i++)
	{
		int x;
		h.deleteMin(x);
		if(i != x)
			cout<<"error: expected="<<i<<" got="<<x<<endl;
	}
	for(i=1; i<numItems; i++)
	{
		int x;
		h2.deleteMin(x);
		if(i != x)
			cout<<"error: expected="<<i<<" got="<<x<<endl;
	}

	if(!h1.isEmpty())
		cout<<"h1 should be empty."<<endl;

	cout<<"End of test... no output is good."<<endl;
}
