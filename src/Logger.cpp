#include <ExerciseCollection/Logger.hpp>
#include <chrono>
#include <filesystem>
#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <shared_mutex>
#include <mutex>
#include <atomic>

namespace fs = std::filesystem;

namespace Log
{
	/// @brief The Logger singleton
	/// @details Ensures that all log messages are handled in a thread safe way.
	class Logger::Logger_Impl final
	{
	  public:
		~Logger_Impl()
		{
			if(logfile_.is_open())
			{
				try
				{
					logfile_.close();
				}
				catch(...)
				{
					// do not throw in a destructor
				}
			}
		};

		Logger_Impl(Logger_Impl const&) = delete;
		Logger_Impl& operator=(Logger_Impl const&) = delete;

		void log(Verbosity verbosity, std::string_view message, bool to_file)
		{
			using namespace fmt;
			std::scoped_lock lock(mut_);

			auto timestamp =
				use_timestamp_ ? fmt::format("{:%H:%M:%S} ", std::chrono::system_clock::now()) : "";

#define PRINT_TO_FILE(verbosity)                                                       \
	if(to_file)                                                                        \
	{                                                                                  \
		logfile_ << fmt::format("{}{:<5}  {}", timestamp, verbosity, message).c_str(); \
	}
			switch(verbosity)
			{
				using enum fmt::color;
				using enum Log::Verbosity;
				case Off:
					break;
				case Debug:
					print(fg(dim_gray), "{}{:<5}  {}", timestamp, "Debug", message);
					PRINT_TO_FILE("Debug")
					break;
				case Info:
					print(fg(white), "{}{:<5}  {}", timestamp, "Info", message);
					PRINT_TO_FILE("Info")
					break;
				case Warn:
					print(fg(dark_orange), "{}{:<5}  ", timestamp, "Warn");
					print(fg(white), "{}", message);
					PRINT_TO_FILE("Warn")
					break;
				case Error:
					print(fg(crimson), "{}{:<5}  ", timestamp, "Error");
					print(fg(white), "{}", message);
					PRINT_TO_FILE("Error")
					break;
				case Fatal:
					print(fg(white) | bg(dark_red), "{}{:<5}  {}", timestamp, "Fatal", message);
					PRINT_TO_FILE("Fatal")
					break;
			}
#undef PRINT_TO_FILE
		}

		static Logger::Logger_Impl& get()
		{
			static Logger::Logger_Impl instance;
			return instance;
		}

	  protected:
		Logger_Impl()
		{
			logpath_ = "Log.txt";
			logfile_.open(logpath_.c_str(), std::ios::out | std::ios::trunc);
		}

	  private:
		std::shared_mutex	  mut_;
		std::atomic<bool>	  use_timestamp_ = true;
		std::ofstream		  logfile_;
		std::filesystem::path logpath_;
	};

	Logger::Logger() : pImpl(Logger::Logger_Impl::get()){};

	void Logger::log(Verbosity verbosity, std::string_view message)
	{
		pImpl.log(verbosity, message, use_logfile_);
	}
};	// namespace Log