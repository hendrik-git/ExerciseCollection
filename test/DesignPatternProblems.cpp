#include <ExerciseCollection/DesignPatternProblems.hpp>
#include <catch2/catch_all.hpp>

using namespace DesignPatterns;

//=================================================================================================
// Write a program that validates password strength based on predefined rules, which may then be
// selected in various combinations.At a minimum, every password must meet a minimum length
// requirement.In addition, other rules could be enforced, such as the presence of at least one
// symbol, digit, uppercase and lowercase letter, and so on
TEST_CASE("Validating passwords", "[DesignPatterns]")
{
	using namespace Decorator;

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
TEST_CASE("Generating passwords", "[DesignPatterns]")
{
	using namespace Composite;
	using namespace Decorator;

	composite_password_generator generator;
	generator.add(std::make_unique<symbol_generator>(2));
	generator.add(std::make_unique<digit_generator>(2));
	generator.add(std::make_unique<upper_letter_generator>(2));
	generator.add(std::make_unique<lower_letter_generator>(4));
	auto password = generator.generate();
	INFO("The generated password is " + password);

	// Using the password validator for verifying the generated password
	auto has_length = std::make_unique<length_validator>(10);
	auto has_digits = std::make_unique<digit_password_validator>(std::move(has_length));
	auto has_case	= std::make_unique<case_password_validator>(std::move(has_digits));
	auto validator	= std::make_unique<symbol_password_validator>(std::move(has_case));
	CHECK(validator->validate(password) == true);
}

TEST_CASE("Chain of Responsibility", "[DesignPatterns]")
{
	using namespace ChainOfResponsibility;

	auto john	= std::make_shared<employee>("John", std::make_unique<employee_role>());
	auto robert = std::make_shared<employee>("Robert", std::make_unique<team_manager_role>());
	auto david	= std::make_shared<employee>("Dave", std::make_unique<department_manager_role>());
	auto frank	= std::make_shared<employee>("Frank", std::make_unique<president_role>());

	john->set_direct_manager(robert)->set_direct_manager(david)->set_direct_manager(frank);

	CHECK(john->approve(expense{500, "magazins"}) == "John approved");
	CHECK(john->approve(expense{5000, "hotel accomodation"}) == "Robert approved");
	CHECK(john->approve(expense{50000, "conference costs"}) == "Dave approved");
	CHECK(john->approve(expense{200000, "new lorry"}) == "Frank approved");
}