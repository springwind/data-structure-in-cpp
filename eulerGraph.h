#ifndef EULER_GRAPH_H
#define EULER_GRAPH_H

#include <vector>
#include <forward_list>
#include <iostream>
#include <iomanip>

using namespace std;

class EulerGraph
{
public:

	EulerGraph(int V)
		:vexArr(V)
	{

	}

	void addEdge(int v, int w)
	{
		vexArr[v].push_front(Edge(w, edgeid));
		vexArr[w].push_front(Edge(v, edgeid));
		edgeid++;
	}

	/*find euler circuit of the graph, 
	 * s is the starting point
	 * */	
	void findEulerCircuit(int s)
	{
		path.clear();
		
		nextEdgeArr.resize(vexArr.size());
		for(int v=0; v<vexArr.size(); v++)
		{
			nextEdgeArr[v] = vexArr[v].begin();
		}

		visited.assign(edgeid, false);

		currPath.clear();
		curritr = currPath.before_begin();
		
		dfs(s);
		path.splice_after(path.before_begin(), currPath,  currPath.before_begin(), currPath.end()); //splice all elements of currPath into path

		for(PathItr it = path.begin(); it != path.end(); it++)
		{
			int w = *it;
			while(nextEdgeArr[w] != vexArr[w].end() && visited[nextEdgeArr[w]->id]) //find next splice point
				nextEdgeArr[w]++;
			if(nextEdgeArr[w] != vexArr[w].end())
			{
				currPath.clear();
				curritr = currPath.before_begin();
				dfs(w);	//find next path
				path.splice_after(it, currPath,  currPath.begin(), currPath.end()); //splice all elements but the first one of currPath into path
			}
		}

		//check if all edges visited
		for(int v = 0; v<vexArr.size(); v++)
		{
			if(nextEdgeArr[v] != vexArr[v].end()) //some edges not visited, there's no euler circuit
			{
				path.clear();
				return;
			}
		}

	}


	//print euler circuit path
	void printPath()
	{
		for(int v: path)
		{
			cout<<v<<" ";
		}
		cout<<endl;
	}

	//print matrix representation of graph
	void printGraphMatrix()
	{
		cout<<setw(4)<<"";
		for(int v = 0; v < vexArr.size(); v++)
			cout<<setw(4)<<v;
		cout<<endl;

		vector<bool> colArr;
		for(int v = 0; v<vexArr.size(); v++)
		{
			cout<<setw(4)<<v;
			colArr.assign(vexArr.size(), false);
			for(Edge e: vexArr[v])
			{
				colArr[e.vex] = true;
			}

			for(bool w: colArr)
			{
				if(w)
					cout<<setw(4)<<1;
				else
					cout<<setw(4)<<"";
			}
			cout<<endl;
		}
		cout<<endl;
	}


private:
	struct Edge
	{
		int vex;
		int id;

		Edge(int v, int id)
			:vex(v), id(id)
		{}
	};

	vector<forward_list<Edge> > vexArr;
	int edgeid = 0;

	//used to find euler circuit
	typedef typename forward_list<Edge>::iterator EdgeItr;
	vector<EdgeItr> nextEdgeArr;

	forward_list<int> path;
	forward_list<int> currPath;
	typedef typename forward_list<int>::iterator PathItr;
	PathItr curritr;

	vector<int> visited; //whether edge visited or not

	
	void dfs(int v)
	{
		curritr = currPath.insert_after(curritr, v);

		while(nextEdgeArr[v] != vexArr[v].end() && visited[nextEdgeArr[v]->id])
			nextEdgeArr[v]++;
		if(nextEdgeArr[v] != vexArr[v].end())
		{
			int eid = nextEdgeArr[v]->id;
			visited[eid] = true;
			int w = nextEdgeArr[v]->vex;
			nextEdgeArr[v]++;
			dfs(w);
		}
	}
};


#endif
