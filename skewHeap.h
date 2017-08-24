#ifndef SKEW_HEAP_H
#define SKEW_HEAP_H

#include "dsexceptions.h"


template<typename Comparable>
class SkewHeap
{
public:
	SkewHeap()
		:root(nullptr)
	{
	}

	SkewHeap(const SkewHeap & rhs)
		:root(nullptr)
	{
		root = clone(rhs.root);
	}

	SkewHeap(SkewHeap && rhs)
		:root(rhs.root)
	{
		rhs.root = nullptr;
	}

	~SkewHeap()
	{
		makeEmpty();
	}

	SkewHeap & operator= (const SkewHeap & rhs)
	{
		SkewHeap copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	SkewHeap & operator= (SkewHeap && rhs)
	{
		std::swap(root, rhs.root);
		return *this;
	}

	bool isEmpty()
	{
		return nullptr == root;
	}

	void makeEmpty()
	{
		reclaimMemory(root);
		root = nullptr;
	}

	Comparable findMin() const
	{
		if(nullptr == root)
			throw UnderflowException();
		
		return root->element;
	}

	void deleteMin()
	{
		if(nullptr == root)
			throw UnderflowException();

		SkewNode * oldroot = root;
		root = merge(root->left, root->right);

		delete oldroot;
	}

	void deleteMin(Comparable & item)
	{
		item = findMin();

		deleteMin();
	}

	void insert(const Comparable & x)
	{
		root = merge(root, new SkewNode(x));
	}

	void insert(Comparable && x)
	{
		root = merge(root, new SkewNode(x));
	}

	void merge(SkewHeap & rhs)
	{
		if(this == &rhs)
			return;

		root = merge(root, rhs.root);

		rhs.root = nullptr;
	}


private:
	struct SkewNode
	{
		Comparable element;
		SkewNode *left;
		SkewNode *right;

		SkewNode(const Comparable & e, SkewNode *lt = nullptr, SkewNode *rt = nullptr)
		:element(e), left(lt), right(rt)
		{}

		SkewNode(Comparable && e, SkewNode *lt = nullptr, SkewNode *rt = nullptr)
			:element(std::move(e)), left(lt), right(rt)
		{}
	};

	SkewNode * root;
	SkewNode * merge(SkewNode *h1, SkewNode *h2)
	{
		if(nullptr == h1)
			return h2;
		else if(nullptr == h2)
			return h1;
		else
		{
			if(h1->element < h2->element)
				return merge1(h1, h2);
			else
				return merge1(h2, h1);
		}
	}


	SkewNode * merge1(SkewNode *h1, SkewNode *h2)
	{
		SkewNode *p = h1;
		SkewNode *p1 = h1->right;
		SkewNode *p2 = h2;

		while(p1 && p2)
		{
			if(p1->element < p2->element)
			{
				p->right = p1;
				p1 = p1->right;
			}
			else
			{
				p->right = p2;
				p2 = p2->right;
			}

			p = p->right;
		}

		p->right = nullptr != p1 ? p1 : p2;

		p = h1;
		while(p->right)
		{
			swapChildren(p);
			p = p->left;
		}

		return h1;
	}

	void swapChildren(SkewNode * t)
	{
		SkewNode * tmp = t->left;
		t->left = t->right;
		t->right = tmp;
	}

	/*
	void reclaimMemory(SkewNode * t)
	{
		if(nullptr != t)
		{
			SkewNode *lt = t->left;
			SkewNode *rt = t->right;
			delete t;
			reclaimMemory(lt);
			reclaimMemory(rt);
		}
	}*/


	void reclaimMemory(SkewNode * t)
	{
		if(nullptr != t)
		{
			SkewNode *lt = t;
			SkewNode *rt = t;
			while(lt && rt)
			{
				lt = lt->left;
				rt = rt->right;
			}

			if(nullptr == rt)
			{
				while(t->left)
				{
					rt = t->right;
					lt = t->left;

					delete t;

					reclaimMemory(rt);
					t = lt;
				}
			}
			else
			{
				while(t->right)
				{
					lt = t->left;
					rt = t->right;

					delete t;

					reclaimMemory(lt);
					t = rt;
				}
			}
		}
	}


	SkewNode * clone(SkewNode * t) const
	{
		if(nullptr == t)
			return nullptr;

		SkewNode *lt = t;
		SkewNode *rt = t;
		while(lt && rt)
		{
			lt = lt->left;
			rt = rt->right;
		}

		SkewNode *ret = new SkewNode(t->element);
		SkewNode *t2 = ret;
		if(nullptr == rt)
		{
			while(t->left)
			{
				t2->right = clone(t->right);
				t2->left = new SkewNode(t->left->element);
				
				t2 = t2->left;
				t = t->left;
			}
		}
		else
		{
			while(t->right)
			{
				t2->left = clone(t->left);
				t2->right = new SkewNode(t->right->element);

				t2 = t2->right;
				t = t->right;
			}
		}

		return ret;
	}


	
};



#endif
