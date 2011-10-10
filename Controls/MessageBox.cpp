#include "MessageBox.h"
#include "Form.h"
#include "Label.h"
#include "Button.h"
#include "PictureBox.h"

namespace OSHGui
{
	void MessageBox::Show(const Misc::UnicodeString &text)
	{
		Show(text, Misc::UnicodeString(L""));
	}
	//---------------------------------------------------------------------------
	void MessageBox::Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption)
	{
		Show(text, caption, MessageBoxButtons::ButtonOK);
	}
	//---------------------------------------------------------------------------
	void MessageBox::Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons)
	{
		Show(text, caption, buttons, 0);
	}
	//---------------------------------------------------------------------------
	void MessageBox::Show(const Misc::UnicodeString &text, std::function<void(DialogResult result)> closeFunction)
	{
		Show(text, Misc::UnicodeString(L""), closeFunction);
	}
	//---------------------------------------------------------------------------
	void MessageBox::Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, std::function<void(DialogResult result)> closeFunction)
	{
		Show(text, caption, MessageBoxButtons::ButtonOK, closeFunction);
	}
	//---------------------------------------------------------------------------
	void MessageBox::Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons, std::function<void(DialogResult result)> closeFunction)
	{
		std::shared_ptr<MessageBoxForm> messageBox(new MessageBoxForm(text, caption, buttons));
		
		messageBox->ShowDialog(messageBox, [messageBox, closeFunction]()
		{
			if (closeFunction != 0)
			{
				closeFunction(messageBox->GetDialogResult());
			}
		});
	}	
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	MessageBox::MessageBoxForm::MessageBoxForm(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons)
	{
		InitializeComponent(text, caption, buttons);
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void MessageBox::MessageBoxForm::InitializeComponent(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons)
	{
		this->SetText(caption);
		
		Label *textLabel = new Label(this);
		textLabel->SetText(text);
		
		int formMinWidth = textLabel->GetWidth() + 20;
		int formHeight = textLabel->GetHeight();
		
		std::vector<Misc::UnicodeString> label;
		std::vector<std::function<void(Control* control)> >event;
		switch (buttons)
		{
			default:
			case ButtonOK:
				label.push_back(L"OK");
				event.push_back([this](Control *control)
				{
					this->dialogResult = ResultOK;
					Close();
				});
				break;
			case ButtonOKCancel:
				label.push_back(L"Cancel");
				label.push_back(L"OK");
				event.push_back([this](Control *control)
				{
					this->dialogResult = ResultCancel;
					Close();
				});
				event.push_back([this](Control *control)
				{
					this->dialogResult = ResultOK;
					Close();
				});
				break;
			case ButtonAbortRetryIgnore:
				label.push_back(L"Ignore");
				label.push_back(L"Retry");
				label.push_back(L"Abort");
				event.push_back([this](Control *control)
				{
					this->dialogResult = ResultIgnore;
					Close();
				});
				event.push_back([this](Control *control)
				{
					this->dialogResult = ResultRetry;
					Close();
				});
				event.push_back([this](Control *control)
				{
					this->dialogResult = ResultAbort;
					Close();
				});
				break;
			case ButtonYesNo:
				label.push_back(L"No");
				label.push_back(L"Yes");
				event.push_back([this](Control *control)
				{
					this->dialogResult = ResultNo;
					Close();
				});
				event.push_back([this](Control *control)
				{
					this->dialogResult = ResultYes;
					Close();
				});
				break;
			case ButtonYesNoCancel:
				label.push_back(L"Cancel");
				label.push_back(L"No");
				label.push_back(L"Yes");
				event.push_back([this](Control *control)
				{
					this->dialogResult = ResultCancel;
					Close();
				});
				event.push_back([this](Control *control)
				{
					this->dialogResult = ResultNo;
					Close();
				});
				event.push_back([this](Control *control)
				{
					this->dialogResult = ResultYes;
					Close();
				});
				break;
			case ButtonRetryCancel:
				label.push_back(L"Cancel");
				label.push_back(L"Retry");
				event.push_back([this](Control *control)
				{
					this->dialogResult = ResultCancel;
					Close();
				});
				event.push_back([this](Control *control)
				{
					this->dialogResult = ResultRetry;
					Close();
				});
				break;
		}
		
		AddButton(label, event);
		
		this->SetHeight(formHeight);
		this->SetWidth(formMinWidth);
	}
	//---------------------------------------------------------------------------
	void MessageBox::MessageBoxForm::AddButton(const std::vector<Misc::UnicodeString> &label, const std::vector<std::function<void(Control *control)> > &event)
	{
		/*Button *buttonCancel = new Button(this);
		buttonCancel->SetText(L"Cancel");
		buttonCancel->SetLocation(this->GetWidth() - buttonCancel->GetWidth() - 10, this->GetHeight() - buttonCancel->GetHeight() - 10);
		buttonCancel->GetClickEvent().Add([this](Control *control)
		{
			this->dialogResult = ResultCancel;
			Close();
		});
		this->AddControl(buttonCancel);
		Button *buttonOK = new Button(this);
		buttonOK->SetText(L"OK");
		buttonOK->SetLocation(buttonCancel->GetLeft() - buttonOK->GetWidth() - 10, this->GetHeight() - buttonOK->GetHeight() - 10);
		buttonOK->GetClickEvent().Add([this](Control *control)
		{
			this->dialogResult = ResultOK;
			Close();
		});
		this->AddControl(buttonOK);
		if (formMinWidth < 2 * (buttonYes->GetWidth() + 10) + 10)
		{
			formMinWidth = 2 * (buttonYes->GetWidth() + 10) + 10;
		}
		formHeight += buttonRetry->GetHeight() + 20;*/
	}
	//---------------------------------------------------------------------------
}