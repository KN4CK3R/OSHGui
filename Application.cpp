#include "Application.h"
#include "Controls\Form.h"
#include "Drawing\TextureAnimator.h"
#include "Misc\Exceptions.h"
#include "FormManager.h"
#include "TimerManager.h"
#include "Cursor\Cursors.h"

namespace OSHGui
{
	bool Application::enabled = false;
	Drawing::IRenderer *Application::renderer = 0;
	Application::MouseInfo Application::mouse;
	Misc::DateTime Application::now = Misc::DateTime::GetNow();
	FormManager Application::formManager;
	TimerManager Application::timerManager;
	std::shared_ptr<Form> Application::mainForm;
	//---------------------------------------------------------------------------
	void Application::Create(Drawing::IRenderer *renderer)
	{
		if (renderer == 0)
		{
			throw Misc::ArgumentNullException(L"renderer", __WFILE__, __LINE__);
		}
		
		Application::renderer = renderer;

		mouse.Cursor = Cursors::Get(Cursors::Default);
	}
	//---------------------------------------------------------------------------
	const Misc::DateTime& Application::GetNow()
	{
		return now;
	}
	//---------------------------------------------------------------------------
	Drawing::IRenderer* Application::GetRenderer()
	{
		return renderer;
	}
	//---------------------------------------------------------------------------
	void Application::Enable()
	{
		enabled = true;

		if (!mainForm->GetVisible())
		{
			mainForm->Show(mainForm);
		}
	}
	//---------------------------------------------------------------------------
	void Application::Disable()
	{
		enabled = false;
	}
	//---------------------------------------------------------------------------
	void Application::Run(const std::shared_ptr<Form> &mainForm)
	{
		if (mainForm == 0)
		{
			throw Misc::ArgumentNullException(L"form", __WFILE__, __LINE__);
		}

		if (Application::mainForm != 0)
		{
			return;
		}

		Application::mainForm = mainForm;

		mainForm->Show(mainForm);
	}
	//---------------------------------------------------------------------------
	IEvent::NextEventTypes Application::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException(L"event", __WFILE__, __LINE__);
		}
		
		if (!enabled)
		{
			return IEvent::Continue;
		}

		if (event->Type == IEvent::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			//grab mouse position here for cursor rendering
			Application::mouse.Position = mouse->Position;
		}

		formManager.ForwardEventToForms(event);

		return IEvent::Continue;
	}
	//---------------------------------------------------------------------------
	void Application::Render()
	{
		if (renderer == 0)
		{
			throw Misc::InvalidOperationException(L"Call Application::Create first.", __WFILE__, __LINE__);
		}
	
		if (!enabled)
		{
			return;
		}
		
		now = Misc::DateTime::GetNow();

		Drawing::TextureAnimator::UpdateFrames();

		timerManager.Update();

		formManager.RenderForms(renderer);
		
		//render startbar
		renderer->SetRenderColor(Drawing::Color(0xFF5C5C5C));
		Drawing::Size size = renderer->GetRenderDimension();
		renderer->Fill(0, size.Height - 30, size.Width, 2);
		renderer->SetRenderColor(Drawing::Color(0xFF2D2D2D));
		renderer->FillGradient(0, size.Height - 28, size.Width, 28, Drawing::Color(0xFF121212));
		renderer->SetRenderColor(Drawing::Color(0xFF2F2F2F));
		renderer->Fill(34, size.Height - 30, 1, 30);

		int tabWidth = (size.Width - 35) / formManager.GetFormCount();
		if (tabWidth > 165)
		{
			tabWidth = 165;
		}
		int tabPos = 35;
		std::shared_ptr<Form> foreMost = formManager.GetForeMost();
		for (int i = 0; i < formManager.GetFormCount(); ++i, tabPos += tabWidth)
		{
			if (foreMost == formManager[i])
			{
				renderer->SetRenderColor(Drawing::Color::Color(115, 110, 31));
				renderer->Fill(tabPos, size.Height - 28, tabWidth, 30);
			}
			renderer->SetRenderColor(Drawing::Color::White());
			renderer->RenderText(renderer->GetDefaultFont(), tabPos + 10, size.Height - 21, tabWidth - 10, 21, formManager[i]->GetText());
			renderer->SetRenderColor(Drawing::Color(0xFF2F2F2F));
			renderer->Fill(tabPos, size.Height - 30, 1, 30);
		}
		
		mouse.Cursor->Render(renderer, mouse.Position);
	}
	//---------------------------------------------------------------------------
}