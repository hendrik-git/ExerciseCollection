#include <ExerciseCollection/RegexProblems.hpp>
#include <boost/regex.hpp>
#include <regex>

namespace Regex
{
	namespace Boost
	{
		auto validate_license_plate_format(const std::string_view str) -> bool
		{
			static const boost::regex reg{R"([A-Z]{3}-[A-Z]{2} \d{3,4})"};
			return boost::regex_match(str.data(), reg);
		}

		auto extract_license_plate_numbers(const std::string& str) -> std::vector<std::string>
		{
			boost::regex			 reg(R"(([A-Z]{3}-[A-Z]{2} \d{3,4})*)");
			boost::smatch			 match;
			std::vector<std::string> results;

			for(auto i = boost::sregex_iterator(std::cbegin(str), std::cend(str), reg);
				i != boost::sregex_iterator();
				++i)
			{
				if((*i)[1].matched)
				{
					results.push_back(i->str());
				}
			}
			return results;
		}

		auto parse_url(const std::string& url) -> std::optional<Regex::url_parts>
		{
			boost::regex rx(
				R"(^(\w+):\/\/([\w.-]+)(:(\d+))?([\w\/\.]+)?(\?([\w=&]*)(#?(\w+))?)?$)");

			if(auto matches = boost::smatch{}; boost::regex_match(url, matches, rx))
			{
				if(matches[1].matched && matches[2].matched)
				{
					url_parts parts;
					parts.protocol = matches[1].str();
					parts.domain   = matches[2].str();
					if(matches[4].matched)
						parts.port = std::stoi(matches[4]);
					if(matches[5].matched)
						parts.path = matches[5];
					if(matches[7].matched)
						parts.query = matches[7];
					if(matches[9].matched)
						parts.fragment = matches[9];
					return parts;
				}
			}
			return {};
		}

		auto transform_date(const std::string& text) -> std::string
		{
			auto rx = boost::regex{R"((\d{1,2})(\.|-|/)(\d{1,2})(\.|-|/)(\d{4}))"};
			return boost::regex_replace(text, rx, std::string{R"($5-$3-$1)"});
		}
	}  // namespace Boost

	namespace Standard
	{
		auto validate_license_plate_format(const std::string_view str) -> bool
		{
			static const std::regex reg(R"([A-Z]{3}-[A-Z]{2} \d{3,4})");
			return std::regex_match(str.data(), reg);
		}

		auto extract_license_plate_numbers(const std::string& str) -> std::vector<std::string>
		{
			std::regex				 reg(R"(([A-Z]{3}-[A-Z]{2} \d{3,4})*)");
			std::smatch				 match;
			std::vector<std::string> results;

			for(auto i = std::sregex_iterator(std::cbegin(str), std::cend(str), reg);
				i != std::sregex_iterator();
				++i)
			{
				if((*i)[1].matched)
				{
					results.push_back(i->str());
				}
			}
			return results;
		}

		auto parse_url(const std::string& url) -> std::optional<Regex::url_parts>
		{
			std::regex rx(R"(^(\w+):\/\/([\w.-]+)(:(\d+))?([\w\/\.]+)?(\?([\w=&]*)(#?(\w+))?)?$)");

			if(auto matches = std::smatch{}; std::regex_match(url, matches, rx))
			{
				if(matches[1].matched && matches[2].matched)
				{
					url_parts parts;
					parts.protocol = matches[1].str();
					parts.domain   = matches[2].str();
					if(matches[4].matched)
						parts.port = std::stoi(matches[4]);
					if(matches[5].matched)
						parts.path = matches[5];
					if(matches[7].matched)
						parts.query = matches[7];
					if(matches[9].matched)
						parts.fragment = matches[9];
					return parts;
				}
			}
			return {};
		}

		auto transform_date(std::string_view text) -> std::string
		{
			auto rx = std::regex{R"((\d{1,2})(\.|-|/)(\d{1,2})(\.|-|/)(\d{4}))"};
			return std::regex_replace(text.data(), rx, R"($5-$3-$1)");
		}
	}  // namespace Standard

}  // namespace Regex