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
