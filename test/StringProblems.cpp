#include <ExerciseCollection/StringProblems.hpp>
#include <catch2/catch_all.hpp>

using namespace StringProblems;

//=================================================================================================
// Write a function that, given a range of 8-bit integers (such as an array or vector), returns a
// string that contains a hexadecimal representation of the input data.The function should be able
// to produce both uppercase and lowercase content. Here are some input and output examples:
// Input: { 0xBA, 0xAD, 0xF0, 0x0D }, output: "BAADF00D" or "baadf00d"
// Input: { 1,2,3,4,5,6 },            output: "010203040506"
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
TEST_CASE("Concatenating input", "[String]")
{
	using namespace std::string_literals;
	auto input = std::vector{"this"s, "is"s, "an"s, "example"s};
	auto empty = std::vector<std::string>{};
	CHECK(concatenate(input, " "s) == "this is an example"s);
	CHECK(concatenate(empty, " "s) == ""s);
}