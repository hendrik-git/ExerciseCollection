#include <ExerciseCollection/DesignPatternProblems.hpp>
#include <algorithm>
#include <array>
#include <random>
#include <ranges>
#include <stdexcept>
using namespace DesignPatterns;

namespace DesignPatterns::Decorator
{
#pragma region length_validator
	length_validator::length_validator(unsigned min_length) : length(min_length) {}

	auto length_validator::validate(std::string_view password) -> bool
	{
		return password.length() >= length;
	}
#pragma endregion

#pragma region password_validator_decorator
	password_validator_decorator::password_validator_decorator(
		std::unique_ptr<password_validator> validator)
		: inner(std::move(validator))
	{
	}

	auto password_validator_decorator::validate(std::string_view password) -> bool
	{
		return inner->validate(password);
	}
#pragma endregion

#pragma region digit_password_validator
	digit_password_validator::digit_password_validator(
		std::unique_ptr<password_validator> validator)
		: password_validator_decorator(std::move(validator))
	{
	}

	auto digit_password_validator::validate(std::string_view password) -> bool
	{
		if(!password_validator_decorator::validate(password))
		{
			return false;
		}
		return password.find_first_of("0123456789") != std::string::npos;
	}
#pragma endregion

#pragma region case_password_validator

	case_password_validator::case_password_validator(std::unique_ptr<password_validator> validator)
		: password_validator_decorator(std::move(validator))
	{
	}

	auto case_password_validator::validate(std::string_view password) -> bool
	{
		if(!password_validator_decorator::validate(password))
		{
			return false;
		}
		bool haslower = false;
		bool hasupper = false;
		for(size_t i = 0; i < password.length() && !(hasupper && haslower); ++i)
		{
			if(islower(password[i]))
			{
				haslower = true;
			}
			else if(isupper(password[i]))
			{
				hasupper = true;
			}
		}
		return haslower && hasupper;
	}
#pragma endregion

#pragma region symbol_password_validator
	symbol_password_validator::symbol_password_validator(
		std::unique_ptr<password_validator> validator)
		: password_validator_decorator(std::move(validator))
	{
	}

	auto symbol_password_validator::validate(std::string_view password) -> bool
	{
		return password_validator_decorator::validate(password)
				   ? password.find_first_of("!@#$%^&*(){}[]?<>") != std::string::npos
				   : false;
	}
#pragma endregion
}  // namespace DesignPatterns::Decorator

namespace DesignPatterns::Composite
{
#pragma region basic_password_generator
	basic_password_generator::basic_password_generator(size_t const len) noexcept : len_(len) {}

	auto basic_password_generator::generate() -> std::string
	{
		throw std::runtime_error("not implemented");
	}

	auto basic_password_generator::add(std::unique_ptr<password_generator> generator) -> void
	{
		throw std::runtime_error("not implemented");
	}

	auto basic_password_generator::length() const -> size_t
	{
		return len_;
	}
#pragma endregion

#pragma region generators
	auto digit_generator::allowed_chars() const -> std::string
	{
		return "0123456789";
	}

	auto symbol_generator::allowed_chars() const -> std::string
	{
		return "!@#$%^&*(){}[]?<>";
	}

	auto upper_letter_generator::allowed_chars() const -> std::string
	{
		return "ABCDEFGHIJKLMNOPQRSTUVXYWZ";
	}

	auto lower_letter_generator::allowed_chars() const -> std::string
	{
		return "abcdefghijklmnopqrstuvxywz";
	}
#pragma endregion

#pragma region composite_password_generator
	auto composite_password_generator::allowed_chars() const -> std::string
	{
		throw std::runtime_error("not implemented");
	};

	auto composite_password_generator::length() const -> size_t
	{
		throw std::runtime_error("not implemented");
	};

	composite_password_generator::composite_password_generator()
	{
		auto seed_data = std::array<int, std::mt19937::state_size>{};
		std::ranges::generate(seed_data, std::ref(rdev_));
		std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
		engine_.seed(seq);
	}

	auto composite_password_generator::generate() -> std::string
	{
		std::string password;
		for(const auto& generator : generators_)
		{
			std::string					  chars = generator->allowed_chars();
			std::uniform_int_distribution udistr(0, static_cast<int>(chars.length() - 1));
			for(size_t i = 0; i < generator->length(); ++i)
				password += chars[udistr(engine_)];
		}
		std::ranges::shuffle(password, engine_);
		return password;
	}

	auto composite_password_generator::add(std::unique_ptr<password_generator> generator) -> void
	{
		generators_.push_back(std::move(generator));
	}
#pragma endregion
}  // namespace DesignPatterns::Composite
{