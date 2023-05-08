#include <ExerciseCollection/Logger.hpp>
#include <catch2/catch_all.hpp>
#include <thread>

using namespace Log;

TEST_CASE("Verbosity", "[Logging]")
{
	using enum Log::Verbosity;
	auto& logger = Logger::get();

	std::string filtered = "Filtered\n";
	std::string printing = "Printing\n";

	logger.verbosity(Off);
	CHECK(logger.verbosity() == Off);
	CHECK(logf<Off>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Debug>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Verbosity::Info>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Warn>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Error>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Fatal>(filtered) == PrintStatus::Filtered);

	logger.verbosity(Debug);
	CHECK(logger.verbosity() == Debug);
	CHECK(logf<Off>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Debug>(printing) == PrintStatus::Printed);
	CHECK(logf<Verbosity::Info>(printing) == PrintStatus::Printed);
	CHECK(logf<Warn>(printing) == PrintStatus::Printed);
	CHECK(logf<Error>(printing) == PrintStatus::Printed);
	CHECK(logf<Fatal>(printing) == PrintStatus::Printed);

	logger.verbosity(Info);
	CHECK(logger.verbosity() == Info);
	CHECK(logf<Off>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Debug>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Info>(printing) == PrintStatus::Printed);
	CHECK(logf<Warn>(printing) == PrintStatus::Printed);
	CHECK(logf<Error>(printing) == PrintStatus::Printed);
	CHECK(logf<Fatal>(printing) == PrintStatus::Printed);

	logger.verbosity(Warn);
	CHECK(logger.verbosity() == Warn);
	CHECK(logf<Off>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Debug>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Info>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Warn>(printing) == PrintStatus::Printed);
	CHECK(logf<Error>(printing) == PrintStatus::Printed);
	CHECK(logf<Fatal>(printing) == PrintStatus::Printed);

	logger.verbosity(Error);
	CHECK(logger.verbosity() == Error);
	CHECK(logf<Off>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Debug>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Info>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Warn>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Error>(printing) == PrintStatus::Printed);
	CHECK(logf<Fatal>(printing) == PrintStatus::Printed);

	logger.verbosity(Fatal);
	CHECK(logger.verbosity() == Fatal);
	CHECK(logf<Off>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Debug>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Info>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Warn>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Error>(filtered) == PrintStatus::Filtered);
	CHECK(logf<Fatal>(printing) == PrintStatus::Printed);
}

TEST_CASE("Multithreaded Logging", "[Logging]")
{
	using enum Log::Verbosity;
	Logger::get().verbosity(Debug);

	std::vector<std::jthread> modules;
	for(int id = 0; id <= 10; ++id)
	{
		modules.emplace_back(
			[id]()
			{
				std::random_device			  rd;
				std::mt19937				  mt(rd());
				std::uniform_int_distribution ud(100, 1000);
				REQUIRE_NOTHROW(logf("Module {:>2} started\n", id));
				std::this_thread::sleep_for(std::chrono::milliseconds(ud(mt)));
				REQUIRE_NOTHROW(logf("Module {:>2} finished\n", id));
			});
	}
	for(auto& m : modules)
	{
		m.join();
	}
}