#include "MessageBox.hpp"
#include "Form.hpp"
#include "Label.hpp"
#include "Button.hpp"
#include "PictureBox.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	void MessageBox::Show(const Misc::UnicodeString &text)
	{
		Show(text, Misc::UnicodeString(L""));
	}
	//---------------------------------------------------------------------------
	void MessageBox::Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption)
	{
		Show(text, caption, ButtonOK);
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
		Show(text, caption, ButtonOK, closeFunction);
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
		this->AddControl(textLabel);
		
		int formWidth = textLabel->GetWidth() + 20;
		int formHeight = textLabel->GetHeight() + 40;

		Button *tempButton = new Button(this);
		Drawing::Size defaultButtonSize = tempButton->GetSize();
		delete tempButton;

		formHeight += defaultButtonSize.Height;
		int neededWidthForButtons = 0;
		
		std::vector<Misc::UnicodeString> label;
		std::vector<ClickEventHandler> eventHandler;
		switch (buttons)
		{
			default:
			case ButtonOK:
				neededWidthForButtons = defaultButtonSize.Width + 20;

				label.push_back(L"OK");
				eventHandler.push_back(ClickEventHandler([this](Control *control)
				{
					this->dialogResult = ResultOK;
					Close();
				}));
				break;
			case ButtonOKCancel:
				neededWidthForButtons = 2 * (defaultButtonSize.Width + 10) + 10;

				label.push_back(L"Cancel");
				label.push_back(L"OK");
				eventHandler.push_back(ClickEventHandler([this](Control *control)
				{
					this->dialogResult = ResultCancel;
					Close();
				}));
				eventHandler.push_back(ClickEventHandler([this](Control *control)
				{
					this->dialogResult = ResultOK;
					Close();
				}));
				break;
			case ButtonAbortRetryIgnore:
				neededWidthForButtons = 3 * (defaultButtonSize.Width + 10) + 10;

				label.push_back(L"Ignore");
				label.push_back(L"Retry");
				label.push_back(L"Abort");
				eventHandler.push_back(ClickEventHandler([this](Control *control)
				{
					this->dialogResult = ResultIgnore;
					Close();
				}));
				eventHandler.push_back(ClickEventHandler([this](Control *control)
				{
					this->dialogResult = ResultRetry;
					Close();
				}));
				eventHandler.push_back(ClickEventHandler([this](Control *control)
				{
					this->dialogResult = ResultAbort;
					Close();
				}));
				break;
			case ButtonYesNo:
				neededWidthForButtons = 2 * (defaultButtonSize.Width + 10) + 10;

				label.push_back(L"No");
				label.push_back(L"Yes");
				eventHandler.push_back(ClickEventHandler([this](Control *control)
				{
					this->dialogResult = ResultNo;
					Close();
				}));
				eventHandler.push_back(ClickEventHandler([this](Control *control)
				{
					this->dialogResult = ResultYes;
					Close();
				}));
				break;
			case ButtonYesNoCancel:
				neededWidthForButtons = 3 * (defaultButtonSize.Width + 10) + 10;

				label.push_back(L"Cancel");
				label.push_back(L"No");
				label.push_back(L"Yes");
				eventHandler.push_back(ClickEventHandler([this](Control *control)
				{
					this->dialogResult = ResultCancel;
					Close();
				}));
				eventHandler.push_back(ClickEventHandler([this](Control *control)
				{
					this->dialogResult = ResultNo;
					Close();
				}));
				eventHandler.push_back(ClickEventHandler([this](Control *control)
				{
					this->dialogResult = ResultYes;
					Close();
				}));
				break;
			case ButtonRetryCancel:
				neededWidthForButtons = 2 * (defaultButtonSize.Width + 10) + 10;

				label.push_back(L"Cancel");
				label.push_back(L"Retry");
				eventHandler.push_back(ClickEventHandler([this](Control *control)
				{
					this->dialogResult = ResultCancel;
					Close();
				}));
				eventHandler.push_back(ClickEventHandler([this](Control *control)
				{
					this->dialogResult = ResultRetry;
					Close();
				}));
				break;
		}

		if (neededWidthForButtons > formWidth)
		{
			formWidth = neededWidthForButtons;
		}

		this->SetSize(formWidth, formHeight);
		Drawing::Size screen = Application::Instance()->GetRenderer()->GetRenderDimension();
		this->SetLocation(screen.Width / 2 - formWidth / 2, screen.Height / 2 - formHeight / 2);
		
		AddButtons(label, eventHandler);
	}
	//---------------------------------------------------------------------------
	void MessageBox::MessageBoxForm::AddButtons(const std::vector<Misc::UnicodeString> &label, const std::vector<ClickEventHandler> &eventHandler)
	{
		if (label.size() != eventHandler.size())
		{
			throw new Misc::ArgumentException(L"label + eventHandler");
		}

		for (unsigned int i = 0; i < label.size(); ++i)
		{
			Button *button = new Button(this);
			button->SetText(label[i]);
			button->GetClickEvent() += ClickEventHandler(eventHandler[i]);
			button->SetLocation(this->GetWidth() - (i + 1) * (button->GetWidth() + 10) , this->GetHeight() - button->GetHeight() - 25);

			this->AddControl(button);
		}
	}
	//---------------------------------------------------------------------------
}