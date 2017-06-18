#include <iostream>
#include "algraph.h"

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


	
template<typename VertexType>
void testGraph(ALGraph<VertexType> &g)
{
	cout<<"************************************************"<<endl;

	//cout<<"matrix representation of graph:"<<endl;
//	g.printMatrix();

/*	cout<<"matrix representationof the graph's copy:"<<endl;
	ALGraph<VertexType> gcopy(g);
	gcopy.printMatrix();
*/	
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

void test1()
{
	string vs[] = {"", "V1", "V2", "V3", "V4"};

	ALGraph<string> g1 (vs + 1, 4, true);

	string *p = vs;
	g1.insertArc(p + 1, p + 2);
	g1.insertArc(p + 1, p + 3);
	g1.insertArc(p + 3, p + 4);
	g1.insertArc(p + 4, p + 1);

	cout<<"G1:"<<endl;

	testGraph(g1);
}

void test2()
{
	string vs[] = {"", "V1", "V2", "V3", "V4", "V5"};
	
	ALGraph<string>  g2 (vs + 1, 5, false);
	g2.insertArc(vs[1], vs [2]);
	g2.insertArc(vs[1], vs [ 4]);
	g2.insertArc(vs [2], vs[ 3]);
	g2.insertArc(vs[ 2], vs [ 5]);
	g2.insertArc(vs[3], vs[4]);
	g2.insertArc(vs [ 3], vs[ 5]);
	
	cout<<"G2"<<endl;
	testGraph(g2);

	
}


void test3()
{
	char vs[13] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M'};

	
	ALGraph<char> g3(vs,  13, false);
	g3.insertArc('A', 'B');
	g3.insertArc('A', 'C');
	g3.insertArc('A', 'F');
	g3.insertArc('A', 'L');

	g3.insertArc('B', 'M');
	
	g3.insertArc('D', 'E');
	
	g3.insertArc('G', 'H');
	g3.insertArc('G', 'I');
	g3.insertArc('G', 'K');
	
	g3.insertArc('H', 'K');
	g3.insertArc('J', 'L');
	g3.insertArc('J', 'M');
	g3.insertArc('L', 'M');


	cout<<"-----------------G3------------------"<<endl;
	testGraph(g3);
}

void test4()
{
	string vs[] = {"", "V1","V2", "V3", "V4", "V5", "V6", "V7", "V8"};

	ALGraph<string> g4(vs + 1, 8, false);
	g4.insertArc(vs + 1, vs + 2);
	g4.insertArc(vs + 1, vs + 3);
	g4.insertArc(vs + 2, vs + 4);
	g4.insertArc(vs + 2, vs + 5);
	g4.insertArc(vs + 3, vs + 6);
	g4.insertArc(vs + 3, vs + 7);
	g4.insertArc(vs + 4, vs + 8);
	g4.insertArc(vs + 5, vs + 8);
	g4.insertArc(vs + 6, vs + 7);

	cout<<"-----------------------------G4-----------------------------"<<endl;
	testGraph(g4);
}


void test5()
{
	char a = 'A';
	char vs[13];
	int n = 13;
	for(int i = 0; i< n; i++)
		vs[i] = a + i;

	ALGraph<char> g5(vs, n, false);

	g5.insertArc('A', 'B');
	g5.insertArc('A', 'C');
	g5.insertArc('A', 'F');
	g5.insertArc('A', 'L');

	g5.insertArc('B', 'C');
	g5.insertArc('B', 'D');
	g5.insertArc('B', 'G');
	g5.insertArc('B', 'H');
	g5.insertArc('B', 'M');

	g5.insertArc('D', 'E');

	g5.insertArc('G', 'H');
	g5.insertArc('G', 'I');
	g5.insertArc('G', 'K');
	
	g5.insertArc('H', 'K');
	g5.insertArc('J', 'L');	
	g5.insertArc('J', 'M');
	g5.insertArc('L', 'M');
	
	cout<<"--------------------------G5-------------------------"<<endl;
	testGraph(g5);
	for(int i = 0; i<n; i++)
	{
		g5.findArticul(i);
		g5.findArticul2(i);
	}
}

void testTopSort()
{
	string vex[] = {"V1", "V2", "V3", "V4"};
	int n = 4;
	ALGraph<string> g25 (vex, n, true);
	g25.insertArc("V1", "V2");
	g25.insertArc("V1", "V3");
	g25.insertArc("V2", "V4");
	g25.insertArc("V3", "V4");
	
	testGraph(g25);
}

void testTopSort2()
{
	string vex[] = {"", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "C10", "C11", "C12"};
	int n = 12;

	ALGraph<string> g27(vex + 1, n, true);
	string *p = vex;
	g27.insertArc(p + 1, p + 2);
	g27.insertArc(p + 1, p + 3);
	g27.insertArc(p + 1, p + 4);
	g27.insertArc(p + 1, p + 12);
	g27.insertArc(p + 2, p + 3);
	g27.insertArc(p + 3, p + 5);
	g27.insertArc(p + 3, p + 7);
	g27.insertArc(p + 3, p + 8);
	g27.insertArc(p + 4, p + 5);
	g27.insertArc(p + 5, p + 7);
	g27.insertArc(p + 6, p + 8);
	g27.insertArc(p + 9, p + 10);
	g27.insertArc(p + 9, p + 11);
	g27.insertArc(p + 9, p + 12);
	g27.insertArc(p + 10, p + 12);
	g27.insertArc(p + 11, p + 6);

	testGraph(g27);
}

void testTopSort3()
{
	string vex [] = {"", "V1", "V2", "V3", "V4", "V5", "V6"};
	int n = 6;

	ALGraph<string> g28(vex + 1, n, true);

	string *p = vex;
	g28.insertArc(p + 1, p + 2);
	g28.insertArc(p + 1, p + 3);
	g28.insertArc(p + 1, p + 4);
	g28.insertArc(p + 3, p + 2);
	g28.insertArc(p + 3, p + 5);
	g28.insertArc(p + 4, p + 5);
	g28.insertArc(p + 6, p + 4);
	g28.insertArc(p + 6, p + 5);

	testGraph(g28);
}
int main()
{
	
	test1();
	test2();

	test3();
	test4();
	test5();

	testTopSort();
	testTopSort2();
	testTopSort3();
	return 0;
}
