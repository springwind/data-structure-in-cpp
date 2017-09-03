#ifndef BINOMIAL_QUEUE_H
#define BINOMIAL_QUEUE_H

#include <vector>
#include "dsexceptions.h"

template<typename Comparable>
class BinomialQueue
{
public:
	BinomialQueue()
	:currentSize(0), theTrees(DEFAULT_TREES)
	{}

	BinomialQueue(const BinomialQueue & rhs)
		:currentSize(rhs.currentSize),
		theTrees(rhs.theTrees.size())
	{
		for(int i = 0; i<rhs.theTrees.size(); i++)
		{
			theTrees[i] = clone(rhs.theTrees[i]);
		}
	}

	BinomialQueue( BinomialQueue && rhs)
		:currentSize(rhs.currentSize),
		theTrees(std::move(rhs.theTrees))
	{}

	~BinomialQueue()
	{
		makeEmpty();
	}

	BinomialQueue & operator= (const BinomialQueue & rhs)
	{
		BinomialQueue copy = rhs;
		std::swap(*this, copy);

		return *this;
	}

	BinomialQueue & operator= (BinomialQueue && rhs)
	{
		std::swap(currentSize, rhs.currentSize);
		std::swap(theTrees, rhs.theTrees);

		return *this;
	}

	bool isEmpty()
	{
		return 0 == currentSize;
	}

	void makeEmpty()
	{
		for(BinomialNode * t : theTrees)
		{
			if(t)
				makeEmpty(t);
		}
	}

	const Comparable & findMin() const
	{
		if(isEmpty())
			throw UnderflowException();

		int minIdx = findMinIndex();
		return theTrees[minIdx]->element;
	}

	void insert(const Comparable & e)
	{
		BinomialNode * t = new BinomialNode(e);
		insert(t, 0);
	}

	void insert(Comparable && e)
	{
		BinomialNode * t = new BinomialNode(e);
		insert(t, 0);
	}

	void deleteMin(Comparable & e)
	{
		if(isEmpty())
			throw UnderflowException();

		int minIdx = findMinIndex();
		e = theTrees[minIdx]->element;

		BinomialNode * child = theTrees[minIdx]->firstChild;
		delete theTrees[minIdx];
		theTrees[minIdx] = nullptr;
		currentSize -= 1;

		for(int k = minIdx - 1; k>=0; k--)
		{
			BinomialNode * t = child;
			child = child->nextSibling;
			t->nextSibling = nullptr;

			for(int j = k; t != nullptr; j++)
			{
				if(theTrees[j])
				{
					t = combineTrees(t, theTrees[j]);
					theTrees[j] = nullptr;
				}
				else
				{
					theTrees[j] = t;
					t = nullptr;
				}
			}
		}
	}

	void merge(BinomialQueue & rhs)
	{
		if(this->currentSize < rhs.currentSize)
		{
			rhs.merge(*this);
			std::swap(*this, rhs);
			return;
		}

		merge1(rhs);

	}



private:
	struct BinomialNode
	{
		Comparable element;
		BinomialNode *firstChild;
		BinomialNode *nextSibling;

		BinomialNode(const Comparable & e, BinomialNode * fc = nullptr, BinomialNode * ns = nullptr)
			:element(e), firstChild(fc), nextSibling(ns)
		{}

		BinomialNode(Comparable && e, BinomialNode *fc = nullptr, BinomialNode *ns = nullptr)
			:element(std::move(e)), firstChild(fc), nextSibling(ns)
		{}
	};

	int currentSize;
	std::vector<BinomialNode *> theTrees;
	enum {DEFAULT_TREES = 1};

	BinomialNode * combineTrees(BinomialNode *t1, BinomialNode *t2)
	{
		if(t2->element < t1->element)
			return combineTrees(t2, t1);

		t2->nextSibling = t1->firstChild;
		t1->firstChild = t2;
		return t1;
	}


	//rhs is smalller
	void merge1(BinomialQueue &rhs)
	{
		enum {MISSING = 0x000, THISSIDE = 0x001, RHS = 0x010, CARRY = 0x100};

		//size
		currentSize += rhs.currentSize;
		if(currentSize > capacity())
		{
			int newNumTrees = theTrees.size() + 1;

			theTrees.resize(newNumTrees);
			theTrees[newNumTrees-1] = nullptr;
		}


		//trees
		BinomialNode * carry = nullptr;
		for(int i = 0, j = 1; j <= rhs.currentSize || carry != nullptr; i++, j*=2)
		{
			int whichcase = (theTrees[i] ? THISSIDE : MISSING) | (rhs.theTrees[i] ? RHS : MISSING) | (carry ? CARRY : MISSING);
			switch(whichcase)
			{
				case MISSING:
				case THISSIDE:
					break;
				case RHS:
					theTrees[i] = rhs.theTrees[i];
					rhs.theTrees[i] = nullptr;
					break;
				case THISSIDE + RHS:
					carry = combineTrees(theTrees[i], rhs.theTrees[i]);
					theTrees[i] = nullptr;
					rhs.theTrees[i] = nullptr;
					break;
				case CARRY:
					theTrees[i] = carry;
					carry = nullptr;
					break;
				case THISSIDE + CARRY:
					carry = combineTrees(carry, theTrees[i]);
					theTrees[i] = nullptr;
					break;
				case RHS + CARRY:
					carry = combineTrees(carry, rhs.theTrees[i]);
					rhs.theTrees[i] = nullptr;
					break;
				case THISSIDE + RHS + CARRY:
					carry = combineTrees(carry, rhs.theTrees[i]);
					rhs.theTrees[i] = nullptr;
					break;
			}
		}

		rhs.currentSize = 0;
		for(BinomialNode * &root : rhs.theTrees)
			root = nullptr;

	}


	void insert(BinomialNode * B, int rank)
	{
		//size
		currentSize += 1<<rank;
		if(currentSize > capacity())
		{
			int oldNumTrees = theTrees.size();
			int newNumTrees = std::max(rank + 1, oldNumTrees) + 1;
			theTrees.resize(newNumTrees);
			for(int i = oldNumTrees; i<newNumTrees; i++)
				theTrees[i] = nullptr;
		}

		//insert B
		BinomialNode * t = B;
		for(int i = rank; t != nullptr; i++)
		{
			if(theTrees[i])
			{
				t = combineTrees(t, theTrees[i]);
				theTrees[i] = nullptr;
			}
			else
			{
				theTrees[i] = t;
				t = nullptr;
			}
		}
	}


	int findMinIndex() const
	{
		int minIdx;
		int i;
		for(i = 0; nullptr == theTrees[i]; i++)
			;

		for(minIdx = i++; i<theTrees.size(); i++)
		{
			if(theTrees[i] && theTrees[i]->element < theTrees[minIdx]->element)
				minIdx = i;
		}

		return minIdx;
	}


	int capacity()
	{
		return (1<<theTrees.size()) - 1;
	}

	void makeEmpty(BinomialNode * &  t)
	{
		if(t)
		{
			makeEmpty(t->firstChild);
			makeEmpty(t->nextSibling);
			delete t;
			t = nullptr; 
		}
	}


	BinomialNode * clone(BinomialNode * t) const
	{
		if(t)
		{
			return new BinomialNode(t->element, clone(t->firstChild), clone(t->nextSibling));
		}
		return nullptr;
	}
};




#endif //BINOMIAL_QUEUE_H
