#include <sqtree.hpp>

#include <iostream>
#include <string>
#include <vector>

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

	SQTree<int, string> tree(1024);

	vector<string> objects = {"obj1","obj2", "obj3"};

	tree.insert(b2, &objects[0]);
	tree.insert(b3, &objects[1]);
	tree.insert(b4, &objects[2]);

	vector<string *> found;

	tree.search(AABB<int>(8,8,20,20),found);

	cout << "Query" << endl;
	for (auto v : found)
		cout << "\t " << *v << endl;


	return 0;
}