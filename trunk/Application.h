#ifndef OSHGUI_APPLICATION_H_
#define OSHGUI_APPLICATION_H_

#include <functional>
#include <list>
#include <map>
#include "Drawing\IRenderer.h"
#include "Misc\DateTime.h"
#include "Cursor\Cursor.h"
#include "Event\IEvent.h"

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
		 * Initialisiert die Application-Klasse.
		 *
		 * @param renderer Instanz des verwendeten Renderers
		 */
		static void Create(Drawing::IRenderer *renderer);
		
		/**
		 * Ruft die aktuelle Uhrzeit ab.
		 *
		 * @return DateTime::Now
		 */
		static const Misc::DateTime& GetNow() const;
		/**
		 * Ruft den verwendeten Renderer ab.
		 *
		 * @return renderer
		 */
		static Drawing::IRenderer GetRenderer() const;
	
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
		 * Gibt ein IEvent an die ge�ffneten Formen weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		static IEvent::NextEventTypes ProcessEvent(IEvent *event);
		/**
		 * Zeichnet die ge�ffneten Formen.
		 */
		static void Render();

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

		static void RegisterForm(Form *form, const std::function<void()> &modalFunc = 0);
		static void UnregisterForm(Form *form);
		struct ModalInfo
		{
			Form *form;
			std::function<void()> func;
		};
		static std::list<ModalInfo> modals; //we can't use std::stack here because we need an iterator

		static std::list<Form*> forms;
		static Form *focusForm,
					*mainForm;
		static std::list<Form*> removeForms;
		static bool enabled;

		static Drawing::IRenderer *renderer;
		
		static Misc::DateTime now;

		typedef struct
		{
			Drawing::Point Position;
			std::shared_ptr<Cursor> Cursor;
		} MouseInfo;
		static MouseInfo mouse;
	};
}

#endif