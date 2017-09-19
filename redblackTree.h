#ifndef REDBLACK_TREE
#define REDBLACK_TREE

#include <iostream>
#include "dsexceptions.h"

using namespace std;

template<typename Comparable>
class RedBlackTree
{
public:
	//ctor
	RedBlackTree(const Comparable & negInfo)
	{
		nullNode = new RedBlackNode;
		nullNode->left = nullNode->right = nullNode;

		header = new RedBlackNode(negInfo);
		header->left = header->right = nullNode;
	}

	//copy ctor
	RedBlackTree(const RedBlackTree & rhs)
	{
		nullNode = new RedBlackNode;
		nullNode->left = nullNode->right = nullNode;

		header = new RedBlackNode(rhs.header->element);
		header->left = nullNode;

		header->right = clone(rhs.header->right);
	}

	//move ctor
	RedBlackTree(RedBlackTree && rhs)
		:nullNode(rhs.nullNode), header(rhs.header)
	{
		rhs.nullNode = nullptr;
		rhs.header = nullptr;
	}

	//dtor
	~RedBlackTree()
	{
		//clear the tree
		makeEmpty(); 

		//delete sentinel node
		delete nullNode;
		delete header;
	}


	//deep copy
	RedBlackTree & operator= (const RedBlackTree & rhs)
	{
		RedBlackTree copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	//move
	RedBlackTree & operator= (RedBlackTree && rhs)
	{
		std::swap(nullNode, rhs.nullNode);
		std::swap(header, rhs.header);

		return *this;
	}


	bool isEmpty() const
	{
		return header->right == nullNode;
	}

	//make the tree empty
	void makeEmpty()
	{
		//header could be empty because of move operation
		if(nullptr == header)//could happen in dtor of && variable
			return;

		reclaimMemory(header->right); //clear the tree
		header->right = nullNode; //fix header's right link
	}

	bool contains(const Comparable x) const
	{
		nullNode->element = x;
		RedBlackNode * curr =  header->right;
		for(; ;)
		{
			if(x<curr->element)
				curr = curr->left;
			else if(curr->element < x)
				curr = curr->right;
			else
				return curr != nullNode;
		}
	}


	const Comparable & findMin() const
	{
		if(isEmpty())
			throw UnderflowException();

		RedBlackNode * X = header->right;
		for(X = header->right; X->left != nullNode; X = X->left) //start from root, go left, left ...
			;

		return X->element;
	}

	const Comparable & findMax() const
	{
		if(isEmpty())
			throw UnderflowException();

		RedBlackNode * X;
		for(X = header->right; X->right != nullNode; X = X->right)
			;

		return X->element;
	}

	template<typename VisitFun>
	void printTree(VisitFun vf) const
	{
		if(isEmpty())
			cout<<"Empty tree"<<endl;
		else
			printTree(header->right, vf);
	}

	void checkBlacks() const
	{
		int blacks = 0;
		
		checkBlacks(header->right, blacks);
	}

	void checkReds() const
	{
		checkReds(header->right);
	}

	void insert(const Comparable &x)
	{
		//find the position to insert

		nullNode->element = x; //set the sentinel

		great = grand = parent = current = header;
		while(current->element != x)
		{
			//go to next level
			great = grand;
			grand = parent;
			parent = current;

			current = x < current->element ? current->left : current->right;

			//check if current has two red children; and fix if so
			if(RED == current->left->color && RED == current->right->color)
				handleReorient(x);
		}

		if(current != nullNode) //x already present
			return;

		//new node for x
		current = new RedBlackNode(x, nullNode, nullNode);

		//attatch the new node to the tree
		if(x < parent->element)
			parent->left = current;
		else
			parent->right = current;

		//fix color
		handleReorient(x);
	}

	void remove(const Comparable &x)
	{
		//find the node to remove

		RedBlackNode *P = header;
		RedBlackNode *X = header->right;
		nullNode->element = x;
		while(X->element != x)
		{
			P = X;
			X = x<X->element ? X->left : X->right;
		}

		if(X == nullNode) //x not present
		{
			//cout<<x<<" does not exist!!!"<<endl;
			return;
		}

		RedBlackNode *nodeToDelete = X;

		
		if(X->left != nullNode && X->right != nullNode) //the node to delete has 2 children, find the min node of its right child
		{
			P = X;
			X = X->right;
			while(X->left != nullNode)
			{
				P = X;
				X = X->left;
			}
		}

		if(BLACK == X->color && X->left == nullNode  && X->right == nullNode) //X is a black leaf
		{
			topDownRed(X, P);//makes X red
		}

		if(nodeToDelete != X)
		{
			nodeToDelete->element = X->element;
		}

		//now either X is a red leaf or X is black with a red child
		RedBlackNode *child = (X->left != nullNode )? X->left : X->right;
		if(child != nullNode)
			child->color = BLACK;
		if(X == P->left)
			P->left = child;
		else if(X == P->right)
			P->right = child;
		else
		{
		//	cout<<"delete error"<<endl;
		}
		delete X;

	}


private:
	enum {BLACK, RED};

	struct RedBlackNode
	{
		Comparable element;
		RedBlackNode *left;
		RedBlackNode *right;
		int	color;

		RedBlackNode(const Comparable & e = Comparable(), RedBlackNode * lt = nullptr, RedBlackNode * rt = nullptr, int c = BLACK)
			:element(e), left(lt), right(rt), color(c)
		{}

		RedBlackNode(Comparable && e, RedBlackNode * lt = nullptr, RedBlackNode * rt = nullptr, int c = BLACK)
			:element(std::move(e)), left(lt), right(rt), color(c)
		{}
	};
	

	//private data members
	RedBlackNode * header; //root sentinel (stores negInfo, and a right link to the real root)
	RedBlackNode * nullNode; //indicates a NULL pointer

	//use in top-down functions and its helpers
	RedBlackNode *current;
	RedBlackNode *parent;
	RedBlackNode *grand;
	RedBlackNode *great;


	void reclaimMemory(RedBlackNode *t)
	{
		if(t != nullNode)
		{
			reclaimMemory(t->left);
			reclaimMemory(t->right);

			delete t;
		}
	}

	RedBlackNode * clone(const RedBlackNode * t)
	{
		//first make sure t is not the nullNode in the tree it blongs.
		if(t == t->left)//in a tree, only the nullNode has it's left and right pointing to itsef. can't test against nullNode!!!
			return nullNode;

		return new RedBlackNode(t->element, clone(t->left), clone(t->right), t->color);
	}

	template<typename VisitFun>
	void printTree(const RedBlackNode *t, VisitFun vf) const
	{
		//first make sure t is not the nullNode
		if(t != t->left) // can be writen as t != nullNode
		{
			printTree(t->left, vf);
			vf(t->element);
			printTree(t->right, vf);
		}
	}
	
	//called during an insertion if a node has two red children
	//and also when we insert a leaf
	//perform flip and rotations
	void handleReorient(const Comparable &x)
	{
		current->color = RED;
		current->left->color = BLACK;
		current->right->color = BLACK;

		if(RED == parent->color)//now both current and its parent are red, violate the property
		{
			grand->color = RED;

			if((x < grand->element) != (x < parent->element)) //current, parent, and grand form an angle
				parent = rotate(x, grand);
			current = rotate(x, great); //after rotation, makes current the root of the subtree
			
			current->color = BLACK;
			
		}

		header->right->color = BLACK; //makes the root red
	}

	
	//theParent is the parent of  the root of the rotated subtree
	RedBlackNode * rotate(const Comparable & x, RedBlackNode * theParent)
	{
		if(x < theParent->element) //the subtree is theParent's left child
		{
			x < theParent->left->element ?
				rotateWithLeftChild(theParent->left) :
				rotateWithRightChild(theParent->left);
			return theParent->left; //return the root of the subtree
		}
		else
		{
			x < theParent->right->element ?
				rotateWithLeftChild(theParent->right) :
				rotateWithRightChild(theParent->right);
			return theParent->right; //return root of the subtree
		}
	}

	void rotateWithLeftChild(RedBlackNode * & k2)
	{
		RedBlackNode * k1 = k2->left;
		k2->left = k1->right;
		k1->right = k2;

		k2 = k1; //return k1 as the new root
	}

	void rotateWithRightChild(RedBlackNode * &k1)
	{
		RedBlackNode * k2 = k1->right;
		k1->right = k2->left;
		k2->left = k1;

		k1 = k2; //return k2 as the new root
	}


	/*
	 * makes t red
	 * t is a black leaf , or a black node with one child, the child is red
	**/
	void topDownRed(RedBlackNode *t, RedBlackNode *&theParent)
	{
		RedBlackNode * G = header;
		RedBlackNode * P = header;
		if(BLACK == header->right->left->color && BLACK == header->right->right->color)
		{
			header->right->color = RED;
			G = header;
			P = header->right;
		}

/*
		if(RED == t->color)
		{
			cout<<"t is red"<<endl;
			cout.flush();
		}
		if(t->left != nullNode && t->right != nullNode)
		{
			cout<<"t has two children;"<<endl;
			cout.flush();
			return;
		}

		if(t == nullNode)
		{
			cout<<"t is nullNode"<<endl;
			return;
		}
*/
		const Comparable & x = t->element;
		while(P != t)
		{
			//go to next level
			RedBlackNode * X = (x < P->element) ? P->left : P->right;
			RedBlackNode * T = (X == P->left) ? P->right : P->left;
			
			//attempt makes X red
			if(BLACK == X->left->color && BLACK == X->right->color)//X has two black child or X is a leaf(nullNode is considered to be black)
			{
				if(BLACK == T->left->color && BLACK == T->right->color)
				{
					X->color = RED;
					T->color = RED;
					P->color = BLACK;
				}
				else
				{
					RedBlackNode *R = RED == T->left->color ? T->left : T->right;
					if((R == T->left) != (T == P->left)) //P, T, R form an angle
					{
						rotate(R->element, P);
						rotate(R->element, G);
						
						X->color = RED;
						P->color = BLACK;
					}
					else //form a line
					{
						rotate(R->element, G);
						
						X->color = RED;
						P->color = BLACK;
						T->color = RED;
						R->color = BLACK;
					}
				}

				G = P;
				P = X;
			}
			else //X has one red child, or 
			{
				if(X != t) //not reach t yet; can to to the next level
				{
					G = P;
					P = X;
					X = x < X->element ? X->left : X->right;
					T = (X == P->left) ? P->right : P->left;

					if(RED == X->color)
					{
						G = P;
						P = X;
					}
					else //X is black, T is red
					{
						rotate(T->element, G);

						P->color = RED;
						X->color = BLACK;
						T->color = BLACK;

						G = T;
					}
				}
				else //X == t, and X has one red child
				{
					RedBlackNode *R = X->left != nullNode ? X->left : X->right;
					rotate(R->element, P);

					X->color = RED;
					R->color = BLACK;

					G = R;
					P = X;
				}
			}
		}

		header->right->color = BLACK;//make the root black

		theParent = G;
	}
	
	//the number of black nodes on a path from a node to its leaf
	void checkBlacks(RedBlackNode *t, int blacks) const
	{
		if(t != t->left) // t is not null
		{
			if(BLACK == t->color)
				blacks ++;
			if(t->left == nullNode || t->right == nullNode)
				cout<<t->element<<"--"<<blacks<<endl;
			checkBlacks(t->left, blacks);
			checkBlacks(t->right, blacks);
		}
	}


	//check consecutive red nodes
	void checkReds(RedBlackNode *t) const
	{
		if(t != nullNode)
		{
			if(RED == t->color)
			{
				if(RED == t->left->color)
					cout<<"consecutive reds: "<<t->element<<" "<<t->left->element<<endl;
				else if(RED == t->right->color)
					cout<<"consecutive reds: "<<t->element<<" "<<t->right->element<<endl;
			}

			checkReds(t->left);
			checkReds(t->right);
		}
	}





};



#endif
