#include <iostream>
#include "weightedALG.h"

using namespace std;

template<typename VertexType>
class VisitFun
{
public:
	bool operator() (VertexType val)
	{
		cout<<val<<"\t";
		return true;
	}
};


	
template<typename VertexType, typename CostType>
void testGraph(WeightedALGraph<VertexType, CostType> &g)
{
	cout<<"************************************************"<<endl;

	//cout<<"matrix representation of graph:"<<endl;
	g.printMatrix();

	cout<<"depth-first traverse:"<<endl;
	g.dfsTraverse(VisitFun<VertexType> ());
	cout<<endl;

	cout<<"breadth-first traverse:"<<endl;
	g.bfsTraverse(VisitFun<VertexType> ());

	g.topologicalSort();

	int n = g.getVexNum();
	for(int i = 0; i<n; i++)
	{
		g.unweightedShortestPath(i);
		g.printShortestPath();
	}
	cout<<endl<<endl;

}

void test6()
{
	string vs[] = {"V0", "V1", "V2", "V3", "V4", "V5"};
	int N = 6;
	WeightedALGraph<string, int> g6(vs, N, true);

	string *p = vs;
	g6.insertArc(p + 0, p + 2, 10);
	g6.insertArc(p + 0, p + 4, 30);
	g6.insertArc(p + 0, p + 5, 100);
	g6.insertArc(p + 1, p + 2, 5);
	g6.insertArc(p + 2, p + 3, 50);
	g6.insertArc(p + 3, p + 5, 10);
	g6.insertArc(p + 4, p + 3, 20);
	g6.insertArc(p + 4, p + 5, 60);
	
	cout<<endl<<"========================G6=========================="<<endl;

	g6.printMatrix();

	for(int i = 0; i<N; i++)
	{
		g6.dijkstra(i);
		g6.printShortestPaths();
	}
}


void test7()
{
	//string vs[] = {"V0", "V1", "V2"};
	char vs [] = {'a', 'b', 'c'};
	int N = 3;
	WeightedALGraph<char, int> g7(vs, N, true);

	char *p = vs;
	g7.insertArc(p + 0, p + 1, 4); 
	g7.insertArc(p + 0, p + 2, 11);
	g7.insertArc(p + 1, p + 0, 6);
	g7.insertArc(p + 1, p + 2, 2);
	g7.insertArc(p + 2, p + 0, 3);

	cout<<endl<<"========================G7=========================="<<endl;
	g7.allPairShortestPaths();
	g7.printAllPairShortestPaths();
}


void test9_20()
{
	string vs[] = {"", "v1", "v2", "v3", "v4", "v5", "v6", "v7"};
	int N = 7;

	WeightedALGraph<string, int> g9_20(vs + 1, N, true);
	string *p = vs;
	g9_20.insertArc(p + 1, p + 2, 2); 
	g9_20.insertArc(p + 1, p + 4, 1);
	g9_20.insertArc(p + 2, p + 4, 3);
	g9_20.insertArc(p + 2, p + 5, 10);
	g9_20.insertArc(p + 3, p + 1, 4);
	g9_20.insertArc(p + 3, p + 6, 5);
	g9_20.insertArc(p + 4, p + 3, 2);
	g9_20.insertArc(p + 4, p + 5, 2);
	g9_20.insertArc(p + 4, p + 6, 8);
	g9_20.insertArc(p + 4, p + 7, 4);
	g9_20.insertArc(p + 5, p + 7, 6);
	g9_20.insertArc(p + 7, p + 6, 1);

	cout<<endl<<"=========================G9.20========================"<<endl;
	g9_20.printMatrix();

	int i = g9_20.locateVex(vs[1]);
	//g9_20.dijkstra(i);
//	g9_20.printShortestPath();

	g9_20.allPairShortestPaths();
	g9_20.printAllPairShortestPaths();
}


void test7_30()
{
	string p[] = {"", "V1", "V2", "V3", "V4", "V5", "V6"};
	int N = 6;

	WeightedALGraph<string, int> g7_30(p + 1, N, true);
	
	WeightedALGraph<string, int> & g = g7_30;

	g.insertArc(p + 1, p + 2, 3);
	g.insertArc(p + 1, p + 3, 2);
	g.insertArc(p + 2, p + 5, 3);
	g.insertArc(p + 2, p + 4, 2);
	g.insertArc(p + 3, p + 4, 4);
	g.insertArc(p + 3, p + 6, 3);
	g.insertArc(p + 4, p + 6, 2);
	g.insertArc(p + 5, p + 6, 1);

	cout<<endl<<"=========================G7.30==============================="<<endl;
	g.printMatrix();

	g.topologicalSort();
	g.printTopSort();

	g.criticalPath();
	g.printActivies();
	g.printCriticalActivities();


}

void test7_29()
{
	string p[] = {"", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9"};
	int N = 9;

	WeightedALGraph<string, int> g(p + 1, N, true);

	g.insertArc( p + 1, p + 2, 6);
	g.insertArc( p + 1, p + 3, 4);
	g.insertArc( p + 1, p + 4, 5);
	g.insertArc( p + 2, p + 5, 1);
	g.insertArc( p + 3, p + 5, 1);
	g.insertArc( p + 4, p + 6, 2);
	g.insertArc( p + 5, p + 7, 9);
	g.insertArc( p + 5, p + 8, 7);
	g.insertArc( p + 6, p + 8, 4);
	g.insertArc( p + 7, p + 9, 2);
	g.insertArc( p + 8, p + 9, 4);

	cout<<endl<<"===============G7.29=============================="<<endl;
	g.printMatrix();
	g.criticalPath();
	g.printActivies();
	g.printCriticalActivities();

}

int main()
{

	test6();
	test9_20();
	test7();

	test7_30();

	test7_29();
	return 0;
}
