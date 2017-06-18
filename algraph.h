#include <algorithm>
#include <iostream>
#include <queue>
#include <limits>

using namespace std;




template<typename VertexType >
class ALGraph
{
public:
	ALGraph(bool directed)
		:_vertices(nullptr), _vexNum(0), _vexCap(0), _directed(directed)
	{
	}

	ALGraph(VertexType vertices[], int vertexNum, bool directed)
		:_vexNum(0), _vexCap(0), _directed(directed)
	{
		_vertices = new VNode[vertexNum];
		_vexCap = vertexNum;
		for(int i = 0; i< vertexNum; i++)
		{
			_vertices[i].data = vertices[i];
			_vertices[i].firstarc = nullptr;
		}
		_vexNum = vertexNum;
	}

	ALGraph(const ALGraph &rhs)
		:_directed(rhs._directed)
	{
		clone(rhs);	
	}

	ALGraph(ALGraph && rhs)
		:_vertices(rhs._vertices), _vexNum(rhs._vexNum), _vexCap(rhs._vexCap), _directed(rhs._directed)
	{
		rhs._vertices = nullptr;
		rhs._vexNum = 0;
	}


	ALGraph & operator= (const ALGraph & rhs)
	{
		ALGraph copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	
	ALGraph & operator= (ALGraph && rhs)
	{
		std::swap(_vertices, rhs._vertices);
		std::swap(_vexCap, rhs._vexCap);
		std::swap(_vexNum, rhs._vexNum);

		return *this;
	}

	~ALGraph()
	{
		clear();
	}

	void clear()
	{
		if(_vertices)
		{
			deleteArcs();
			delete[] _vertices;
			_vertices = nullptr;
		}

		tops.clear();
		dfses.clear();
		paths.clear();
		visited.clear();

		_vexCap = 0;
		_vexNum = 0;
		_vexCapInc = 2;	
	}


	int getVexNum() const
	{
		return _vexNum;
	}

	//delete all arc of the graph
	void deleteArcs()
	{	
		if(_vertices)
		{
			for(int i = 0; i<_vexNum; i++)
			{
				ArcNode *p = _vertices->firstarc;
				while(p)
				{
					ArcNode *next = p->nextarc;
					delete p;
					p = next;
				}
				_vertices->firstarc = nullptr;
			}
		}
	}
	
	bool createGraph(VertexType v[], int vexnum, VertexType arc[][2], int arcnum)
	{
		clear();

		_vertices = new VNode[vexnum];
		_vexCap = vexnum;
		_vexNum = 0;
		
		for(int i = 0; i<vexnum; i++)
		{
			_vertices[i].data = v[i];
			_vertices[i].firstarc = nullptr;
			_vexNum ++;
		}

		for(int k = 0; k<arcnum; k++)
		{
			if(!insertArc(arc[k][0], arc[k][1]))
				return false;
		}

		return true;
	}

	//v's position
	int locateVex(VertexType v)
	{
		for(int i = 0; i<_vexNum; i++)
			if(_vertices[i].data == v)
				return i;
		return -1;
	}


	int firstAdjVexPos(VertexType vpos)
	{
		if(vpos >=0)
		{
			ArcNode * firstarc = _vertices[vpos].firstarc;
			if(firstarc)
			{
				return  firstarc->adjvex;
			}
		}
		
		return NOT_A_VERTEX;
	}

	int nextAdjVexPos(int vpos, int wpos)
	{
		if(vpos >=0 && wpos >=0)
		{
			ArcNode *parc = _vertices[vpos];
			while(parc && parc->adjvex != wpos)
			{
				parc = parc->nextarc;
			}//after while, parc is null or parc->adjvex == wpos

			if(parc && parc->nextarc)
				return parc->nextarc->adjvex;
			else
				return NOT_A_VERTEX;
		}
		return NOT_A_VERTEX;
	}

	//insert vertex v
	void insertVex(VertexType v)
	{
		if(_vexCap == _vexNum)
		{
			resize();
		}

		int i = _vexNum + 1;
		_vertices[i].data = v;
		_vertices[i].firstarc = nullptr;
	}

	void deleteVex(VertexType v)
	{
		cout<<"delete Vertex: not implemented"<<endl;	
	}


	//insert arc <*pv, *pw>
	bool insertArc(const VertexType * pv, const VertexType *pw)
	{
		int i = NOT_A_VERTEX, j = NOT_A_VERTEX;
		for(int k = 0; k<_vexNum &&(NOT_A_VERTEX == i || NOT_A_VERTEX == j); k++)
		{
			if(*pv == _vertices[k].data && NOT_A_VERTEX == i)
				i = k;
			if(*pw == _vertices[k].data && NOT_A_VERTEX == j)
				j = k;
		}

		if(i != NOT_A_VERTEX && j != NOT_A_VERTEX)
		{
			insertArc(i, j);
			if(!_directed)
				insertArc(j, i);
			return true;
		}

		return false;
	}

	//insert arc <v, w>
	bool insertArc(const VertexType & v, const VertexType & w)
	{
		int i = NOT_A_VERTEX, j = NOT_A_VERTEX;
		
		for(int k = 0; k<_vexNum && (NOT_A_VERTEX == i || NOT_A_VERTEX == j); k++)
		{
			if(v == _vertices[k].data)
			{
				i = k;
			}
			if(w == _vertices[k].data)
			{
				j = k;
			}
		}

		if(i != NOT_A_VERTEX && j != NOT_A_VERTEX)
		{
			insertArc(i, j);
			if(!_directed)
				insertArc(j, i);
			return true;
		}

		return false;
	}


/*	
	bool insertArc(VertexType v, VertexType w)
	{
		int i = NOT_A_VERTEX, j = NOT_A_VERTEX;
		
		for(int k = 0; k<_vexNum && (NOT_A_VERTEX == i || NOT_A_VERTEX == j); k++)
		{
			if(_vertices[k].data == v)
			{
				i = k;
			}
			if(_vertices[k].data == w)
			{
				j = k;
			}
		}

		if(i != NOT_A_VERTEX && j != NOT_A_VERTEX)
		{
			insertArc(i, j);
			if(!_directed)
				insertArc(j, i);
			return true;
		}

		return false;
	}
///*/

	//delete arc <v, w>
	void deleteArc(VertexType v, VertexType w)
	{
		int i = NOT_A_VERTEX, j = NOT_A_VERTEX;
		
		for(int k = 0; k<_vexNum && ( NOT_A_VERTEX == i || NOT_A_VERTEX == j); k++)
		{
			if(_vertices[k].data == v)
			{
				i = k;
			}
			else if(_vertices[k].data == w)
			{
				j = k;
			}
		}

		if(i != NOT_A_VERTEX && j != NOT_A_VERTEX)
		{
			deleteArc(i, j);
			if(!_directed)
				deleteArc(j, i);
		}
	}



	//depth-first traverse, start with the first vertex
	template<typename VisitFun>
	bool dfsTraverse(VisitFun vf) 
	{
		visited.reset(_vexNum);

		for(int v = 0; v < _vexNum; v++)
			if(!visited[v])
				if(!dfs(v, vf))
					return false;

		visited.clear();
		return true;
	}


	//breadth-first traverse, start with the first vertex
	template<typename VisitFun>
	bool bfsTraverse(VisitFun vf) 
	{
		visited.reset(_vexNum);

		bool succ = true;
		for(int u = 0; u < _vexNum; u++)
		{
			if(!(visited[u]))
			{
				if(!bfs(u, vf))
				{
					succ = false ;
					break;
				}
			}
		}

		return succ;
	}

	
	//matrix representation of the graph
	void printMatrix() const
	{
		const int HasEdge = 1;
		const int NOEdge = 0;
		cout<<"\t";
		for(int i = 0; i<_vexNum; i++)
		{
			cout<<_vertices[i].data<<"\t";
		}
		cout<<endl;


		int *cols = new int[_vexNum];
		for(int i = 0; i<_vexNum; i++)
		{	
			for(int j = 0; j<_vexNum; j++)
				cols[j] = NOEdge;

			cout<<_vertices[i].data<<"\t";

			ArcNode *pArc = _vertices[i].firstarc;
			while(pArc)
			{
				cols[pArc->adjvex] = HasEdge;
				pArc = pArc->nextarc;
			}

			for(int j = 0; j<_vexNum; j++)
				cout<<cols[j]<<"\t";
			cout<<endl;
		}

		delete[] cols;
	}

	
	//find the shortest unweighted path from s to every other vertex in the graph
	void unweightedShortestPath(int s)
	{
		paths.reset(s, _vexNum); 


		paths[s].dist = 0;
		
		queue<int> q;

		//start from s, to s's adjs, to adjs...
		q.push(s);
		while(!q.empty())
		{
			int v = q.front();
			q.pop();

			//v's adj
			for(ArcNode *pArc = _vertices[v].firstarc; pArc; pArc = pArc->nextarc)
			{
				int w = pArc->adjvex;
				if(paths.INFINITE == paths[w].dist)
				{
					paths[w].dist = paths[v].dist + 1;
					paths[w].path = v;

					q.push(w);
				}
			}
		}
	}

	
	void printShortestPath()
	{
		if(NOT_A_VERTEX == paths.start)
			return ;
		cout<<endl<<"shortest paths from "<<_vertices[paths.start].data<<endl;
		for(int i = 0; i<_vexNum; i++)
		{
			cout<<"to "<<_vertices[i].data<<":\t";
			if(paths[i].dist < paths.INFINITE)
			{
				cout<<paths[i].dist<<"\t";
				int u = i;
				for( ; paths[u].dist != 0; u = paths[u].path)
					cout<<_vertices[u].data<<"\t";
				cout<<_vertices[u].data<<endl;
			}
			else
				cout<<"INFINITE"<<endl;
		}
		cout<<endl;
	}

	//find articulation points of graph's connected component. dfs from v[start]. 
	void findArticul(const int start)
	{
		if(_directed)
		{
			return ;
		}

		dfses.reset(_vexNum);
		dfses[start].num = dfses[start].low = ++dfses.counter;
		
		ArcNode *p = _vertices[start].firstarc;
		int v = p->adjvex;
		dfsArticul(v);
		if(dfses.counter < _vexNum)
		{
			dfses[start].art = true;
			while(p->nextarc)
			{
				p = p->nextarc;
				v = p->adjvex;
				if(0 == dfses[v].num)
					dfsArticul(v);
			}
		}
		printArt();
	}

	//find articulation points of graph's connected component
	void findArticul2(const int start)
	{
		if(_directed)
		{
			return ;
		}

		dfses.reset(_vexNum);
		dfses[start].num = dfses[start].low = ++dfses.counter;
		
		ArcNode *p = _vertices[start].firstarc;
		int v = p->adjvex;
		dfsArticul2(v);
		if(dfses.counter < _vexNum)
		{
			dfses[start].art = true;
			while(p->nextarc)
			{
				p = p->nextarc;
				v = p->adjvex;

				if(0 == dfses[v].num)
				{
					dfses[v].parent = start;
					dfsArticul2(v);
				}
			}
		}

		printArt();
	}

	//display articulation points
	void printArt()
	{
		cout<<endl<<"articulation points are:\t";
		for(int i = 0; i<_vexNum; i++)
		{
			if(dfses[i].art)
				cout<<_vertices[i].data<<"\t";
		}
		cout<<endl;
	}

	//topological sort. display vertices in topoloical order
	bool topologicalSort()
	{
		if(!_directed)
			return false;
		tops.reset(_vexNum);
		for(int i = 0; i<_vexNum; i++)
		{
			for(ArcNode *p = _vertices[i].firstarc; p; p = p->nextarc)
			{
				int w = p->adjvex;
				tops[w].indegree ++;
			}
		}


		queue<int> q;
		for(int i = 0; i<_vexNum; i++)
		{
			if(0 == tops[i].indegree)
				q.push(i);
		}

		int counter = 0;
		while(!q.empty())
		{
			int v = q.front();
			q.pop();
			tops[v].topNum = ++counter; //assign next number

			for(ArcNode *p = _vertices[v].firstarc; p; p = p->nextarc)
			{
				int w  = p->adjvex;
				if(0 == -- tops[w].indegree)
					q.push(w);				
			}
		}

		if(counter < _vexNum)
			return false; //or throw

		for(int i = 0; i<_vexNum; i++)
		{
			int topNum = tops[i].topNum;
			tops[topNum - 1].vexNum = i; //topNum and vexNum is one to one mapping
		}

		printTopSort();
		return true;
	}


//assistent class
	
	//used for topological sort
	class Tops
	{
	public:

		struct TopSortData
		{
			int indegree = 0;
			int topNum = 0;
			int vexNum = 0; //vexNum has no relationship with topNum
		};

		~Tops()
		{
			clear();
		}
		
		void reset(int n)
		{
			clear();
			tops = new TopSortData [n];
			for(int i = 0; i<n; i++)
				tops[i].topNum = 0;
		}
		
		void clear()
		{
			if(tops)
				delete[] tops;
			tops = nullptr;
		}

		TopSortData & operator[] (int i)
		{
			return tops[i];
		}
	private:
		TopSortData *tops = nullptr;
	};


	//used for single-source shortest path
	class Paths
	{
	public:		
		struct PathInfo
		{
			int dist;
			int path;
		};


		~Paths()
		{
			clear();
		}

		void reset(int start, int n)
		{
			clear();

			paths = new PathInfo[n];
			for(int i = 0; i<n; i++)
			{
				paths[i].dist = INFINITE;
				paths[i].path = NOT_A_VERTEX;
			}

			this->start = start;
		}

		
		void clear()
		{
			if(paths)
				delete[] paths;
			paths = nullptr;

			start = NOT_A_VERTEX;
		}
	
		PathInfo & operator[] (int i)
		{
			return paths[i];
		}

		const int INFINITE = numeric_limits<int32_t>::max();
		int start= NOT_A_VERTEX;
	private:
		PathInfo *paths = nullptr;
	};

	//used for finding articulation points
	class Dfses
	{
	public:
		struct DfsNumInfo
		{
			int num; //preorder number, begin by 1. 0 means not ited
			int low; //the lowest numbered vertex, low(v), that is reachable from v by taking zero or more edges and then possibly one back edge
			int parent;
			bool art;
		};


		~Dfses()
		{
			clear();
		}

		void reset(int n)
		{
			clear();
			dfses = new DfsNumInfo[n];
			for(int i = 0; i<n; i++)
			{
				dfses[i].num = 0;
				dfses[i].low = 0;
				dfses[i].parent = NOT_A_VERTEX;
				dfses[i].art = false;
			}

			counter = 0;
		
		}

		void clear()
		{
			if(dfses)
				delete[] dfses;
			dfses = nullptr;
		}


		DfsNumInfo & operator[] (int i)
		{
			return dfses[i];
		}
		int counter = 0;

	private:
		DfsNumInfo *dfses = nullptr;
	};

	//used for depth-first traverse/ breath-first traverse
	class Visited
	{
	public:
		~Visited()
		{
			clear();
		}
		void reset(int n)
		{
			clear();
			visited = new bool[n];
			for(int i = 0; i<n; i++)
				visited[i] = false;
		}

		void clear()
		{
			if(visited)
				delete[] visited;
			visited = nullptr;
		}

		bool & operator[] (int i )
		{
			return visited[i];
		}

	private:
		bool *visited = nullptr;
	};

	Dfses dfses;
	Paths paths;
	Tops tops;
	Visited visited;

private:
	static const int NOT_A_VERTEX = -1;
	struct ArcNode
	{
		int			adjvex;
		ArcNode		*nextarc;
	};


	//vertex node
	struct VNode
	{
		VertexType 	data;
		ArcNode		*firstarc;
	};



	int _vexNum = 0;
	VNode *_vertices = nullptr;
	
	int _vexCap = 0; //_vertices capacity
	int _vexCapInc = 2;



	const bool _directed; //directed graph or undirected graph


	void initNodes(int vexNum)
	{
		if(_vertices)
			clear();
		_vertices = new VNode[vexNum];
		
		_vexNum = vexNum;
		_vexCap = vexNum;

		for(int i = 0; i<_vexNum; i++)
			_vertices[i].firstarc = nullptr;
	}


	void resize()
	{
		VNode *pnew = new VNode[_vexNum + _vexCapInc];
		memcpy(pnew, _vertices, sizeof(VNode) * _vexNum);
		delete[] _vertices;
		_vertices = pnew;

		_vexCapInc *=2;//next capacity increase size
	}

	template<typename VisitFun>
	bool dfs(int v, VisitFun vf)
	{
		if(vf(_vertices[v].data))
			visited[v] = true;
		else
			return false;

		for(ArcNode *arc = _vertices[v].firstarc; arc!= nullptr; arc = arc->nextarc)
		{
			int w = arc->adjvex;
			if(!visited[w])
				if(!dfs(w, vf))//one fail, all fail
					return false;
		}

		return true;
	}

	template<typename VisitFun>
	bool bfs(int v, VisitFun vf)
	{
		queue<int> q; //use a queue or a stack

		//it v
		if(!vf(_vertices[v].data))
			return false;

		visited[v] = true;
		q.push(v);

		//it v's adjs
		while(!q.empty())
		{
			//pop the first ited in the queue
			v = q.front();
			q.pop();

			//it u's adjs
			for(ArcNode *p = _vertices[v].firstarc; p; p = p->nextarc)
			{
				int w = p->adjvex;
				if(!visited[w])
				{
					if(!vf(_vertices[w].data))
						return false;

					visited[w] = true;
					q.push(w);
				}
			}
		}

		return true;
	}

	//insert arc <v[v], v[w]>
	void insertArc(int v, int w)
	{
		ArcNode *pArc = new ArcNode;
		pArc->adjvex = w;
		pArc->nextarc = _vertices[v].firstarc;
		_vertices[v].firstarc = pArc;
	}

	//delete arc <v[v], v[w]>
	void deleteArc(int v, int w)
	{
		ArcNode *arc = _vertices[v].firstarc;
		ArcNode *pre = nullptr;
		while(arc != nullptr && arc->adjvex != w)
		{
			pre = arc;
			arc = arc->nextarc;
		}

		if(arc)
		{
			if(pre)
				pre->nextarc = arc->nextarc;
			else
				_vertices[v].firstarc = arc->nextarc;
			delete arc;
		}
	}

	//clone vertices and arcs
	void clone(const ALGraph &g)
	{
		clear();
		
		//_directed = g._directed;

		_vertices = new VNode[g._vexNum];
		_vexCap = g._vexNum;
		for(int i = 0; i<g._vexNum; i++)
		{
			_vertices[i].data = g._vertices[i].data;
			_vertices[i].firstarc = nullptr;
			ArcNode *pArc = g._vertices[i].firstarc;
			ArcNode **pp = &_vertices[i].firstarc;
			while(pArc)
			{
				ArcNode *p = new ArcNode;
				p->adjvex = pArc->adjvex;
				p->nextarc = nullptr;

				*pp = p;
				pp = &p->nextarc;

				pArc = pArc->nextarc;
			}
			_vexNum ++;
		}
	}


	//dfs traverse the graph to find articulation points. treat edge to parent as back edge
	void dfsArticul(int v)
	{
		
		dfses[v].low = dfses[v].num = ++dfses.counter;

		for(ArcNode *p = _vertices[v].firstarc; p; p = p->nextarc)
		{
			int w = p->adjvex;
			if(0 == dfses[w].num )//w is not ited; forward edge <v, w>, 
			{
				dfsArticul(w);
				if(dfses[w].low < dfses[v].low)
					dfses[v].low = dfses[w].low;
				if(dfses[w].low >= dfses[v].num) //low(w) >= num(v), means v is an articulation point
				{
					dfses[v].art = true;;
				}
			}
			else //w is ited; w is v's parent or ancestor
			{
				if(dfses[w].num < dfses[v].low )
					dfses[v].low = dfses[w].num;
			}
		}
	}

	//dfs traverse the graph to find articulation points
	void dfsArticul2(int v)
	{
		dfses[v].low = dfses[v].num = ++dfses.counter;

		for(ArcNode *p = _vertices[v].firstarc; p; p = p->nextarc)
		{
			int w = p->adjvex;
			if(0 == dfses[w].num) // <v, w> forward edge
			{
				dfses[w].parent = v;

				dfsArticul2(w);
				if(dfses[w].low < dfses[v].low)
					dfses[v].low = dfses[w].low;
				if(dfses[w].low >= dfses[v].num) //Low(w) >= Num(v)
				{
					dfses[v].art = true;
				//	cout<<_vertices[v].data<<dfses[v].num<<"/"<<dfses[v].low<<"-"<<_vertices[w].data<<dfses[w].num<<"/"<<dfses[w].low<<endl;
				}
			}
			else
			{
				if(dfses[v].parent != w) //<v, w> back edge
					if(dfses[w].num < dfses[v].low)
						dfses[v].low = dfses[w].num;
			}
		}
	}

	void printTopSort()
	{
		cout<<endl<<"toplogical sort:"<<endl;
		for(int i = 0; i<_vexNum; i++)
		{
			int v = tops[i].vexNum;
			cout<<tops[v].topNum<<"--"<<_vertices[v].data<<"\t";
		}
		cout<<endl;
	}

};
