#pragma once

#include <stdint.h>
#include <istream>

namespace age
{
	class sound_stream
	{
	public:
		struct info
		{
			uint64_t sample_count{};
			uint32_t channel_count{};
			uint32_t sample_rate{};
		};

		sound_stream() = default;
		sound_stream(const sound_stream& other) = default;
		sound_stream(sound_stream&& other) = default;

		sound_stream& operator = (const sound_stream& other) = default;
		sound_stream& operator = (sound_stream&& other) = default;

		virtual ~sound_stream() = default;

	public:
		inline info open(std::istream& is) { return on_open(is); }
		inline void seek(uint64_t sample_offset) { on_seek(sample_offset); }
		inline size_t read(std::byte samples[], size_t max_count) { return on_read(samples, max_count); }
		
	protected:
		virtual info on_open(std::istream& is) = 0;
		virtual void on_seek(uint64_t sample_offset) = 0;
		virtual size_t on_read(std::byte samples[], size_t max_count) = 0;

	private:
	};
}