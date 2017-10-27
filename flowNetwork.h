#ifndef FLOW_GRAPH_H
#define FLOW_GAPH_H

#include <vector>
#include <forward_list>
#include <map>
#include <iostream>
#include <iomanip>

#include "pairingHeap.h"

/*
 *MAXIMUM FLOW PROBLEM
 *
 * */

class FlowNetwork
{
public:
	FlowNetwork(int V)
		:adj(V)
	{}
	~FlowNetwork()
	{}

	void addEdge(int v, int w, int cap)
	{
		adj[v].push_front(Edge(w, cap));
	}



	/*the maximum flow that can pass from s to t
	 * s is the souce and t is the sink
	 * */
	int findMaxFlow(int s, int t)
	{
		edgeMap.clear();
		for(int v = 0; v<adj.size(); v++)
		{
			for(auto itr = adj[v].begin(); itr != adj[v].end(); itr++)
			{
				int id = getEdgeId(v, itr->w);
				edgeMap[id] = itr;
			}
		}


		int maxflow = 0;

		while(hasAugmentingPath(s, t))
		{
			int bottle = edgeTo[t].flow;

			for(int w = t; w != s; )
			{
				int v = edgeTo[w].path;
				int id = getEdgeId(v, w);

				edgeMap[id]->flow += bottle;

				int id2 = getEdgeId(w, v);
				if(edgeMap.find(id2) == edgeMap.end())
				{
					Edge e(v, edgeMap[id]->cap);
					e.flow = e.cap;
					e.pushback = true;
					adj[w].push_front(e);
					edgeMap[id2] = adj[w].begin();
				}
				edgeMap[id2]->flow -= bottle;

				w = v;
			}

			maxflow += bottle;
		}
		return maxflow;
	}


	void printGraph(int digwidth = 4)
	{
		cout<<endl<<"graph matrix: "<<endl;
		cout<<setw(digwidth)<<"";
		for(int v= 0; v<adj.size(); v++)
			cout<<setw(digwidth)<<v;

		vector<int> col(adj.size(), 0);
		for(int v = 0; v<adj.size(); v++)
		{
			for(Edge e: adj[v])
			{
				col[e.w] = e.cap;
			}

			cout<<endl<<setw(digwidth)<<v;
			for(int w = 0; w < adj.size(); w++)
			{
				cout<<setw(digwidth)<<col[w];
			}
		}
		cout<<endl;
	}


	void printFlowGraph(int digwidth = 4)
	{
		cout<<endl<<"graph with max flow: "<<endl;
		cout<<setw(digwidth)<<"";
		for(int v= 0; v<adj.size(); v++)
			cout<<setw(digwidth*2 + 1)<<v;

		vector<int> col(adj.size(), 0);
		vector<int> colCap(adj.size(), 0);
		for(int v = 0; v<adj.size(); v++)
		{
			col.assign(adj.size(), 0);
			colCap.assign(adj.size(), 0);

			for(Edge e: adj[v])
			{
				if(!e.pushback)
				{
					col[e.w] = e.flow;
					colCap[e.w] = e.cap;
				}
			}

			cout<<endl<<setw(digwidth)<<v;
			for(int w = 0; w < adj.size(); w++)
			{
				if(colCap[w] > 0)
					cout<<setw(digwidth)<<col[w]<<"/"<<setw(digwidth)<<colCap[w];
				else
					cout<<setw(digwidth * 2+1)<<"";
			}
		}
		cout<<endl;

	}



private:
	static const int NOT_A_VERTEX = -1;

	struct Edge
	{
		int w;
		int cap;
		int flow;

		bool pushback;


		Edge(int w, int cap)
			:w(w), cap(cap), flow(0), pushback(false)
		{}

		int residual()
		{
			return cap  - flow;
		}
	};

	vector<forward_list<Edge> > adj;



	//aux data
	struct FlowEdge
	{
		int v;
		int flow;
		int path; //the last vertex cause the flow of the path from s to v to change

		FlowEdge(int v, int flow, int path)
			:v(v), flow(flow), path(path)
		{}

		FlowEdge()
			:v(NOT_A_VERTEX), path(NOT_A_VERTEX), flow(0)
		{}

		bool operator < (const FlowEdge & rhs)
		{
			return rhs.flow < this->flow;
		}
	};


	vector<FlowEdge> edgeTo;

	typedef typename forward_list<Edge>::iterator EdgeItrType;
	map<int, EdgeItrType> edgeMap; //inorder to find an edge quickly by vertex numbers

	/*
	 * */
	int getEdgeId(int v, int w)
	{
		return v * adj.size() + w;
	}



	/*find an augmenting path from s to t that allow the largest increase in flow
	 *use the algorithm similar to Dijstra's algorithm
	 * */
	bool hasAugmentingPath(int s, int t)
	{
		typedef typename PairingHeap<FlowEdge>::Position HeapPosType;
		PairingHeap<FlowEdge> h; //used to find the flow of the paths from s to the vertices we currently reach
		map<int, HeapPosType> posMap; //used when decreaseKey

		edgeTo.resize(adj.size());
		for(int v = 0; v<adj.size(); v++)
			edgeTo[v]  = FlowEdge(v, 0, NOT_A_VERTEX);

		for(Edge e: adj[s])
		{
			if(e.residual() > 0)
			{
				FlowEdge fe(e.w, e.residual(), s);
				posMap[fe.v] = h.insert(fe);
			}
		}


		while(!h.isEmpty())
		{
			FlowEdge maxfe = h.findMin(); //find the path with the largest residual flow
			edgeTo[maxfe.v] = maxfe; //the max flow from s to maxfe.v is found

			if(maxfe.v == t) //reach the sink, augmenting path found
				return true;

			posMap.erase(maxfe.v);
			h.deleteMin();

			for(Edge e : adj[maxfe.v])//check each adj vex, if the flow from s to this vertex is larger through this edge, the update
			{
				int residual = e.residual() < maxfe.flow ? e.residual() : maxfe.flow;
				if(residual > 0)
				{
					if(edgeTo[e.w].path == NOT_A_VERTEX)
					{
						FlowEdge fe(e.w, residual, maxfe.v);

						if(posMap.find(fe.v) == posMap.end())
						{
							posMap[fe.v] = h.insert(fe);
						}
						else
						{
							if(fe < h.retrieve(posMap[fe.v]))
								h.decreaseKey(posMap[fe.v], fe);
						}
					}
				}
			}
		}

		return false;
	}

};


	



#endif
