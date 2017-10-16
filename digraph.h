#ifndef	DIGRAPH_H 
#define DIGRAPH_H

#include <vector>
#include <forward_list>
#include <map>
#include <limits>
#include <iostream>
#include <iomanip>
#include "pairingHeap.h"

using namespace std;

/**
 *Directed graph: 
 *	addVertex, 
 *	addArc, 
 *	findStrongComponents
 * */
template<typename VertexType>
class Digraph
{
public:
	Digraph()
	{
	}

	Digraph(const Digraph & rhs)
		:vexArr(rhs.vexArr)
	{
	}

	Digraph(Digraph && rhs)
		:vexArr(rhs.vexArr)
	{		
	}

	~Digraph()
	{
	}

	Digraph & operator= (const Digraph & rhs)
	{
		Digraph copy = rhs;
		std::swap(*this, copy);
		return rhs;
	}

	Digraph & operator= (Digraph && rhs)
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


	bool addArc(VertexType v, VertexType w)
	{
		if(vexMap.end() == vexMap.find(v) || vexMap.end() == vexMap.find(w))
			return false;
		int i = vexMap[v];
		int j = vexMap[w];

		vexArr[i].adj.push_front(j);
		return true;
	}

	void printGraph()
	{
		cout<<endl<<setw(6)<<"";
		for(int i = 0; i<vexArr.size(); i++)
		{
			cout<<setw(6)<<vexArr[i].element;
		}

		vector<bool> colArr(vexArr.size());
		for(int i = 0; i<vexArr.size(); i++)
		{
			colArr.assign(vexArr.size(), false);
			for(int w: vexArr[i].adj)
				colArr[w] = true;
			cout<<endl<<setw(6)<<vexArr[i].element;
			for(int j = 0; j<colArr.size(); j++)
				cout<<setw(6)<<colArr[j];
		}
		cout<<endl;
	}

	/*find the strongly connected components of the graph
	 * */
	void findStrongComponents()
	{
		//create the reversed graph
		createReversedGraph();

		// pass1: dfs traverse  the graph to get the finished number of 
		dfs_postTraverse();

		//pass2: dfs traverse the reversed graph 
		//(always starting a new depth-first search at unvisited vertex with the highest postorder number)
		dfs_reverseTraverse();
	}


	void findStrongComponents2()
	{
		scc.assign(vexArr.size(), -1);
		sccid = 0;
		visited.assign(vexArr.size(), false);
		
		dfsNum.assign(vexArr.size(), -1);
		low.assign(vexArr.size(), -1);
		count = 0;
		desc.clear();

		for(int v = 0; v<vexArr.size(); v++)
		{
			if(!visited[v])
			{
				dfs(v);
			}
		}

	}


	void printStrongComponents()
	{
		cout<<"strong components of the graph: "<<endl;
		
		map<int, vector<int> > m;
		for(int v = 0; v<scc.size(); v++)
		{
			int id = scc[v];
			m[id].push_back(v);
		}

		for(auto p : m)
		{
			cout<<endl;
			for(int v: p.second)
			{
				cout<<vexArr[v].element<<" ";
			}
		}

		cout<<endl;
	}




private:
	enum {NOT_A_VERTEX = -1};

	struct Vertex
	{
		VertexType element;
		forward_list<int> adj;

		Vertex (VertexType v)
			:element(v)
		{
		}
	};


	vector<Vertex> vexArr;
	map<VertexType, int> vexMap;
	
	//strongly connected components
	vector<int> scc;
	int sccid;

	//assist
	vector<Vertex> vexRArr; //reverse agencency list

	vector<bool> visited; //visited[v] tells whether vertex v is visited or not
	vector<int> finished; //finished[v] tells the postorder traversal number

	
	vector<int> desc; //the path of the dfs tree: from root to current node
	vector<int> dfsNum; //the dfs number array of all vertices
	vector<int> low;	//if w == low[v], w = min{u|<v, u> is back edge }
	int count;

	void dfs(int v)
	{
		visited[v] = true;
		low[v] = dfsNum[v] = ++count;

		desc.push_back(v);
		int pos = desc.size() - 1;

		for(int w: vexArr[v].adj)
		{
			if(!visited[w])
			{
				dfs(w);

				if(low[w] < low[v])
					low[v] = low[w];
			}
			else
			{
				if(NOT_A_VERTEX == scc[w])
				{
					low[v] = dfsNum[w];
				}
			}
		}

		if(low[v] == dfsNum[v])
		{
			for(int i = pos; i<desc.size(); i++)
			{
				int v = desc[i];
				scc[v] = sccid;
			}
			desc.erase(desc.begin() + pos, desc.end());
			sccid ++;
		}
	
	}

	void dfs_postTraverse()
	{
		visited.assign(vexArr.size(), false);
		finished.clear();

		for(int v = 0; v < vexArr.size(); v++)
		{
			if(!visited[v])
				dfs_post(v);
		}
	}

	void dfs_reverseTraverse()
	{
		visited.assign(vexArr.size(), false);
		scc.assign(vexArr.size(), -1);
		sccid = 0;

		for(int i = finished.size() - 1; i>=0; i--)
		{
			int v = finished[i];
			if(!visited[v])
			{
				dfs_reverse(v);
				sccid ++;
			}
		}
	}


	/*create reverse graph(reverse all edges)
	 *
	 * */
	void createReversedGraph()
	{
		vexRArr.clear();
		for(int v = 0; v<vexArr.size(); v++)
		{
			vexRArr.push_back(Vertex( vexArr[v].element));
		}
		for(int v = 0; v<vexArr.size(); v++)
		{
			for(int w: vexArr[v].adj)
				vexRArr[w].adj.push_front(v);
		}
	}


	/*depth-first search the graph, remember finish order
	 *
	 * */
	void dfs_post(int v)
	{
		visited[v] = true;

		for(int w: vexArr[v].adj)
		{
			if(!visited[w])
				dfs_post(w);
		}

		finished.push_back(v);

	}

	/*depth-first search of the reverse graph
	 *
	 * */
	void dfs_reverse(int v)
	{
		visited[v] = true;
		scc[v] = sccid;

		for(int w: vexRArr[v].adj)
		{
			if(!visited[w])
				dfs_reverse(w);
		}
	}

};

#endif
