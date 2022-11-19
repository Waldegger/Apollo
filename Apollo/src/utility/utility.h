#pragma once

#include <utility>
#include <functional>

namespace agl
{
	template <typename T, auto fn>
	struct deleter
	{
		void operator()(T* ptr)
		{
			fn(ptr);
		}
	};

	template <auto fn_init, auto fn_uninit, typename... init_params>
	class  initializer
	{
	public:
		initializer(init_params&&... params)
		{
			fn_init(std::forward<init_params>(params)...);
		}

		~initializer()
		{
			fn_uninit();
		}
	
		initializer(const initializer& other) = delete;
		initializer(initializer&& other) = delete;

		initializer& operator = (const initializer& other) = delete;
		initializer& operator = (initializer&& other) = delete;

	public:

	protected:

	private:
	};

	template <typename T, auto delete_func, T invalid_value = T{}>
	class unique_handle
	{
	public:
		unique_handle() = default;
		unique_handle(const T& handle)
			: m_handle{ handle }
		{}

		~unique_handle()
		{
			delete_handle();
		}

		unique_handle(unique_handle&& other) noexcept
		{
			m_handle = std::move(other.m_handle);
			other.m_handle = invalid_value;
		}

		unique_handle& operator = (unique_handle&& other) noexcept
		{
			delete_hanlde(m_handle);
			m_handle = std::move(other.m_handle);

			other.m_handle = invalid_value;
		}

		unique_handle(const unique_handle& other) = delete;
		unique_handle& operator = (const unique_handle& other) = delete;
	public:
		T get() { return m_handle; }

		void reset(T handle)
		{
			delete_handle();
			m_handle = handle;
		}

		operator const T&() const { return m_handle; }

	protected:

	private:
		void delete_handle()
		{
			if (m_handle != invalid_value)
				delete_func(m_handle);
		}

		T m_handle{ invalid_value };
	};
}