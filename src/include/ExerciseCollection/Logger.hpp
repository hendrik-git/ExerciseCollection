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
	// PrintStatus logf([[maybe_unused]] fmt::format_string<Args...>&& format_str, Args&&... args)
	PrintStatus logf([[maybe_unused]] std::string_view format_str, Args&&... args)
	{
		try
		{
			// early return, this message was disabled
			if constexpr(verbosity == Verbosity::Off)
			{
				return PrintStatus::Filtered;
			}

			// cached reference to the logger (there can only be one Logger due to singleton)
			static auto& logger = Logger::get();

			// the check for verbosity has bee explicitly pulled out of the log function, so that
			// the expensive formating does not happen for a message, that will be discarded due to
			// a low log verbosity
			if(verbosity < logger.verbosity() || logger.verbosity() == Verbosity::Off)
			{
				return PrintStatus::Filtered;
			}

			// pass the message to the logger for printing
			//// auto str = fmt::format(format_str, args...); requires constexpr
			// see https://stackoverflow.com/questions/68675303/how-to-create-a-function-that-forwards-its-arguments-to-fmtformat-keeping-the
			auto str = fmt::vformat(format_str, fmt::make_format_args(std::forward<Args>(args)...));
			logger.log(verbosity, str);
			return PrintStatus::Printed;
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return PrintStatus::Error;
		}
	}
}  // namespace Log