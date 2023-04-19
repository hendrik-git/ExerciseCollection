#include <ExerciseCollection/Cryptography.hpp>
#include <catch2/catch_all.hpp>

using namespace Cryptography;

//  Write a program that can encrypt and decrypt messages using a Caesar cipher with a right
//  rotation and any shift value. For simplicity, the program should consider only uppercase text
//  messages and only encode letters, ignoring digits, symbols, and other types of characters.
TEST_CASE("Caesar Cipher", "[Cryptography]") 
{
	auto text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for(int i = 1; i < 26; ++i)
	{
		const auto enc = caesar_encrypt(text, i);
		const auto dec = caesar_decrypt(enc, i);
		INFO("Shift by " << i);
		CHECK(text != enc);
		CHECK(text == dec);
	}
}