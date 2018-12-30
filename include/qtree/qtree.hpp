//
//	Quad Tree base class and utils
//
//
#ifndef QTREE_HPP
#define QTREE_HPP


#include <vector>


//AABB Axis aligned rectangle
template <typename ScalarTy>
class AABB
{
public:
	AABB()
	{}

	AABB(ScalarTy ix1, ScalarTy iy1, ScalarTy ix2, ScalarTy iy2) : x1(ix1), y1(iy1), x2(ix2), y2(iy2)
	{}

	bool operator ==(const AABB & b) const
	{
		if (x1 == b.x1 && x2 == b.x2 && y1 == b.y1 && y2 == b.y2)
			return true;
		else
			return false;
	}
	
	inline bool intersects(const AABB & b) const
	{
		return !(b.x1 > x2 || b.x2 < x1 || b.y1 > y2 || b.y2 < y1);
	}

	inline bool contains(const AABB & b) const
	{
		return b.x1 > x1 && b.x2 < x2 && b.y1 > y1 && b.y2 < y2;
	}

	ScalarTy x1;
	ScalarTy y1;
	ScalarTy x2;
	ScalarTy y2;
};

// QuadTree configurable parameters
struct DefaultQTreeTraits
{
	enum
	{
		MIN_NODE_WIDTH = 11,		// Limits tree growth (Eqv. to max tree depth)
		MAX_NODE_ELEMENTS = 128 	// Max elements that a node can hold. When limit is reached the node splits (except if MIN_NODE_WIDTH is reached)
	};
};

/// QuadTree base interface
//
// ScalarTy    : scalar type for coordinates
// ObjectRefTy : object reference type, info that nodes store
// DerivImp    : QuadTree implementation
template <typename ScalarTy, class ObjectRefTy, class DerivImp> 
class QTree
{
public:

	using ScalarType =  ScalarTy;
	using ObjectRefType = ObjectRefTy;
	using AABBType = AABB<ScalarType>;

	QTree(ScalarType size) : _impl(size)
	{
	}

	// Insert object (reference) providing its respective area
	bool insert(const AABBType & newobj_aabb, const ObjectRefType & newobj)
	{
		return _impl.insert(newobj_aabb, newobj);
	}

	// Search for all objects intersecting the queried area
	void search(const AABBType & query_aabb, std::vector<ObjectRefType> & found_objs) const
	{
		_impl.search(query_aabb, found_objs);
	}

	//Remove object 
	void remove(const AABBType & obj_aabb, const ObjectRefType & obj)
	{
		_impl.remove(obj_aabb, obj);
	}

	DerivImp _impl;
};

#endif