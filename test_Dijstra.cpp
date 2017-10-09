#include <iostream>
#include <vector>
#include <string>
#include "weightedDigraph.h"

using namespace std;


int main()
{
	vector<string> vArr{"", "v1", "v2", "v3", "v4", "v5", "v6", "v7"};

	WeightedDigraph<string, int> g;
	for(auto itr = vArr.begin() + 1; itr != vArr.end(); itr++)
		g.addVertex(*itr);

	auto itr = vArr.begin() + 1;
	g.addArc(vArr[1], vArr[2], 2);
	g.addArc(vArr[1], vArr[4], 1);
	g.addArc(vArr[2], vArr[4], 3);
	g.addArc(vArr[2], vArr[5], 10);
	g.addArc(vArr[3], vArr[1], 4);
	g.addArc(vArr[3], vArr[6], 5);
	g.addArc(vArr[4], vArr[3], 2);
	g.addArc(vArr[4], vArr[5], 2);
	g.addArc(vArr[4], vArr[6], 8);
	g.addArc(vArr[4], vArr[7], 4);
	g.addArc(vArr[5], vArr[7], 6);
	g.addArc(vArr[7], vArr[6], 1);

	g.Dijstra(vArr[1]);

	for(int i = 1; i<vArr.size(); i++)
	{
		cout<<"to "<<vArr[i]<<" : dist = "<<g.getDist(vArr[i])<<"  path = ";
		g.printPath(vArr[i]);

		cout<<endl;
	}

	return 0;
}
