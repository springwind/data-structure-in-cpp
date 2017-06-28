#include <algorithm>
#include <iostream>
#include <queue>
#include <limits>
#include <stack>

using namespace std;

class DisjSets
{
public:
	explicit DisjSets(int numElements)
		:s(numElements)
	{
		s.assign(numElements, -1);
	}

	int find(int x) const
	{
		if(x < 0 || x >= s.size()) //x illegal
			return -1;

		if(s[x] < 0)
			return x;
		else
			return find(s[x]);
	}

	void unionSets(int root1, int root2)
	{
		if(s[root1] >=0)
			root1 = find(root1);
		if(s[root2] >=0)
			root2 = find(root2);

		if(s[root2] < s[root1])
			s[root1] = root2;
		else
		{
			if(s[root2 ] == s[root1])
				s[root1]--;
			s[root2] = root1;
		}
	}
private:
	vector<int> s;
};


template<typename VertexType , typename CostType>
class WeightedALGraph
{
public:
	WeightedALGraph(bool directed)
		:_vertices(nullptr), _vexNum(0), _vexCap(0), _directed(directed)
	{
	}

	WeightedALGraph(VertexType vertices[], int vertexNum, bool directed)
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

	WeightedALGraph(const WeightedALGraph &rhs)
		:_directed(rhs._directed)
	{
		clone(rhs);	
	}

	WeightedALGraph(WeightedALGraph && rhs)
		:_vertices(rhs._vertices), _vexNum(rhs._vexNum), _vexCap(rhs._vexCap), _directed(rhs._directed)
	{
		rhs._vertices = nullptr;
		rhs._vexNum = 0;
	}


	WeightedALGraph & operator= (const WeightedALGraph & rhs)
	{
		WeightedALGraph copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	
	WeightedALGraph & operator= (WeightedALGraph && rhs)
	{
		std::swap(_vertices, rhs._vertices);
		std::swap(_vexCap, rhs._vexCap);
		std::swap(_vexNum, rhs._vexNum);

		return *this;
	}

	~WeightedALGraph()
	{
		clear();
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
	bool insertArc(const VertexType * pv, const VertexType *pw, CostType c)
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
			insertArc(i, j, c);
			if(!_directed)
				insertArc(j, i, c);
			return true;
		}

		return false;
	}

