/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "MessageBox.hpp"
#include "Form.hpp"
#include "Label.hpp"
#include "Button.hpp"
#include "PictureBox.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	void MessageBox::Show(const Misc::AnsiString &text)
	{
		Show(text, Misc::AnsiString(""));
	}
	//---------------------------------------------------------------------------
	void MessageBox::Show(const Misc::AnsiString &text, const Misc::AnsiString &caption)
	{
		Show(text, caption, ButtonOK);
	}
	//---------------------------------------------------------------------------
	void MessageBox::Show(const Misc::AnsiString &text, const Misc::AnsiString &caption, MessageBoxButtons buttons)
	{
		Show(text, caption, buttons, 0);
	}
	//---------------------------------------------------------------------------
	void MessageBox::Show(const Misc::AnsiString &text, std::function<void(DialogResult result)> closeFunction)
	{
		Show(text, "", closeFunction);
	}
	//---------------------------------------------------------------------------
	void MessageBox::Show(const Misc::AnsiString &text, const Misc::AnsiString &caption, std::function<void(DialogResult result)> closeFunction)
	{
		Show(text, caption, ButtonOK, closeFunction);
	}
	//---------------------------------------------------------------------------
	void MessageBox::Show(const Misc::AnsiString &text, const Misc::AnsiString &caption, MessageBoxButtons buttons, std::function<void(DialogResult result)> closeFunction)
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
	MessageBox::MessageBoxForm::MessageBoxForm(const Misc::AnsiString &text, const Misc::AnsiString &caption, MessageBoxButtons buttons) : Form()
	{
		InitializeComponent(text, caption, buttons);
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void MessageBox::MessageBoxForm::InitializeComponent(const Misc::AnsiString &text, const Misc::AnsiString &caption, MessageBoxButtons buttons)
	{
		this->SetText(caption);
		
		Label *textLabel = new Label();
		textLabel->SetText(text);
		this->AddControl(textLabel);
		
		int formWidth = textLabel->GetWidth() + 20;
		int formHeight = textLabel->GetHeight() + 40;

		Button *button = new Button();
		int buttonWidth = button->GetWidth();
		formHeight += button->GetHeight();
		delete button;
		int neededWidthForButtons = 0;
		
		std::vector<Misc::AnsiString> label;
		std::vector<ClickEventHandler> eventHandler;
		switch (buttons)
		{
			default:
			case ButtonOK:
				neededWidthForButtons = buttonWidth + 20;

				label.push_back("OK");
				eventHandler.push_back(ClickEventHandler([this](Control *control)
				{
					this->dialogResult = ResultOK;
					Close();
				}));
				break;
			case ButtonOKCancel:
				neededWidthForButtons = 2 * (buttonWidth + 10) + 10;

				label.push_back("Cancel");
				label.push_back("OK");
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
				neededWidthForButtons = 3 * (buttonWidth + 10) + 10;

				label.push_back("Ignore");
				label.push_back("Retry");
				label.push_back("Abort");
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
				neededWidthForButtons = 2 * (buttonWidth + 10) + 10;

				label.push_back("No");
				label.push_back("Yes");
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
				neededWidthForButtons = 3 * (buttonWidth + 10) + 10;

				label.push_back("Cancel");
				label.push_back("No");
				label.push_back("Yes");
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
				neededWidthForButtons = 2 * (buttonWidth + 10) + 10;

				label.push_back("Cancel");
				label.push_back("Retry");
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
	void MessageBox::MessageBoxForm::AddButtons(const std::vector<Misc::AnsiString> &label, const std::vector<ClickEventHandler> &eventHandler)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (label.size() != eventHandler.size())
		{
			throw new Misc::ArgumentException("label + eventHandler");
		}
		#endif

		for (unsigned int i = 0; i < label.size(); ++i)
		{
			Button *button = new Button();
			button->SetText(label[i]);
			button->GetClickEvent() += ClickEventHandler(eventHandler[i]);
			button->SetLocation(GetWidth() - (i + 1) * (button->GetWidth() + 10) , GetHeight() - button->GetHeight() - 25);

			this->AddControl(button);
		}
	}
	//---------------------------------------------------------------------------
}