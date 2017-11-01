#include <iostream>
#include <vector>
#include "edgeWeightedGraph.h"
#include "lazyPrimMST.h"
#include "eagerPrimMST.h"
#include "kruskalMST.h"

using namespace std;

template<typename WeightedGraph>
void testMST(const WeightedGraph &g)
{
	cout<<endl<<"matrix representation of the graph: "<<endl;
	g.printMatrix();

	cout<<"mst using lazy Prim's algorithm"<<endl;
	LazyPrimMST<WeightedGraph > lazyPrim;
	lazyPrim.findMST(g);
	lazyPrim.printMST();

	cout<<"mst using eager Prim's algorithm"<<endl;
	EagerPrimMST<WeightedGraph> eagerPrim;
	eagerPrim.findMST(g);
	eagerPrim.printMST();

	cout<<"mst using Kruskal's algorithm"<<endl;
	KruskalMST<WeightedGraph> kruskal;
	kruskal.findMST(g);
	kruskal.printMST();
	
}

void test1()
{
	cout<<endl<<"==============================G7.16=============================="<<endl;
	string p[] = {"", "V1", "V2", "V3", "V4", "V5", "V6"};
	typedef EdgeWeightedGraph<int> WeightedGraph;
	WeightedGraph g(6);

	g.addEdge(0, 0, 6);
	g.addEdge(0, 1, 1);
	g.addEdge(0, 2, 5);
	g.addEdge(0, 1, 5);
	g.addEdge(0, 3, 3);
	g.addEdge(1, 2, 5);
	g.addEdge(1, 3, 6);
	g.addEdge(1, 4, 4);
	g.addEdge(2, 4, 2);
	g.addEdge(3, 4, 6);

	testMST(g);
}



void test2()
{
	cout<<endl<<"==================G9.48======================="<<endl;
	string p[] = {"", "v1", "v2", "v3", "v4", "v5", "v6", "v7"};
	EdgeWeightedGraph<int> g(7);

	g.addEdge(0, 1, 2);
	g.addEdge(0, 2, 4);
	g.addEdge(0, 3, 1);
	g.addEdge(1, 3, 3);
	g.addEdge(1, 4, 10);
	g.addEdge(2, 5, 5);
	g.addEdge(3, 4, 7);
	g.addEdge(3, 5, 8);
	g.addEdge(3, 6, 4);
	g.addEdge(4, 6, 6);
	g.addEdge(5, 6, 1);


	testMST(g);

}

int main()
{
	test1();
	test2();

}
