#include <iostream>
#include "flowNetwork.h"


using namespace std;


void test1()
{
	FlowNetwork g(6); //Figure 9.39
	
	g.addEdge(0, 1, 3);
	g.addEdge(0, 2, 2);
	g.addEdge(1, 2, 1);
	g.addEdge(1, 3, 3);
	g.addEdge(1, 4, 4);
	g.addEdge(2, 4, 2);
	g.addEdge(3, 5, 2);
	g.addEdge(4, 5, 3);

	g.printGraph();

	int maxflow = g.findMaxFlow(0, 5);
	cout<<"max flow of the graph: "<<maxflow<<endl;
	g.printFlowGraph();

}

void test2()
{
	FlowNetwork g(7);

	g.addEdge(0, 1, 7);
	g.addEdge(0, 2, 6);
	g.addEdge(0, 3, 5);
	g.addEdge(1, 3, 1);
	g.addEdge(1, 4, 2);
	g.addEdge(2, 3, 3);
	g.addEdge(2, 5, 9);
	g.addEdge(3, 4, 5);
	g.addEdge(3, 6, 3);
	g.addEdge(4, 6, 6);
	g.addEdge(5, 6, 8);

	g.printGraph();

	int maxflow = g.findMaxFlow(0, 6);
	cout<<endl<<"max flow of the graph: "<<maxflow<<endl;
	g.printFlowGraph();
}

void test3()
{
	FlowNetwork g(4); //Figure 9.47

	g.addEdge(0, 1, 1000000);
	g.addEdge(0, 2, 1000000);
	g.addEdge(1, 2, 1);
	g.addEdge(1, 3, 1000000);
	g.addEdge(2, 3, 1000000);

	g.printGraph(8);

	int maxflow = g.findMaxFlow(0, 3);
	cout<<endl<<"max flow of the graph: "<<maxflow<<endl;
	g.printFlowGraph(8);

}

int main()
{
	test1();
	test2();
	test3();
}
