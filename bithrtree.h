#include <algorithm>
#include <vector>

using namespace std;

template<typename Object>
class BinaryThreadTree
{
public:
	BinaryThreadTree()
		:_root(nullptr), _thrt(nullptr)
	{}

	BinaryThreadTree(const BinaryThreadTree &rhs)
		:_root(nullptr), _thrt(nullptr)
	{
		_root = clone(rhs);
	}

	BinaryThreadTree(BinaryThreadTree && rhs)
		:_root(rhs->_root), _thrt(rhs->_thrt)
	{
		rhs->_root = nullptr;
		rhs->_thrt = nullptr;
	}

	BinaryThreadTree & operator= (const BinaryThreadTree rhs)
	{
		if(this != rhs)
		{
			BinaryThreadTree copy = rhs;
			std::swap(*this, copy);
		}
		return *this;
	}

	BinaryThreadTree & operator= (BinaryThreadTree && rhs)
	{
		if(this != rhs)
		{
			std::swap(*this, rhs);
		}
		return *this;
	}


	void createTree(const vector<Object> &vsrc, Object empty)
	{
		int i = 0;
		_root = createTree(vsrc, i, empty); 	
	}

	template<typename VisitFun>
	bool preorderTraverse_r(VisitFun vf)
	{
		return preorderTraverse_r(_root, vf);
	}

	template<typename VisitFun>
	bool inorderTraverse_r(VisitFun vf)
	{
		return inorderTraverse_r(_root, vf);
	}

	template<typename VisitFun>
	bool inOrderTraverse_Thr(VisitFun vf)
	{
		BiThrNode *p = _thrt->left;
		while(p != _thrt)
		{
			while(p->ltag == PointerTag::Link)
				p = p->left;
			if(!vf.visit(p->element))
				return false;
			while(p->rtag == PointerTag::Thread && p->right != _thrt)
			{
				p = p->right;
				if(!vf.visit(p->element))
					return false;
			}
			p = p->right;
		}
		return true;
	}


	void inOrderThreading()
	{
		if(_thrt)
			delete _thrt;
		_thrt = new BiThrNode(Object(), nullptr, nullptr);
		_thrt->ltag = PointerTag::Link;
		_thrt->rtag = PointerTag::Thread;
		_thrt->right = _thrt;
		if(!_root)
			_thrt->left = _thrt;
		else
		{
			_thrt->left = _root;
			BiThrNode *pre = _thrt;
			inThreading(_root, pre);
			pre->right = _thrt;
			pre->rtag = PointerTag::Thread;
			_thrt->right = pre;
		}


	}


private:
	enum class PointerTag {Link = 0, Thread = 1};
	struct BiThrNode
	{
		Object element;
		BiThrNode *left;
		BiThrNode *right;
		PointerTag ltag;
		PointerTag rtag;

		BiThrNode(const Object &elem, BiThrNode *lt, BiThrNode *rt)
			:element(elem), left(lt), right(rt), ltag(PointerTag::Link), rtag(PointerTag::Link)
		{}

		BiThrNode(Object && elem, BiThrNode *lt, BiThrNode *rt)
		:element(std::move(elem)), left(lt), right(rt), ltag(PointerTag::Link), rtag(PointerTag::Link)
		{}
	};

	BiThrNode *_root;
	BiThrNode *_thrt;



	BiThrNode *createTree(const vector<Object> &v, int &i, Object empty)
	{

		if(i<v.size())
		{
			if(v[i] != empty)
			{
				BiThrNode * t = new BiThrNode(v[i], nullptr, nullptr);
				t->left = createTree(v, ++i, empty);
				t->right = createTree(v, ++i, empty);
				return t;
			}
		}
		return nullptr;
	}


	void inThreading(BiThrNode * t, BiThrNode *& pre)
	{
		if(t)
		{
			inThreading(t->left, pre);
			if(!t->left)
			{
				t->left = pre;
				t->ltag = PointerTag::Thread;
			}
			if(!pre->right)
			{
				pre->right = t;
				pre->rtag = PointerTag::Thread;
			}
			pre = t;
			inThreading(t->right, pre);
		}
	}


	BiThrNode *clone(BiThrNode *t)
	{
		if(!t)
			return nullptr;
		return new BiThrNode(t->element, clone(t->left), clone(t->right));
	}

	void makeEmpty(BiThrNode *t)
	{
		if(t)
		{
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
		t = nullptr;
	}

	template<typename VisitFun>
	bool preorderTraverse_r(BiThrNode *t, VisitFun vf)
	{
		if(t)
		{
			if(vf.visit(t->element))
				if(t->ltag == PointerTag::Thread || preorderTraverse_r(t->left, vf))
					if(PointerTag::Thread == t->rtag || preorderTraverse_r(t->right, vf))
						return true;
			return false;
		}
		return true;
	}

	template<typename VisitFun>
	bool inorderTraverse_r(BiThrNode *t, VisitFun vf)
	{	
		if(t)
		{
			
			if(t->ltag == PointerTag::Thread || inorderTraverse_r(t->left, vf))
				if(vf.visit(t->element))
					if(t->rtag == PointerTag::Thread || inorderTraverse_r(t->right, vf))
						return true;
			return false;
		}
		return true;
	}
};
