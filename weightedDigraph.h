#ifndef WEIGHTED_ALG
#define WEIGHTED_ALG

#include <vector>
#include <forward_list>
#include <map>
#include <limits>
#include "pairingHeap.h"

using namespace std;


template<typename VertexType, typename DistType>
class WeightedDigraph
{
public:
	WeightedDigraph()
	{
	}

	WeightedDigraph(const WeightedDigraph & rhs)
		:vexArr(rhs.vexArr)
	{
	}

	WeightedDigraph(WeightedDigraph && rhs)
		:vexArr(rhs.vexArr)
	{		
	}

	~WeightedDigraph()
	{
	}

	WeightedDigraph & operator= (const WeightedDigraph & rhs)
	{
		WeightedDigraph copy = rhs;
		std::swap(*this, copy);
		return rhs;
	}

	WeightedDigraph & operator= (WeightedDigraph && rhs)
	{
		std::swap(vexArr, rhs.vexArr);
	}


	void addVertex(VertexType v)
	{
		if(vexMap.end() == vexMap.find(v))
		{
			vexArr.push_back(Vertex(v));
			vexMap[v] = vexArr.size() - 1;
		}
	}

	bool addArc(VertexType v, VertexType w, DistType d)
	{
		if(vexMap.end() == vexMap.find(v) || vexMap.end() == vexMap.find(w))
			return false;
		int i = vexMap[v];
		int j = vexMap[w];

		vexArr[i].adj.push_front(Arc(j, d));
		return true;
	}


	/*Single-Source Shortest-Path Problem
	 *Dijstra algrithm
	 *using pairing heap
	 * */
	void Dijstra(VertexType s)
	{
		PairingHeap<PathToVertex> h;
		typedef typename PairingHeap<PathToVertex>::Position PosType;

		map<int, PosType > posMap;

		pathArr.resize(vexArr.size());
		for(int i = 0; i<pathArr.size(); i++)
		{
			pathArr[i].vex = i;
			pathArr[i].path = NOT_A_VERTEX;
			pathArr[i].dist = INFINITE;
		}

		int is = vexMap[s];
		pathArr[is].dist = 0;

		posMap[is] = h.insert(PathToVertex(is, 0, NOT_A_VERTEX));

		while(!h.isEmpty())
		{
			PathToVertex min = h.findMin();
			pathArr[min.vex] = min;

			h.deleteMin();
			posMap.erase(min.vex);

			for(auto a : vexArr[min.vex].adj)
			{
				if(pathArr[a.adjvex].dist == INFINITE)
				{
					if(posMap.end() == posMap.find(a.adjvex))
					{
						posMap[a.adjvex] = h.insert(PathToVertex(a.adjvex, min.dist + a.cost, min.vex));
					}
					else
					{
						PosType p = posMap[a.adjvex];
						if(min.dist + a.cost < h.retrieve(p).dist)
							h.decreaseKey(p, PathToVertex(a.adjvex, min.dist + a.cost, min.vex));
					}
				}
			}
		}
	}


	void printPath(VertexType v)
	{
		printPath(vexMap[v]);
	}


	DistType getDist(VertexType v)
	{
		return pathArr[vexMap[v]].dist;
	}

	struct PathToVertex
	{
		int vex;
		DistType dist;
		int path;

		PathToVertex()
		{}
		PathToVertex(int v, DistType d, int p)
			:vex(v), dist(d), path(p)
		{}

		bool operator< (const PathToVertex & rhs)
		{
			return dist < rhs.dist;
		}
	};


	enum {NOT_A_VERTEX = -1};
private:
	static const int INFINITE = std::numeric_limits<DistType>::max();
	struct Arc
	{
		int adjvex;
		DistType cost; 

		Arc(int adjv, const DistType &c)
			:adjvex(adjv), cost(c)
		{}

		Arc(int adjv, DistType && c)
			:adjvex(adjv), cost(std::move(c))
		{}
	};

	struct Vertex
	{
		VertexType element;
		forward_list<Arc> adj;

		Vertex (VertexType v)
			:element(v)
		{
		}
	};


	vector<Vertex> vexArr;
	map<VertexType, int> vexMap;
	vector<PathToVertex> pathArr;

	void printPath(int v)
	{
		if(v != NOT_A_VERTEX)
		{
			printPath(pathArr[v].path);
			cout<<" to ";
			cout<<vexArr[v].element;
		}
	}

};

#endif
