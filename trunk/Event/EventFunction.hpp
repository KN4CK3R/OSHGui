#ifndef OSHGUI_EVENT_EVENTFUNCTION_HPP_
#define OSHGUI_EVENT_EVENTFUNCTION_HPP_

#include <functional>

#include "../Exports.hpp"

namespace OSHGui
{
	template <typename Signature>
	class OSHGUI_EXPORT EventFunction
	{
	private:
		typedef std::function<Signature> Handler;
		Handler handler;
		int id;
		
		static unsigned int NextID()
		{
			static unsigned int id = 0;
			++id;
			return id;
		}
		
	public:
		EventFunction(const Handler &handler)
		{
			id = NextID();
			this->handler = handler;
		}
		
		bool operator == (const EventFunction &eventFunction)
		{
			return id == eventFunction.id;
		}
		
		Handler& GetHandler()
		{
			return handler;
		}
	};
}

#endif