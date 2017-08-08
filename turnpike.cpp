#include <algorithm>
#include <set>
#include <vector>
#include <iostream>
#include <iomanip>
#include <map>


using namespace std;


/*	The Turnpike Reconstruction Problem
 *	Given distances of every pair of N points. Construct a point set from these distances.
 *
 * */


/* backtracing algorithm to place the points x[left].. x[right]
 * 
 * x[1]..x[left - 1] and x[right + 1]..x[n] already tentatively placed
 * if place return true, then x[left]..x[right] will have values.
 * */
bool place(vector<int> & x, multiset<int> & d, int n, int left, int right)
{	
	if(d.empty())
		return true;
	
	bool found = false;
	
	int dmax = *(--d.end());

	bool exist = true;

	int coord = dmax;

	map<int, int> m;


	for(int i = left - 1; i>=1; i--)
		++m[coord - x[i]];
	for(int i = right + 1; i<=n; i++)
		++m[x[i]-coord];
	for(auto it = m.begin(); exist && it != m.end(); it++)
	{
		auto ret = d.equal_range(it->first);
		int cnt = it->second;
		for(auto i = ret.first; i != ret.second && cnt > 0; i++, cnt--)
			;
		if(cnt > 0)
			exist = false;
	}


	if(exist)//distances to determined coords exists
	{
		x[right] = coord;
		for(int i = 1; i<=left - 1; i++)
			d.erase(d.find(coord - x[i]));
		for(int i = right + 1; i<=n; i++)
			d.erase(d.find(x[i] - coord));
	
		found = place(x, d, n, left, right - 1);
		if(!found)
		{
			for(int i = 1; i<= left - 1; i++)
				d.insert(coord - x[i]);
			for(int i = right + 1; i<=n; i++)
				d.insert(x[i] - coord);
		}
	}

	if(!found )
	{
		exist = true;
		
		coord = x[n] - dmax;
		m.clear();

		for(int i = left - 1; i>= 1; i--)
			++m[coord-x[i]];
		for(int i = right + 1; i<=n; i++)
			++m[x[i] - coord];

		for(auto it = m.begin(); it != m.end() && exist; it++)
		{
			auto ret = d.equal_range(it->first);
			int cnt = it->second;
			for(auto i = ret.first; i!= ret.second && cnt >0; i++, cnt--)
				;
			if(cnt > 0)
				exist = false;
		}

		if(exist)
		{
			x[left] = coord;
			for(int i = 1; i<= left - 1; i++)
				d.erase(d.find(coord - x[i]));
			for(int i = right + 1; i<=n; i++)
				d.erase(d.find(x[i] - coord));

			found = place(x, d, n, left + 1, right);

			if(!found)
			{
				for(int i = 1; i<=left - 1; i++)
					d.insert(coord - x[i]);
				for(int i = right + 1; i<=n; i++)
					d.insert(x[i] - coord);
			}
		}
	}

	return found;

}

//turnpike reconstruction algorithm: driver routine
bool turnpike(vector<int> & x, multiset<int> & d, int n)
{
	x[1] = 0;

	auto itr = d.end();
	x[n] = *(--itr);
	d.erase(itr);

	itr = d.end();
	x[n-1] = *(--itr);
	d.erase(itr);

	if(d.end() != (itr = d.find(x[n]-x[n-1])))
	{
		d.erase(itr);
		return place(x, d, n, 2, n - 2);
	}
	else
		return false;
}


void test1()
{
	int dists[] = {1, 2, 2, 2, 3, 3, 3, 4, 5, 5, 5, 6, 7, 8, 10};
	multiset<int> d(dists, dists + 15);

	vector<int> x(7);
	bool found = turnpike(x, d, 6);

	cout<<endl<<"------------------------test1----------------------------"<<endl;
	cout<<"distances are: "<<endl;
	for(auto d: dists)
		cout<<setw(4)<<d;
	cout<<endl;

	
	if(found)
	{
		cout<<"coords found: "<<found<<endl;
		cout<<"x-coords are: "<<endl;
		for(int i = 1; i<=6; i++)
			cout<<setw(4)<<x[i];
		cout<<endl;
	}
	else
		cout<<"coords not found"<<endl;

}

void test2()
{
	int N = 10;
	int n = N + 1;
	int coords[11];

	for(int i = 1, j = 0; i <= N; i++, j+=71)
		coords[i] = j%11;

	sort(coords + 1, coords + n);

	multiset<int> d1;
	for(int i = 1; i<= N; i++)
		for(int j = i + 1; j<= N; j++)
			d1.insert(coords[j] - coords[i]);
	


	cout<<endl<<"---------------------test2---------------------------------"<<endl;
	cout<<"original coords are:"<<endl;
	for(int i = 1; i<= N; i++)
		cout<<setw(4)<<coords[i];
	cout<<endl;

	cout<<"distances are:"<<endl;
	for(auto d: d1)
		cout<<setw(4)<<d;
	cout<<endl;
	
	multiset<int> d2(d1);
	vector<int> x(n);
	bool found = turnpike(x, d2, N);


	if(found)
	{
		cout<<"coords computed by turnpike:"<<endl;
		for(int i = 1; i<=N; i++)
			cout<<setw(4)<<x[i];
		cout<<endl;

		cout<<"distance of these coords are:"<<endl;
		multiset<int> d3;
		for(int i = 1; i<=N; i++)
			for(int j = i + 1; j<= N; j++)
				d3.insert(x[j] - x[i]);

		for(auto d: d3)
			cout<<setw(4)<<d;
		
		for(auto it1 = d1.begin(), it2 = d3.begin(); it1 != d1.end() && it2 != d3.end(); it1++, it2++)
			if(*it1 != *it2)
				cout<<"error:\t"<<setw(4)<<*it1<<setw(4)<<*it2<<endl;
		cout<<endl;

	}
	else
		cout<<"coords not found"<<endl;
}

int main()
{
	test1();
	test2();
}




