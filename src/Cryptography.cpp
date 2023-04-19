#include <ExerciseCollection/Cryptography.hpp>

namespace Cryptography
{
	auto caesar_encrypt(std::string_view text, const int shift) -> std::string
	{
		std::string result;
		result.reserve(text.length());
		for(const auto cha : text)
		{
			result += (isalpha(cha) && isupper(cha)) ? 'A' + (cha - 'A' + shift) % 26 : cha;
		}
		return result;
	}

	auto caesar_decrypt(std::string_view text, const int shift) -> std::string
	{
		std::string result;
		result.reserve(text.length());
		for(const auto cha : text)
		{
			result += (isalpha(cha) && isupper(cha)) ? 'A' + (26 + cha - 'A' - shift) % 26 : cha;
		}
		return result;
	}
}  // namespace Cryptography