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
	 * Stellt static-Methoden und Eigenschaften für die Verwaltung einer
	 * Anwendung zur Verfügung, z.B. Methoden zum Starten und Beenden einer
	 * Anwendung sowie für das Abrufen von Informationen zu einer Anwendung.
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
		static void Run(const std::shared_ptr<Form> &form);

		/**
		 * Gibt ein Event an die geöffneten Formen weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		static Event::NextEventTypes ProcessEvent(const std::shared_ptr<Event> &event);
		/**
		 * Zeichnet die geöffneten Formen.
		 */
		static void Render();

	private:
		static void RegisterTimer(const std::shared_ptr<Timer> &timer, Misc::TimeSpan &interval);
		static void UnregisterTimer(const std::shared_ptr<Timer> &timer);
		struct TimerInfo
		{
			std::shared_ptr<Timer> timer;
			Misc::TimeSpan interval;
			Misc::DateTime next;
		};
			
		static std::map<std::shared_ptr<Timer>, TimerInfo> timers;

		static void RegisterForm(const std::shared_ptr<Form> &form);
		static void UnregisterForm(const std::shared_ptr<Form> &form);

		static std::list<std::shared_ptr<Form>> forms;
		static std::shared_ptr<Form> focusForm,
									 mainForm;
		static std::list<std::shared_ptr<Form>> removeForms;
		static bool enabled;

	public:
		static std::shared_ptr<Drawing::IRenderer> Renderer;
	};
}

#endif