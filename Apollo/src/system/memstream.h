#pragma once

#include <streambuf>
#include <istream>

namespace age
{
	class mem_streambuf
		: public std::streambuf
	{
	public:
		mem_streambuf();
		mem_streambuf(std::byte data[], size_t size);

		//virtual ~mem_streambuf() = default;

		void open(std::byte data[], size_t size);

	public:

	protected:
		virtual std::streambuf::int_type underflow() override;
		virtual std::streambuf::pos_type seekoff(std::streambuf::off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) override;
		virtual std::streambuf::pos_type seekpos(std::streambuf::pos_type pos, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) override;

	private:
		std::byte* m_data;
	};

	class memistream
		: public std::istream
	{
	public:
		memistream();
		memistream(std::byte data[], size_t size);

		//virtual ~memistream() override;
	public:

	protected:

	private:
		mem_streambuf m_streambuf;
	};
}