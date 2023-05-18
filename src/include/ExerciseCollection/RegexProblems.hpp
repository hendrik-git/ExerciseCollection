#pragma once
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace Regex
{
	struct url_parts
	{
		std::string				   protocol;
		std::string				   domain;
		std::optional<int>		   port;
		std::optional<std::string> path;
		std::optional<std::string> query;
		std::optional<std::string> fragment;
	};

	inline namespace Boost
	{
		[[nodiscard]] auto validate_license_plate_format(const std::string_view input) -> bool;

		[[nodiscard]] auto extract_license_plate_numbers(const std::string& str)
			-> std::vector<std::string>;

		[[nodiscard]] auto parse_url(const std::string& url) -> std::optional<Regex::url_parts>;

		[[nodiscard]] auto transform_date(const std::string& text) -> std::string;
	}  // namespace Boost

	namespace Standard
	{
		[[nodiscard]] auto validate_license_plate_format(const std::string_view input) -> bool;

		[[nodiscard]] auto extract_license_plate_numbers(const std::string& str)
			-> std::vector<std::string>;

		[[nodiscard]] auto parse_url(const std::string& url) -> std::optional<Regex::url_parts>;

		[[nodiscard]] auto transform_date(std::string_view text) -> std::string;

	}  // namespace Standard

}  // namespace Regex