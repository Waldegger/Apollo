#pragma once

#include <string>

namespace agl
{
	class text
	{
	public:
		void set_string(const std::string_view& value);
		const std::u32string& get_string();

	public:

	protected:

	private:

		std::u32string m_string;
	};
}