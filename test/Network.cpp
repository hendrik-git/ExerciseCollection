#include <ExerciseCollection/Network.hpp>
#include <catch2/catch_all.hpp>
#include <iostream>

using namespace Network;

TEST_CASE("Get IP Adresses", "[Network]")
{
	auto check = get_ip_address("localhost");
	for(auto const& ip : check) {
		std::cout << ip << std::endl;
	}

	CHECK(true);
}