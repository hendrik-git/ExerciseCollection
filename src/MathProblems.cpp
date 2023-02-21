#include <catch2/catch_all.hpp>
#include <iostream>
#include <numeric>
#include <ranges>

//=================================================================================================
// Write a function that returns all values up to a given number,
// for which all numbers are divisible by 3 or 5

/// @brief Returns all divisors up to the maximum value for a set of divisors
/// @param max_val up to this value divisors will be searched for
/// @param valids a value is considered valid, if any value can be divided by a value of this set
/// @return a vector containing all values found
auto get_divisible_numbers(int max_val, std::set<int> valids)
{
	std::vector<int> result(max_val);
	std::iota(result.begin(), result.end(), 0);	 // ranges::iota in C++23

	std::erase_if(result,
				  [&valids](auto current_value)
				  {
					  return std::ranges::none_of(valids,
												  [current_value](auto dividor)
												  { return current_value % dividor == 0; });
				  });
	return result;
}

TEST_CASE("Divisible by 3 or 5", "[Math]")
{
	auto values				 = get_divisible_numbers(100, {3, 5});
	auto divisible_by_3_or_5 = [](auto x) { return (x % 3) == 0 || (x % 5) == 0; };
	CHECK(std::ranges::all_of(values, divisible_by_3_or_5));
}


//=================================================================================================
// Write a function that, given two positive integers, will calculate and print the greatest common
// divisor of the two

/// @brief Returns the greatest common divisor of two positive input numbers
/// @note there is also std::gcd() in the <numerics> header
auto greatest_common_divisor(auto a, auto b) -> int
{
	assert(a > 0 && "requires a positive input for a");
	assert(b > 0 && "requires a positive input for b");

	for(auto i = std::min(a, b); i > 0; --i)
	{
		if(a % i == 0 && b % i == 0)
			return i;
	}
};

TEST_CASE("Greatest common divisor", "[Math]")
{
	CHECK(greatest_common_divisor(3, 1) == std::gcd(3, 1));
	CHECK(greatest_common_divisor(12, 16) == std::gcd(12, 16));
	CHECK(greatest_common_divisor(99, 33) == std::gcd(99, 33));
}


//=================================================================================================
// Write a program that will, given two or more positive integers, calculate and print the least
// common multiple of them all.

/// @brief Returns the least common multiple of two positive input numbers
/// @note there is also std::lcm() in the <numerics> header
auto least_common_multiple(auto a, auto b) -> int
{
	assert(a > 0 && "requires a positive input for a");
	assert(b > 0 && "requires a positive input for b");

	for(auto i = std::max(a, b); i <= (a * b); ++i)
	{
		if(i % a == 0 && i % b == 0)
			return i;
	}
};

TEST_CASE("Least common multiple", "[Math]")
{
	CHECK(least_common_multiple(3, 1) == std::lcm(3, 1));
	CHECK(least_common_multiple(12, 16) == std::lcm(12, 16));
	CHECK(least_common_multiple(99, 33) == std::lcm(99, 33));
}