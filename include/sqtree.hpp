#ifndef SQTREE_HPP
#define SQTREE_HPP

#include <array>
#include <list>
#include <vector>

//AABB Axis aligned rectangle
template <typename Scalar>
class AABB
{
public:
	AABB(Scalar ix1, Scalar iy1, Scalar ix2, Scalar iy2) : x1(ix1), y1(iy1), x2(ix2), y2(iy2)
	{}
	
	bool intersects(const AABB & b) const
	{
		return !(b.x1 > x2 || b.x2 < x1 || b.y1 > y2 || b.y2 < y1);
	}

	bool contains(const AABB & b) const
	{
		return b.x1 > x1 && b.x2 < x2 && b.y1 > y1 && b.y2 < y2;
	}

	Scalar x1;
	Scalar y1;
	Scalar x2;
	Scalar y2;
};

//Node
template <typename Scalar, class Object, std::size_t MAX_ITEMS = 16>
class Node : public AABB<Scalar>
{
public:
	typedef AABB<Scalar> AABBType;
	typedef std::pair<AABBType, Object*> ObjectListType;

	//TopLeft TopRight BottomLeft BottomRight
	enum
	{
		TL, TR, BL, BR
	};


	Node(Node * parent, AABB<Scalar> rect) : _parent(parent), AABB<Scalar>(rect)
	{
		_child[0] = _child[1] = _child[2] = _child[3] = nullptr;
	}

	void insert(const AABBType & objrect, Object * newobj)
	{
		Scalar halfs = (Scalar)0.5*(x2 - x1);
		if (_objects.empty())
		{
			_objects.push_front(ObjectListType(objrect, newobj));
		}
		else if (AABBType(x1, y1, x1 + halfs, y1 + halfs).contains(objrect))
		{
			if (_child[TL] == nullptr)
				_child[TL] = new Node<Scalar, Object, MAX_ITEMS>(this, AABBType(x1, y1, x1 + halfs, y1 + halfs));
			_child[TL]->insert(objrect, newobj);
		}
		else if (AABBType(x1 + halfs, y1, x2, y1 + halfs).contains(objrect))
		{
			if (_child[TR] == nullptr)
				_child[TR] = new Node<Scalar, Object, MAX_ITEMS>(this, AABBType(x1 + halfs, y1, x2, y1 + halfs));
			_child[TR]->insert(objrect, newobj);
		}
		else if (AABBType(x1, y1 + halfs, x1 + halfs, y2).contains(objrect))
		{
			if (_child[BL] == nullptr)
				_child[BL] = new Node<Scalar, Object, MAX_ITEMS>(this, AABBType(x1, y1 + halfs, x1 + halfs, y2));
			_child[BL]->insert(objrect, newobj);
		}
		else if (AABBType(x1 + halfs, y1 + halfs, x2, y2).contains(objrect))
		{
			if (_child[BR] == nullptr)
				_child[BR] = new Node<Scalar, Object, MAX_ITEMS>(this, AABBType(x1 + halfs, y1 + halfs, x2, y2));
			_child[BR]->insert(objrect, newobj);
		}
		//Not contained on childs
		else
		{
			_objects.push_front(ObjectListType(objrect, newobj));
		}
	}

	void search(const AABBType & query, std::vector<Object*> & found) const
	{
		if (intersects(query))
		{
			for (auto it = _objects.begin(); it != _objects.end(); ++it)
			{
				if (it->first.intersects(query))
				{
					found.push_back(it->second);
				}
			}
		}

		for (auto i = 0; i < 4; ++i)
			if (_child[i] != nullptr)
				_child[i]->search(query, found);
	}

	Node * _parent;
	std::array<Node *, 4> _child;

	std::list<ObjectListType > _objects;
};

template <typename Scalar, class Object>
class SQTree
{
public:

	typedef Node<Scalar, Object> NodeType;
	typedef AABB<Scalar> AABBType;

	SQTree(Scalar size)
	{
		_root = new NodeType(nullptr, AABBType(0,0,size,size));
	}

	void insert(const AABBType & objrect, Object * newobj)
	{
		if (_root->contains(objrect))
			_root->insert(objrect, newobj);
	}

	void search(const AABBType & query, std::vector<Object*> & found) const
	{
		_root->search(query, found);
	}

	NodeType * _root;
};

#endif