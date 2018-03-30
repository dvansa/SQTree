#ifndef SQTREE_HPP
#define SQTREE_HPP

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

template <typename Scalar, class Object>
class SQTree
{
public:
	typedef AABB<Scalar> Box;

	SQTree()
	{}
};

#endif