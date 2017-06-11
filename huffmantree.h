#include <algorithm>
#include <vector>
#include <cstring>

class HuffmanTree
{
public:
	HuffmanTree(int *weights, int n)
	{
		createHuffmanTree(weights, n);

	}

	HuffmanTree()
	{
		makeEmpty();
	}

	char **getHuffmanCode()
	{
		return _codes + 1;
	}

	void huffmanCoding_leaf()
	{
		clearHuffmanCode();

		_codes = new char *[_leafCount + 1];
		char *code = new char[_leafCount];
		code[_leafCount - 1] = '\0';
		for(int i =1; i<=_leafCount; i++)
		{
			int start = _leafCount -1;
			for(int node = i, pa = _ht[node].parent; pa !=0; node = pa, pa = _ht[node].parent)
			{
				if(node == _ht[pa].left)
					code[--start] = LeftCode;
				else
					code[--start] = RightCode;
			}
			_codes[i] = new char[_leafCount-start];
			strcpy(_codes[i], &code[start]);
		}

		delete[] code;
	}


	void huffmanCoding_root()
	{
		enum  Dir{TurnLeft, TurnRight, BackToParent};
		
		clearHuffmanCode();
		_codes = new char *[_leafCount + 1];
		char *code = new char[_leafCount];

		for(int i = _leafCount+1; i<= _nodeCount; i++)
			_ht[i].weight = (int)TurnLeft;

		int p = _nodeCount;
		int codeLen = 0;
		while(p)
		{
			if(p <= _leafCount)
			{
				code[codeLen] = '\0';
				_codes[p] = new char[codeLen + 1];
				strcpy(_codes[p], code);

				codeLen--;
				p = _ht[p].parent;
			}
			else 
			{
				if((int)TurnLeft == _ht[p].weight)
				{
					_ht[p].weight = (int)TurnRight;
					if(_ht[p].left)
					{
						code[codeLen ++] = LeftCode;
						p = _ht[p].left;
					}					
				}
				else if((int)TurnRight == _ht[p].weight)
				{
					_ht[p].weight = BackToParent;
					if(_ht[p].right)
					{
						code[codeLen ++] = RightCode;
						p = _ht[p].right;
					}
				}
				else
				{
					codeLen --;
					p = _ht[p].parent;
				}
			}
		}

	}
	template<typename VisitFun>
	void traverse(VisitFun vf)
	{
		traverse(_nodeCount, vf);
	}



private:
	typedef char **HuffmanCode;
	struct HTNode
	{
		unsigned int weight = 0;
		int parent = 0;
		int left = 0;
		int right = 0;

		
	};

	HTNode *_ht = nullptr;
	int _leafCount = 0; //leafcount;
	int _nodeCount = 0; //huffman tree node count
	HTNode *_htree = nullptr;
	char ** _codes  = nullptr;

	const char LeftCode = '0';
	const char RightCode = '1';

	class CmpHtNode
	{
	public:
		bool operator() (const HTNode & lhs, const HTNode &rhs)
		{
			return lhs.weight < rhs.weight;
		}
		
	};

	void createHuffmanTree(int *wt, int n)
	{
		makeEmpty();
		std::sort(wt, wt +n);

		int N = 2 * n - 1;
		HTNode *htrees = new HTNode[N + 1]; //the 1st element not used


		HTNode *pt = htrees + 1;;
		int *pw = wt;
		for(int i = 1; i<=n; i++, pt++, pw ++)
		{
			pt->weight = *pw;
		}

//		std::sort(htrees + 1, htrees + n + 1, CmpHtNode());
		
		int first1 = 1;
		int last1 = n + 1;
		int first2 = n + 1;
		int last2 = n + 1;

		int s[2];
		//create huffman tree
		for(int i = n + 1; i<= N; i++)
		{
			last2 = i;
			select2(htrees, first1, last1, first2, last2, s);
			htrees[i].left = s[0];
			htrees[i].right = s[1];
			htrees[i].weight = htrees[s[0]].weight + htrees[s[1]].weight;
			htrees[s[0]].parent = i;
			htrees[s[1]].parent = i;
		}


		_ht = htrees;
		_htree = &htrees[N];
		_leafCount = n;
		_nodeCount = N;
	}

	void makeEmpty()
	{
		if(_ht)
			delete[] _ht;
		clearHuffmanCode();
	}

	void clearHuffmanCode()
	{
		if(_codes)
		{
			for(int i = 0; i<= _leafCount; i++)
				delete[] _codes[i];
		}
		delete[] _codes;
	}


	int select2(HTNode *htrees, int &first1, int last1, int &first2, int last2, int s[2])
	{
		int i = 0;
		while(i < 2 && first1 < last1 && first2 < last2)
		{
			if(htrees[first1].weight <= htrees[first2].weight)
				s[i++] = first1++;
			else
				s[i++] = first2++;
		}

		while(i < 2 && first1 < last1)
			s[i++] = first1++;
		while(i < 2 && first2 < last2)
			s[i++] = first2++;

		return 2 == i;
	}
		
	template<typename VisitFun>
	void traverse(int node, VisitFun vf)
	{
		if(node)
		{
			vf.visit(_ht[node].weight);
			traverse(_ht[node].left, vf);
			traverse(_ht[node].right, vf);		
		}
	}


};
