#ifndef OSHGUI_APPLICATION_H_
#define OSHGUI_APPLICATION_H_

#include <memory>
#include "Drawing\IRenderer.h"
#include "Misc\DateTime.h"
#include "Cursor\Cursor.h"
#include "Event\IEvent.h"

namespace OSHGui
{
	class Form;
	class Timer;
	class FormManager;
	class TimerManager;

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
		static const Misc::DateTime& GetNow();
		/**
		 * Ruft den verwendeten Renderer ab.
		 *
		 * @return renderer
		 */
		static Drawing::IRenderer* GetRenderer();
	
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
		static void Run(const std::shared_ptr<Form> &mainForm);
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
		static bool enabled;
		
		static Drawing::IRenderer *renderer;
	
		static FormManager formManager;
		static TimerManager timerManager;
		
		static Misc::DateTime now;
	public:
		typedef struct
		{
			Drawing::Point Position;
			std::shared_ptr<Cursor> Cursor;
		} MouseInfo;
		static MouseInfo mouse;
	};
}

#endif