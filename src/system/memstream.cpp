#include "system/memstream.h"

namespace age
{
	mem_streambuf::mem_streambuf()
		: m_data{}
	{}

	mem_streambuf::mem_streambuf(std::byte data[], size_t size)
		: m_data{ data }
	{
		setg(reinterpret_cast<char*>(data), reinterpret_cast<char*>(data), reinterpret_cast<char*>(data + size));
		setp(reinterpret_cast<char*>(data), reinterpret_cast<char*>(data + size));
	}

	void mem_streambuf::open(std::byte data[], size_t size)
	{
		m_data = data;
		
		setg(reinterpret_cast<char*>(data), reinterpret_cast<char*>(data), reinterpret_cast<char*>(data + size));
		setp(reinterpret_cast<char*>(data), reinterpret_cast<char*>(data + size));
	}

	std::streambuf::int_type mem_streambuf::underflow()
	{
		//As we passed in all the data when opening or in constructor, an underflow can only mean eof
		return traits_type::eof();
	}
	
	std::streambuf::pos_type mem_streambuf::seekoff(std::streambuf::off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which)
	{
		switch (dir)
		{
			case std::ios_base::beg:
			{
				std::streambuf::pos_type pos = off;
				setg(reinterpret_cast<char*>(m_data), reinterpret_cast<char*>(m_data + pos), egptr());

				return pos;
			}
			break;

			case std::ios_base::end:
			{
				std::streambuf::pos_type pos = (egptr() - reinterpret_cast<char*>(m_data)) + off;
				setg(reinterpret_cast<char*>(m_data), reinterpret_cast<char*>(m_data + pos), egptr());

				return pos;                                                       
			}
			break;

			case std::ios_base::cur:
			{
				std::streambuf::pos_type pos = (gptr() - reinterpret_cast<char*>(m_data)) + off;
				setg(reinterpret_cast<char*>(m_data), reinterpret_cast<char*>(m_data + pos), egptr());
				
				return pos;
			}
			break;
		}

		return std::streambuf::pos_type{};
	}

	std::streambuf::pos_type mem_streambuf::seekpos(std::streambuf::pos_type pos, std::ios_base::openmode which)
	{
		setg(reinterpret_cast<char*>(m_data), reinterpret_cast<char*>(m_data + pos), reinterpret_cast<char*>(egptr()));
		return pos;
	}
	


	memistream::memistream()
		: std::istream{ &m_streambuf }
	{}

	memistream::memistream(std::byte data[], size_t size)
		: std::istream{ &m_streambuf }
		, m_streambuf{ data, size }
	{}
}