#ifndef SQTREE_HPP
#define SQTREE_HPP

#include <array>
#include <list>
#include <vector>

template <class Ty>
class PoolAlloc
{
public:
	PoolAlloc()
	{

	}

	static void reserve(std::size_t size)
	{
		_used = 0;
		_alloc.resize(size);
	}
	
	static Ty * alloc(Ty& x)
	{
		return &(_alloc[_used++] = x);
	}

	static std::vector<Ty> _alloc;
	static std::size_t _used;
};


//AABB Axis aligned rectangle
template <typename Scalar>
class AABB
{
public:
	AABB()
	{}

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
template <typename Scalar, class Object, std::size_t MAX_ITEMS = 128>
class Node : public AABB<Scalar>
{
public:
	typedef AABB<Scalar> AABBType;
	typedef Object * ObjectListType;

	//TopLeft TopRight BottomLeft BottomRight
	enum
	{
		TL, TR, BL, BR
	};

	Node()
	{

	}

	Node(Node * parent, const AABB<Scalar> & rect) : _parent(parent), AABB<Scalar>(rect)
	{
		_child[TL] = _child[TR] = _child[BL] = _child[BR] = nullptr;
		_count_objects = 0;
	}

	bool insert(const AABBType & objrect, Object * newobj)
	{
		//Half parent size
		Scalar halfs = (Scalar)(0.5*(x2 - x1));
		if (_count_objects < MAX_ITEMS)
		{
			//_objects[_count_objects++] = newobj;
			_objects.push_back(newobj);
			_count_objects++;

			//Node full, create childs
			if (_count_objects == MAX_ITEMS)
			{
				_child[TL] = new Node<Scalar, Object, MAX_ITEMS>(this, AABBType(x1, y1, x1 + halfs, y1 + halfs));
				_child[TR] = new Node<Scalar, Object, MAX_ITEMS>(this, AABBType(x1 + halfs, y1, x2, y1 + halfs));
				_child[BL] = new Node<Scalar, Object, MAX_ITEMS>(this, AABBType(x1, y1 + halfs, x1 + halfs, y2));
				_child[BR] = new Node<Scalar, Object, MAX_ITEMS>(this, AABBType(x1 + halfs, y1 + halfs, x2, y2));

				//_child[TL] = PoolAlloc<Node<Scalar, Object, MAX_ITEMS>>::alloc( Node<Scalar, Object, MAX_ITEMS>(this, AABBType(x1, y1, x1 + halfs, y1 + halfs)) );
				//_child[TR] = PoolAlloc<Node<Scalar, Object, MAX_ITEMS>>::alloc( Node<Scalar, Object, MAX_ITEMS>(this, AABBType(x1 + halfs, y1, x2, y1 + halfs)) );
				//_child[BL] = PoolAlloc<Node<Scalar, Object, MAX_ITEMS>>::alloc( Node<Scalar, Object, MAX_ITEMS>(this, AABBType(x1, y1 + halfs, x1 + halfs, y2)) );
				//_child[BR] = PoolAlloc<Node<Scalar, Object, MAX_ITEMS>>::alloc( Node<Scalar, Object, MAX_ITEMS>(this, AABBType(x1 + halfs, y1 + halfs, x2, y2)) );

			}

			return true;
		}
		else if (AABBType(x1, y1, x1 + halfs, y1 + halfs).contains(objrect))
		{
			return _child[TL]->insert(objrect, newobj);
		}
		else if (AABBType(x1 + halfs, y1, x2, y1 + halfs).contains(objrect))
		{
			return _child[TR]->insert(objrect, newobj);
		}
		else if (AABBType(x1, y1 + halfs, x1 + halfs, y2).contains(objrect))
		{
			return _child[BL]->insert(objrect, newobj);
		}
		else if (AABBType(x1 + halfs, y1 + halfs, x2, y2).contains(objrect))
		{
			return _child[BR]->insert(objrect, newobj);
		}
		else
		{

			//TODO : warning dropping points
			//_objects.push_back(newobj);
			//_count_objects++;
		}

		return false;
	}

	void search(const AABBType & query, std::vector<Object*> & found) const
	{
		if (intersects(query))
		{
			found.insert(found.end(),_objects.begin(), _objects.begin() + _count_objects);
		}

		if (_child[TL])
		{
			_child[TL]->search(query, found);
			_child[TR]->search(query, found);
			_child[BL]->search(query, found);
			_child[BR]->search(query, found);
		}
	}

	Node * _parent;
	std::array<Node *, 4> _child;

	std::size_t _count_objects;
	std::vector<ObjectListType> _objects;
	//std::vector<ObjectListType > _objects;
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

	bool insert(const AABBType & objrect, Object * newobj)
	{
		if (_root->contains(objrect))
			return _root->insert(objrect, newobj);
		return false;
	}

	void search(const AABBType & query, std::vector<Object*> & found) const
	{
		_root->search(query, found);
	}

	NodeType * _root;
};

#endif