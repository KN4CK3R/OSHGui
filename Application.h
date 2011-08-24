#ifndef __OSHGUI_APPLICATION_H__
#define __OSHGUI_APPLICATION_H__

#include <list>
#include "Drawing\IRenderer.h"
#include "Event.h"

namespace OSHGui
{
	class Form;

	class Application
	{
		friend Form;

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
		 * Gibt ein Event an die geöffneten Formen weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		static Event::NextEventTypes ProcessEvent(Event *event);
		/**
		 * Zeichnet die geöffneten Formen.
		 *
		 * @param renderer
		 */
		static void Render(Drawing::IRenderer *renderer);

	private:
		static void RegisterForm(Form *form);
		static void UnregisterForm(Form *form);

		static std::list<Form*> forms;
		static Form *focusForm,
					*mainForm;
		static bool enabled;
	};
}

#endif