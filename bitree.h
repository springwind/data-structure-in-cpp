#include <algorithm>
#include <vector>
#include <stack>

using namespace std;

template<typename Object>
class BiTree
{
public:
	BiTree():_root(nullptr){}
	
	BiTree(const BiTree &rhs)
		:_root(nullptr)
	{
		_root = clone(rhs._root);
	}

	BiTree(BiTree && rhs)
		:_root(rhs._root)
	{
		rhs._root = nullptr;
	}

	//deep copy
	BiTree & operator= (const BiTree & rhs)
	{
		BiTree copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	//move
	BiTree & operator= (BiTree && rhs)
	{
		std::swap(_root, rhs._root);
		return *this;
	}


	~BiTree()
	{
		makeEmpty();
	}

	void makeEmpty()
	{
		makeEmpty(_root);
	}

	void createBiTree(vector<Object> vsrc, Object emptyVal)
	{
		
		if(vsrc.front() != emptyVal)
			_root = new BiNode(vsrc.front(), nullptr, nullptr);
		else
			return; //root is empty value, then no tree
	
		vector<BiNode *>v; // v{_root};
		v.push_back(_root);

		BiNode *p = nullptr;
		Object nextVal;
		auto isrc = vsrc.begin();
		while(++isrc != vsrc.end())
		{
			nextVal = *isrc;
			if(nextVal != emptyVal)
			{				
				p = new BiNode(nextVal, nullptr, nullptr);
				BiNode *pa = v.back();
				if(pa)
					pa->left = p;
				else
				{
					pa = *(v.end() -2);
					pa->right = p;
				}
				v.push_back(p);
			}
			else 
			{
				BiNode *pa = v.back();
				while(!pa)
				{
					v.pop_back(); //pop null
					v.pop_back(); //
					pa = v.back();
				}
				v.push_back(nullptr); // means pa's left finished
			}
		}
	}


	template<typename VisitFun>
	bool preorderTraverse(VisitFun vf)
	{
		stack<BiNode *> s; //visited nodes, but left child of these nodes may not visited yet
		BiNode *pGo = _root;

		while(!s.empty() || pGo)
		{
			while(pGo) // visit and put into stack, and go to left
			{
				if(!vf.visit(pGo->element)) // one fail, all fail
					return false;
				s.push(pGo);
				pGo = pGo->left;
			}//after while, pGo is null, which means pGo's ancestors and the left subtree  pGo is in have all visited

			BiNode * pa = s.top();
			s.pop();
			pGo = pa->right;
		}

		return true;
	}

	template<typename VisitFun>
	bool inorderTraverse(VisitFun vf)
	{
		stack<BiNode *> s; //unvisited nodes in revrse order
		BiNode *pGo = _root;

		while(!s.empty() || pGo) // s not empty=>there's some node unvisited; pGo=> some nodes unreached
		{
			while(pGo)
			{
				s.push(pGo);
				pGo = pGo->left;
			}//after while, p is null, which means p's parent can be visited
			
			BiNode *pa = s.top();
			s.pop();
			if(!vf.visit(pa->element)) // one fail, all fail
				return false;
			pGo = pa->right;
		}

		return true;
	}


	template<typename VisitFun>
	bool postorderTraverse(VisitFun vf)
	{
		stack<BiNode *> s;
		BiNode *pGo  = _root;

		while(!s.empty() || pGo)
		{
			while(pGo)
			{
				s.push(pGo);
				pGo = pGo->left;
			}

			BiNode *pa = s.top();
			
			if(pa)
			{
				s.push(nullptr);
				pGo = pa->right;
			}
			else //!pa
			{
				s.pop();
				pa = s.top();
				if(!vf.visit(pa->element))
					return false;
				s.pop();
			}
		}
		return true;
	}

	template<typename VisitFun>
	bool preorderTraverse_r(VisitFun vf)
	{
		preorderTraverse_r(_root, vf);
	}

	template<typename VisitFun>
	bool inorderTraverse_r(VisitFun vf)
	{
		inorderTraverse_r(_root, vf);
	}
	template<typename VisitFun>
	bool postorderTraverse_r(VisitFun vf)
	{
		postorderTraverse_r(_root, vf);
	}


private:
	struct BiNode
	{
		Object element;
		BiNode * left;
		BiNode * right;

		BiNode(const Object& ele, BiNode *lt, BiNode *rt)
			:element(ele), left(lt), right(rt)
		{}

		BiNode(Object && ele, BiNode *lt, BiNode *rt)
			:element(std::move(ele)), left(lt), right(rt)
		{}
	};

	BiNode *_root;

	template<typename VisitFun>
	bool preorderTraverse_r(BiNode *t, VisitFun vf)
	{
		if(t)
		{
			if(vf.visit(t->element))
				if(preorderTraverse_r(t->left, vf))
					if(preorderTraverse_r(t->right, vf))
						return true;
			return false;
		}
		return true;
	}

	template<typename VisitFun>
	bool inorderTraverse_r(BiNode *t, VisitFun vf)
	{
		if(t)
		{
			if(inorderTraverse_r(t->left, vf))
				if(vf.visit(t->element))
					if(inorderTraverse_r(t->right, vf))
						return true;
			return false;
		}
		else
			return true;
	}

	template<typename VisitFun>
	bool postorderTraverse_r(BiNode *t, VisitFun vf)
	{
		if(t)
		{
			if(postorderTraverse_r(t->left, vf))
				if(postorderTraverse_r(t->right, vf))
					if(vf.visit(t->element))
						return true;
			return false;
		}
		else
			return true;
	}

	BiNode * clone(BiNode *t)
	{
		if(t)
			return new BiNode(t->element, clone(t->left), clone(t->right));
		return nullptr;
	}

	void makeEmpty(BiNode *&t)
	{
		if(t)
		{
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
		t = nullptr;
	}
};
