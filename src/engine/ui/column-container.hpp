// SPDX-FileCopyrightText: 2025 C. J. Howard
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANTKEEPER_UI_COLUMN_CONTAINER_HPP
#define ANTKEEPER_UI_COLUMN_CONTAINER_HPP

#include <engine/ui/container.hpp>
#include <engine/ui/alignment.hpp>

namespace ui {

/// Arranges child elements as rows in a column.
class column_container: public container
{
public:
	/// Destructs a column container.
	~column_container() override = default;

	[[nodiscard]] inline constexpr element_type get_type() const noexcept override
	{
		return element_type::column_container;
	}

	/// Sets the alignment of all child elements in the column.
	/// @param alignment Alignment of all child elements in the column.
	void set_alignment(ui::alignment alignment);

	/// Sets the spacing between child elements.
	/// @param spacing Spacing between child elements.
	void set_spacing(float spacing);
	
private:
	void redistribute_children() override;

	alignment m_alignment{alignment::center};
	float m_spacing{0.0f};
};

} // namespace ui

#endif // ANTKEEPER_UI_COLUMN_CONTAINER_HPP
