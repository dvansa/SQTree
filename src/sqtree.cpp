#include <sqtree.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <timer.hpp>

using namespace std;

template class PoolAlloc<Node<int, int>>;

std::vector<Node<int, int>> PoolAlloc<Node<int, int>>::_alloc = std::vector<Node<int, int>>();
std::size_t PoolAlloc<Node<int, int>>::_used = 0;

int main(int argc, char ** argv)
{
	cout << "SQTree" << endl;

	cout << "Testing AABB" << endl;
	cout << "- - - - - - - - - - - - - - - - - -" << endl;
	AABB<int> b1(0,0,10,10);
	AABB<int> b2(5,5,15,15);
	AABB<int> b3(2,2, 7,7);
	AABB<int> b4(15, 15, 20, 20);

	cout << b1.intersects(b2) << " - " << b1.contains(b2) << endl;
	cout << b1.intersects(b3) << " - " << b1.contains(b3) << endl;
	cout << b1.intersects(b4) << " - " << b1.contains(b4) << endl;

	cout << "Testing SQTree" << endl;
	cout << "- - - - - - - - - - - - - - - - - -" << endl;

	//SQTree<int, string> tree(1024);

	//vector<string> objects = {"obj1","obj2", "obj3"};

	//cout << "insert" << endl;
	//tree.insert(b2, &objects[0]); cout << "insert" << endl;
	//tree.insert(b3, &objects[1]); cout << "insert" << endl;
	//tree.insert(b4, &objects[2]);

	//vector<string *> found;

	//cout << "search" << endl;
	//tree.search(AABB<int>(8,8,20,20),found);

	//cout << "Query" << endl;
	//for (auto v : found)
	//	cout << "\t " << *v << endl;

	cout << "Benchmark SQTree" << endl;
	cout << "- - - - - - - - - - - - - - - - - -" << endl;

	PoolAlloc<SQTree<int, int>::NodeType>::reserve(1e7);

	//Parameters
	int tree_dim = 100;
	int points = 1e6;
	int points_size = 1;
	int query_size = 20;

	//Structures
	vector<AABB<int>> areas;
	int value = 1;
	vector<int *> results;

	//Tree
	SQTree<int, int> btree(tree_dim);
	//Timer time;
	TimerHigh time;

	
	//Insert vec
	time.start();
	for (int i = 0; i < points; ++i)
	{
		int x = rand() % tree_dim;
		int y = rand() % tree_dim;
		int size = points_size;
		areas.push_back(AABB<int>(x, y, x + size, y + size));
	}

	cout << "Inserted (Vector) " << points << " points in " << time.get_elapsed_millis() << " microsec" << endl;

	//Insert
	time.start();
	for (int i = 0; i < points; ++i)
	{
		int x = rand() % tree_dim;
		int y = rand() % tree_dim;
		int size = points_size;
		btree.insert(AABB<int>(x, y, x + size, y + size), &value);
		
	}

	cout << "Inserted " << points << " points in " << time.get_elapsed_millis() << " microsec" << endl;

	
	AABB<int> query(0,0, query_size, query_size);

	//Search vector
	time.start();


	for (auto& a : areas)
	{
		if (a.intersects(query))
			results.push_back(&value);
	}

	cout << "Found (vector) " << results.size() << " points in " << time.get_elapsed_millis() << " microsec" << endl;
	results.clear();

	//Search

	results.reserve(50000);
	time.start();

	
	btree.search(query, results);

	cout << "Found " << results.size() << " points in " << time.get_elapsed_millis() << " microsec" << endl;

	


	//Vector
	return 0;
}