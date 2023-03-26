#include <ExerciseCollection/MathProblems.hpp>
#include <algorithm>
#include <cassert>
#include <concepts>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>

namespace MathProblems
{
	auto get_divisible_numbers(int max_val, std::set<int> valids) -> std::vector<int>
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

	auto find_largest_prime_up_to(int value) -> int
	{
		assert(value > 0 && "requires a positive non zero input");

		if(value == 1)
			return 1;

		// a prime number can only be divided by itself and 1
		auto is_prime_number = [](auto val)
		{
			bool is_prime = true;

			for(auto i = val - 1; i > 1; --i)
			{
				if(val % i == 0)
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

}  // namespace MathProblems