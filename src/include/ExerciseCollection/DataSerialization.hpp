#pragma once
#include <filesystem>
#include <map>
#include <string>
#include <vector>


/// @brief Exercises related to loading and saving in different formats
namespace DataSerialization
{
	namespace fs = std::filesystem;

	/// @brief Maps an actor to their role
	using casting_role = std::map<std::string, std::string, std::less<>>;

	/// @brief Holds all data relevant to a single movie
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

} // namespace DataSerialization