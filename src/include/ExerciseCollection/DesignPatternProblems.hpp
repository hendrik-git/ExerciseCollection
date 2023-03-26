#pragma once
#include <memory>
#include <random>
#include <string>
#include <string_view>

namespace DesignPatterns
{
	class password_validator
	{
	  public:
		virtual ~password_validator()							 = default;
		virtual auto validate(std::string_view password) -> bool = 0;
	};

	class length_validator final : public password_validator
	{
	  public:
		explicit length_validator(unsigned min_length);
		~length_validator() override = default;
		auto validate(std::string_view password) -> bool override;

	  private:
		unsigned length;  //!< password should meet or exceed this length
	};

	/// @brief Implements a decorator for a password_validator
	/// @details The decorator design pattern allows for adding behavior to an object without
	/// affecting other objects of the same type. This is achieved by wrapping an object within
	/// another object. Multiple decorators could be stackedon top of each other, each time adding
	/// new functionality.
	class password_validator_decorator : public password_validator
	{
		/// @brief The actual class for which the decorator adds functionality
		std::unique_ptr<password_validator> inner;

	  public:
		explicit password_validator_decorator(std::unique_ptr<password_validator> validator);

		~password_validator_decorator() override = default;

		auto validate(std::string_view password) -> bool override;
	};

	class digit_password_validator final : public password_validator_decorator
	{
	  public:
		explicit digit_password_validator(std::unique_ptr<password_validator> validator);

		~digit_password_validator() final = default;

		auto validate(std::string_view password) -> bool override;
	};

	class case_password_validator final : public password_validator_decorator
	{
	  public:
		explicit case_password_validator(std::unique_ptr<password_validator> validator);

		~case_password_validator() override = default;

		auto validate(std::string_view password) -> bool override;
	};

	class symbol_password_validator final : public password_validator_decorator
	{
	  public:
		explicit symbol_password_validator(std::unique_ptr<password_validator> validator);

		~symbol_password_validator() override = default;

		auto validate(std::string_view password) -> bool override;
	};

	/// @brief Interface class for the composite pattern
	class password_generator
	{
	  public:
		virtual ~password_generator() = default;

		virtual auto generate() -> std::string									= 0;
		virtual auto allowed_chars() const -> std::string						= 0;
		virtual auto length() const -> size_t									= 0;
		virtual auto add(std::unique_ptr<password_generator> generator) -> void = 0;
	};

	class basic_password_generator : public password_generator
	{
		size_t len_;

	  public:
		explicit basic_password_generator(size_t const len) noexcept;

		auto generate() -> std::string override;

		auto add(std::unique_ptr<password_generator> generator) -> void override;

		auto length() const -> size_t final;
	};

	class digit_generator : public basic_password_generator
	{
	  public:
		using basic_password_generator::basic_password_generator;

		auto allowed_chars() const -> std::string override;
	};

	class symbol_generator : public basic_password_generator
	{
	  public:
		using basic_password_generator::basic_password_generator;

		auto allowed_chars() const -> std::string override;
	};

	class upper_letter_generator final : public basic_password_generator
	{
	  public:
		using basic_password_generator::basic_password_generator;

		auto allowed_chars() const -> std::string override;
	};

	class lower_letter_generator final : public basic_password_generator
	{
	  public:
		using basic_password_generator::basic_password_generator;

		auto allowed_chars() const -> std::string override;
	};

	class composite_password_generator final : public password_generator
	{
		[[noreturn]] auto allowed_chars() const -> std::string override;

		[[noreturn]] auto length() const -> size_t override;

	  public:
		composite_password_generator();

		auto generate() -> std::string override;

		auto add(std::unique_ptr<password_generator> generator) -> void override;

	  private:
		std::random_device								 rdev_;
		std::mt19937									 engine_;
		std::vector<std::unique_ptr<password_generator>> generators_;
	};

}  // namespace DesignPatterns