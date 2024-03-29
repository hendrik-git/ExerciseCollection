#include <ExerciseCollection/StringProblems.hpp>
#include <cassert>
#include <fmt/format.h>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace StringProblems
{
	namespace
	{
		/// @brief Helper function to materialize views
		/// @param view a range / view of a container
		/// @return a materialized vector of the range / view
		auto to_vector(auto&& view)
		{
			namespace stdr = std::ranges;

			std::vector<stdr::range_value_t<decltype(view)>> vec;
			if constexpr(stdr::sized_range<decltype(view)>)
			{
				vec.reserve(stdr::size(view));
			}
			stdr::copy(view, std::back_inserter(vec));
			return vec;
		}
	}  // namespace


	auto to_hexadecimal_string(std::span<int> valids) -> std::string
	{
		auto to_string = [](auto val) { return fmt::format("{:02X}", val); };
		auto strings   = valids | std::views::transform(to_string);

		return std::accumulate(strings.begin(), strings.end(), std::string(""));
	}

	auto to_hexadecimal(std::string_view str) -> std::vector<int>
	{
		assert(str.size() % 2 == 0 && "invalid amount of characters");

		auto convert_to_char = [](auto cha) -> unsigned char
		{
			const char offset = 10;

			if(cha >= '0' && cha <= '9')
			{
				return cha - '0';
			}
			if(cha >= 'A' && cha <= 'F')
			{
				return cha - 'A' + offset;
			}
			if(cha >= 'a' && cha <= 'f')
			{
				return cha - 'a' + offset;
			}

			assert(false && "invalid character passed for conversion");
			return char{};
		};
		auto chars = str | std::views::transform(convert_to_char);

		std::vector<int> result;
		result.reserve(str.size() / 2);

		for(auto i = 0; i < str.size() / 2; ++i)
		{
			// take the next two digits each iteration and push them combined into the vector
			auto subrange = chars | std::views::drop(i * 2) | std::views::take(2);
			result.push_back(static_cast<int>(16 * subrange[0] + subrange[1]));
		}

		return result;
	}

	auto capitalize(std::string_view input) -> std::string
	{
		assert(!input.empty() && "nothing to capitalize");

		std::string result;

		// needs to be a string_view, as a rvalue string ends with \0
		constexpr std::string_view delimiter{" "};

		for(const auto word : std::views::split(input, delimiter))
		{
			std::string capitalized_word;
			std::ranges::copy(word, std::back_inserter(capitalized_word));
			capitalized_word[0] = std::toupper(capitalized_word[0]);

			result += capitalized_word + " ";
		}
		result.pop_back();	// remove the final whitespace

		return result;
	}

	auto concatenate(std::span<std::string> input, std::string_view delimiter) -> std::string
	{
		std::string result;
		if(input.empty())
		{
			return result;
		}

		for(auto& word : input)
		{
			result = fmt::format("{}{}{}", result, word, delimiter);
		}
		for(auto i = delimiter.size(); i > 0; --i)	// removes the final delimiter
		{
			result.pop_back();
		}
		return result;
	}

}  // namespace StringProblems