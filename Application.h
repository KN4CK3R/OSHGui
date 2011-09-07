#ifndef __OSHGUI_APPLICATION_H__
#define __OSHGUI_APPLICATION_H__

#include <list>
#include <map>
#include "Drawing\IRenderer.h"
#include "Misc\DateTime.h"
#include "Event.h"

namespace OSHGui
{
	class Form;
	class Timer;

	/**
	 * Stellt static-Methoden und Eigenschaften f�r die Verwaltung einer
	 * Anwendung zur Verf�gung, z.B. Methoden zum Starten und Beenden einer
	 * Anwendung sowie f�r das Abrufen von Informationen zu einer Anwendung.
	 */
	class Application
	{
		friend Form;
		friend Timer;

	public:
		/**
		 * Aktiviert das GUI.
		 */
		static void Enable();
		/**
		 * Deaktiviert das GUI.
		 */
		static void Disable();

		/**
		 * Legt die Hauptform des GUI fest.
		 *
		 * @param form
		 */
		static void Run(Form *form);

		/**
		 * Gibt ein Event an die ge�ffneten Formen weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		static Event::NextEventTypes ProcessEvent(Event *event);
		/**
		 * Zeichnet die ge�ffneten Formen.
		 *
		 * @param renderer
		 */
		static void Render(Drawing::IRenderer *renderer);

	private:
		static void RegisterTimer(Timer *timer, Misc::TimeSpan &interval);
		static void UnregisterTimer(Timer *timer);
		struct TimerInfo
		{
			Timer *timer;
			Misc::TimeSpan interval;
			Misc::DateTime next;
		};
			
		static std::map<Timer*, TimerInfo> timers;

		static void RegisterForm(Form *form);
		static void UnregisterForm(Form *form);

		static std::list<Form*> forms;
		static Form *focusForm,
					*mainForm;
		static std::list<Form*> removeForms;
		static bool enabled;
	};
}

#endif