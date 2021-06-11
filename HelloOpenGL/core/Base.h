#pragma once
// import opengl third part library
// ---------------------------------
// -- basic expand
// -- 使用静态链接库 glew32s.lib,需要定义 GLEW_STATIC
#define GLEW_STATIC
#include <GLEW/glew.h>
#pragma comment(lib,"glew32s.lib")
// -- basic
#include <GLFW/glfw3.h>
#pragma comment(lib,"glfw3.lib")
// ---------------------------------
#pragma comment(lib,"OpenGL32.Lib")

#include <cstdint>
#include <memory>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

/*!
	The version of C++ standard that is being used.
	The C++17 standard.
*/
#ifndef MY_HAS_CPP17
#	ifdef _MSVC_LANG
#		if _MSVC_LANG > 201402L
#			define MY_HAS_CPP17	1
#		else /* _MSVC_LANG > 201402L */
#			define MY_HAS_CPP17	0
#		endif /* _MSVC_LANG > 201402L */
#	else /* _MSVC_LANG */
#		if __cplusplus > 201402L
#			define MY_HAS_CPP17	1
#		else /* __cplusplus > 201402L */
#			define MY_HAS_CPP17	0
#		endif /* __cplusplus > 201402L */
#endif /* _MSVC_LANG */
#endif /* MY_HAS_CPP17 */

/*!
	The version of C++ standard that is being used.
	The C++14 standard.
*/
#ifndef MY_HAS_CPP14
#	ifdef _MSVC_LANG
#		if _MSVC_LANG > 201103L
#			define MY_HAS_CPP14	1
#		else /* _MSVC_LANG > 201103L */
#			define MY_HAS_CPP14	0
#		endif /* _MSVC_LANG > 201103L */
#	else /* _MSVC_LANG */
#		if __cplusplus > 201103L
#			define MY_HAS_CPP14	1
#		else /* __cplusplus > 201103L */
#			define MY_HAS_CPP14	0
#		endif /* __cplusplus > 201103L */
#	endif /* _MSVC_LANG */
#endif /* MY_HAS_CPP14 */

/*!
	The version of C++ standard that is being used.
	The C++11 standard.
*/
#ifndef MY_HAS_CPP11
#	ifdef _MSVC_LANG
#		if _MSVC_LANG > 199711L
#			define MY_HAS_CPP11	1
#		else /* _MSVC_LANG > 199711L */
#			define MY_HAS_CPP11	0
#		endif /* _MSVC_LANG > 199711L */
#	else /* _MSVC_LANG */
#		if __cplusplus > 199711L
#			define MY_HAS_CPP11	1
#		else /* __cplusplus > 199711L */
#			define MY_HAS_CPP11	0
#		endif /* __cplusplus > 199711L */
#	endif /* _MSVC_LANG */
#endif /* MY_HAS_CPP11 */

#if MY_HAS_CPP11
#define  constexpr constexpr
#else 
#define constexpr
#endif

template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

#define OPENGL_VERSION 4.4