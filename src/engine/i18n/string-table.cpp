// SPDX-FileCopyrightText: 2025 C. J. Howard
// SPDX-License-Identifier: GPL-3.0-or-later

#include <engine/i18n/string-table.hpp>
#include <engine/resources/deserializer.hpp>
#include <engine/resources/resource-loader.hpp>

/// Deserializes a string table.
/// @param[out] value String table to deserialize.
/// @param[in,out] ctx Deserialize context.
/// @throw deserialize_error Read error.
template <>
void deserializer<i18n::string_table>::deserialize(i18n::string_table& value, deserialize_context& ctx)
{
	value.rows.clear();
	
	std::vector<std::string> row;
	std::string entry;
	
	char c;
	while (ctx.read8(reinterpret_cast<std::byte*>(&c), 1) == 1)
	{
		if (c == '\t')
		{
			row.push_back(entry);
			entry.clear();
		}
		else if (c == '\n')
		{
			row.push_back(entry);
			entry.clear();
			value.rows.push_back(row);
			row.clear();
		}
		else if (c != '\r')
		{
			entry.push_back(c);
		}
	}
	
	if (!entry.empty())
	{
		row.push_back(entry);
	}
	
	if (!row.empty())
	{
		value.rows.push_back(row);
	}
}

template <>
std::unique_ptr<i18n::string_table> resource_loader<i18n::string_table>::load([[maybe_unused]] ::resource_manager& resource_manager, std::shared_ptr<deserialize_context> ctx)
{
	auto resource = std::make_unique<i18n::string_table>();
	
	deserializer<i18n::string_table>().deserialize(*resource, *ctx);
	
	return resource;
}
