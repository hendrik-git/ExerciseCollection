#include <ExerciseCollection/DataSerialization.hpp>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <tomlplusplus/toml.hpp>

namespace DataSerialization
{
#pragma region TOML
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
#pragma endregion

#pragma region JSON
	using json = nlohmann::json;

	/// @brief Implicit conversion function from movie to json-data
	/// @param j input movie to be converted
	/// @param p output json-data
	void to_json(json& j, const movie& m)
	{
		j = json{{"id", m.id},
				 {"title", m.title},
				 {"year", m.year},
				 {"length", m.length},
				 {"directors", m.directors},
				 {"writers", m.writers},
				 {"cast", m.cast}};
	}

	/// @brief Implicit conversion function from json-data to movie
	/// @param j input movie to be converted
	/// @param p output json-data
	void from_json(const json& j, movie& m)
	{
		m.id		= j.at("id").get<unsigned>();
		m.title		= j.at("title").get<std::string>();
		m.year		= j.at("year").get<unsigned>();
		m.length	= j.at("length").get<unsigned>();
		m.cast		= j.at("cast").get<casting_role>();
		m.directors = j.at("directors").get<std::vector<std::string>>();
		m.writers	= j.at("writers").get<std::vector<std::string>>();
	}

	void save_as_json(const movie_list& movies, const fs::path& save_to)
	{
		if(std::ofstream jsonfile(save_to); jsonfile.is_open())
		{
			jsonfile << json{{"movies", movies}}.dump(4) << std::endl;
		}
	}

	auto load_from_json(const fs::path& load_from) -> movie_list
	{
		movie_list result;

		if(std::ifstream ifile(load_from); ifile.is_open())
		{
			try
			{
				json jdata;
				if(ifile >> jdata; jdata.is_object())
				{
					for(const auto& movie : jdata.at("movies"))
					{
						// implicitly converted by from_json()
						result.push_back(movie);
					}
				}
			}
			catch(const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		return result;
	}
#pragma endregion

#pragma region XML
	/// @brief Saves a collection of movies to a file in the XML format
	/// @param movies to save
	/// @param save_to filepath to save to
	void save_as_xml(const movie_list& movies, const fs::path& save_to) {}

	/// @brief Loads a collection of movies from a XML file
	/// @details Parses the file to a TOML table and converts this to a movie_list
	/// @param load_from filepath to load from
	/// @return a collection of movies
	auto load_from_xml(const fs::path& load_from) -> movie_list {}
#pragma endregion

}  // namespace DataSerialization