	//insert arc <v, w>
	bool insertArc(const VertexType & v, const VertexType & w, CostType c)
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
			insertArc(i, j, c);
			if(!_directed)
				insertArc(j, i, c);
			return true;
		}

		return false;
	}


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
		visited.assign(_vexNum, false);

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
		visited.assign(_vexNum, false);

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
		cout<<endl<<"-----------------graph matrix------------------"<<endl;
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
				cols[j] = INFINITE;

			cout<<_vertices[i].data<<"\t";

			ArcNode *pArc = _vertices[i].firstarc;
			while(pArc)
			{
				cols[pArc->adjvex] = pArc->cost;
				pArc = pArc->nextarc;
			}

			for(int j = 0; j<_vexNum; j++)
			{
				if(cols[j] < INFINITE)
					cout<<cols[j]<<"\t";
				else
					cout<<"-"<<"\t";
			}
			cout<<endl;
		}

		delete[] cols;
	}

	
	//find the shortest unweighted path from s to every other vertex in the graph
	void unweightedShortestPaths(int s)
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
				if(INFINITE == paths[w].dist)
				{
					paths[w].dist = paths[v].dist + 1;
					paths[w].path = v;

					q.push(w);
				}
			}
		}
	}


	void dijkstra(int s)
	{
		paths.reset(s, _vexNum);

		paths[s].dist = 0;

		for( ; ; )
		{
			int v = paths.selectSmallestUnknown();
			if(NOT_A_VERTEX == v)
				break;

			paths[v].known = true;

			//update v's adjs distance
			for(ArcNode *p = _vertices[v].firstarc; p; p = p->nextarc)
			{
				int w = p->adjvex;
				if(!paths[w].known)
				{
					if(paths[v].dist + p->cost < paths[w].dist)
					{
						paths[w].dist = paths[v].dist + p->cost;
						paths[w].path = v;
					}
				}
			}
		}
	}
	
	void printShortestPaths()
	{
		if(NOT_A_VERTEX == paths.start)
			return ;
		cout<<endl<<"-------------------shortest path-------------------"<<endl;
		cout<<endl<<"shortest paths from "<<_vertices[paths.start].data<<endl;
		for(int i = 0; i<_vexNum; i++)
		{
			cout<<"to "<<_vertices[i].data<<":\t";
			if(paths[i].dist <INFINITE)
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


	void allPairsShortestPath()
	{
		allpairs.reset(_vexNum);
		for(int v = 0; v<_vexNum; v++)
		{
			for(ArcNode *p = _vertices[v].firstarc; p; p = p->nextarc)
			{
				int w = p->adjvex;
				allpairs.D[v][w] = p->cost;
			}
		}

		for(int k = 0; k < _vexNum; k++)
		{
			for(int i = 0; i<_vexNum; i++)
				for(int j = 0; j<_vexNum; j++)
				{
					if( i != k && j != k)
						if(allpairs.D[i][k] < INFINITE && allpairs.D[k][j] < INFINITE)
							if(allpairs.D[i][k] + allpairs.D[k][j] < allpairs.D[i][j])
							{
								allpairs.D[i][j] = allpairs.D[i][k] + allpairs.D[k][j];
								allpairs.P[i][j] = k;
							}
				}
		}
	}


	void printAllPairsShortestPath()
	{
//		return ;
		//allpairs.reset(_vexNum);
//		return ;
//
		cout<<endl<<"-------------------all pair shortest paths:------------------------"<<endl;


		for(int i = 0; i<_vexNum; i++)
		{
			cout<<endl;
			cout<<"from: "<<_vertices[i].data<<endl;
			for(int j = 0; j<_vexNum; j++)
			{
				cout<<"to: "<<_vertices[j].data<<"\t";
				if(allpairs.D[i][j] < INFINITE)
				{
					cout<<allpairs.D[i][j] <<endl<<"\t";
					allpairs.computePath(i, j);

					const vector<int> & path = allpairs.apath;
					for(int idx = 0; idx < _vexNum && path[idx] != NOT_A_VERTEX ; idx++)
						cout<<path[idx]<<"\t";
					cout<<endl<<"\t";
					int k = NOT_A_VERTEX;
					for(int i = 0; i<_vexNum; i++)
					{
						k = path[i];
						if(NOT_A_VERTEX == k)
							break;
						cout<<_vertices[k].data<<"\t";
					}
					for(int idx = 0, k = path[idx]; idx < _vexNum && k != NOT_A_VERTEX  ; k = path[++idx]) 
						cout<<_vertices[k].data<<"\t";
				}
				else
					cout<<"INFINITE";
				cout<<endl;
			}
		}

//		allpairs.printPath();
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

	void findIndegrees()
	{
		if(!_directed)
			return ;

		indegrees.assign(_vexNum, 0);


		for(int i = 0; i<_vexNum; i++)
		{
			for(ArcNode *p = _vertices[i].firstarc; p; p = p->nextarc)
			{
				int w = p->adjvex;
				indegrees[w]++;
			}
		}
		
	}


	void criticalPath()
	{
		if(!_directed)
			return ;

		topologicalSort();	
		if(topInfo.foundCycle)
			return ;

		//compute earliest completion time by topological order
		eventInfo.ve.assign(_vexNum, 0);
		for(int i = 0; i< _vexNum; i++)
		{
			int v = topInfo.sorted[i];
			for(ArcNode *p = _vertices[v].firstarc; p; p = p->nextarc)
			{
				int w = p->adjvex;
				
				if(eventInfo.ve[w] < eventInfo.ve[v] + p->cost)
					eventInfo.ve[w] = eventInfo.ve[v] + p->cost;
			}
		}

		//compute latest completion time by reverse topological order

		eventInfo.vl.assign(_vexNum, eventInfo.ve[_vexNum - 1]);
		for(int i = _vexNum -1; i>=0; i--)
		{
			int v = topInfo.sorted[i];
			for(ArcNode *p = _vertices[v].firstarc; p; p = p->nextarc)
			{
				int w = p->adjvex;
				if(eventInfo.vl[w] - p->cost < eventInfo.vl[v])
					eventInfo.vl[v] = eventInfo.vl[w] - p->cost;
			}
		}
	}


	void printActivies()
	{
		cout<<endl<<"-----------------------activies--------------------"<<endl;
		cout<<"act\tcost\tEC\tLC\tslack\tcritical"<<endl;

		for(int i = 0; i<_vexNum; i++)
		{
			for(ArcNode *p = _vertices[i].firstarc; p; p= p->nextarc)
			{
				int j = p->adjvex;
				CostType ee = eventInfo.ve[i];
				CostType el = eventInfo.vl[j] - p->cost;
				CostType slack = el - ee;
				cout<<"<"<<_vertices[i].data<<","<<_vertices[j].data<<">\t"<<p->cost<<"\t"<<ee<<"\t"<<el<<"\t"<<slack;
				if(0 == slack)
					cout<<"\t*";
				cout<<endl;
			}
		}
	}

	void printCriticalActivities()
	{
		cout<<endl<<"---------------critical activities------------------"<<endl;
		
		cout<<"event nodes that earlist completion time  == latest completion time"<<endl;
		for(int i= 0; i<_vexNum; i++)
		{
			if(eventInfo.vl[i] == eventInfo.ve[i])
				cout<<_vertices[i].data<<"\t";
		}

		cout<<endl<<"critical activities:"<<endl;
		cout<<"act\tcost\tEC\tLC\tslack"<<endl;
		for(int i = 0; i<_vexNum; i++)
		{
			if(eventInfo.ve[i] == eventInfo.vl[i]) 
			{
				for(ArcNode *p = _vertices[i].firstarc; p; p = p->nextarc)
				{
					int j = p->adjvex;
					if(eventInfo.ve[j] == eventInfo.vl[j])
					{
						CostType earliestStart = eventInfo.ve[i];
						CostType latestStart = eventInfo.vl[j] - p->cost;
						CostType slack = eventInfo.vl[j] - eventInfo.ve[i] - p->cost ;
						if(0 == slack)
							cout<<"<"<<_vertices[i].data<<","<<_vertices[j].data<<">\t"<<p->cost<<"\t"<<earliestStart<<"\t"<<latestStart<<"\t"<<slack<<endl;
					}
				}
			}
		}
		
		cout<<endl;


	}



	//topological sort. display vertices in topoloical order
	bool topologicalSort()
	{
		if(!_directed)
			return false;
		topInfo.reset(_vexNum);

		findIndegrees();

		queue<int> q;
		for(int i = 0; i<_vexNum; i++)
		{
			if(0 == indegrees[i])
				q.push(i);
		}

		int counter = 0;
		while(!q.empty())
		{
			int v = q.front();
			q.pop();
			topInfo.tops[v] = ++counter; //assign next number

			for(ArcNode *p = _vertices[v].firstarc; p; p = p->nextarc)
			{
				int w  = p->adjvex;
				if(0 == -- indegrees[w])
					q.push(w);				
			}
		}

		if(counter < _vexNum)
			return false; //or throw
		
		topInfo.sortVertices();

		return true;
	}

	void printTopSort()
	{
		cout<<endl<<"----------------topological sort------------------"<<endl;
		if(topInfo.foundCycle)	
		{
			cout<<"cycle found in the graph"<<endl;
			return ;//not top sorted
		}
		cout<<endl<<"vertices' topological number"<<endl;
		cout<<"V:\t";
		for(int i = 0 ; i<_vexNum; i++)
			cout<<_vertices[i].data<<"\t";
		cout<<endl;
		cout<<"NO.:\t";
		for(int i = 0; i<_vexNum; i++)
			cout<<topInfo.tops[i]<<"\t";

		cout<<endl<<"vertices in topological order:"<<endl;

		for(int i = 0; i<_vexNum; i++)
			cout<<i<<"\t";
		cout<<endl;
		for(int i = 0; i<_vexNum; i++)
		{
			int v = topInfo.sorted[i];
			cout<<_vertices[v].data<<"\t";
		}
		cout<<endl;
	}


	void mst_Prim()
	{
		if(_directed)
			return ; //not implemented

		//assume the graph is connected
		if(!prim)
			prim = new Prim(_vexNum);
		else
			prim->reset(_vexNum);

		prim->vinfo[0].dist = 0;

		for(; ;)
		{
			int v = prim->selectSmallestUnknown();
			if(NOT_A_VERTEX == v)
				break;

			prim->vinfo[v].known = true;

			for(ArcNode *p = _vertices[v].firstarc; p; p = p->nextarc)
			{
				int w = p->adjvex;
				if(!prim->vinfo[w].known)
				{
					if(p->cost < prim->vinfo[w].dist)
					{
						prim->vinfo[w].dist = p->cost;
						prim->vinfo[w].path = v;
					}
				}
			}
		}
	}


	void printMST_Prim()
	{
		cout<<endl<<"--------------MST: Prim------------------------------"<<endl;
		decltype(prim->vinfo) & vec = prim->vinfo;
		CostType total = 0;
		for(int i = 1; i<vec.size(); i++)
		{
			total += vec[i].dist;
			int j = vec[i].path;
			cout<<"<"<<_vertices[i].data<<","<<_vertices[j].data<<">\t"<<vec[i].dist<<endl;
		}

		cout<<"total cost: "<<total;
	}
	


	void mst_Kruskal()
	{
		typedef priority_queue<Edge, vector<Edge>, CmpEdge> mypq_type;
		mypq_type pq;
		for(int i = 0 ; i<_vexNum; i++)
		{
			for(ArcNode *p = _vertices[i].firstarc; p; p = p->nextarc)
			{
				int j = p->adjvex;
				if(i <= j)
					pq.push(Edge(i,j, p->cost));
/*				else
					pq.push(Edge(j, i, p->cost));*/
			}
		}

		DisjSets s(_vexNum);
		int counter = 0;
		mstEdges.clear();
		while(counter < _vexNum - 1)
		{
			Edge e = pq.top();
			pq.pop();
			int s1 = s.find(e.v);
			int s2 = s.find(e.w);
			if(s1 != s2)
			{
				counter ++;
				mstEdges.push_back(e);
				s.unionSets(s1, s2);
			}
		}
	}


	void printMST_Kruskal()
	{
		cout<<endl<<"------------------MST: Kruskal----------------------"<<endl;
		CostType total = 0;
		for(auto e : mstEdges)
		{
			total += e.cost;
			cout<<"<"<<_vertices[e.v].data<<","<<_vertices[e.w].data<<">\t"<<e.cost<<endl;
		}

		cout<<"total cost is:\t"<<total<<endl;
	}
//assistent class
	struct Edge
	{
		int v;
		int w;
		CostType cost;

		Edge(int v, int w, CostType c)
			:v(v), w(w), cost(c)
		{}

/*		bool operator< (const Edge & e)const
		{
			return cost > e.cost;
		}*/
	};

	class CmpEdge
	{
	public:
		bool operator() (const Edge& e1, const Edge& e2) const
		{
			return e1.cost > e2.cost;
		}
	};
	class Prim
	{
	public:

		struct PrimInfo
		{
			CostType dist = INFINITE;
			int		path = NOT_A_VERTEX;
			bool 	known = false;
		};
		Prim(int n)
			:vinfo(n)
		{
			vinfo.assign(n, PrimInfo());
		}


		void reset(int n)
		{
			vinfo.assign(n, PrimInfo());
		}
		int selectSmallestUnknown() //scan the array
		{
			int v  = NOT_A_VERTEX;
			CostType min = INFINITE;
			for(int i = 0; i<vinfo.size(); i++)
			{
				
				if(!vinfo[i].known && vinfo[i].dist < min)
				{
					min = vinfo[i].dist; 
					v= i;
				}
			}

			return v;
		}
		vector<PrimInfo> vinfo;
	};

	
	class EventNodeInfo
	{
	public:
		vector<CostType> ve; //earlist complete time
		vector<CostType> vl; //latest complete time
	};


	template<typename ValueType>
	class SquareMatrix
	{
	public:
		SquareMatrix()
			:pp(nullptr), N(0)
		{
		}

		~SquareMatrix()
		{
			clear();
		}

		void clear()
		{
			if(pp)
			{
				for(int i = 0; i<N; i++)
					delete[] pp[i];
				delete[] pp;
			}
			
			pp = nullptr;
			N = 0;
		}

		void reset(int n, ValueType val)
		{
			if(N != n)
			{
				clear();

				pp = new ValueType*[n];
				for(int i = 0; i<n; i++)
					pp[i] = new ValueType[n];
				
				N = n;
			}
			
			for(int i = 0; i<n; i++)
				for(int j = 0; j < n; j++)
					pp[i][j] = val;
		}

		ValueType *& operator[] (int i)
		{
			return pp[i];
		}

		void printMatrix()
		{
			cout<<"\t";
			//column headers
			for(int i = 0; i<N; i++)
				cout<<i<<"\t";
			cout<<endl;
			for(int i = 0; i<N; i++)
				cout<<"____"<<"\t";
			cout<<endl;

			for(int i=0; i<N; i++)
			{
				cout<<i<<"|\t"; //row headers
				for(int j = 0; j<N; j++)
				{
					cout<<pp[i][j];
				}
			}
		}

	private:
		ValueType **pp = nullptr;
		int N = 0;
	};

	class AllPairsShortestPath
	{
	public:
		
		void reset(int n)
		{
			P.reset(n, NOT_A_VERTEX);
			D.reset(n, INFINITE);
			N = n;
		}
		
		void printDists()
		{
			cout<<endl<<"--------------allpairs shortest path: dist matrix-------------"<<endl;
			D.printMatrix();
		}

		void printPath()
		{
			cout<<endl<<"--------------allpairs shortest path: path matrix-------------"<<endl;
			P.printMatrix();
		}
		
		//compute the shortest path from v to w
		void computePath(int v, int w)
		{
			//apath.assign(N + 1, NOT_A_VERTEX);//the path is as most N vertices, the last place apath[N] is NOT_A_VERTEX, which means the path ends

			int vex = NOT_A_VERTEX;
			apath.assign(N + 1, vex);
			if( v == w)
			{
				apath[0] = v;
				return;
			}
			apath[0] = v;
			apath[1] = w;
			int n = 1;
			int i = 0;
			while(i < n)
			{
				int u1 = apath[i];
				int v1 = apath[i + 1];
				if(P[u1][v1] != NOT_A_VERTEX)
				{
					for(int j=n; j>i; j--)
						apath[j+1] = apath[j];
					apath[i + 1] = P[u1][v1];
					n++;
				}
				else
					i++;
			}
		}

		SquareMatrix<int> P;
		SquareMatrix<CostType> D;
		vector<int> apath;
	
	private:
		int N = 0;
	};


	class TopologicalInfo
	{
	public:
		~TopologicalInfo()
		{
		}

		void clear()
		{
			tops.clear();
			sorted.clear();
			
			counter = 0;
			foundCycle = false;
			N = 0;
		}
		void reset(int n)
		{
			counter = 0;
			foundCycle = false;
			tops.assign(n, 0);
			//sorted.assign(n, NOT_A_VERTEX);
			int vex = NOT_A_VERTEX;
			sorted.assign(n, vex);
			N = n;
		}
				
		void sortVertices()
		{
			//sorted.assign(N, NOT_A_VERTEX);
			int vex = NOT_A_VERTEX;
			sorted.assign(N, vex);

			for(int v = 0; v<N; v++)
			{
				int topNum = tops[v];
				sorted[topNum -1] = v; //top number start with 1
			}
		}
		vector<int> tops; //topological number of vertices
		vector<int> sorted; //vertices topological sorted

		int counter = 0; //number of vertices topological sorted
		bool foundCycle = false; //if not topological sorted, there is a cycle in the graph
		int N = 0;
	};


	//used for single-source shortest path
	class ShortestPaths
	{
	public:		
		struct VertexPathInfo
		{
			CostType 	dist = INFINITE;		//distance from source vertex
			int 		path = NOT_A_VERTEX;	//
			bool 		known = false;			//
		};
		
		void clear()
		{
			vpis.clear();
			start = NOT_A_VERTEX;
		}
		
		void reset(int s, int n)
		{
			vpis.assign(n, VertexPathInfo());
			this->start = s;
		}

		//select the smallest unknown distance vertex
		int selectSmallestUnknown()
		{
			CostType min = INFINITE;
			int v = NOT_A_VERTEX;
			for(int i = 0; i<vpis.size(); i++)
			{
				if(!vpis[i].known)
				{
					if(vpis[i].dist < min)
					{
						min = vpis[i].dist;
						v = i;
					}
				}
			}

			return v;
		}


		VertexPathInfo & operator[] (int i)
		{
			return vpis[i];
		}

		int start= NOT_A_VERTEX;
	private:
		vector<VertexPathInfo > vpis;
	};

	//used for finding articulation points
	struct DfsNumInfo
	{
		int num = 0; 	//preorder number, begin by 1. 0 means not ited
		int low = 0; 	//the lowest numbered vertex, low(v), that is reachable from v by taking zero or more edges and then possibly one back edge
		int parent = NOT_A_VERTEX;
		bool art = false; 	//articulation point or not
	};

	vector<int> 			indegrees;
	vector<DfsNumInfo> 	dfses;
	ShortestPaths 		paths;
	TopologicalInfo 	topInfo;
	vector<bool> 		visited;
	AllPairsShortestPath allpairs;
	EventNodeInfo 		eventInfo;
	Prim				*prim = nullptr;

	vector<Edge>		mstEdges ;

private:
	static const int NOT_A_VERTEX = -1;
	static const CostType INFINITE  = numeric_limits<CostType>::max();
	struct ArcNode
	{
		int			adjvex;
		ArcNode		*nextarc;
		CostType	cost;

		ArcNode(int adj, ArcNode *next, CostType c)
			:adjvex(adj), nextarc(next), cost(c){}

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
	void insertArc(int v, int w, CostType c)
	{
		ArcNode *pArc = new ArcNode(w, _vertices[v].firstarc, c);
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
	void clone(const WeightedALGraph &g)
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
				ArcNode *p = new ArcNode(pArc->adjvex, nullptr, pArc->cost);

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

	


	void clear()
	{
		if(_vertices)
		{
			deleteArcs();
			delete[] _vertices;
			_vertices = nullptr;
		}

		topInfo.clear();
		dfses.clear();
		paths.clear();
		visited.clear();

		_vexCap = 0;
		_vexNum = 0;
		_vexCapInc = 2;	

		if(prim)
			delete prim;
	}


};
