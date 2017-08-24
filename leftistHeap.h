#ifndef LEFTIST_HEAP_H
#define LEFTIST_HEAP_H

#include "dsexceptions.h"
#include <iostream>
#include <algorithm>
#include <stack>
#include <queue>
#include <vector>

template<typename Comparable>
class LeftistHeap
{
public:
	LeftistHeap()
		:root(nullptr)
	{}

	/*	Copy ctor
	 *
	 * */
	LeftistHeap(const LeftistHeap & rhs)
		:root(nullptr)
	{
		root = clone(rhs.root);
	}

	/* Move ctor
	 *
	 */
	LeftistHeap(LeftistHeap && rhs)
		:root(rhs.root)
	{
		rhs.root = nullptr;
	}

	~LeftistHeap()
	{
		makeEmpty();
	}


	/**
	 *	Copy assignment
	 * */
	LeftistHeap & operator= (const LeftistHeap & rhs)
	{
		LeftistHeap copy = rhs;
		std::swap(*this, copy);
//		std::swap(root, copy.root);
		return *this;
	}

	/*
	 *	Move assignment
	 **/
	LeftistHeap & operator= (LeftistHeap && rhs)
	{
		std::swap(root, rhs.root);
		return *this;
	}



	bool isEmpty() const
	{
		return nullptr == root;
	}

	const Comparable findMin() const
	{
		if(isEmpty())
			throw UnderflowException();

		return root->element;
	}

	void insert(const Comparable & x)
	{
		root = merge(new LeftistNode(x), root);
	}

	void insert(Comparable && x)
	{
		root = merge(new LeftistNode(std::move(x)), root);
	}

	void deleteMin()
	{
		if(isEmpty())
			throw UnderflowException();
		LeftistNode *oldroot = root;
		root = merge(root->left, root->right);

		delete oldroot;
	}

	void deleteMin(Comparable & minItem)
	{
		minItem = findMin();

		deleteMin();
	}

	void makeEmpty()
	{
		reclaimMemory(root);
		root = nullptr;
	}


	void merge(LeftistHeap & rhs)
	{
		if(this == &rhs)
			return;

		root = merge(root, rhs.root);

		rhs.root = nullptr;
	}

	void levelOrder(std::vector<Comparable> & v)
	{
		LeftistNode * p = root;
		if(p)
		{
			std::queue<LeftistNode *> q;
			q.push(p);
			while(!q.empty())
			{
				p = q.front();
				q.pop();

				if(p)
				{
					q.push(p->left);
					q.push(p->right);
					
					v.push_back(p->element);
				}
				else
					v.push_back(-1);
			}
		}
	}

private:
	struct LeftistNode
	{
		Comparable 		element;
		LeftistNode		*left;
		LeftistNode		*right;
		int				npl;

		LeftistNode(const Comparable & e, LeftistNode *lt = nullptr, LeftistNode *rt = nullptr, int np = 0)
			:element(e), left(lt), right(rt), npl(np)
		{}

		LeftistNode(Comparable && e, LeftistNode *lt = nullptr, LeftistNode * rt = nullptr, int np = 0)
			:element(std::move(e)), left(lt), right(rt), npl(np)
		{}
	};


	LeftistNode * root;



	LeftistNode * merge(LeftistNode * h1, LeftistNode * h2)
	{
		
		if(nullptr == h1)
			return h2;
		if(nullptr == h2)
			return h1;

		if(h1->element < h2->element)
			return merge1(h1, h2);
		else 
			return merge1(h2, h1);
	}

	/*
	//internal method to merge two roots recursively
	//
	//assume h1 and h2 are not empty, and the element of h1's root 
	//is smaller than that of h2's root
	LeftistNode * merge1(LeftistNode * h1, LeftistNode * h2)
	{
		//h1 is single node
		//
		if(nullptr == h1->left)
			h1->left = h2;
		else //h1's left child is not empty
		{
			h1->right = merge(h1->right, h2);

			if(h1->right->npl > h1->left->npl)//make it leftist
			{
				swapChildren(h1);
			}

			h1->npl = h1->right->npl + 1; //update npl of h1
		}

		return h1;
	}*/

	//internal method to merge two roots.
	//nonrecursive version.
	//assume h1 and h2 are not empty, and the element of h1's root 
	//is smaller than that of h2's root
	LeftistNode * merge1(LeftistNode * h1, LeftistNode * h2)
	{
		if(nullptr == h1->left)
			h1->left = h2;
		else
		{
			std::stack<LeftistNode *> s;
			LeftistNode * p = h1;
			LeftistNode * p1 = h1->right;
			LeftistNode * p2 = h2;

			s.push(p);

			//merge the right paths of both heaps
			while(nullptr != p1 && nullptr != p2)
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
				s.push(p);
			}
		
			p->right = nullptr != p1? p1 : p2;


			//fix the nodes that violate the leftist heap property
			while(!s.empty())
			{
				p = s.top();
				s.pop();

				if(nullptr == p->left)
				{
					p->left = p->right;
					p->right = nullptr;
				}
				else
				{
					if(p->left->npl < p->right->npl)
					{
						swapChildren(p);
					}
					p->npl = p->right->npl + 1;
				}
			}			
		}

		return h1;
	}

	void swapChildren(LeftistNode *t)
	{
		LeftistNode *p = t->left;
		t->left = t->right;
		t->right = p;
	}

	/*
	void reclaimMemory(LeftistNode *t)
	{
		if(nullptr != t)
		{
			reclaimMemory(t->left);
			reclaimMemory(t->right);

			delete t;
		}
	}
*/

	
	void reclaimMemory(LeftistNode *t)
	{
		if(nullptr != t)
		{
			while(t->left)
			{
				reclaimMemory(t->right);
				
				LeftistNode *lt = t->left;
				delete t;

				t = lt;
			}

			delete t;
		}
	}

	/*
	Leftistnode * clone(LeftistNode * t) const
	{
		if(nullptr == t)
			return nullptr;
		
		return new LeftistNode(t->element, clone(t->left), cone(t->right), t->npl);
	}*/


	LeftistNode * clone(LeftistNode * t) const
	{
		if(nullptr == t)
			return nullptr;

		LeftistNode * ret = nullptr;
		LeftistNode * t2 = nullptr;

		ret = new LeftistNode(t->element, nullptr, nullptr, t->npl);
		t2 = ret;
		while(t->left)
		{
			t2->right = clone(t->right);
			t2->left = new LeftistNode(t->left->element, nullptr, nullptr, t->left->npl);

			t = t->left;
			t2 = t2->left;
		}

		return ret;
	}



};




#endif

