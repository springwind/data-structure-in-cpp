#include <iostream>
#include <iomanip>
#include <vector>
#include "skewHeap.h"

using namespace std;


int main()
{
	SkewHeap<int> h1, h2, h;
	int numItems = 100;
/*
	vector<int> v;
	for(int i= 37; i != 0; i = (i+37) %numItems)
		v.push_back(i);
	
//	sort(v.begin(), v.end());
	int cnt = 0;
	for(int i: v)
	{
		if(cnt == 20)
		{
			cnt = 0;
			cout<<endl;
		}
		cnt ++;
		cout<<setw(4)<<i;
	}

	return 0;
*/

	for(int i= 37; i!=0; i = (i+37) %numItems)
	{
		if(i %2 == 0)
			h1.insert(i);
		else
			h2.insert(i);
	}
	
	h1.merge(h2);
	h = h1;
	cout<<endl<<"h1:"<<endl;
	for(int i = 1; i<numItems; i++)
	{
		int x;
		h1.deleteMin(x);

		if(x != i)
			cout<<"error: expected = "<<i<<" got = "<<x<<endl;
	}

	cout<<endl<<"h:"<<endl;
	for(int i = 1; i<numItems; i++)
	{
		int x;
		h.deleteMin(x);

		if(x != i)
			cout<<"error: expected = "<<i<<" got = "<<x<<endl;
	}


	return 0;
}
