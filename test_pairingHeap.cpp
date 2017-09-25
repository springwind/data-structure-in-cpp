#include <vector>
#include <iostream>
#include "pairingHeap.h"

using namespace std;


int main()
{
	PairingHeap<int> h;

	int numItems = 8000;
	int GAP = 37;
	int GAP2 = 71;
	int GAP3 = 53;
	int i;
	int j;

	cout<<endl<<"Checking"<<endl;
	cout<<endl<<"test insert and deleteMin..."<<endl;
	for(i = GAP; i != 0; i = (i+GAP)%numItems)
		h.insert(i);

	for(i = 1; i<numItems; i++)
	{
		int x;
		h.deleteMin(x);
		if(x != i)
			cout<<"h error: expect = " <<i<<" got = "<<x<<endl;
	}


	cout<<endl<<"test decreaseKey..."<<endl;
	vector<PairingHeap<int>::Position > p(numItems);
	for(i = 0, j = numItems/2; i<numItems; i++, j = (j+71)%numItems)
		p[j] = h.insert(j + numItems);
	for(i = 0, j = numItems/2; i<numItems; i++, j = (j+53)%numItems)
		h.decreaseKey(p[j], j);

	PairingHeap<int> h1(h);
	h1 = h1;
	i = -1;
	while(!h1.isEmpty())
	{
		int x;
		h1.deleteMin(x);
		if(x != ++i)
			cout<<"h1 error: expect = "<<i<< " got = "<<x<<endl;
	}


	i = -1;
	PairingHeap<int> h2;
	h2 = h;
	while(!h2.isEmpty())
	{
		int x;
		h2.deleteMin(x);
		if(x != ++i)
			cout<<"h2 error: expect= "<<i<<" got = "<<x<<endl;
	}


	i = -1;
	while(!h.isEmpty())
	{
		int x;
		h.deleteMin(x);
		if(x != ++i)
			cout<<"h error: expect= "<<i<<" got = "<<x<<endl;
	}

	cout<<endl<<"Check completed."<<endl;

	return 0;
}
