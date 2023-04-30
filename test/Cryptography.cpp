#include <ExerciseCollection/Cryptography.hpp>
#include <catch2/catch_all.hpp>
#include <fstream>

using namespace Cryptography;

// Write a program that can encrypt and decrypt messages using a Caesar cipher with a right
// rotation and any shift value. For simplicity, the program should consider only uppercase text
// messages and only encode letters, ignoring digits, symbols, and other types of characters.
TEST_CASE("Caesar Cipher", "[Cryptography]")
{
	auto text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for(int i = 1; i < 26; ++i)
	{
		const auto encrypted = caesar_encrypt(text, i);
		const auto decrypted = caesar_decrypt(encrypted, i);
		INFO("Shift by " << i);
		CHECK(text != encrypted);
		CHECK(text == decrypted);
	}
}

// Write a program that can encrypt and decrypt messages using the Vigenère cipher. For simplicity,
// the input plain-text messages for encryption should consist of only uppercase letters.
TEST_CASE("Vigenere Cipher", "[Cryptography]")
{
	auto text	   = "EXERCISECOLLECTION";
	auto encrypted = vigenere_encrypt(text, "KEY");
	auto decrypted = vigenere_decrypt(encrypted, "KEY");
	CHECK(text != encrypted);
	CHECK(text == decrypted);
}


// Write a program that simulates the way users authenticate to a secured system.
// In order to log in, a user must be already registered with the system.
// The user enters a username and a password and the program checks if it matches any of its
// registered users, if it does, the user is granted access, otherwise, the operation fails. For
// security reasons, the system must not record the password but use an SHA hash instead.
TEST_CASE("Login verification", "[Cryptography]")
{
	// registering a new user to the database, the fields could've been aquired by user input
	std::vector<user> database = {
		{0,
		 "Henny",
		 "6B97ED68D14EB3F1AA959CE5D49C7DC612E1EB1DAFD73B1E705847483FD6A6C809F2CEB4E8DF6FF9"
		 "984C6298FF0285CACE6614BF8DAA9F0070101B6C89899E22",
		 "Hendrik",
		 "P."}};

	// try to log in
	const std::string login_username = "Henny";
	const std::string login_password = "hunter2";

	for(const auto& user : database)
	{
		// find the user by username
		if(user.username == login_username)
		{
			// check if the password is correct
			CHECK(user.password == get_hash(login_password));
			break;
		}
	}
}

// Write a program that, given a path to a file, computes and prints to the console the SHA1,
// SHA256, and MD5 hash values for the content of the file.
TEST_CASE("File hashing", "[Cryptography]")
{
	static const auto filepath = fs::path{"ExampleFile.txt"};

	std::ofstream ofs(filepath);
	ofs << "This is some text in the new file\n";
	ofs.close();

	REQUIRE(fs::exists(filepath));

	SECTION("SHA1")
	{
		auto filehash = get_file_hash_SHA1(filepath);
		INFO(filehash);
		CHECK(!filehash.empty());
	}
	SECTION("SHA256")
	{
		auto filehash = get_file_hash_SHA256(filepath);
		INFO(filehash);
		CHECK(!filehash.empty());
	}
}

auto readFile(const fs::path& path) -> std::string
{
	std::ifstream file(path, std::ios::in | std::ios::binary);	// Open the stream to lock the file
	const auto	  size = fs::file_size(path);					// Obtain the size of the file.
	std::string	  result(size, '\0');							// Create a buffer.
	file.read(result.data(), size);								// Read the file into the buffer.
	return result;
}

// Write a program that can encrypt and decrypt files using the Advanced Encryption Standard(AES or
// Rijndael). It should be possible to specify both a source file and a destination file path, as
// well as a password.
TEST_CASE("File encryption", "[Cryptography]")
{
	const auto	  text	   = std::string{"This file will be encrypted and decrypted.\n"};
	const auto	  filepath = fs::path{"EncryptionFile.txt"};
	std::ofstream ofs(filepath);
	ofs << text;
	ofs.close();

	const auto password = "hunter2";

	REQUIRE(fs::exists(filepath));

	SECTION("Encryption with destination")
	{
		REQUIRE_NOTHROW(encrypt_file(filepath, "EncryptionFile.txt.enc", password));
		REQUIRE(fs::exists("EncryptionFile.txt.enc"));
		REQUIRE_NOTHROW(decrypt_file("EncryptionFile.txt.enc", "EncryptionFile.txt.dec", password));
		REQUIRE(fs::exists("EncryptionFile.txt.dec"));
		CHECK(text == readFile("EncryptionFile.txt.dec"));
	}

	SECTION("Same file encryption")
	{
		REQUIRE_NOTHROW(encrypt_file(filepath, password));
		REQUIRE(fs::exists(filepath));
		CHECK(text != readFile(filepath));

		REQUIRE_NOTHROW(decrypt_file(filepath, password));
		REQUIRE(fs::exists(filepath));
		CHECK(text == readFile(filepath));
	}
}

TEST_CASE("File signing", "[Cryptography]")
{
	const auto	  text		  = std::string{"This file will be signed.\n"};
	const auto	  public_key  = fs::path{"rsa-public.key"};
	const auto	  private_key = fs::path{"rsa-private.key"};
	const auto	  filepath	  = fs::path{"SignedFile.txt"};
	const auto	  signed_file = fs::path{"SignedFile.sign"};
	std::ofstream ofs(filepath);
	ofs << text;
	ofs.close();

	generate_keys(private_key, public_key);
	rsa_sign_file(filepath, private_key, signed_file);
	REQUIRE(fs::exists(signed_file));
	CHECK(rsa_verify_file(filepath, public_key, signed_file));
}