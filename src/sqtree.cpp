#include <qtree/vector_qtree.hpp>
#include <qtree/naive_qtree.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <set>

#include <timer.hpp>

using namespace std;

// Performance evaluation for a QuadTree implementation
//
template <class QTreeType>
void performance_qtree(const std::string & name, QTreeType & qtree, const std::vector< typename QTreeType::AABBType > & points, const std::vector< typename QTreeType::AABBType > & queries)
{
	cout << "Performance : " << name << endl;
	cout << "- - - - - - - - - - - - - - - - - -" << endl;

	QTreeType::ObjectRefType default_ref_value;

	TimerHigh time;
	float elaps;

	// Operation : insert
	time.start();
	for (const auto & p : points)
	{
		qtree.insert(p, default_ref_value);
	}

	elaps = time.get_elapsed_millis();
	cout << "\t INSERT\t " << points.size() << " points\t\t" << (points.size() / elaps) << " ops/ms" << endl;

	// Operation : search
	time.start();

	vector< QTreeType::ObjectRefType > q_results;
	for (const auto & q : queries)
	{
		qtree.search(q, q_results);
	}

	elaps = time.get_elapsed_millis();
	cout << "\t SEARCH\t " << queries.size() << " queries\t\t" << (queries.size() / elaps) << " ops/ms" << endl;

	// Operation : remove
	time.start();
	const float rm_percent = 0.01f;
	for (size_t i = 0; i < rm_percent*points.size(); ++i)
	{
		qtree.remove(points[i], default_ref_value);
	}

	elaps = time.get_elapsed_millis();
	cout << "\t REMOVE\t " << rm_percent*points.size() << " points\t\t" << (rm_percent*queries.size() / elaps) << " ops/ms" << " millisec" << endl;

	cout << "- - - - - - - - - - - - - - - - - -" << endl;
	cout << endl;

}


//Unit Testing Quad Tree
template <class QTreeType>
void test_qtree(const string & name, QTreeType & qtree)
{
	cout << "Testing : " << name << endl;
	cout << "- - - - - - - - - - - - - - - - - -" << endl;

	vector< pair< AABB<int>, int> > test_set;

	test_set.push_back({ AABB<int>(1, 1, 100, 100), 1 });
	test_set.push_back({ AABB<int>(50, 50, 150, 150), 2 });
	test_set.push_back({ AABB<int>(20, 20, 70, 70), 3 });
	test_set.push_back({ AABB<int>(160, 160, 200, 200), 4 });
	
	for (auto & p : test_set)
	{
		qtree.insert(p.first, &(p.second) );
	}

	//Queries
	vector< pair< AABB<int>, set<int> > > test_queries = {
		{ AABB<int>(-1, -1, 3, 3), {1} },
		{ AABB<int>(25, 25, 30, 30), { 1, 3 } },
		{ AABB<int>(155, 155, 157, 157), { } },
		{ AABB<int>(140, 140, 170, 170), { 2,4 } },
		{ AABB<int>(0,0,200,200), { 1,2,3,4 } }
	};
	
	//Evaluate search queries after insert
	int passed_queries = 0;
	for (const auto & tq : test_queries)
	{
		vector< int * > q_res;
		qtree.search(tq.first, q_res);

		set<int> resulting_set;
		for (const auto & r : q_res)
		{
			resulting_set.insert(*r);
		}

		if (resulting_set == tq.second)
			passed_queries++;
	}

	//Remove points (odds)
	for (size_t i = 1; i < test_set.size(); i += 2)
	{
		qtree.remove(test_set[i].first, &test_set[i].second);

		//Remove from resulting queries
		for (auto & tq : test_queries)
		{
			tq.second.erase(test_set[i].second);
		}
	}

	//Evaluate search queries after removals
	for (const auto & tq : test_queries)
	{
		vector< int * > q_res;
		qtree.search(tq.first, q_res);

		set<int> resulting_set;
		for (const auto & r : q_res)
		{
			resulting_set.insert(*r);
		}

		if (resulting_set == tq.second)
			passed_queries++;
	}

	std::cout << "PASSED\t" << passed_queries << " / " << (2*test_queries.size()) << std::endl;
	
	cout << "- - - - - - - - - - - - - - - - - -" << endl;
	cout << endl;
}

int main(int argc, char ** argv)
{

	//Parameters
	const int tree_dim = 1000;
	const int num_points = 1000000;
	const int points_size = 10;
	const int num_queries = 1000;
	const int query_size = 20;

	//Run Unit Tests
	if (argc > 1 && string(argv[1]) == "--run_tests")
	{

		NaiveQTree<int, int*, DefaultQTreeTraits > naive_qtree(tree_dim);
		VectorQTree<int, int*, DefaultQTreeTraits > vector_qtree(tree_dim);

		//Unit Test
		test_qtree< NaiveQTree<int, int*, DefaultQTreeTraits > >("Naive QTree", naive_qtree);
		test_qtree< VectorQTree<int, int*, DefaultQTreeTraits > >("Vector QTree", vector_qtree);

		return 0;
	}


	// Gen. point set
	vector<AABB<int>> points_set(num_points);
	
	for (int i = 0; i < num_points; ++i)
	{
		int x = rand() % tree_dim;
		int y = rand() % tree_dim;
		int size = points_size;
		points_set[i] = AABB<int>(x, y, x + size, y + size);
	}

	// Gen. query set
	vector<AABB<int>> queries_set(num_queries);

	for (int i = 0; i < num_queries; ++i)
	{
		int x = rand() % tree_dim;
		int y = rand() % tree_dim;
		int size = query_size;
		queries_set[i] = AABB<int>(x, y, x + size, y + size);
	}

	//Performance
	NaiveQTree<int, int*, DefaultQTreeTraits > naive_qtree(tree_dim);
	performance_qtree< NaiveQTree<int, int*, DefaultQTreeTraits > >("Naive QTree", naive_qtree, points_set, queries_set);

	VectorQTree<int, int*, DefaultQTreeTraits > vector_qtree(tree_dim);
	performance_qtree< VectorQTree<int, int*, DefaultQTreeTraits > >("Vector", vector_qtree, points_set, queries_set);

	return 0;
}