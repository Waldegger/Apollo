#include "assetstream.h"

#include <SDL2/SDL.h>

#if defined(ANDROID) || defined(__ANDROID__)
namespace agl
{
	asset_streambuf::asset_streambuf()
	{
		
	}

	asset_streambuf::asset_streambuf(const std::string_view& fn)
	{
		open(fn);
	}

	asset_streambuf::~asset_streambuf()
	{
		sync();
		//(asset);
	}

	void asset_streambuf::open(const std::string_view& fn)
	{
		/*
		auto activity = SDL_AndroidGetActivity();

		asset = AAssetManager_open(manager, filename.c_str(), AASSET_MODE_STREAMING);
		buffer.resize(1024);

		setg(0, 0, 0);
		setp(&buffer.front(), &buffer.front() + buffer.size());
		*/
	}

	std::streambuf::int_type asset_streambuf::underflow()
	{
		/*
		auto bufferPtr = &m_buffer.front();
		auto counter = AAsset_read(asset, bufferPtr, buffer.size());

		if (counter == 0)
			return traits_type::eof();
		if (counter < 0) //error, what to do now?
			return traits_type::eof();

		setg(bufferPtr, bufferPtr, bufferPtr + counter);

		return traits_type::to_int_type(*gptr());
		*/

		return traits_type::eof();
	}


	//ToDo: Check if can pass in the streambuf as a membervariable. If this doesnt work it needs to be done via the heap
	assetistream::assetistream()
		: std::istream{ &m_streambuf }
	{}

	assetistream::assetistream(const std::string_view& fn)
		: std::istream{ &m_streambuf }
		, m_streambuf{ fn }
	{}

	assetistream::~assetistream()
	{}
}
#endif