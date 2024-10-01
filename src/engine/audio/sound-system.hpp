// SPDX-FileCopyrightText: 2024 C. J. Howard
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANTKEEPER_AUDIO_SOUND_SYSTEM_HPP
#define ANTKEEPER_AUDIO_SOUND_SYSTEM_HPP

#include <engine/audio/listener.hpp>
#include <memory>

namespace audio {

/// Sound system.
class sound_system
{
public:
	/// Constructs a sound system.
	sound_system();
	
	/// Destructs a sound system.
	~sound_system();
	
	sound_system(const sound_system&) = delete;
	sound_system(sound_system&&) = delete;
	sound_system& operator=(const sound_system&) = delete;
	sound_system& operator=(sound_system&&) = delete;
	
	/// Returns the name of the playback device.
	[[nodiscard]] inline constexpr const auto& get_playback_device_name() const noexcept
	{
		return m_playback_device_name;
	}
	
	/// Returns the listener.
	[[nodiscard]] inline constexpr auto& get_listener() noexcept
	{
		return *m_listener;
	}

	/// @copydoc get_listener() noexcept
	[[nodiscard]] inline constexpr const auto& get_listener() const noexcept
	{
		return *m_listener;
	}

private:
	std::string m_playback_device_name;
	std::unique_ptr<listener> m_listener;
	
	void* m_alc_device{};
	void* m_alc_context{};
};

} // namespace audio

#endif // ANTKEEPER_AUDIO_SOUND_SYSTEM_HPP
