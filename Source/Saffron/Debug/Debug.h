﻿#pragma once

#include "Saffron/Core/Macros.h"
#include "Saffron/Core/TypeDefs.h"
#include "Saffron/Log/Log.h"

namespace Se
{
class Debug
{
public:

#ifdef SE_DEBUG
	template <typename Arg, typename... Args>
	static constexpr void Assert(bool what, const String& message, Arg&& arg, Args&& ... args)
	{
		if (!what)
		{
			Log::CoreDebug((AssertionFailed + " " + message).c_str(), std::forward<Arg>(arg), std::forward<Args>(args)...);
			SE_DEBUGBREAK();
		}
	}

	static constexpr void Assert(bool what, const String& message)
	{
		if (!what)
		{
			Log::CoreDebug(AssertionFailed + " " + message);
			SE_DEBUGBREAK();
		}
	}

	static constexpr void Assert(bool what)
	{
		Assert(what, "");
	}

	template <typename Arg, typename... Args>
	static constexpr void Break(const String& message, Arg&& arg, Args&& ... args)
	{
		Assert(false, message, std::forward<Arg>(arg), std::forward<Args>(args)...);
	}

	static constexpr void Break(const String& message)
	{
		Assert(false, message);
	}

	static constexpr void Break()
	{
		Assert(false, "");
	}

#else
	template <typename Arg, typename... Args>
	static constexpr void Assert(bool what, const String& message, Arg&& arg, Args&& ... args)
	{
	}

	static constexpr void Assert(bool what, const String& message)
	{
	}

	static constexpr void Assert(bool what)
	{
	}

	template <typename Arg, typename... Args>
	static constexpr void Break(const String& message, Arg&& arg, Args&& ... args)
	{
	}

	static constexpr void Break(const String& message)
	{
	}

	static constexpr void Break()
	{
	}
#endif

private:
	inline static String AssertionFailed = Log::Fmt::OnRed + Log::Fmt::White + "Assertion failed" + Log::Fmt::Reset +
		Log::Fmt::Cyan;
};
}
