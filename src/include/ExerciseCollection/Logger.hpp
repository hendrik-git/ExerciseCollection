#include <fmt/core.h>
#include <iostream>
#include <string_view>

namespace Log
{
	/// @brief Return value used for testing the Logging functionality
	enum class PrintStatus
	{
		Filtered,  //!< This message will not be printed at the current verbosity
		Printed,   //!< this message has been printed
		Error	   //!< Something went wrong...
	};

	/// @brief Describes how important a message is
	enum class Verbosity
	{
		Off	  = 0,	//!< Disable this message
		Debug = 1,	//!< Message for debugging
		Info  = 2,	//!< Normal message
		Warn  = 3,	//!< Important message
		Error = 4,	//!< Something went wrong
		Fatal = 5	//!< Execution has run into an irrecoverable issue
	};

	class Logger
	{
	  public:
		void log(Verbosity verbosity, std::string_view message);

		[[nodiscard]] static auto get() -> Logger&
		{
			static Logger instance;
			return instance;
		};

		[[nodiscard]] auto verbosity() const noexcept -> Verbosity
		{
			return verbosity_;
		};

		void verbosity(Verbosity verb) noexcept
		{
			verbosity_ = verb;
		};

	  protected:
		Logger();

	  private:
		struct Logger_Impl;	 // forward declaration of the implementation class

		/// Current Verbosity threshold to log messages Info by default
		Verbosity verbosity_   = Verbosity::Info;
		bool	  use_logfile_ = true;

		// unique-ownership opaque pointer to the forward-declared implementation class
		Logger_Impl& pImpl;
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