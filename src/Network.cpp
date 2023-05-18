#include <ExerciseCollection/Network.hpp>
#include <boost/asio.hpp>
#include <iostream>

namespace Network
{
	auto get_ip_address(std::string_view hostname) -> std::vector<std::string>
	{
		std::vector<std::string> ips;
		try
		{
			using namespace boost::asio;
			io_context		  context;
			ip::tcp::resolver resolver(context);
			auto			  endpoints = resolver.resolve(ip::tcp::v4(), hostname.data(), "");
			for(auto e = endpoints.begin(); e != endpoints.end(); ++e)
			{
				ips.push_back(((ip::tcp::endpoint)*e).address().to_string());
			}
		}
		catch(std::exception const& e)
		{
			std::cerr << "exception: " << e.what() << std::endl;
		}
		return ips;
	}
}  // namespace Network