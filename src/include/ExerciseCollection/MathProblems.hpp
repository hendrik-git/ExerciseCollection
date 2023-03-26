#pragma once
#include <algorithm>
#include <cassert>
#include <concepts>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>

namespace MathProblems
{
	/// @brief Returns all divisors up to the maximum value for a set of divisors
	/// @param max_val up to this value divisors will be searched for
	/// @param valids a value is valid, if any value can be divided by a value of this set
	/// @return a vector containing all values found
	auto get_divisible_numbers(int max_val, std::set<int> valids) -> std::vector<int>;

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

	/// @brief Returns the largest prime number up to a given value
	/// @param value Upper limit for which to search prime numbers for
	auto find_largest_prime_up_to(int value) -> int;

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

}  // namespace MathProblems