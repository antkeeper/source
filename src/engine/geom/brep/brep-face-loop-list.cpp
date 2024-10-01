// SPDX-FileCopyrightText: 2024 C. J. Howard
// SPDX-License-Identifier: GPL-3.0-or-later

#include <engine/geom/brep/brep-face-loop-list.hpp>

namespace geom {

void brep_face_loop_list::push_back(brep_loop* loop)
{
	if (empty())
	{
		// List empty, initialize
		m_head = loop;
		loop->m_face_next = loop;
		loop->m_face_previous = loop;
	}
	else
	{
		// Append loop
		loop->m_face_next = m_head;
		loop->m_face_previous = m_head->m_face_previous;
		m_head->m_face_previous->m_face_next = loop;
		m_head->m_face_previous = loop;
	}
	
	++m_size;
}

void brep_face_loop_list::insert(brep_loop* next, brep_loop* loop)
{
	loop->m_face_next = next;
	loop->m_face_previous = next->m_face_previous;
	next->m_face_previous->m_face_next = loop;
	next->m_face_previous = loop;
	
	++m_size;
}

void brep_face_loop_list::remove(brep_loop* loop)
{
	// Directly link next and previous loops
	loop->m_face_next->m_face_previous = loop->m_face_previous;
	loop->m_face_previous->m_face_next = loop->m_face_next;
	
	// If loop was the list head, update head
	if (m_head == loop)
	{
		m_head = loop->m_face_next;
	}
	
	--m_size;
}

} // namespace geom
