#ifndef KRUSKAL_MST_H
#define KRUSKAL_MST_H

#include <vector>
#include <queue>
#include "disjSets.h"

using namespace std;

template<typename WeightedGraph>
class KruskalMST
{
public:
	typedef typename WeightedGraph::FullEdge FullEdge;
	typedef typename WeightedGraph::weight_type WeightType;

	KruskalMST()
	{}
	~KruskalMST()
	{}

	class EdgeCmp
	{
		bool reverse;
	public:
		EdgeCmp(bool reverse)
			:reverse(reverse)
		{}

		bool operator() (const FullEdge &lhs, const FullEdge &rhs)
		{
			return reverse? rhs < lhs : lhs < rhs;
		}
	};

	void findMST(const WeightedGraph &g)
	{
		mst.clear();
		DisjSets s(g.V());

		priority_queue<FullEdge, vector<FullEdge>, EdgeCmp> pq(EdgeCmp(true));
		//all edges are candidate edges
		for(int v = 0; v < g.V(); v++)
		{
			for(auto e : g.adj(v))
			{
				if(v < e.adjvex)
					pq.push(FullEdge(v, e.adjvex, e.weight));
			}
		}

		while(!pq.empty() && mst.size() < g.V() - 1)
		{
			FullEdge e = pq.top();
			pq.pop();

			int s1 = s.find(e.v);
			int s2 = s.find(e.w);
			if(s1 != s2)  //if no circle will form
			{
				mst.push_back(e);
				s.unionSets(s1, s2);
			}
		}

	}

	const vector<FullEdge> & getMST()
	{
		return mst;
	}

	void printMST()
	{
		WeightType totalWeight = 0.0;
		for(int v = 0; v<mst.size(); v++) //tbc
		{
			cout<<"("<<mst[v].v<<", "<<mst[v].w<<")"<<mst[v].weight<<endl;
			totalWeight += mst[v].weight;
		}
		cout<<"total weight: "<<totalWeight<<endl;
	}

private:
	vector<FullEdge> mst;
};

#endif
