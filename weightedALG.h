#include <algorithm>
#include <iostream>
#include <queue>
#include <limits>
#include <stack>

using namespace std;




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


	void allPairShortestPaths()
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


	void printAllPairShortestPaths()
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
					const int *path = allpairs.computePath(i, j);

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

		indegrees.reset(_vexNum, 0);


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
		eventInfo.ve.reset(_vexNum, 0);
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

		eventInfo.vl.reset(_vexNum, eventInfo.ve[_vexNum - 1]);
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

//assistent class

	template<typename ValueType>
	class Array
	{
	public:
		~Array()
		{
			clear();
		}

		void reset(int n)
		{
			if(N != n)
			{
				clear();
				p = new ValueType[n];
				N = n;
			}
			else
			{
				ValueType v;
				for(int i = 0; i<N; i++)
				{
					p[i] = v;
				}
			}
		}

		void reset(int n, ValueType val)
		{
			if(N != n)
			{
				clear();
				p = new ValueType[n];
			}
			for(int i = 0; i<n; i++)
				p[i] = val;
			N = n;
		}

		void clear()
		{
			if(p)
				delete[] p;
			p = nullptr;
			N = 0;
		}

		ValueType & operator[] (int i)
		{
			return p[i];
		}

		const ValueType * retrieve() const
		{
			return p;
		}

		ValueType * retrieve()
		{
			return p;
		}

		int getN()const
		{
			return N;
		}

	private:
		ValueType *p = nullptr;
		int N = 0;
	};





	
	class EventNodeInfo
	{
	public:
		struct ActivityEdge
		{
			int v;
			int w;
			CostType duration;	//activity duration 
			CostType slack;		//slack time
		};
		
		Array<CostType> ve; //earlist complete time
		Array<CostType> vl; //latest complete time
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

	class AllPairShortestPaths
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
		const int * computePath(int v, int w)
		{
			apath.reset(N + 1, NOT_A_VERTEX);//the path is as most N vertices, the last place apath[N] is NOT_A_VERTEX, which means the path ends
			apath.reset(N + 1, NOT_A_VERTEX);
			if( v == w)
			{
				apath[0] = v;
				return apath.retrieve();
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

			return apath.retrieve();
		}

		SquareMatrix<int> P;
		SquareMatrix<CostType> D;
	
	private:
		int N = 0;
		Array<int> apath;
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
			tops.reset(n, 0);
			sorted.reset(n, NOT_A_VERTEX);
			N = n;
		}
				
		void sortVertices()
		{
			sorted.reset(N, NOT_A_VERTEX);

			for(int v = 0; v<N; v++)
			{
				int topNum = tops[v];
				sorted[topNum -1] = v; //top number start with 1
			}
		}
		Array<int> tops; //topological number of vertices
		Array<int> sorted; //vertices topological sorted

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
			vpis.reset(n);
			this->start = s;
		}

		//select the smallest unknown distance vertex
		int selectSmallestUnknown()
		{
			CostType min = INFINITE;
			int v = NOT_A_VERTEX;
			for(int i = 0; i<vpis.getN(); i++)
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
		Array<VertexPathInfo > vpis;
	};

	//used for finding articulation points
	struct DfsNumInfo
	{
		int num = 0; 	//preorder number, begin by 1. 0 means not ited
		int low = 0; 	//the lowest numbered vertex, low(v), that is reachable from v by taking zero or more edges and then possibly one back edge
		int parent = NOT_A_VERTEX;
		bool art = false; 	//articulation point or not
	};

	Array<int> 			indegrees;
	Array<DfsNumInfo> 	dfses;
	ShortestPaths 		paths;
	TopologicalInfo 	topInfo;
	Array<bool> 		visited;
	AllPairShortestPaths allpairs;
	EventNodeInfo 		eventInfo;

private:
	static const CostType INFINITE  = numeric_limits<CostType>::max();
	static const int NOT_A_VERTEX = -1;
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
	}

};
