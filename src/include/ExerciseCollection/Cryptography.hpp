#pragma once
#include <filesystem>
#include <string>
#include <string_view>

namespace Cryptography
{
	namespace fs = std::filesystem;

	/// @brief Encrypts a given text by shifting the letters
	/// @param text to encrypt
	/// @param shift how much to shift each letter
	/// @return encrypted string
	[[nodiscard]] auto caesar_encrypt(std::string_view text, const int shift) -> std::string;

	/// @brief Decrypts a given text by shifting the letters
	/// @param text to decrypt
	/// @param shift for correctly decrypting the text
	/// @return decrypted string
	[[nodiscard]] auto caesar_decrypt(std::string_view text, const int shift) -> std::string;

	/// @brief
	/// @param text
	/// @param key
	/// @return
	[[nodiscard]] auto vigenere_encrypt(std::string_view text, std::string_view key) -> std::string;

	/// @brief
	/// @param text
	/// @param key
	/// @return
	[[nodiscard]] auto vigenere_decrypt(std::string_view text, std::string_view key) -> std::string;

	/// @brief
	struct user
	{
		int			id;			//!<
		std::string username;	//!<
		std::string password;	//!<
		std::string firstname;	//!<
		std::string lastname;	//!<
	};

	/// @brief
	/// @param password
	/// @return
	[[nodiscard]] auto get_hash(std::string_view password) -> std::string;

	/// @brief
	/// @param filepath
	/// @return
	[[nodiscard]] auto get_file_hash_SHA1(fs::path const& filepath) -> std::string;

	/// @brief
	/// @param filepath
	/// @return
	[[nodiscard]] auto get_file_hash_SHA256(fs::path const& filepath) -> std::string;


	/// @brief
	/// @param sourcefile
	/// @param destfile
	/// @param password
	void
	encrypt_file(const fs::path& sourcefile, const fs::path& destfile, std::string_view password);

	/// @brief
	/// @param filepath
	/// @param password
	void encrypt_file(const fs::path& filepath, std::string_view password);

	/// @brief
	/// @param sourcefile
	/// @param destfile
	/// @param password
	void
	decrypt_file(const fs::path& sourcefile, const fs::path& destfile, std::string_view password);

	/// @brief
	/// @param filepath
	/// @param password
	void decrypt_file(const fs::path& filepath, std::string_view password);

	/// @brief
	/// @param privateKeyPath
	/// @param publicKeyPath
	void generate_keys(fs::path const& privateKeyPath, fs::path const& publicKeyPath);

	/// @brief
	/// @param filepath
	/// @param privateKeyPath
	/// @param signaturePath
	void rsa_sign_file(const fs::path& filepath,
					   const fs::path& privateKeyPath,
					   const fs::path& signaturePath);

	/// @brief
	/// @param filepath
	/// @param publicKeyPath
	/// @param signaturePath
	/// @return
	[[nodiscard]] auto rsa_verify_file(const fs::path& filepath,
									   const fs::path& publicKeyPath,
									   const fs::path& signaturePath) -> bool;
}  // namespace Cryptography