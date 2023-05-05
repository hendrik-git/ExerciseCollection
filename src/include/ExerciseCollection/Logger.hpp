#include <chrono>
#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <iostream>
#include <shared_mutex>

namespace Log
{

	/// @brief Return value used for testing the Logging functionality
	enum class PrintStatus
	{
		Filtered,  //!< This message will not be printed at the current verbosity
		Printed	   //!< this message has been printed
	};

	/// @brief Describes how important a message is
	enum class Verbosity
	{
		Off,	//!< Disable this message
		Debug,	//!< Message for debugging
		Info,	//!< Normal message
		Warn,	//!< Important message
		Error,	//!< Something went wrong
		Fatal	//!< Execution has run into an irrecoverable issue
	};

		/// @brief The Logger singleton
	/// @details Ensures that all log messages are handled in a thread safe way.
	/// @todo PIMP it
	class Logger final
	{
	  public:
		static Logger& instance()
		{
			static Logger logger;
			return logger;
		}
		Logger(Logger const&) = delete;
		Logger& operator=(Logger const&) = delete;

		void log(Verbosity verbosity, std::string_view message);

	  protected:
		Logger() = default;

	  private:
		std::shared_mutex mut_;
		std::atomic<bool> use_timestamp_ = false;
	};


	template<Verbosity verbosity = Verbosity::Info, typename... Args>
	PrintStatus logf([[maybe_unused]] fmt::format_string<Args...>&& format_str, Args&&... args)
	{
		// early return, this message was disabled
		if constexpr(verbosity == Verbosity::Off)
		{
			return PrintStatus::Printed;
		}

		// if(verbosity < Verbosity::Debug || globalVerbosity.index() == 0)
		//{
		//	return PrintStatus::FilteredByLogLevel;
		//}

		auto  output = fmt::format(format_str, args...);
		auto& logger = Logger::instance();
		logger.log(verbosity, output);

		return PrintStatus::Printed;
	}
}  // namespace Log