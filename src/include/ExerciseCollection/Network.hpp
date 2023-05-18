#pragma once
#include <string>
#include <vector>

namespace Network
{
	auto get_ip_address(std::string_view hostname) -> std::vector<std::string>;
}  // namespace Network