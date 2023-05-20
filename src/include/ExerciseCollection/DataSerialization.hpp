#pragma once
#include <filesystem>
#include <map>
#include <string>
#include <vector>


/// @brief Exercises related to loading and saving in different formats
namespace DataSerialization
{
	namespace fs = std::filesystem;

	/// @brief Example data to (de)serialize
	inline namespace Data
	{
		/// @brief Maps an actor to their role
		using casting_role = std::map<std::string, std::string, std::less<>>;

		/// @brief Data for a Movie
		struct movie
		{
			unsigned				 id;		 //!< identifier
			std::string				 title;		 //!< of the movie
			unsigned				 year;		 //!< of release
			unsigned				 length;	 //!< movie length
			std::vector<std::string> directors;	 //!< list of directors
			std::vector<std::string> writers;	 //!< list of writers
			casting_role			 cast;		 //!< actors and their role

			/// @brief Default generated comparison operators
			auto operator<=>(const movie&) const = default;
		};

		/// @brief A collection of movies
		using movie_list = std::vector<movie>;

	}  // namespace Data

	/// @brief Functions to (de)serialize data to the TOML format
	inline namespace TOML
	{
		/// @brief Saves a collection of movies to a file in the TOML format
		/// @param movies to save
		/// @param save_to filepath to save to
		void save_as_toml(const movie_list& movies, const fs::path& save_to);

		/// @brief Loads a collection of movies from a TOML file
		/// @details Parses the file to a TOML table and converts this to a movie_list
		/// @param load_from filepath to load from
		/// @return a collection of movies
		auto load_from_toml(const fs::path& load_from) -> movie_list;
	}  // namespace TOML

	/// @brief Functions to (de)serialize data to the JSON format
	inline namespace JSON
	{
		/// @brief Saves a collection of movies to a file in the TOML format
		/// @param movies to save
		/// @param save_to filepath to save to
		void save_as_json(const movie_list& movies, const fs::path& save_to);

		/// @brief Loads a collection of movies from a TOML file
		/// @param load_from filepath to load from
		/// @return a collection of movies
		auto load_from_json(const fs::path& load_from) -> movie_list;

		namespace Boost
		{
			void save_as_json(const movie_list& movies, const fs::path& save_to);
		}
	}  // namespace JSON


	/// @brief Functions to (de)serialize data to the XML format
	inline namespace XML
	{
		/// @brief Saves a collection of movies to a file in the XML format
		/// @param movies to save
		/// @param save_to filepath to save to
		void save_as_xml(const movie_list& movies, const fs::path& save_to);

		/// @brief Loads a collection of movies from a XML file
		/// @param load_from filepath to load from
		/// @return a collection of movies
		auto load_from_xml(const fs::path& load_from) -> movie_list;

		/// @brief Loads a xml-file and filters the nodes with the xpath syntax
		/// @details XPath is a query language used for selecting and navigating through elements
		/// and attributes in an XML or HTML document. It provides a way to traverse the
		/// hierarchical structure of an XML document and to identify specific nodes that match
		/// certain criteria. XPath expressions are written using a syntax similar to that of a file
		/// path, where each element in the path is separated by a forward slash (/) and can be
		/// selected using various filters and predicates.
		/// @see https://en.wikipedia.org/wiki/XPath
		/// @param load_from filepath to load from
		/// @param xpath special parsing syntax to look for matching nodes
		/// @param attribute name of the attribute to read the data from
		/// @return all matching entries as a vector<string>
		auto get_from_xpath(const fs::path&		   load_from,
							const std::string_view xpath,
							const std::string_view attribute) -> std::vector<std::string>;
	}  // namespace XML
}  // namespace DataSerialization