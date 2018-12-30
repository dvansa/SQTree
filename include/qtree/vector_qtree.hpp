//
//	Quad Tree functionality implemented with a plain vector
//
// Notes:
// - This is not a Quad Tree. This class is used just for performance comparisons
//
#ifndef VECTOR_QTREE_HPP
#define VECTOR_QTREE_HPP

#include "qtree/qtree.hpp"

#include <array>


// VectorQTreeImpl. Not a Quad Tree just implementing the QuadTree interface with a vector for performance comparisons
//
template <typename ScalarTy, class ObjectRefTy, class QTreeTraits>
class VectorQTreeImpl
{
public:

	using ScalarType = ScalarTy;
	using ObjectRefType = ObjectRefTy;
	using AABBType = AABB<ScalarType>;

	VectorQTreeImpl(ScalarType size)
	{
	}

	bool insert(const AABBType & objrect, const ObjectRefType & newobj)
	{
		_objects.push_back(std::pair< AABBType, ObjectRefType >(objrect, newobj));
		return true;
	}

	void search(const AABBType & query, std::vector<ObjectRefType> & found) const
	{
		for (const auto & obj : _objects)
		{
			if (obj.first.intersects(query) )
				found.push_back(obj.second);
		}
	}

	void remove(const AABBType & obj_aabb, const ObjectRefType & obj)
	{
		std::pair<AABBType, ObjectRefType > rm_obj(obj_aabb, obj);
		auto rm_it = std::find(_objects.begin(), _objects.end(), rm_obj);
		if (rm_it != _objects.end())
		{
			_objects.erase(rm_it);
		}
	}

	std::vector< std::pair<AABBType, ObjectRefType > > _objects;

};

template <typename ScalarType, class ObjectRefType, class QTreeTraits >
using VectorQTree = QTree<ScalarType, ObjectRefType, VectorQTreeImpl<ScalarType, ObjectRefType, QTreeTraits> >;

#endif