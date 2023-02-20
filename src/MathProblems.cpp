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
	auto values = get_divisible_numbers(100, {3, 5});
	REQUIRE(std::ranges::all_of(values, [](auto x) { return (x % 3) == 0 || (x % 5) == 0; }));
}


//=================================================================================================
// Write a function that, given two positive integers, will calculate and print the greatest common
// divisor of the two

/// @brief Returns the greatest common divisor of two positive input numbers
/// @note there is also std::gcd() in the <numerics> header
auto greatest_common_divisor(auto a, auto b) -> std::optional<int>
{
	assert(a > 0 && "requires a positive input for a");
	assert(b > 0 && "requires a positive input for b");

	for(auto i = std::min(a, b); i > 0; --i)
	{
		if(a % i == 0 && b % i == 0)
			return i;
	}
	return {};
};

TEST_CASE("Greatest common divisor", "[Math]")
{
	REQUIRE(greatest_common_divisor(03, 01).value() == std::gcd(03, 01));
	REQUIRE(greatest_common_divisor(12, 16).value() == std::gcd(12, 16));
	REQUIRE(greatest_common_divisor(99, 33).value() == std::gcd(99, 33));
}