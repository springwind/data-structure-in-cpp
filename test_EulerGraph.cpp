#include "eulerGraph.h"

#include <iostream>

using namespace std;

void test2()
{
	EulerGraph eg(12); //Figure 9.70

	eg.addEdge(0, 2);
	eg.addEdge(0, 3);
	eg.addEdge(1, 2);
	eg.addEdge(1, 7);
	eg.addEdge(2, 3);
	eg.addEdge(2, 5);
	eg.addEdge(2, 6);
	eg.addEdge(2, 8);
	eg.addEdge(3, 4);
	eg.addEdge(3, 6);
	eg.addEdge(3, 9);
	eg.addEdge(3, 10);
	eg.addEdge(4, 9);
	eg.addEdge(5, 8);
	eg.addEdge(6, 8);
	eg.addEdge(6, 9);
	eg.addEdge(7, 8);
	eg.addEdge(8, 9);
	eg.addEdge(8, 11);
	eg.addEdge(9, 10);
	eg.addEdge(9, 11);

	eg.printGraphMatrix();

	for(int s = 0; s < 12; s++)
	{
		eg.findEulerCircuit(s);
		eg.printPath();
	}

}


void test1()
{
	EulerGraph eg(7);//Figure 9.69, the second one 
	eg.addEdge(0, 1);
	eg.addEdge(0, 2);
	eg.addEdge(1, 2);
	eg.addEdge(1, 3);
	eg.addEdge(1, 4);

	eg.addEdge(2, 3);
	eg.addEdge(2, 5);

	eg.addEdge(3, 4);
	eg.addEdge(3, 5);

	eg.addEdge(4, 5);
	eg.addEdge(4, 6);
	eg.addEdge(5, 6);

	eg.printGraphMatrix();
	for(int s = 0; s<7; s++)
	{
		eg.findEulerCircuit(s);
		eg.printPath();
	}
}


int main()
{
	test1();
	cout<<endl<<"------------------------"<<endl;
	test2();
}

