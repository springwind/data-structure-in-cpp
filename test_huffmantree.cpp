#include "huffmantree.h"
#include <iostream>
#include <iomanip>

using namespace std;

class VisitFun
{
	public:
		void visit(int i)
		{
			cout<<i<<"\t";
		}
};

void test(int w[], int n)
{
	HuffmanTree ht(w, n);

	ht.huffmanCoding_leaf();

	ht.traverse(VisitFun());
	cout<<endl<<"-----------------"<<endl;

	char ** code = ht.getHuffmanCode();
	for(int i = 0; i<n; i++)
	{
		cout<<setw(2)<<w[i]<<":\t"<<code[i]<<endl;
	}


}

int main()
{
	int w[] = {7, 5, 2, 4};
	int n = 4;
	int w2[] = {5, 29, 7, 8, 14, 23, 3, 11};
	int n2 = 8;
	test(w, n);
	cout<<"*****************"<<endl;
	test(w2, n2);
	return 0;
}
