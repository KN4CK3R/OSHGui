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
		Show(text, caption, MessageBoxButtons::OK);
	}
	void MessageBox::Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons)
	{
		Show(text, caption, buttons, MessageBoxIcon::None);
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
		Show(text, caption, MessageBoxButtons::OK, closeFunction);
	}
	void MessageBox::Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons, std::function<void(DialogResult result)> closeFunction)
	{
		Show(text, caption, buttons, MessageBoxIcon::None, closeFunction);
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
		InitializeComponent();

		this->SetText(caption);
		this->textLabel->SetText(text);

		switch (buttons)
		{
		case OK:

		}

		switch (icon)
	}

	void MessageBox::::MessageBoxForm::InitializeComponent()
	{

	}
}