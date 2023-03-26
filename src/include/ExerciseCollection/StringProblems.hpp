#pragma once
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace StringProblems
{
	auto to_hexadecimal_string(std::span<int> valids) -> std::string;


	/// @brief Converts an input string to a vector of values
	/// @param str input string to convert, 2 characters form a value
	/// @return a vector of converted hexadecimal values
	auto to_hexadecimal(std::string_view str) -> std::vector<int>;


	/// @brief Capitalizes the first letter of each word
	/// @param input a sentence to capitalize
	/// @return a string of capitalized words
	auto capitalize(std::string_view input) -> std::string;


	/// @brief
	/// @param input
	/// @param delimiter
	/// @note this algorithm could be made easier with C++23's std::views::join_with(delimiter)
	/// @return
	auto concatenate(std::span<std::string> input, std::string_view delimiter) -> std::string;

}  // namespace StringProblems