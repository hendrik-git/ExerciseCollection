#include <catch2/catch_all.hpp>
#include <iostream>

TEST_CASE("Audio", "[Audio]")
{
	namespace fs = std::filesystem;

	fs::path data_directory{"../../data/sounds"};
	REQUIRE(fs::exists(data_directory));

	sf::SoundBuffer buffer;
	REQUIRE(buffer.loadFromFile(data_directory / "sfx_laser1.ogg"));

	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.play();

	// so the sound can play in a different thread without going out of scope
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(0.8s); 
}