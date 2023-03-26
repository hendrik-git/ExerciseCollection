#include <ExerciseCollection/LanguageFeatures.hpp>
#include <algorithm>
#include <fmt/format.h>
#include <ranges>
#include <vector>

namespace LanguageFeatures
{
#pragma region IPv4

	IPv4::IPv4(uint32_t input)
	{
		address_[0] = input & 0xff;
		address_[1] = (input >> 8) & 0xff;
		address_[2] = (input >> 16) & 0xff;
		address_[3] = (input >> 24) & 0xff;
	}

	IPv4::IPv4(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d)
	{
		address_[3] = a;
		address_[2] = b;
		address_[1] = c;
		address_[0] = d;
	}

	IPv4::IPv4(std::string_view input)
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

	/// @brief Conversion operator to an uint32_t()
	/// @details This represents the array of smaller values as a single value of bigger type
	IPv4::operator uint32_t() const
	{
		constexpr uint32_t m1 = 256;
		constexpr uint32_t m2 = 256 * 256;
		constexpr uint32_t m3 = 256 * 256 * 256;
		return m3 * address_[2] + m2 * address_[2] + m1 * address_[1] + address_[0];
	}

	/// @brief Preincrement operator
	IPv4& IPv4::operator++()
	{
		*this = IPv4(1 + static_cast<uint32_t>(*this));
		return *this;
	}

	/// @brief Postincrement operator
	/// @param dummy parameter to disambiguate from preincrement operator
	IPv4& IPv4::operator++(int)
	{
		*this = IPv4(1 + static_cast<uint32_t>(*this));
		return *this;
	}

	/// @bug error type
	std::ostream& operator<<(std::ostream& os, const IPv4& obj)
	{
		os << fmt::format(
			"{}.{}.{}.{}", obj.address_[3], obj.address_[2], obj.address_[1], obj.address_[0]);

		return os;	// write obj to stream
	}
#pragma endregion

	/// @brief Returns a vector of all IPv4 object inside the defined range
	auto list_all_ipv4_between(const IPv4& start, const IPv4& end) -> std::vector<IPv4>
	{
		std::vector<IPv4> result;
		const auto [min, max] = std::minmax(uint32_t{start}, uint32_t{end});
		for(auto i = min; i < max; i++)
		{
			result.push_back(IPv4{i});
		}
		return result;
	}

}  // namespace LanguageFeatures