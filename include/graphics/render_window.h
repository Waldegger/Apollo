#pragma once

#include "render_target.h"

#include <memory>
#include <functional>
#include <type_traits>
#include <string_view>

#include <glm/vec2.hpp>
#include <graphics/context.h>
#include "../utility/utility.h"

namespace age
{
	class render_window
		: public render_target
	{
	public:
		friend class engine;
		friend class transient_context_lock;

		virtual ~render_window() = default;

		render_window(const render_window& other) = delete;
		render_window(render_window&& other) = delete;

		render_window& operator = (const render_window& other) = delete;
		render_window& operator = (render_window&& other) = delete;

		void* get_internal_handle() const;
	
	public:
		void clear();
		void display();

		glm::u32vec2 get_size() const override;
	protected:

	private:
		render_window();

		void open(const std::string_view& title, uint32_t display_index, uint32_t width, uint32_t height, uint32_t flags);
		const context& get_context() const;
		context &get_context();

		static void destroy_window_lib(void* window);

		context m_context;

		using window_handle_deleter = deleter<void, destroy_window_lib>;
		std::unique_ptr<void, window_handle_deleter> m_windowhandle;

		uint32_t m_clear_flags = 0;
	};
}
