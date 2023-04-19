#pragma once
#include <string>
#include <string_view>

namespace Cryptography
{
	auto caesar_encrypt(std::string_view text, const int shift) -> std::string;

	auto caesar_decrypt(std::string_view text, const int shift) -> std::string;
}  // namespace Cryptography