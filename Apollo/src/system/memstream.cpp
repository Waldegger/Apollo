#include "memstream.h"

namespace age
{
	mem_streambuf::mem_streambuf()
		: m_data{}
		, m_size{}
		, m_pos{}
	{}

	mem_streambuf::mem_streambuf(std::byte data[], size_t size)
		: m_data{ data }
		, m_size{ size }
		, m_pos{}
	{
		if (!data || !size) return;

		setg(reinterpret_cast<char*>(data), reinterpret_cast<char*>(data), reinterpret_cast<char*>(data + size));
		setp(reinterpret_cast<char*>(data), reinterpret_cast<char*>(data + size));
	}

	void mem_streambuf::open(std::byte data[], size_t size)
	{
		m_data = data;
		m_size = size;
		m_pos = 0;

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
				m_pos = off;
			}
			break;

			case std::ios_base::end:
			{
				m_pos = m_size + off;                                                              
			}
			break;

			case std::ios_base::cur:
			{
				m_pos += off;
			}
			break;
		}

		//Shall I keep track of current postion like that?
		/*
		std::streambuf::pos_type test_beg = reinterpret_cast<size_t>(eback());
		std::streambuf::pos_type test_end = reinterpret_cast<size_t>(gptr());
		*/

		setg(reinterpret_cast<char*>(m_data), reinterpret_cast<char*>(m_data + m_pos), reinterpret_cast<char*>(m_data + m_size));

		return m_pos;
	}

	std::streambuf::pos_type mem_streambuf::seekpos(std::streambuf::pos_type pos, std::ios_base::openmode which)
	{
		m_pos = pos;

		setg(reinterpret_cast<char*>(m_data), reinterpret_cast<char*>(m_data + m_pos), reinterpret_cast<char*>(m_data + m_size));

		return m_pos;
	}
	


	memistream::memistream()
		: std::istream{ &m_streambuf }
	{}

	memistream::memistream(std::byte data[], size_t size)
		: std::istream{ &m_streambuf }
		, m_streambuf{ data, size }
	{}
}