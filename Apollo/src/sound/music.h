#pragma once

#include "sound_interface.h"

#include <string_view>
#include <istream>
#include <memory>

namespace age
{
	class music
		: public sound_interface
	{
	public:
		music() = default;
		music(const music& other) = default;
		music(music&& other) = default;

		music& operator = (const music& other) = default;
		music& operator = (music&& other) = default;

		virtual ~music() override;
	public:
		virtual void play(bool looped = false) override;
		virtual void stop() override;

		void open(std::string_view fn);
		void open(std::istream& is);
		void open(std::unique_ptr<std::istream> is);
		void open(std::byte data[], size_t size);
	protected:

	private:
		std::unique_ptr<std::istream> m_istream;
	};
}