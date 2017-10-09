#ifndef PAIR_HEAP_H
#define PAIR_HEAP_H

#include <vector>
#include <queue>

#include "dsexceptions.h"
using namespace std;

template<typename Comparable>
class PairingHeap
{
private:
	struct PairNode ;

public:
	PairingHeap()
	{
		nullNode = new PairNode();
		nullNode->leftChild = nullNode;
		nullNode->nextSibling = nullNode;
		nullNode->prev = nullNode;

		root = nullNode;
	}

	PairingHeap(const PairingHeap & rhs)
	{
		nullNode = new PairNode();
		nullNode->leftChild = nullNode;
		nullNode->nextSibling = nullNode;
		nullNode->prev = nullNode;

		root = clone(rhs.root);
	}


	PairingHeap(PairingHeap && rhs)
		:nullNode(rhs.nullNode), root(rhs.root)
	{
		rhs.nullNode = nullptr;
		rhs.root = nullptr;
	}

	~PairingHeap()
	{
		makeEmpty();
		delete nullNode;
	}

	PairingHeap & operator= (const PairingHeap & rhs)
	{
		PairingHeap copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	PairingHeap & operator= (PairingHeap && rhs)
	{
		std::swap(nullNode, rhs.nullNode);
		std::swap(root, rhs.root);

		return *this;
	}

	bool isEmpty() const
	{
		return root == nullNode;
	}

	const Comparable & findMin() const
	{
		if(isEmpty())
			throw UnderflowException();

		return root->element;
	}


	

	typedef PairNode * Position;




	const Position insert(const Comparable & x)
	{
		PairNode * p = new PairNode(x, nullNode, nullNode, nullNode);

		if(root == nullNode)
			root = p;
		else
			compareAndLink(root, p);
		return p;
	}

	const Position insert(Comparable && x)
	{
		PairNode *p = new PairNode(x, nullNode, nullNode, nullNode);

		if(root == nullNode)
			root = p;
		else
			compareAndLink(root, p);

		return p;
	}
	
	const Comparable & retrieve(const Position p) const
	{
		return p->element;
	}

	void deleteMin()
	{
		//case 1: empty
		if(isEmpty())
			throw UnderflowException();

		PairNode * oldroot = root;
		//case 2: only root
		if(root->leftChild == nullNode)
			root = nullNode;
		else //case 3: has child
			root = combineSiblings(root->leftChild);

		delete oldroot;
	}

	void deleteMin(Comparable & minItem)
	{
		if(isEmpty())
			throw UnderflowException();

		minItem = root->element;
		deleteMin();
	}

	void makeEmpty()
	{
		reclaimMemory(root);
		root = nullNode;
	}

	void decreaseKey(Position p, const Comparable & newVal)
	{
		if(p->element < newVal)
			throw IllegalArgumentException();

		p->element = newVal;

		//2 cases: p is root, heap property not violated; p is not root, p may violate heap property
		if(p != root)
		{
			p->nextSibling->prev = p->prev;
			//cut p 
			if(p->prev->leftChild == p) // p is the leftist node, p's prev is its parent
				p->prev->leftChild = p->nextSibling;
			else //p is not the leftist node, p's prev is a left sibling
				p->prev->nextSibling = p->nextSibling;

			p->nextSibling = nullNode;
			//p->prev = nullNode; //not needed

			compareAndLink(root, p);
		}
	}


	void decreaseKey(Position p, Comparable && newVal)
	{
		if(p->element < newVal)
			throw IllegalArgumentException();

		p->element = std::move(newVal);

		if(p != root)
		{
			if(p->prev->leftChild == p)
				p->prev->leftChild = p->nextSibling;
			else
				p->prev->nextSibling = p->nextSibling;

			p->nextSibling = nullNode;

			compareAndLink(root, p);
		}
	}

private:
	struct PairNode
	{
		Comparable element;
		PairNode * leftChild;
		PairNode * nextSibling;
		PairNode * prev;

		PairNode(const Comparable & e = Comparable(), PairNode *lc = nullptr, PairNode * ns = nullptr, PairNode *pr = nullptr)
			:element(e), leftChild(lc), nextSibling(ns), prev(pr)
		{}

		PairNode(Comparable && e, PairNode *lc = nullptr, PairNode * ns = nullptr, PairNode *pr = nullptr)
			:element(std::move(e)), leftChild(lc), nextSibling(ns), prev(pr)
		{}
	};

	PairNode * nullNode;
	PairNode * root;

	void compareAndLink(PairNode *&first, PairNode *second)
	{
		if(nullNode == second || nullptr == second)
		{
			return;
		}

		if(second->element < first->element)
		{
			second->prev = first->prev;

			first->nextSibling = second->leftChild;
			first->nextSibling->prev = first;

			second->leftChild = first;
			first->prev = second;

			first = second;
		}
		else //if(first->element < second->element)
		{
			first->nextSibling = second->nextSibling;
			first->nextSibling->prev = first;

			second->nextSibling = first->leftChild;
			second->nextSibling->prev = second;

			first->leftChild = second;
			second->prev = first;
		}
	}

	/*Internal method that implements two-pass merging.
	 *
	 **/
	PairNode * combineSiblings(PairNode * firstSibling)
	{
		if(firstSibling == nullNode || firstSibling->nextSibling == nullNode )
			return firstSibling;

		static vector<PairNode *> treeArray(5);
		int numSiblings = 0;
		for(; firstSibling != nullNode; numSiblings ++, firstSibling = firstSibling->nextSibling)
		{
			if(numSiblings == treeArray.size())
				treeArray.resize(numSiblings * 2);
			treeArray[numSiblings] = firstSibling;
			firstSibling->prev->nextSibling = nullNode; //break links
		}
		if(numSiblings == treeArray.size())
			treeArray.resize(numSiblings + 1);
		treeArray[numSiblings] = nullNode;

		//first pass
		int i;
		for(i = 0; i<numSiblings; i+=2)
			compareAndLink(treeArray[i], treeArray[i + 1]);

		//second pass
		int j;
		for(j = i -2; j-2 >=0; j-= 2)
			compareAndLink(treeArray[j-2], treeArray[j]);

		return treeArray[0];
	}



	/*an alternative method for combineSiblings
	 * use a queue
	 **/
	PairNode * combineSiblings2(PairNode * firstSibling)
	{
		if(firstSibling->nextSibling == nullNode)
			return firstSibling;

		static queue<PairNode *> theTrees;

		for(; firstSibling != nullNode; firstSibling = firstSibling->nextSibling)
			theTrees.push(firstSibling);

		PairNode *s1, *s2;
		while(theTrees.size() >=2)
		{
			s1 = theTrees.front();
			theTrees.pop();
			s2 = theTrees.front();
			theTrees.pop();
			compareAndLink(s1, s2);
			theTrees.push(s1);
		}

		s1 = theTrees.front();
		theTrees.pop();

		return s1;
	}

	void reclaimMemory(PairNode *t)
	{
		if(nullNode != t)
		{
			reclaimMemory(t->nextSibling);
			reclaimMemory(t->leftChild);
			delete t;
		}
	}


	PairNode * clone(PairNode * t)
	{
		if(t == t->leftChild)
			return nullNode;

		PairNode * p = new PairNode(t->element, nullNode, nullNode, nullNode);
		p->leftChild = clone(t->leftChild);
		p->leftChild->prev = p;

		p->nextSibling = clone(t->nextSibling);
		p->nextSibling->prev = p;

		return p;
	}
};



#endif
