#include <catch2/catch_all.hpp>
#include <concepts>
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


//=================================================================================================
// Largest prime number below given value

/// @brief Returns the largest prime number up to a given value
/// @param value Upper limit for which to search prime numbers for
auto find_largest_prime_up_to(int value)
{
	assert(value > 0 && "requires a positive non zero input");

	// if(value == 1)
	//	return 1;

	// a prime number can only be divided by itself and 1
	auto is_prime_number = [](auto value)
	{
		bool is_prime = true;

		for(auto i = value - 1; i > 1; --i)
		{
			if(value % i == 0)
				is_prime = false;
		}

		return is_prime;
	};

	while(value > 0)
	{
		if(is_prime_number(value))
			return value;
		--value;
	}
	return 1;
}

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

/// @brief Approximates PI using the Gregory-Leibniz series
/// @details The equation is 
/// $$\pi = (4/1)-(4/3)+(4/5)-(4/7)+(4/9)-(4/11)+(4/13)-(4/15) ...$$
/// @tparam T float or double
/// @param iterations how many iterations are run to approximate pi
/// @return a floating point type
template<typename T>
requires std::floating_point<T>
auto approximate_pi(size_t iterations)
{
	T pi = 0;
	for(auto i = 0; i < iterations; ++i)
	{
		T sign = i % 2 == 0 ? 1.0 : -1.0;
		pi += sign * static_cast<T>((4.0 / (1.0 + 2.0 * i)));
	}
	return pi;
}

TEST_CASE("Approximate Pi", "[Math]")
{
	CHECK_THAT(approximate_pi<float>(100), Catch::Matchers::WithinRel(3.14159, 0.01));
	CHECK_THAT(approximate_pi<double>(1000), Catch::Matchers::WithinRel(3.14159, 0.001));
}