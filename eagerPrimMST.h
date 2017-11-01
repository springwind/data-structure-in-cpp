#ifndef EAGER_PRIM_MST_H
#define EAGER_PRIM_MST_H

#include <vector>
#include <map>
#include <iostream>

#include "pairingHeap.h"

template<typename WeightedGraph>
class EagerPrimMST
{
	typedef typename WeightedGraph::weight_type WeightType;

public:
	typedef typename WeightedGraph::FullEdge FullEdge;

	EagerPrimMST()
	{}
	~EagerPrimMST()
	{}

	void findMST(const WeightedGraph & g)
	{
		mst.clear();
		marked.assign(g.V(), false);
		
		h.makeEmpty();
		pos.clear();

		visit(g, 0);		
		while(!h.isEmpty() && mst.size() < g.V() - 1)
		{
			FullEdge e = h.findMin();
			h.deleteMin();

			//add e to mst and update neignbours the newly joined vertex
			mst.push_back( e );
			pos.erase(e.w);

			visit(g, e.w); //e.v is on the mst and e.w not
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
	vector<bool> marked;

	PairingHeap<FullEdge> h;
	typedef typename PairingHeap<FullEdge>::Position PosType;
	map<int, PosType> pos;

	void visit(const WeightedGraph & g, int v)
	{
		marked[v] = true;
		for(auto e: g.adj(v)) //update neighbours path to mst
		{
			int w = e.adjvex;
			if(!marked[w]) //w is not on the mst yet
			{
				FullEdge fullEdge(v, e.adjvex, e.weight);
				if(pos.find(w) == pos.end()) // w is reachable from v
					pos[w] = h.insert(FullEdge(v, w, e.weight));
				else
				{
					if(fullEdge < h.retrieve(pos[w])) //w can be nearer to mst by passing from v
						h.decreaseKey(pos[w], fullEdge);
				}
			}
		}
	}
};

#endif
