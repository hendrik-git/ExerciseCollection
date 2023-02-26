#include <catch2/catch_all.hpp>
#include <compare>
#include <fmt/format.h>
#include <iostream>
#include <numeric>
#include <ranges>

//=================================================================================================
// Write a class that represents an IPv4 address. Implement the functions required to be able to
// read and write such addresses from or to the console. The user should be able to input values in
// dotted form, such as 127.0.0.1 or 168.192.0.100. This is also the form in which IPv4 addresses
// should be formatted to an output stream

/// @brief
/// @details An IPv4 adress is a 32-bit value, usually represented in decimal dotted format, such as
/// 168.192.0.100; each part of it is an 8-bit value, ranging from 0 to 255
class IPv4
{
  public:
	/// @brief Constructs an IPv4 object from an absolute value
	IPv4(uint32_t input)
	{
		address_[0] = input & 0xff;
		address_[1] = (input >> 8) & 0xff;
		address_[2] = (input >> 16) & 0xff;
		address_[3] = (input >> 24) & 0xff;
	}

	/// @brief Constructs an IPv4 object in the form of "a.b.c.d"
	IPv4(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d)
	{
		address_[3] = a;
		address_[2] = b;
		address_[1] = c;
		address_[0] = d;
	}

	/// @brief Constructs an IPv4 object from a string delimited by '.'
	IPv4(std::string_view input)
	{
		// needs to be a string_view, as a rvalue string ends with \0
		constexpr std::string_view delimiter{"."};

		auto index = 3;
		for(const auto word : std::views::split(input, delimiter))
		{
			std::string number;
			std::ranges::copy(word, std::back_inserter(number));
			address_[index] = static_cast<uint8_t>(std::stoi(number));

			if(0 == index)
				return;
			index--;
		}
	}

	/// @brief Spaceship operator
	/// @details Defaulting this operator enables the compiler to generate all comparator functions
	auto operator<=>(const IPv4&) const = default;

	/// @brief Conversion operator to an uint32_t()
	/// @details This represents the array of smaller values as a single value of bigger type
	explicit operator uint32_t() const
	{
		constexpr uint32_t m1 = 256;
		constexpr uint32_t m2 = 256 * 256;
		constexpr uint32_t m3 = 256 * 256 * 256;
		return m3 * address_[2] + m2 * address_[2] + m1 * address_[1] + address_[0];
	}

	/// @brief Preincrement operator
	IPv4& operator++()
	{
		*this = IPv4(1 + static_cast<uint32_t>(*this));
		return *this;
	}

	/// @brief Postincrement operator
	/// @param dummy parameter to disambiguate from preincrement operator
	IPv4& operator++(int)
	{
		*this = IPv4(1 + static_cast<uint32_t>(*this));
		return *this;
	}

	/// @brief Stream insertion operator
	/// @param os target output stream
	/// @param obj insertion target
	/// @return input stream to allow chaining
	friend std::ostream& operator<<(std::ostream& os, const IPv4& obj);

  private:
	std::array<uint8_t, 4> address_;
};

std::ostream& operator<<(std::ostream& os, const IPv4& obj)
{
	os << fmt::format(
		"{}.{}.{}.{}", obj.address_[3], obj.address_[2], obj.address_[1], obj.address_[0]);

	// write obj to stream
	return os;
}

//=================================================================================================
// Write a program that allows the user to input two IPv4 addresses representing a range and list
// all the addresses in that range. Extend the structure defined for the previous problem to
// implement the requested functionality

/// @brief Returns a vector of all IPv4 object inside the defined range
///
auto list_all_ipv4_between(const IPv4& start, const IPv4& end)
{
	std::vector<IPv4> result;
	const auto [min, max] = std::minmax(uint32_t{start}, uint32_t{end});
	for(auto i = min; i < max; i++)
	{
		result.push_back({i});
	}
	return result;
}

TEST_CASE("IPv4 class", "[LanguageFeatures]")
{
	using namespace std::string_literals;
	std::stringstream ss1;
	std::stringstream ss2;

	SECTION("String constructor")
	{
		auto ip1 = IPv4{"127.0.0.1"};
		auto ip2 = IPv4{"168.192.0.100"};
		ss1 << ip1;
		ss2 << ip2;
		CHECK(ss1.str() == "127.0.0.1");
		CHECK(ss2.str() == "168.192.0.100");
	}

	SECTION("Value constructor")
	{
		auto ip1 = IPv4{10};
		auto ip2 = IPv4{257};
		ss1 << ip1;
		ss2 << ip2;
		CHECK(ss1.str() == "0.0.0.10");
		CHECK(ss2.str() == "0.0.1.1");
	}

	SECTION("Values constructor")
	{
		auto ip1 = IPv4{127, 0, 0, 1};
		auto ip2 = IPv4{168, 192, 0, 100};
		ss1 << ip1;
		ss2 << ip2;
		CHECK(ss1.str() == "127.0.0.1");
		CHECK(ss2.str() == "168.192.0.100");
	}

	SECTION("Comparison")
	{
		auto ip1 = IPv4{127, 0, 0, 1};
		auto ip2 = IPv4{168, 192, 0, 100};

		CHECK(ip1 < ip2);
	}

	SECTION("Incrementing")
	{
		auto ip = IPv4{0, 0, 0, 1};
		ss1 << ip;
		ss2 << ++ip;
		CHECK(ss1.str() == "0.0.0.1");
		CHECK(ss2.str() == "0.0.0.2");
	}

	SECTION("Range function")
	{
		auto ip1 = IPv4{0, 0, 0, 1};
		auto ip2 = IPv4{0, 0, 0, 4};
		auto list_of_ipv4 = list_all_ipv4_between(ip1, ip2);
		CHECK(list_of_ipv4.size() == 3);
		CHECK(list_of_ipv4.at(0) == 1);
		CHECK(list_of_ipv4.at(1) == 2);
		CHECK(list_of_ipv4.at(2) == 3);
	}
}
