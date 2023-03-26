#include <ExerciseCollection/MathProblems.hpp>
#include <catch2/catch_all.hpp>
#include <numeric>

using namespace MathProblems;

//=================================================================================================
// Write a function that returns all values up to a given number,
// for which all numbers are divisible by 3 or 5
TEST_CASE("Divisible by 3 or 5", "[Math]")
{
	auto values				 = get_divisible_numbers(100, {3, 5});
	auto divisible_by_3_or_5 = [](auto x) { return (x % 3) == 0 || (x % 5) == 0; };
	CHECK(std::ranges::all_of(values, divisible_by_3_or_5));
}


//=================================================================================================
// Write a function that, given two positive integers, will calculate and print the greatest common
// divisor of the two
TEST_CASE("Greatest common divisor", "[Math]")
{
	CHECK(greatest_common_divisor(3, 1) == std::gcd(3, 1));
	CHECK(greatest_common_divisor(12, 16) == std::gcd(12, 16));
	CHECK(greatest_common_divisor(99, 33) == std::gcd(99, 33));
}


//=================================================================================================
// Write a program that will, given two or more positive integers, calculate and print the least
// common multiple of them all.
TEST_CASE("Least common multiple", "[Math]")
{
	CHECK(least_common_multiple(3, 1) == std::lcm(3, 1));
	CHECK(least_common_multiple(12, 16) == std::lcm(12, 16));
	CHECK(least_common_multiple(99, 33) == std::lcm(99, 33));
}


//=================================================================================================
// Largest prime number below given value
TEST_CASE("Largest prime number", "[Math]")
{
	// 2 3 5 7 11 ...
	CHECK(find_largest_prime_up_to(2) == 2);
	CHECK(find_largest_prime_up_to(3) == 3);
	CHECK(find_largest_prime_up_to(5) == 5);
	CHECK(find_largest_prime_up_to(7) == 7);
	CHECK(find_largest_prime_up_to(8) == 7);
	CHECK(find_largest_prime_up_to(11) == 11);
	CHECK(find_largest_prime_up_to(12) == 11);
}


//=================================================================================================
// Approximate pi
TEST_CASE("Approximate Pi", "[Math]")
{
	CHECK_THAT(approximate_pi<float>(100), Catch::Matchers::WithinRel(3.14159, 0.01));
	CHECK_THAT(approximate_pi<double>(1000), Catch::Matchers::WithinRel(3.14159, 0.001));
}