//
//	Quad Tree straightforward implementation
//
// Notes:
// - Nodes store childs as 4 independent pointers
// - Nodes store object references on themselves
//
#ifndef NAIVE_QTREE_HPP
#define NAIVE_QTREE_HPP

#include "qtree/qtree.hpp"

#include <array>


// Node class
//
template <typename ScalarType, class ObjectRefType, class QTreeTraits >
class Node : public AABB<ScalarType>
{
public:
	using AABBType = AABB<ScalarType>;

	//TopLeft TopRight BottomLeft BottomRight
	enum
	{
		TL, TR, BL, BR
	};

	Node()
	{
		_child[TL] = _child[TR] = _child[BL] = _child[BR] = nullptr;
		_count_objects = 0;
	}

	Node( const AABB<ScalarType> & rect) :  AABB<ScalarType>(rect)
	{
		_child[TL] = _child[TR] = _child[BL] = _child[BR] = nullptr;
		_count_objects = 0;
	}

	bool insert(const AABBType & objrect, ObjectRefType newobj)
	{
		//Half parent size
		ScalarType halfs = (ScalarType)(0.5*(x2 - x1));

		if (_count_objects < QTreeTraits::MAX_NODE_ELEMENTS || halfs < QTreeTraits::MIN_NODE_WIDTH)
		{
			_objects.push_back( std::pair< AABBType, ObjectRefType >( objrect ,newobj) );
			_count_objects++;

			//Node full, create childs and split objects
			if (_count_objects == QTreeTraits::MAX_NODE_ELEMENTS && halfs > QTreeTraits::MIN_NODE_WIDTH)
			{
				_child[TL] = new Node<ScalarType, ObjectRefType, QTreeTraits>(AABBType(x1, y1, x1 + halfs, y1 + halfs));
				_child[TR] = new Node<ScalarType, ObjectRefType, QTreeTraits>(AABBType(x1 + halfs, y1, x2, y1 + halfs));
				_child[BL] = new Node<ScalarType, ObjectRefType, QTreeTraits>(AABBType(x1, y1 + halfs, x1 + halfs, y2));
				_child[BR] = new Node<ScalarType, ObjectRefType, QTreeTraits>(AABBType(x1 + halfs, y1 + halfs, x2, y2));

				for (const auto & obj : _objects)
				{
					//Insert to self node (now it has >= QTreeTraits::MAX_NODE_ELEMENTS)
					this->insert(obj.first, obj.second);
				}

				// Remove objects but keep the count
				_objects.clear();
			}

			return true;
		}
		else
		{
			if (AABBType(x1, y1, x1 + halfs, y1 + halfs).intersects(objrect))
			{
				_child[TL]->insert(objrect, newobj);
			}

			if (AABBType(x1 + halfs, y1, x2, y1 + halfs).intersects(objrect))
			{
				_child[TR]->insert(objrect, newobj);
			}

			if (AABBType(x1, y1 + halfs, x1 + halfs, y2).intersects(objrect))
			{
				_child[BL]->insert(objrect, newobj);
			}

			if (AABBType(x1 + halfs, y1 + halfs, x2, y2).intersects(objrect))
			{
				_child[BR]->insert(objrect, newobj);
			}
		}


		return true;
	}

	void search(const AABBType & query, std::vector<ObjectRefType> & found) const
	{
		// Query intersects with current node
		if (intersects(query))
		{
			// Propagate to its childs
			if (_child[TL])
			{
				_child[TL]->search(query, found);
				_child[TR]->search(query, found);
				_child[BL]->search(query, found);
				_child[BR]->search(query, found);
			}
			// Traverse current node objects and append them
			else
			{
				for (const auto & obj : _objects)
				{
					if (obj.first.intersects(query))
					{
						found.push_back(obj.second);
					}
				}
			}
		}
		
	}

	void remove(const std::pair<AABBType, ObjectRefType> & rm_obj )
	{
		// Query intersects with current node
		if (intersects(rm_obj.first))
		{
			// Propagate to its childs
			if (_child[TL])
			{
				_child[TL]->remove(rm_obj);
				_child[TR]->remove(rm_obj);
				_child[BL]->remove(rm_obj);
				_child[BR]->remove(rm_obj);

				//If all childs empty -> remove them
				if (_child[TL]->_count_objects == 0 && _child[TR]->_count_objects == 0 && _child[BL]->_count_objects == 0 && _child[BR]->_count_objects == 0)
				{
					_count_objects = 0;
					delete _child[TL];
					delete _child[TR];
					delete _child[BL];
					delete _child[BR];
				}
			}
			// Remove from current node
			else
			{
				auto rm_it = std::find(_objects.begin(), _objects.end(), rm_obj);
				if (rm_it != _objects.end())
				{
					_objects.erase(rm_it);
					_count_objects--;
				}
			}
		}

	}

	std::array<Node *, 4> _child;

	std::size_t _count_objects;
	std::vector< std::pair<AABB, ObjectRefType> > _objects;
};



// NaiveQTreeImpl. QTree straightforward implementation
//
template <typename ScalarTy, class ObjectRefTy, class QTreeTraits>
class NaiveQTreeImpl
{
public:

	using ScalarType = ScalarTy;
	using ObjectRefType = ObjectRefTy;
	using NodeType = Node<ScalarType, ObjectRefType, QTreeTraits>;
	using AABBType = AABB<ScalarType>;

	NaiveQTreeImpl(ScalarType size)
	{
		_root = new NodeType(AABBType(0, 0, size, size));
	}

	bool insert(const AABBType & objrect, const ObjectRefType & newobj)
	{
		if (_root->contains(objrect))
			return _root->insert(objrect, newobj);
		return false;
	}

	void search(const AABBType & query, std::vector<ObjectRefType> & found) const
	{
		_root->search(query, found);
	}

	void remove(const AABBType & obj_aabb, const ObjectRefType & obj)
	{
		std::pair<AABBType, ObjectRefType> rm_obj(obj_aabb, obj);

		_root->remove(rm_obj);
	}

	NodeType * _root;
};

template <typename ScalarType, class ObjectRefType, class QTreeTraits >
using NaiveQTree = QTree<ScalarType, ObjectRefType, NaiveQTreeImpl<ScalarType, ObjectRefType, QTreeTraits> >;

#endif