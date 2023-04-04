#include <ExerciseCollection/DataSerialization.hpp>
#include <catch2/catch_all.hpp>
#include <iostream>
#include <ranges>

using namespace DataSerialization;

const movie_list movies{{11001,
						 "The Matrix",
						 1999,
						 196,
						 {"Lana Wachowski", "Lilly Wachowski"},
						 {"Lana Wachowski", "Lilly Wachowski"},
						 {{"Keanu Reeves", "Neo"},
						  {"Laurence Fishburne", "Morpheus"},
						  {"Carrie-Anne Moss", "Trinity"},
						  {"Hugo Weaving", "Agent Smith"}}},
						{9871,
						 "Forrest Gump",
						 1994,
						 202,
						 {"Robert Zemeckis"},
						 {"Winston Groom", "Eric Roth"},
						 {{"Tom Hanks", "Forrest Gump"},
						  {"Sally Field", "Mrs. Gump"},
						  {"Robin Wright", "Jenny Curran"},
						  {"Mykelti Williamson", "Bubba Blue"}}}};


TEST_CASE("Saving data to TOML", "[DataSerialization][TOML]")
{
	fs::path savefile = "movies.toml";
	save_as_toml(movies, savefile);
	REQUIRE(fs::exists(savefile));

	auto loaded_movies = load_from_toml(savefile);

	for(auto i = 0; i < movies.size(); i++)
	{
		CHECK(movies[i] == loaded_movies[i]);

		CHECK(movies[i].title == loaded_movies[i].title);
		CHECK(movies[i].id == loaded_movies[i].id);
		CHECK(movies[i].year == loaded_movies[i].year);
		CHECK(movies[i].length == loaded_movies[i].length);
		CHECK(movies[i].directors == loaded_movies[i].directors);
		CHECK(movies[i].writers == loaded_movies[i].writers);
		CHECK(movies[i].cast == loaded_movies[i].cast);
	}
}