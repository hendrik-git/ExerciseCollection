#include <ExerciseCollection/RegexProblems.hpp>
#include <catch2/catch_all.hpp>
#include <iostream>

using namespace Regex;

// Considering license plates with the format LLL-LL DDD or	LLL-LL DDDD
// (where L is an uppercase letter from A to Z and D is a digit), write:
// - One function that validates that a license plate number is of the correct format
// - One function that, given an input text, extracts and returns all the license plate numbers
// found in the text
TEST_CASE("Regex expression test", "[Regex]")
{
	std::vector<std::string> expected = {"AAA-AA 123", "ABC-DE 1234", "XYZ-WW 0001"};
	std::string				 text	  = "AAA-AA 123qwe-ty 1234 ABC-DE 123456..XYZ-WW 0001";

	SECTION("using boost::regex")
	{
		CHECK(Boost::validate_license_plate_format("ABC-DE 123"));
		CHECK(Boost::validate_license_plate_format("ABC-DE 1234"));
		CHECK(!Boost::validate_license_plate_format("ABC-DE 12345"));
		CHECK(!Boost::validate_license_plate_format("abc-de 1234"));

		CHECK(expected == Boost::extract_license_plate_numbers(text));
	}
	SECTION("using std::regex")
	{
		CHECK(Standard::validate_license_plate_format("ABC-DE 123"));
		CHECK(Standard::validate_license_plate_format("ABC-DE 1234"));
		CHECK(!Standard::validate_license_plate_format("ABC-DE 12345"));
		CHECK(!Standard::validate_license_plate_format("abc-de 1234"));

		CHECK(expected == Standard::extract_license_plate_numbers(text));
	}
}

// --skip-benchmarks
TEST_CASE("Benchmark boost vs std", "[Regex]")
{
	BENCHMARK("Boost Regex (validate)")
	{
		return validate_license_plate_format("ABC-DE 123");
	};
	BENCHMARK("Standard Regex (validate)")
	{
		return Standard::validate_license_plate_format("ABC-DE 123");
	};

	std::vector<std::string> expected = {"AAA-AA 123", "ABC-DE 1234", "XYZ-WW 0001"};
	std::string				 text	  = "AAA-AA 123qwe-ty 1234 ABC-DE 123456..XYZ-WW 0001";


	BENCHMARK("Boost Regex (extract)")
	{
		return expected == Boost::extract_license_plate_numbers(text);
	};
	BENCHMARK("Standard Regex (extract)")
	{
		return expected == Standard::extract_license_plate_numbers(text);
	};
}

// Write a function that, given a string that represents a URL,	parses and extracts the parts of the
// URL (protocol, domain, port, path, query, and fragment).
TEST_CASE("Extract URL parts", "[Regex]")
{
	SECTION("using boost::regex")
	{
		auto url_parsed1 = Standard::parse_url("https://github.com");
		CHECK(url_parsed1.has_value());
		CHECK(url_parsed1->protocol == "https");
		CHECK(url_parsed1->domain == "github.com");
		CHECK(!url_parsed1->port.has_value());
		CHECK(!url_parsed1->path.has_value());
		CHECK(!url_parsed1->query.has_value());
		CHECK(!url_parsed1->fragment.has_value());

		auto url_parsed2 = Standard::parse_url("https://bbc.com:80/en/index.html?lite=true#ui");
		CHECK(url_parsed2.has_value());
		CHECK(url_parsed2->protocol == "https");
		CHECK(url_parsed2->domain == "bbc.com");
		CHECK(url_parsed2->port == 80);
		CHECK(url_parsed2->path.value() == "/en/index.html");
		CHECK(url_parsed2->query.value() == "lite=true");
		CHECK(url_parsed2->fragment.value() == "ui");
	}

	SECTION("using boost::regex")
	{
		auto url_parsed1 = Boost::parse_url("https://github.com");
		CHECK(url_parsed1.has_value());
		CHECK(url_parsed1->protocol == "https");
		CHECK(url_parsed1->domain == "github.com");
		CHECK(!url_parsed1->port.has_value());
		CHECK(!url_parsed1->path.has_value());
		CHECK(!url_parsed1->query.has_value());
		CHECK(!url_parsed1->fragment.has_value());

		auto url_parsed2 = Boost::parse_url("https://bbc.com:80/en/index.html?lite=true#ui");
		CHECK(url_parsed2.has_value());
		CHECK(url_parsed2->protocol == "https");
		CHECK(url_parsed2->domain == "bbc.com");
		CHECK(url_parsed2->port == 80);
		CHECK(url_parsed2->path.value() == "/en/index.html");
		CHECK(url_parsed2->query.value() == "lite=true");
		CHECK(url_parsed2->fragment.value() == "ui");
	}
}

TEST_CASE("Regex string replace", "[Regex]") 
{
	using namespace std::string_literals;

	SECTION("using boost::regex")
	{
		CHECK(Boost::transform_date("today is 01.12.2017!"s) == "today is 2017-12-01!"s);
	}
	SECTION("using Standard::regex")
	{
		CHECK(Standard::transform_date("today is 01.12.2017!"s) == "today is 2017-12-01!"s);
	}
}