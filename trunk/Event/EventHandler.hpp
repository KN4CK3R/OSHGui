#ifndef OSHGUI_EVENT_EVENTHANDLER_HPP
#define OSHGUI_EVENT_EVENTHANDLER_HPP

#include <functional>
#include "../Exports.hpp"

namespace OSHGui
{
	/**
	 * Stellt die Methode dar, die ein Ereignis behandelt.
	 */
	template <typename Signature>
	class OSHGUI_EXPORT EventHandler
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
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param handler die gebundene Funktion
		 */
		EventHandler(const Handler &handler)
		{
			id = NextID();
			this->handler = handler;
		}
		
		bool operator == (const EventHandler &eventFunction)
		{
			return id == eventFunction.id;
		}
		
		/**
		 * Ruft die gebundene Funktion ab.
		 *
		 * @return handler
		 */
		Handler& GetHandler()
		{
			return handler;
		}
	};
}

#endif