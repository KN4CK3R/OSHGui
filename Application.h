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
	public:
		static void Enable();
		static void Disable();

		static void RegisterForm(Form *form);
		static void UnregisterForm(Form *form);

		static Event::NextEventTypes ProcessEvent(Event *event);
		static void Render(Drawing::IRenderer *renderer);

	private:
		static std::list<Form*> forms;
		static Form *focusForm;
		static bool enabled;
	};
}

#endif