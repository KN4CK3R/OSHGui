#ifndef __OSHGUI_APPLICATION_H__
#define __OSHGUI_APPLICATION_H__

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
		static void Run(Form *form);

		/**
		 * Gibt ein IEvent an die geöffneten Formen weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		static IEvent::NextEventTypes ProcessEvent(IEvent *event);
		/**
		 * Zeichnet die geöffneten Formen.
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

	public:
		/**
		 *
		 */
		static Drawing::IRenderer *Renderer;

		typedef struct
		{
			Drawing::Point Position;
			std::shared_ptr<Cursor> Cursor;
		} MouseInfo;
		static MouseInfo Mouse;
	};
}

#endif