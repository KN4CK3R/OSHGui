#ifndef __OSHGUI_H__
#define __OSHGUI_H__

#include <windows.h>

#include "Drawing\Point.h"
#include "Drawing\Size.h"
#include "Drawing\Rectangle.h"

#include "Misc\List.h"
#include "Misc\HashTable.h"
#include "Misc\TimeHelper.h"

#include "Event.h"
#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include "SystemEvent.h"

#include "Timer.h"

#include "Control.h"

#include "Panel.h"
#include "Form.h"

//#include "ScrollBar.h"

#include "Label.h"
#include "LinkLabel.h"
#include "Button.h"
#include "CheckBox.h"
//#include "RadioButton.h"
//#include "ComboBox.h"

//#include "ListBox.h"
#include "TrackBar.h"
//#include "ProgressBar.h"

namespace OSHGui
{
	class Gui
	{
	public:
		Gui(Drawing::IRenderer *renderer);

		static Gui* GetInstance();
		
		bool InitMessageHook(HWND window);

		void SetActive(bool active);
		bool GetActive();
		bool RegisterForm(Form *form);
		void UnregisterForm(Form *form);
		void ShowMainForm(Form *form);
		void Render();
		
		static IFont* GetDefaultFont();

		static LRESULT CALLBACK MsgProc(int code, WPARAM wParam, LPARAM lparam);
		
		static Misc::TimeHelper GlobalTime;

	private:
		static Gui *instance;
		static HHOOK messageHook;
		LRESULT ProcessMessage(int code, WPARAM wParam, LPARAM lParam);
	
		Drawing::IRenderer *renderer;
		
		static Drawing::IFont *font;

		static bool active;
		Form *focusForm,
			 *mainForm;
		std::vector<Form*> forms;
		//Misc::List<Form*> forms;
	};
}

#endif