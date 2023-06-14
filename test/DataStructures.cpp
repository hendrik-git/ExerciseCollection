#include <ExerciseCollection/DataStructures.hpp>
#include <catch2/catch_all.hpp>
#include <iostream>
#include <thread>

using namespace DataStructures;

TEST_CASE("Priority Queue", "[DataStructures]")
{
	priority_queue<int> queue;
	for(int i : {1, 5, 3, 1, 13, 21, 8})
	{
		queue.push(i);
	}

	REQUIRE(!queue.empty());
	REQUIRE(queue.size() == 7);

	CHECK(queue.top() == 21);
	queue.pop();
	CHECK(queue.top() == 13);
	CHECK(queue.size() == 6);
}

TEST_CASE("Circular buffer", "[DataStructures]")
{
	circular_buffer<int> cbuf(5);  // {0, 0, 0, 0, 0} -> {}
	CHECK(cbuf.empty());
	cbuf.push(1);  // {1, 0, 0, 0, 0} -> {1}
	CHECK(!cbuf.empty());
	cbuf.push(2);			 // {1, 2, 0, 0, 0} -> {1, 2}
	cbuf.push(3);			 // {1, 2, 3, 0, 0} -> {1, 2, 3}
	auto item = cbuf.pop();	 // {1, 2, 3, 0, 0} -> {2, 3}
	CHECK(item == 1);
	cbuf.push(4);		// {1, 2, 3, 4, 0} -> {2, 3, 4}
	cbuf.push(5);		// {1, 2, 3, 4, 5} -> {2, 3, 4, 5}
	cbuf.push(6);		// {6, 2, 3, 4, 5} -> {2, 3, 4, 5, 6}
	cbuf.push(7);		// {6, 7, 3, 4, 5} -> {3, 4, 5, 6, 7}
	cbuf.push(8);		// {6, 7, 8, 4, 5} -> {4, 5, 6, 7, 8}
	item = cbuf.pop();	// {6, 7, 8, 4, 5} -> {5, 6, 7, 8}
	item = cbuf.pop();	// {6, 7, 8, 4, 5} -> {6, 7, 8}
	item = cbuf.pop();	// {6, 7, 8, 4, 5} -> {7, 8}
	item = cbuf.pop();	// {6, 7, 8, 4, 5} -> {8}
	item = cbuf.pop();	// {6, 7, 8, 4, 5} -> {}
	CHECK(cbuf.empty());
	cbuf.push(9);  // {6, 7, 8, 9, 5} -> {9}
}

TEST_CASE("Double buffer", "[DataStructures]")
{
	using namespace std::chrono;
	using namespace std::chrono_literals;

	double_buffer<int> buf(10);
	std::jthread	   thr(
		  [&buf]()
		  {
			  for(int i = 1; i < 1000; i += 10)
			  {
				  int data[] = {i, i + 1, i + 2, i + 3, i + 4, i + 5, i + 6, i + 7, i + 8, i + 9};
				  buf.write(data, 10);
				  std::this_thread::sleep_for(100ms);
			  }
		  });
	auto start = system_clock::now();
	do
	{
		buf.read(std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;

		std::this_thread::sleep_for(150ms);
	} while(duration_cast<seconds>(system_clock::now() - start).count() < 12);
	thr.join();
}
