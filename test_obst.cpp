#include "OBST.h"
#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<int> pi{22, 18, 20, 5, 25, 2, 8};

	OptimalBST<int> obst(pi);

	obst.printDP();
	obst.compute();
	obst.printDP();

	obst.printTree();

}
