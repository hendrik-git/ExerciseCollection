#include <ExerciseCollection/Logger.hpp>

namespace Log
{
	void Logger::log(Verbosity verbosity, std::string_view message)
	{
		using namespace fmt;
		std::scoped_lock lock(mut_);

		if(use_timestamp_)	// print the timestamp at the front, e.g. "14:20:22"
		{
			print("{:%H:%M:%S} ", std::chrono::system_clock::now());
		}

		switch(verbosity)
		{
			using enum fmt::color;
			using enum Log::Verbosity;
			case Off:
				break;
			case Debug:
				print(fg(dim_gray), "{:<5}  {}", "Debug", message);
				break;
			case Info:
				print(fg(white), "{:<5}  {}", "Info", message);
				break;
			case Warn:
				print(fg(dark_orange), "{:<5}  ", "Warn");
				print(fg(white), "{}", message);
				break;
			case Error:
				print(fg(crimson), "{:<5}  ", "Error");
				print(fg(white), "{}", message);
				break;
			case Fatal:
				print(fg(white) | bg(dark_red), "{:<5}  {}", "Fatal", message);
				break;
		}
	}
};	// namespace Log