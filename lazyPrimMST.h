#ifndef LAZY_PRIM_MST_H
#define LAZY_PRIM_MST_H

#include <vector>
#include <queue>
#include <iostream>
#include <iomanip>

using namespace std;

template<typename WeightedGraph>
class LazyPrimMST
{
public:
	typedef typename WeightedGraph::weight_type WeightType;
	typedef typename WeightedGraph::FullEdge FullEdge;

	class EdgeCmp
	{
		bool reverse;
	public:
		EdgeCmp(bool reverse = false)
			:reverse(reverse)
		{}

		bool operator() (const FullEdge & lhs, const FullEdge & rhs) const
		{
			return reverse? rhs < lhs : lhs < rhs;
		}
	};

	LazyPrimMST()
	{
	}

	~LazyPrimMST()
	{}


	void findMST(const WeightedGraph & g)
	{
		mst.clear();
		marked.assign(g.V(), false);
		pq = priority_queue<FullEdge, vector<FullEdge>, EdgeCmp>(EdgeCmp(true));

		visit(g, 0);
		while(!pq.empty() && mst.size() < g.V() - 1)
		{
			FullEdge e = pq.top();
			pq.pop();

			if(/*marked[e.v] && */marked[e.w]) //ignore if both endpoints in mst
				continue;
			else
			{
				/*if(marked[e.v])
				{*/
					mst.push_back(e);
					visit(g, e.w); //because the way we contruct FullEdge when it is pushed into pq, we can be sure that e.v is marked and e.w is not
				/*}
				 * else
				 * {
				 * mst.push_bakc(e);
				 * visit(g. e.v);
				 * }*/
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
		for(int v = 0; v<mst.size(); v++) 
		{
			cout<<"("<<mst[v].v<<", "<<mst[v].w<<")"<<mst[v].weight<<endl;
			totalWeight += mst[v].weight;
		}
		cout<<"total weight: "<<totalWeight<<endl;
	}


private:
	vector<bool> marked;
	vector<FullEdge> mst;

	priority_queue<FullEdge, vector<FullEdge>, EdgeCmp> pq;

	void visit(const WeightedGraph & g, int v)
	{
		marked[v] = true;
		for(auto e: g.adj(v))
		{
			if(!marked[e.adjvex]) //neighbours of v which are not on the mst
			{
				pq.push(FullEdge(v, e.adjvex, e.weight));
			}
		}
	}
};


#endif
