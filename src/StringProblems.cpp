#include <catch2/catch_all.hpp>
#include <fmt/format.h>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>

namespace
{
	/// @brief Helper function to materialize views
	/// @param r a range / view of a container
	/// @return a materialized vector of the range / view
	auto to_vector(auto&& r)
	{
		std::vector<std::ranges::range_value_t<decltype(r)>> v;
		if constexpr(std::ranges::sized_range<decltype(r)>)
		{
			v.reserve(std::ranges::size(r));
		}
		std::ranges::copy(r, std::back_inserter(v));
		return v;
	}
}  // namespace

//=================================================================================================
// Write a function that, given a range of 8-bit integers (such as an array or vector), returns a
// string that contains a hexadecimal representation of the input data.The function should be able
// to produce both uppercase and lowercase content. Here are some input and output examples:
// Input: { 0xBA, 0xAD, 0xF0, 0x0D }, output: "BAADF00D" or "baadf00d"
// Input: { 1,2,3,4,5,6 },            output: "010203040506"

auto to_hexadecimal_string(std::span<int> valids) -> std::string
{
	auto to_string = [](auto i) { return fmt::format("{:02X}", i); };
	auto strings   = valids | std::views::transform(to_string);

	return std::accumulate(strings.begin(), strings.end(), std::string(""));
}

TEST_CASE("Integer to hexadecimal string", "[String]")
{
	using namespace std::string_literals;

	auto input1 = std::vector{0xBA, 0xAD, 0xF0, 0x0D};
	auto input2 = std::array{1, 2, 3, 4, 5, 6};
	CHECK(to_hexadecimal_string(input1) == "BAADF00D"s);
	CHECK(to_hexadecimal_string(input2) == "010203040506"s);
}


//=================================================================================================
// Write a function that, given a string containing hexadecimal digits as the input argument,
// returns a vector of 8 - bit integers that represent the numerical deserialization of the string
// content.The following are examples:
// Input: "BAADF00D" or "baadF00D", output: {0xBA, 0xAD, 0xF0, 0x0D}
// Input "010203040506",            output: {1, 2, 3, 4, 5, 6}

/// @brief Converts an input string to a vector of values
/// @param str input string to convert, 2 characters form a value
/// @return a vector of converted hexadecimal values
auto to_hexadecimal(std::string_view str)
{
	assert(str.size() % 2 == 0 && "invalid amount of characters");

	auto convert_to_char = [](auto cha) -> unsigned char
	{
		if(cha >= '0' && cha <= '9')
			return cha - '0';
		if(cha >= 'A' && cha <= 'F')
			return cha - 'A' + 10;
		if(cha >= 'a' && cha <= 'f')
			return cha - 'a' + 10;

		assert(false && "invalid character passed for conversion");
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


TEST_CASE("String to hexadecimal", "[String]")
{
	using namespace std::string_literals;

	auto input1 = "BAADF00D"s;
	auto input2 = "010203040506"s;
	CHECK(to_hexadecimal(input1) == std::vector{0xBA, 0xAD, 0xF0, 0x0D});
	CHECK(to_hexadecimal(input2) == std::vector{1, 2, 3, 4, 5, 6});
}


//=================================================================================================
// Write a function that transforms an input text into a capitalized version, where every word
// starts with an uppercase letter and has all the other letters in lowercase.For instance, the text
// "the c++ challenger" should be transformed to "The C++ Challenger".

/// @brief Capitalizes the first letter of each word
/// @param input a sentence to capitalize
/// @return a string of capitalized words
auto capitalize(std::string_view input)
{
	assert(input.size() > 0 && "nothing to capitalize");

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


TEST_CASE("Capitalizing an article title", "[String]")
{
	using namespace std::string_literals;
	CHECK(capitalize("the c++ challenger") == "The C++ Challenger"s);
	CHECK(capitalize("do some excercises") == "Do Some Excercises"s);
	CHECK(capitalize("100 simple things") == "100 Simple Things"s);
}


//=================================================================================================
// Write a function that, given a list of strings and a delimiter, creates a new string by
// concatenating all the input strings separated with the specified delimiter. The delimiter must
// not appear after the last string, and when no input string is provided, the function must return
// an empty string.

/// @brief
/// @param input
/// @param delimiter
/// @note this algorithm could be made easier with C++23's std::views::join_with(delimiter)
/// @return
auto concatenate(std::span<std::string> input, std::string_view delimiter)
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


TEST_CASE("Concatenating input", "[String]")
{
	using namespace std::string_literals;
	auto input = std::vector{"this"s, "is"s, "an"s, "example"s};
	auto empty = std::vector<std::string>{};
	CHECK(concatenate(input, " "s) == "this is an example"s);
	CHECK(concatenate(empty, " "s) == ""s);
}