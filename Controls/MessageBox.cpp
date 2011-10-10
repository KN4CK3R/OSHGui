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
	void MessageBox::Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption)
	{
		Show(text, caption, MessageBoxButtons::ButtonOK);
	}
	void MessageBox::Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons)
	{
		Show(text, caption, buttons, MessageBoxIcon::IconNone);
	}
	void MessageBox::Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons, MessageBoxIcon icon)
	{
		Show(text, caption, buttons, icon, 0);
	}
	void MessageBox::Show(const Misc::UnicodeString &text, std::function<void(DialogResult result)> closeFunction)
	{
		Show(text, Misc::UnicodeString(L""), closeFunction);
	}
	void MessageBox::Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, std::function<void(DialogResult result)> closeFunction)
	{
		Show(text, caption, MessageBoxButtons::ButtonOK, closeFunction);
	}
	void MessageBox::Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons, std::function<void(DialogResult result)> closeFunction)
	{
		Show(text, caption, buttons, MessageBoxIcon::IconNone, closeFunction);
	}
	void MessageBox::Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons, MessageBoxIcon icon, std::function<void(DialogResult result)> closeFunction)
	{
		std::shared_ptr<MessageBoxForm> messageBox(new MessageBoxForm(text, caption, buttons, icon));
		
		messageBox->ShowDialog(messageBox, [messageBox, closeFunction]()
		{
			if (closeFunction != 0)
			{
				closeFunction(messageBox->GetDialogResult());
			}
		});
	}

	MessageBox::MessageBoxForm::MessageBoxForm(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons, MessageBoxIcon icon)
	{
		InitializeComponent(text, caption, buttons, icon);
	}

	void MessageBox::MessageBoxForm::MessageBoxForm::InitializeComponent(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons, MessageBoxIcon icon)
	{
		this->SetText(caption);
		
		int leftPos = 6;
		int formMinWidth = 60;
		
		if (icon != IconNone)
		{
			formMinWidth += 40;
		}
		
		int buttonWidth = 0;
		Button *temp = new Button(this);
		buttonWidth = temp->GetWidth();
		delete temp;		
		switch (buttons)
		{
			case ButtonOKCancel:
			case ButtonYesNo:
			case ButtonRetryCancel:
				buttonWidth = 2 * buttonWidth + 20;
				break;
			case ButtonAbortRetryIgnore:
			case ButtonYesNoCancel:
				buttonWidth = 3 * buttonWidth + 30;
				break;
		}
		if (buttonWidth < formMinWidth)
		{
			formMinWidth = buttonWidth;
		}

		switch (icon)
		{
			case IconWarning:
				/*FillGradient(b, 0, 0, 32, 32, Color.FromArgb(255, 247, 50), Color.FromArgb(227, 162, 0));
				Fill(b, 0, 0, 32, 2, Color.Empty);
				Fill(b, 0, 30, 32, 2, Color.Empty);
				Fill(b, 29, 0, 3, 32, Color.Empty);

				Color grau = Color.FromArgb(190, 188, 176);
				grau = Color.DarkGray;
				Color schwarz = Color.FromArgb(86, 86, 86);
				grau = schwarz;
				for (int i = 0; i < 26; i+=2)
				{
					Fill(b, 0, 2 + i, 13 - i / 2, 2, Color.Empty);
					Fill(b, 16 + i / 2, 2 + i, 13 - i / 2, 2, Color.Empty);
					Fill(b, 15 + i / 2, 2 + i, 2, 2, grau);
					Fill(b, 13 - i / 2, 2 + i, 2, 2, grau);
				}
				Fill(b, 0, 28, 30, 2, grau);
				Fill(b, 14, 1, 2, 1, grau);
				Fill(b, 13, 9, 4, 12, schwarz);
				Fill(b, 13, 23, 4, 3, schwarz);*/
				break;
		}

		Label *textLabel = new Label(this);
		textLabel->SetLocation(leftPos, 6);
		textLabel->SetText(text);
		
		if (textLabel->GetWidth() > formMinWidth - 20)
		{
			formMinWidth = textLabel->GetWidth();
		}
		
		int buttonTopPos = 0;
		switch (buttons)
		{
			default:
			case ButtonOK:
				Button *buttonOK = new Button(this);
				buttonOK->SetText(L"OK");
				buttonTopPos = this->GetHeight() - buttonOK->GetHeight() - 10;
				buttonOK->SetLocation(this->GetWidth() - buttonOK->GetWidth() - 10, buttonTopPos);
				buttonOK->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultOK;
					Close();
				});
				this->AddControl(buttonOK);
				break;
			case ButtonOKCancel:
				Button *buttonCancel = new Button(this);
				buttonCancel->SetText(L"Cancel");
				buttonTopPos = this->GetHeight() - buttonCancel->GetHeight() - 10;
				buttonCancel->SetLocation(this->GetWidth() - buttonCancel->GetWidth() - 10, buttonTopPos);
				buttonCancel->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultCancel;
					Close();
				});
				this->AddControl(buttonCancel);
				Button *buttonOK = new Button(this);
				buttonOK->SetText(L"OK");
				buttonOK->SetLocation(buttonCancel->GetLeft() - buttonOK->GetWidth() - 10, buttonTopPos);
				buttonOK->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultOK;
					Close();
				});
				this->AddControl(buttonOK);
				break;
			case ButtonAbortRetryIgnore:
				Button *buttonIgnore = new Button(this);
				buttonIgnore->SetText(L"Ignore");
				buttonTopPos = this->GetHeight() - buttonIgnore->GetHeight() - 10;
				buttonIgnore->SetLocation(this->GetWidth() - buttonIgnore->GetWidth() - 10, buttonTopPos);
				buttonIgnore->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultIgnore;
					Close();
				});
				this->AddControl(buttonIgnore);
				Button *buttonRetry = new Button(this);
				buttonRetry->SetText(L"Retry");
				buttonRetry->SetLocation(buttonIgnore->GetLeft() - buttonOK->GetWidth() - 10, buttonTopPos);
				buttonRetry->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultRetry;
					Close();
				});
				this->AddControl(buttonRetry);
				Button *buttonAbort = new Button(this);
				buttonAbort->SetText(L"Abort");
				buttonAbort->SetLocation(buttonRetry->GetLeft() - buttonOK->GetWidth() - 10, buttonTopPos);
				buttonAbort->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultAbort;
					Close();
				});
				this->AddControl(buttonAbort);
				break;
			case ButtonYesNo:
				Button *buttonNo = new Button(this);
				buttonNo->SetText(L"Cancel");
				buttonTopPos = this->GetHeight() - buttonNo->GetHeight() - 10;
				buttonNo->SetLocation(this->GetWidth() - buttonNo->GetWidth() - 10, buttonTopPos);
				buttonNo->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultNo;
					Close();
				});
				this->AddControl(buttonNo);
				Button *buttonYes = new Button(this);
				buttonYes->SetText(L"OK");
				buttonYes->SetLocation(buttonNo->GetLeft() - buttonYes->GetWidth() - 10, buttonTopPos);
				buttonYes->GetClickEvent().Add([this](Control *control)
				{
					this->dialogResult = ResultYes;
					Close();
				});
				this->AddControl(buttonYes);
				break;
		}
	}
}