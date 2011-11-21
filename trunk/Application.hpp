#ifndef OSHGUI_APPLICATION_HPP_
#define OSHGUI_APPLICATION_HPP_

#include <memory>
#include "Drawing/IRenderer.hpp"
#include "Misc/DateTime.hpp"
#include "Cursor/Cursor.hpp"
#include "Event/IEvent.hpp"
#include "Event/MouseMessage.hpp"
#include "Exports.hpp"

namespace OSHGui
{
	class Control;
	class Form;
	class Timer;
	class FormManager;
	class TimerManager;

	/**
	 * Stellt Methoden und Eigenschaften f�r die Verwaltung einer
	 * Anwendung zur Verf�gung, z.B. Methoden zum Starten und Beenden einer
	 * Anwendung sowie f�r das Abrufen von Informationen zu einer Anwendung.
	 */
	class OSHGUI_EXPORT Application
	{
		friend Form;
		friend Timer;

	public:
		/**
		 * Initialisiert die Application-Klasse.
		 *
		 * @param renderer Instanz des verwendeten Renderers
		 */
		void Create(Drawing::IRenderer *renderer);
		
		/**
		 * Ruft die aktuelle Uhrzeit ab.
		 *
		 * @return DateTime::Now
		 */
		const Misc::DateTime& GetNow() const;
		/**
		 * Ruft den verwendeten Renderer ab.
		 *
		 * @return renderer
		 */
		Drawing::IRenderer* GetRenderer() const;

		const Drawing::Point& GetCursorPosition() const;

		const std::shared_ptr<Cursor>& GetCursor() const;
		void SetCursor(const std::shared_ptr<Cursor> &cursor);
	
		/**
		 * Aktiviert das GUI.
		 */
		void Enable();
		/**
		 * Deaktiviert das GUI.
		 */
		void Disable();
		/**
		 * Wechselt zwischen Enabled und Disabled.
		 */
		void Toggle();
		/**
		 * Legt die Hauptform des GUI fest.
		 *
		 * @param mainForm die Hauptform, die angezeigt wird, sobald das GUI aktiviert wird
		 */
		void Run(const std::shared_ptr<Form> &mainForm);
		/**
		 * Gibt ein IEvent an die ge�ffneten Formen weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		bool ProcessEvent(IEvent *event);
		bool ProcessMouseEvent(MouseMessage &mouse);
		/**
		 * Zeichnet die ge�ffneten Formen.
		 */
		void Render();
		
		static Application* Instance();

	private:
		static Application *instance;
		Application();
	
		bool isEnabled;
		
		Drawing::IRenderer *renderer;
	
		FormManager formManager;
		TimerManager timerManager;
		
		Misc::DateTime now;

	public:
		typedef struct
		{
			Drawing::Point Position;
			std::shared_ptr<Cursor> Cursor;
		} MouseInfo;
		MouseInfo mouse;

		Control *FocusedControl;
		Control *ClickedControl;
		Control *CaptureControl;
		Control *MouseEnteredControl;
	};
}

#endif