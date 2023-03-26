#include <ExerciseCollection/LanguageFeatures.hpp>
#include <catch2/catch_all.hpp>
#include <deque>

using namespace LanguageFeatures;

//=================================================================================================
// Write a class that represents an IPv4 address. Implement the functions required to be able to
// read and write such addresses from or to the console. The user should be able to input values in
// dotted form, such as 127.0.0.1 or 168.192.0.100. This is also the form in which IPv4 addresses
// should be formatted to an output stream
//
// Write a program that allows the user to input two IPv4 addresses representing a range and list
// all the addresses in that range. Extend the structure defined for the previous problem to
// implement the requested functionality
TEST_CASE("IPv4 class", "[LanguageFeatures]")
{
	using namespace std::string_literals;
	std::stringstream ss1;
	std::stringstream ss2;

	SECTION("String constructor")
	{
		auto ip1 = IPv4{"127.0.0.1"};
		auto ip2 = IPv4{"168.192.0.100"};
		ss1 << ip1;
		ss2 << ip2;
		CHECK(ss1.str() == "127.0.0.1");
		CHECK(ss2.str() == "168.192.0.100");
	}

	SECTION("Value constructor")
	{
		auto ip1 = IPv4{10};
		auto ip2 = IPv4{257};
		ss1 << ip1;
		ss2 << ip2;
		CHECK(ss1.str() == "0.0.0.10");
		CHECK(ss2.str() == "0.0.1.1");
	}

	SECTION("Values constructor")
	{
		auto ip1 = IPv4{127, 0, 0, 1};
		auto ip2 = IPv4{168, 192, 0, 100};
		ss1 << ip1;
		ss2 << ip2;
		CHECK(ss1.str() == "127.0.0.1");
		CHECK(ss2.str() == "168.192.0.100");
	}

	SECTION("Comparison")
	{
		auto ip1 = IPv4{127, 0, 0, 1};
		auto ip2 = IPv4{168, 192, 0, 100};

		CHECK(ip1 < ip2);
	}

	SECTION("Incrementing")
	{
		auto ip = IPv4{0, 0, 0, 1};
		ss1 << ip;
		ss2 << ++ip;
		CHECK(ss1.str() == "0.0.0.1");
		CHECK(ss2.str() == "0.0.0.2");
	}

	SECTION("Range function")
	{
		auto ip1		  = IPv4{0, 0, 0, 1};
		auto ip2		  = IPv4{0, 0, 0, 4};
		auto list_of_ipv4 = list_all_ipv4_between(ip1, ip2);
		CHECK(list_of_ipv4.size() == 3);
		CHECK(static_cast<uint32_t>(list_of_ipv4.at(0)) == 1);
		CHECK(static_cast<uint32_t>(list_of_ipv4.at(1)) == 2);
		CHECK(static_cast<uint32_t>(list_of_ipv4.at(2)) == 3);
	}
}

//=================================================================================================
// Write a general-purpose function that can add any number of elements to the end of a container
// that has a method push_back(T&& value).
TEST_CASE("Generic adding a range of values to a container", "[LanguageFeatures]")
{
	std::vector<int>  ivec;
	std::deque<float> fdeq;
	std::string		  str;

	generic_push_back(ivec, 1, 2);
	generic_push_back(fdeq, 1.F, 2.F);
	generic_push_back(str, 'H', 'P');

	CHECK(ivec.size() == 2);
	CHECK(ivec.at(0) == 1);
	CHECK(ivec.at(1) == 2);
	CHECK(fdeq.size() == 2);
	CHECK(fdeq.at(0) == 1.F);
	CHECK(fdeq.at(1) == 2.F);
	CHECK(str.size() == 2);
	CHECK(str == "HP");
}