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
		static void Enable();
		static void Disable();

		static void Run(Form *form);

		static Event::NextEventTypes ProcessEvent(Event *event);
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