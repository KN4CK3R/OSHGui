#ifndef __OSHGUI_H__
#define __OSHGUI_H__

#include <windows.h>

#include "Drawing\Point.h"
#include "Drawing\Size.h"
#include "Drawing\Rectangle.h"

#include "Misc\TimeHelper.h"

#include "EventHandler.h"

#include "Control.h"
#include "Label.h"
#include "LinkLabel.h"
#include "Button.h"
#include "CheckBox.h"
#include "RadioButton.h"
#include "ComboBox.h"
#include "Panel.h"
#include "Form.h"
#include "GroupBox.h"
#include "ListBox.h"
#include "ProgressBar.h"
#include "TrackBar.h"
#include "TextBox.h"
#include "Timer.h"

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
		
		static Drawing::IFont* GetDefaultFont();

		static LRESULT CALLBACK MsgProc(int code, WPARAM wParam, LPARAM lparam);
		
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
	};
}

#endif