#ifndef __TEST_H__
#define __TEST_H__

namespace test
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
}

#endif
