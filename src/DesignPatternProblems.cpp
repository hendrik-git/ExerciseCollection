#include <catch2/catch_all.hpp>
#include <string>
#include <string_view>

//=================================================================================================
// Write a program that validates password strength based on predefined rules, which may then be
// selected in various combinations.At a minimum, every password must meet a minimum length
// requirement.In addition, other rules could be enforced, such as the presence of at least one
// symbol, digit, uppercase and lowercase letter, and so on

class password_validator
{
  public:
	virtual ~password_validator()							 = default;
	virtual auto validate(std::string_view password) -> bool = 0;
};

class length_validator final : public password_validator
{
  public:
	explicit length_validator(unsigned min_length) : length(min_length) {}

	~length_validator() override = default;

	auto validate(std::string_view password) -> bool override
	{
		return password.length() >= length;
	}

  private:
	unsigned length;  //!< password should meet or exceed this length
};

/// @brief Implements a decorator for a password_validator
/// @details The decorator design pattern allows for adding behavior to an object without affecting
/// other objects of the same type. This is achieved by wrapping an object within another object.
/// Multiple decorators could be stackedon top of each other, each time adding new functionality.
class password_validator_decorator : public password_validator
{
	/// @brief The actual class for which the decorator adds functionality
	std::unique_ptr<password_validator> inner;

  public:
	explicit password_validator_decorator(std::unique_ptr<password_validator> validator)
		: inner(std::move(validator))
	{
	}

	~password_validator_decorator() override = default;

	auto validate(std::string_view password) -> bool override
	{
		return inner->validate(password);
	}
};

class digit_password_validator final : public password_validator_decorator
{
  public:
	explicit digit_password_validator(std::unique_ptr<password_validator> validator)
		: password_validator_decorator(std::move(validator))
	{
	}

	~digit_password_validator() final = default;

	auto validate(std::string_view password) -> bool override
	{
		if(!password_validator_decorator::validate(password))
		{
			return false;
		}
		return password.find_first_of("0123456789") != std::string::npos;
	}
};

class case_password_validator final : public password_validator_decorator
{
  public:
	explicit case_password_validator(std::unique_ptr<password_validator> validator)
		: password_validator_decorator(std::move(validator))
	{
	}

	~case_password_validator() final = default;

	auto validate(std::string_view password) -> bool override
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
};

class symbol_password_validator final : public password_validator_decorator
{
  public:
	explicit symbol_password_validator(std::unique_ptr<password_validator> validator)
		: password_validator_decorator(std::move(validator))
	{
	}

	~symbol_password_validator() final = default;

	auto validate(std::string_view password) -> bool override
	{
		if(!password_validator_decorator::validate(password))
		{
			return false;
		}
		return password.find_first_of("!@#$%^&*(){}[]?<>") != std::string::npos;
	}
};


TEST_CASE("Validating passwords", "[DesignPatterns]")
{
	auto validator1 =
		std::make_unique<digit_password_validator>(std::make_unique<length_validator>(8));
	CHECK(validator1->validate("abc123!@#") == true);
	CHECK(validator1->validate("abcde!@#") == false);  // missing digit

	auto has_length = std::make_unique<length_validator>(8);
	auto has_digits = std::make_unique<digit_password_validator>(std::move(has_length));
	auto has_case	= std::make_unique<case_password_validator>(std::move(has_digits));
	auto validator2 = std::make_unique<symbol_password_validator>(std::move(has_case));
	CHECK(validator2->validate("Abc123!@#") == true);
	CHECK(validator2->validate("Abc123567") == false);	// missing special symbol
}


//=================================================================================================
// Write a program that can generate random passwords according to some predefined rules. Every
// password must have a configurable minimum length.In addition, it should be possible to include in
// the generation rules such as the presence of at least one digit, symbol, lower or uppercase
// character, and so on. These additional rules must be configurable and composable.

class password_generator
{
  public:
	virtual ~password_generator() = default;

	virtual auto generate() -> std::string									= 0;
	virtual auto allowed_chars() const -> std::string						= 0;
	virtual auto length() const -> size_t									= 0;
	virtual auto add(std::unique_ptr<password_generator> generator) -> void = 0;
};


TEST_CASE("Generating passwords", "[DesignPatterns]")
{
}