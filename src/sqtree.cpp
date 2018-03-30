#include <sqtree.hpp>

#include <iostream>

using namespace std;



int main(int argc, char ** argv)
{
	cout << "SQTree" << endl;

	AABB<int> b1(0,0,10,10);
	AABB<int> b2(5,5,15,15);
	AABB<int> b3(2,2, 7,7);
	AABB<int> b4(15, 15, 20, 20);

	cout << b1.intersects(b2) << " - " << b1.contains(b2) << endl;
	cout << b1.intersects(b3) << " - " << b1.contains(b3) << endl;
	cout << b1.intersects(b4) << " - " << b1.contains(b4) << endl;
	return 0;
}