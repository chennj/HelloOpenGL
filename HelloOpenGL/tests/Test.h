#ifndef __TEST_H__
#define __TEST_H__

#include <vector>
#include <functional>
#include <memory>

namespace tests
{
	class Test
	{
	private:

	public:
		Test() {}
		virtual ~Test() {}

	public:
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu : public Test
	{
	private:
		std::vector < std::pair < std::string, std::function<Test*()> >> _Tests;
		Test*& _CurrentTest;

	public:
		TestMenu(Test*& currentTestPointer);

	public:
		void OnImGuiRender() override;

	public:
		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;
			//反射的实现方式
			_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}
	};
}

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

#endif
