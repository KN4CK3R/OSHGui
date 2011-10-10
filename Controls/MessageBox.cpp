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
		
		switch (buttons)
		{
			default:
			case ButtonOK:
				Button *buttonOK = new Button(this);
				buttonOK->SetText(L"OK");
				buttonOK->SetLocation(this->GetWidth() - buttonOK->GetWidth() - 10, this->GetHeight() - buttonOK->GetHeight() - 10);
				buttonOK->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultOK;
					Close();
				});
				this->AddControl(buttonOK);
				if (formMinWidth < buttonOK->GetWidth() + 20)
				{
					formMinWidth = buttonOK->GetWidth() + 20;
				}
				formHeight += buttonOK->GetHeight() + 20;
				break;
			case ButtonOKCancel:
				Button *buttonCancel = new Button(this);
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
				if (formMinWidth < 2 * (buttonOK->GetWidth() + 10) + 10)
				{
					formMinWidth = 2 * (buttonOK->GetWidth() + 10) + 10;
				}
				formHeight += buttonOK->GetHeight() + 20;
				break;
			case ButtonAbortRetryIgnore:
				Button *buttonIgnore = new Button(this);
				buttonIgnore->SetText(L"Ignore");
				buttonIgnore->SetLocation(this->GetWidth() - buttonIgnore->GetWidth() - 10, this->GetHeight() - buttonIgnore->GetHeight() - 10);
				buttonIgnore->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultIgnore;
					Close();
				});
				this->AddControl(buttonIgnore);
				Button *buttonRetry = new Button(this);
				buttonRetry->SetText(L"Retry");
				buttonRetry->SetLocation(buttonIgnore->GetLeft() - buttonOK->GetWidth() - 10, this->GetHeight() - buttonRetry->GetHeight() - 10);
				buttonRetry->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultRetry;
					Close();
				});
				this->AddControl(buttonRetry);
				Button *buttonAbort = new Button(this);
				buttonAbort->SetText(L"Abort");
				buttonAbort->SetLocation(buttonRetry->GetLeft() - buttonOK->GetWidth() - 10, this->GetHeight() - buttonAbort->GetHeight() - 10);
				buttonAbort->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultAbort;
					Close();
				});
				this->AddControl(buttonAbort);
				if (formMinWidth < 3 * (buttonIgnore->GetWidth() + 10) + 10)
				{
					formMinWidth = 3 * (buttonIgnore->GetWidth() + 10) + 10;
				}
				formHeight += buttonIgnore->GetHeight() + 20;
				break;
			case ButtonYesNo:
				Button *buttonNo = new Button(this);
				buttonNo->SetText(L"No");
				buttonNo->SetLocation(this->GetWidth() - buttonNo->GetWidth() - 10, this->GetHeight() - buttonNo->GetHeight() - 10);
				buttonNo->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultNo;
					Close();
				});
				this->AddControl(buttonNo);
				Button *buttonYes = new Button(this);
				buttonYes->SetText(L"Yes");
				buttonYes->SetLocation(buttonNo->GetLeft() - buttonYes->GetWidth() - 10, this->GetHeight() - buttonYes->GetHeight() - 10);
				buttonYes->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultYes;
					Close();
				});
				this->AddControl(buttonYes);
				if (formMinWidth < 2 * (buttonYes->GetWidth() + 10) + 10)
				{
					formMinWidth = 2 * (buttonYes->GetWidth() + 10) + 10;
				}
				formHeight += buttonYes->GetHeight() + 20;
				break;
			case ButtonYesNoCancel:
				Button *buttonCancel = new Button(this);
				buttonCancel->SetText(L"Cancel");
				buttonCancel->SetLocation(this->GetWidth() - buttonCancel->GetWidth() - 10, this->GetHeight() - buttonCancel->GetHeight() - 10);
				buttonCancel->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultCancel;
					Close();
				});
				Button *buttonNo = new Button(this);
				buttonNo->SetText(L"No");
				buttonNo->SetLocation(buttonCancel->GetLeft() - buttonNo->GetWidth() - 10, this->GetHeight() - buttonNo->GetHeight() - 10);
				buttonNo->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultNo;
					Close();
				});
				this->AddControl(buttonNo);
				Button *buttonYes = new Button(this);
				buttonYes->SetText(L"Yes");
				buttonYes->SetLocation(buttonNo->GetLeft() - buttonYes->GetWidth() - 10, this->GetHeight() - buttonYes->GetHeight() - 10);
				buttonYes->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultYes;
					Close();
				});
				this->AddControl(buttonYes);
				if (formMinWidth < 3 * (buttonCancel->GetWidth() + 10) + 10)
				{
					formMinWidth = 3 * (buttonCancel->GetWidth() + 10) + 10;
				}
				formHeight += buttonYes->GetHeight() + 20;
				break;
			case ButtonRetryCancel:
				Button *buttonCancel = new Button(this);
				buttonCancel->SetText(L"Cancel");
				buttonCancel->SetLocation(this->GetWidth() - buttonCancel->GetWidth() - 10, this->GetHeight() - buttonCancel->GetHeight() - 10);
				buttonCancel->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultCancel;
					Close();
				});
				this->AddControl(buttonCancel);
				Button *buttonRetry = new Button(this);
				buttonRetry->SetText(L"Retry");
				buttonRetry->SetLocation(buttonCancel->GetLeft() - buttonRetry->GetWidth() - 10, this->GetHeight() - buttonRetry->GetHeight() - 10);
				buttonRetry->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultRetry;
					Close();
				});
				this->AddControl(buttonRetry);
				if (formMinWidth < 2 * (buttonYes->GetWidth() + 10) + 10)
				{
					formMinWidth = 2 * (buttonYes->GetWidth() + 10) + 10;
				}
				formHeight += buttonRetry->GetHeight() + 20;
				break;
		}
		
		this->SetHeight(formHeight);
		this->SetWidth(formMinWidth);
	}
	//---------------------------------------------------------------------------
}