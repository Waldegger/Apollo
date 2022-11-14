#pragma once

#if defined(ANDROID) || defined(__ANDROID__)
#include <streambuf>
#include <istream>
#include <string_view>
#include <array>

#include "../utility/utility.h"
namespace apollo
{
	class asset_streambuf
		: public std::streambuf
	{
	public:
		asset_streambuf();
		asset_streambuf(const std::string_view& fn);

		virtual ~asset_streambuf() override;

		void open(const std::string_view& fn);

	public:

	protected:
		virtual std::streambuf::int_type underflow() override;

	private:
		std::array<char, 1024> m_buffer{};
	};

	class assetistream
		: public std::istream
	{
	public:
		assetistream();
		assetistream(const std::string_view& fn);

		virtual ~assetistream() override;
	public:

	protected:

	private:
		asset_streambuf m_streambuf;
	};
}

#else
#include <fstream>
namespace apollo
{
	using assetistream = std::ifstream;
}
#endif