#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

template<typename ItemType>
class DP
{
public:
	DP(int N) //0, 1, 2, ..., N
		:_N(N)
	{
		_base = new ItemType[(N + 2)*(N+1)/2];
		_rows = new ItemType* [N + 1];
		
		_rows[0] = _base;
		int cnt = N+1;
		for(int i = 1; i<=N; i++)
		{
			_rows[i] = _base + cnt;
			cnt += N + 1 - i;
		}
	}

	~DP()
	{
		if(_base)
			delete[] _base;
		if(_rows)
			delete[] _rows;
	}

	
	ItemType & operator() (int i, int j)
	{
		return _rows[i][j - i];
	}

	const ItemType & operator() (int i, int j) const
	{
		return _rows[i][j-i];
	}
/*
	ItemType & operator() (int i, int j)
	{
		int n = i * ( _N + 1) - (i - 1) * i /2 + j - i;
		return *(_base + n);
	}
*/

	int getN()
	{
		return _N;
	}

private:

	ItemType *_base = nullptr;
	ItemType **_rows = nullptr;
	int _N = 0;
};

template<typename CostType>
class OptimalBST
{
public:
	struct DPItem
	{
		CostType	cost;
		int 		root;
	};

	OptimalBST(const vector<CostType> & pi)
		:_dp(pi.size()), _N(pi.size())
	{
		for(int i = 0; i<=pi.size(); i++)
		{
			_dp(i, i).cost = 0;
			_dp(i, i).root = -1;
		}

		for(int i = 0; i<_N; i++)
		{
			_dp(i, i+1).cost = pi[i];
			_dp(i, i+1).root = i + 1;
		}
		
	}

	~OptimalBST()
	{
	}


	//w(i,j) = sum p (i, j], the i'th p does not included
	void compute()
	{
		int N = _dp.getN();

		CostType w0d = _dp(0, 1).cost; //sum p(0, d]
		for(int d = 2; d<= N; d++)
		{
			w0d += _dp(d-1, d).cost;
			CostType w = w0d; //sum p(i, i+d]

			for(int i = 0; i+d <= N; i++)
			{
				int j = i + d;
				CostType minCost = INFINITY;
				int root = -1;
				int k1 = _dp(i, j-1).root;
				int k2 = _dp(i+1, j).root;
				for(int k = k1; k<= k2; k++)
				{
					CostType c = _dp(i, k-1).cost + _dp(k, j).cost;
					if(c <= minCost)
					{
						minCost = c;
						root = k;
					}
				}
				_dp(i, j).cost = w + minCost;
				_dp(i, j).root = root;

				w = w - _dp(i, i+1).cost + _dp(j, j+1).cost;
			}
		}
	}


	void printDP()
	{
		cout<<"-----------------------DP-------------------------"<<endl;
		cout<<"N = "<<_N<<endl<<endl;
		for(int i = 0; i<=_N; i++)
			cout<<setw(8)<<i;
		
		for(int i = 0; i<=_N; i++)
		{
			cout<<endl<<setw(4)<<i;
			for(int j = 0; j<i; j++)
				cout<<setw(8)<<"";
			for(int j = i; j<=_N; j++)
				cout<<setw(4)<<_dp(i, j).root<<"/"<<setw(3)<<_dp(i,j).cost;
		}
		cout<<endl;
	}

	void printTree()
	{
		getRoots(0, _N);
		cout<<"minimum tree cost is "<<_dp(0, _N).cost<<endl;
		cout<<"---------------Optimal Binary Search Tree (in preOrder)----------------"<<endl;
		for(auto x:_roots)
		{
			cout<<x<<" ";
		}
		cout<<endl;
	}

	void getRoots(int i, int j)
	{
		if(i < j)
		{
			int k = _dp(i, j).root;

			_roots.push_back(k);
			getRoots(i, k -1);
			getRoots(k, j);
		}
		else if(i == j)
			_roots.push_back(_dp(i, j).root);
	}

private:
	DP<DPItem> _dp ;
	vector<int> _roots;
	int _N = 0;
};
