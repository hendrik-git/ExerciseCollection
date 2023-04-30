#pragma once
#include <memory>
#include <random>
#include <string>
#include <string_view>

namespace DesignPatterns
{
	namespace Decorator
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
		/// another object. Multiple decorators could be stackedon top of each other, each time
		/// adding new functionality.
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
	}  // namespace Decorator

	namespace Composite
	{
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

		/// @brief Interface class for the implementation of a password generator
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

	}  // namespace Composite

	/// @brief Chain of responsibility design pattern
	/// @details The Chain of Responsibility pattern is a behavioral design pattern that allows a
	/// group of objects to handle a request sequentially, without specifying which object should
	/// handle the request beforehand. The pattern decouples the sender of a request from its
	/// receiver by allowing multiple objects to handle the request in a chain-like structure.
	/// In this pattern, each object in the chain has a reference to its successor.
	/// If an object cannot handle the request, it forwards it to its successor. This continues
	/// until either an object in the chain handles the request or the end of the chain is reached.
	/// The Chain of Responsibility pattern is often used in situations where multiple objects may
	/// be able to handle a request, and the specific object that can handle the request may not be
	/// known at the time the request is made. It allows for flexibility in the system, as objects
	/// can be added or removed from the chain without affecting the rest of the system's
	/// functionality.
	namespace ChainOfResponsibility
	{
		/// @brief Interface class for a role
		struct role
		{
			virtual double approval_limit() const noexcept = 0;
			virtual ~role()								   = default;
		};

		struct employee_role : public role
		{
			auto approval_limit() const noexcept -> double override;
		};

		struct team_manager_role : public role
		{
			auto approval_limit() const noexcept -> double override;
		};

		struct department_manager_role : public role
		{
			auto approval_limit() const noexcept -> double override;
		};

		struct president_role : public role
		{
			auto approval_limit() const noexcept -> double override;
		};

		struct expense
		{
			double		amount;		  //!< The cost of the expense
			std::string description;  //!< What the expense is for
		};

		/// @brief Represents an employee with a given role
		class employee
		{
		  public:
			/// @brief Constructs an employee with a name and a role
			explicit employee(std::string_view name, std::unique_ptr<role> ownrole);

			/// @brief Sets the next person in line to ask for approval when budget is exceeded
			/// @return the next person (argument) to allow for chaining
			auto set_direct_manager(std::shared_ptr<employee> manager) -> std::shared_ptr<employee>;

			/// @brief Tries to approve an expense or forwards the request to the manager
			auto approve(expense const& e) -> std::string;

		  private:
			std::string				  name_;
			std::unique_ptr<role>	  own_role_;
			std::shared_ptr<employee> direct_manager_;
		};
	}  // namespace ChainOfResponsibility

	namespace Observer
	{
		/// @brief Interface class for the Observer pattern
		class Observer
		{
		  public:
			/// @brief Every Observer is required to have a notify function
			virtual void notify() = 0;
			virtual ~Observer()	  = default;
		};

		/// @brief Counts how many messages were emitted by the observed subject
		class Count_Messages_Observer final : public Observer
		{
		  public:
			void notify() override;

			auto get_message_count() const noexcept -> unsigned;

		  private:
			unsigned message_counter = 0;
		};

		class Subject
		{
		  public:
			void register_observer(Observer& observer);

			void notify_observers();

		  private:
			std::vector<std::reference_wrapper<Observer>> observers;
		};

	}  // namespace Observer

}  // namespace DesignPatterns