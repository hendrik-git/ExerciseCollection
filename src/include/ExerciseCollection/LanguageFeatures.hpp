#pragma once
#include <array>
#include <iostream>
#include <vector>

namespace LanguageFeatures
{
	class IPv4
	{
	  public:
		/// @brief Constructs an IPv4 object from an absolute value
		explicit IPv4(uint32_t input);

		/// @brief Constructs an IPv4 object in the form of "a.b.c.d"
		explicit IPv4(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d);

		/// @brief Constructs an IPv4 object from a string delimited by '.'
		explicit IPv4(std::string_view input);

		/// @brief Spaceship operator
		/// @details Defaulting this operator enables the compiler to generate all comparator
		/// functions
		auto operator<=>(const IPv4&) const = default;

		/// @brief Conversion operator to an uint32_t()
		/// @details This represents the array of smaller values as a single value of bigger type
		explicit operator uint32_t() const;

		/// @brief Preincrement operator
		IPv4& operator++();

		/// @brief Postincrement operator
		/// @param dummy parameter to disambiguate from preincrement operator
		IPv4& operator++(int);

		/// @brief Stream insertion operator
		/// @param os target output stream
		/// @param obj insertion target
		/// @return input stream to allow chaining
		friend std::ostream& operator<<(std::ostream& os, const IPv4& obj);

	  private:
		std::array<uint8_t, 4> address_;
	};

	std::ostream& operator<<(std::ostream& os, const IPv4& obj);

	/// @brief Returns a vector of all IPv4 object inside the defined range
	auto list_all_ipv4_between(const IPv4& start, const IPv4& end) -> std::vector<IPv4>;

	template<typename T, typename... Args>
	concept push_back_able = requires(T& container, Args&&... args)
	{
		(container.push_back(args), ...);
	};

	template<push_back_able T, typename... Args>
	auto generic_push_back(T& container, Args&&... args)
	{
		(container.push_back(args), ...);
	}
}  // namespace LanguageFeatures