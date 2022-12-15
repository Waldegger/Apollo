#pragma once

#include "sound.h"

namespace age
{
	class music
		: public sound
	{
	public:

	public:
		virtual void play(bool looped = false) const override;
		virtual void stop() override;

	protected:

	private:
	};
}