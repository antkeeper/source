// SPDX-FileCopyrightText: 2024 C. J. Howard
// SPDX-License-Identifier: GPL-3.0-or-later

#include <engine/debug/logger.hpp>
#include <thread>
#include <utility>

namespace debug {

void logger::log(std::string&& message, log_message_severity severity, std::source_location&& location)
{
	// Generate message logged event
	m_message_logged_publisher.publish
	(
		{
			this,
			std::chrono::system_clock::now(),
			std::this_thread::get_id(),
			std::move(location),
			severity,
			std::move(message)
		}
	);
}

logger& default_logger() noexcept
{
	static logger instance;
	return instance;
}

} // namespace debug
