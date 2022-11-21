#pragma once

#include "render_target.h"

#include <memory>
#include <functional>
#include <type_traits>

#include "vector2.h"
#include "../utility/utility.h"

namespace agl
{
	class render_window
		: public render_target
	{
	public:
		friend class engine;
		virtual ~render_window() = default;

		render_window(const render_window& other) = delete;
		render_window(render_window&& other) = delete;

		render_window& operator = (const render_window& other) = delete;
		render_window& operator = (render_window&& other) = delete;
	
	public:
		void clear();
		void display();

		vector2u get_size() const;

	protected:

	private:
		render_window();

		void open(const std::string_view& title, uint32_t display_index, uint32_t width, uint32_t height, uint32_t flags);

		static void destroy_window_lib(void* window);
		static void delete_context_lib(void* context);

		using window_handle_deleter = deleter<void, destroy_window_lib>;
		std::unique_ptr<void, window_handle_deleter> m_windowhandle;

		using gl_context_type = void;
		using gl_context_deleter = deleter<gl_context_type, delete_context_lib>;
		std::unique_ptr<gl_context_type, gl_context_deleter> m_GL_context;

		uint32_t m_clear_flags = 0;
	};
}
