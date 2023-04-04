#include <ExerciseCollection/DataSerialization.hpp>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <tomlplusplus/toml.hpp>

namespace DataSerialization
{
	/// @brief Converts a movie to a toml-table representation
	/// @param movie
	/// @return
	auto movie_to_table(const movie& movie) -> toml::table
	{
		toml::table result;
		result.insert("id", movie.id);
		result.insert("title", movie.title);
		result.insert("year", movie.year);
		result.insert("length", movie.length);
		result.insert("directors",
					  [&]()
					  {
						  toml::array arr;
						  for(const auto& director : movie.directors)
							  arr.push_back(director);
						  return arr;
					  }());
		result.insert("writers",
					  [&]()
					  {
						  toml::array arr;
						  for(const auto& writer : movie.writers)
							  arr.push_back(writer);
						  return arr;
					  }());
		result.insert("cast",
					  [&]()
					  {
						  toml::table tbl;
						  for(const auto& [name, role] : movie.cast)
							  tbl.insert(name, role);
						  return tbl;
					  }());
		return result;
	}

	void save_as_toml(const movie_list& movies, const fs::path& save_to)
	{
		toml::array movie_array;
		for(const auto& movie : movies)
		{
			movie_array.push_back(movie_to_table(movie));
		}

		toml::table toml_data;
		toml_data.insert("movies", movie_array);

		std::ofstream tomlfile;
		tomlfile.open(save_to);
		tomlfile << toml_data;
		tomlfile.close();
	}

	auto load_from_toml(const fs::path& load_from) -> movie_list
	{
		assert(fs::exists(load_from) && "toml file does not exist");

		auto result	  = movie_list{};
		auto tomlfile = toml::parse_file(load_from.c_str());

		const toml::array* movies_arr = tomlfile["movies"].as_array();
		if(movies_arr == nullptr)
		{
			return result;
		}
		auto movie_count = movies_arr->size();

		for(int i = 0; i < movie_count; ++i)
		{
			auto data = tomlfile["movies"][i];

			movie temp;
			temp.title	= data["title"].value<std::string>().value_or("---");
			temp.id		= data["id"].value<unsigned>().value_or(0);
			temp.year	= data["year"].value<unsigned>().value_or(0);
			temp.length = data["length"].value<unsigned>().value_or(0);

			// get the underlying array object to do some more advanced stuff
			if(const auto* writers = data["writers"].as_array())
			{
				for(int j = 0; j < writers->size(); ++j)
				{
					auto str = data["writers"][j].value<std::string>().value_or("---");
					temp.writers.push_back(str);
				}
			}

			// get the underlying array object to do some more advanced stuff
			if(const auto* directors = data["directors"].as_array())
			{
				for(int j = 0; j < directors->size(); ++j)
				{
					auto str = data["directors"][j].value<std::string>().value_or("---");
					temp.directors.push_back(str);
				}
			}

			// get the underlying array object to do some more advanced stuff
			if(const auto* cast = data["cast"].as_table())
			{
				for(const auto& [key, val] : *cast)
				{
					temp.cast[std::string{key.str()}] =
						std::string{val.as_string()->value_or("---")};
				}
			}
			result.push_back(temp);
		}
		return result;
	}
}  // namespace DataSerialization
