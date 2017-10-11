#include <vector>
#include "digraph.h"

using namespace std;

int main()
{
	vector<char> v{'A', 'B', 'C','D', 'E', 'F', 'I', 'G', 'H', 'I', 'J'};
	Digraph<char> g;

	for(char c : v)
		g.addVertex(c);
	
	g.addArc('A', 'B');
	g.addArc('A', 'D');
	g.addArc('B', 'C');
	g.addArc('B', 'F');
	g.addArc('C', 'A');
	g.addArc('C', 'D');
	g.addArc('C', 'E');
	g.addArc('D', 'E');
	g.addArc('F', 'C');
	g.addArc('G', 'F');
	g.addArc('G', 'H');
	g.addArc('H', 'F');
	g.addArc('H', 'J');
	g.addArc('I', 'H');
	g.addArc('J', 'I');
	
	g.printGraph();
	g.findStrongComponents();
	g.printStrongComponents();
}
