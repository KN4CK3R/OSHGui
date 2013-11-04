/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include <algorithm>
#include "ContainerControl.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ContainerControl::ContainerControl()
	{

	}
	//---------------------------------------------------------------------------
	ContainerControl::~ContainerControl()
	{
		controls.clear();

		for (auto it = std::begin(internalControls); it != std::end(internalControls); ++it)
		{
			delete *it;
		}
		internalControls.clear();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	bool ContainerControl::IsContainer() const
	{
		return true;
	}
	//---------------------------------------------------------------------------
	void ContainerControl::SetSize(const Drawing::SizeF &size)
	{
		Drawing::SizeF offset = size - GetSize();

		Control::SetSize(size);

		for (auto &control : controls)
		{
			AnchorStyles anchor = control->GetAnchor();

			if (anchor != (AnchorStyles::Top|AnchorStyles::Left))
			{
				if (anchor == (AnchorStyles::Top|AnchorStyles::Left|AnchorStyles::Bottom|AnchorStyles::Right))
				{
					control->SetSize(control->GetSize() + offset);
				}
				else if (anchor == (AnchorStyles::Top|AnchorStyles::Left|AnchorStyles::Right) || anchor == (AnchorStyles::Bottom|AnchorStyles::Left|AnchorStyles::Right))
				{
					control->SetLocation(control->GetLocation() + Drawing::PointF(0, offset.Height));
					control->SetSize(control->GetSize() + Drawing::SizeF(offset.Width, 0));
				}
				else if (anchor == (AnchorStyles::Top|AnchorStyles::Right) || anchor == (AnchorStyles::Bottom|AnchorStyles::Right))
				{
					control->SetLocation(control->GetLocation() + Drawing::PointF(offset.Width, offset.Height));
				}
			}
		}
	}
	//---------------------------------------------------------------------------
	const std::deque<Control*>& ContainerControl::GetControls() const
	{
		return controls;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void ContainerControl::AddControl(Control *control)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (control == nullptr)
		{
			throw Misc::ArgumentNullException("control");
		}
		#endif

		if (control->GetType() == ControlType::Form)
		{
			return;
		}

		AddSubControl(control);

		controls.push_front(control);
	}
	//---------------------------------------------------------------------------
	void ContainerControl::RemoveControl(Control *control)
	{
		if (control != nullptr)
		{
			controls.erase(std::remove(std::begin(controls), std::end(controls), control), std::end(controls));
			internalControls.erase(std::remove(std::begin(internalControls), std::end(internalControls), control), std::end(internalControls));
		}
	}
	//---------------------------------------------------------------------------
	void ContainerControl::AddSubControl(Control* subcontrol)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (subcontrol == nullptr)
		{
			throw Misc::ArgumentNullException("subcontrol");
		}
		#endif

		if (subcontrol->GetType() == ControlType::Form)
		{
			return;
		}
		
		for (auto it = std::begin(internalControls); it != std::end(internalControls); ++it)
		{
			Control *control = *it;
			if (subcontrol == control || (!subcontrol->GetName().empty() && subcontrol->GetName() == control->GetName()))
			{
				return;
			}
		}

		subcontrol->SetParent(this);

		internalControls.push_front(subcontrol);
	}
	//---------------------------------------------------------------------------
	bool ContainerControl::Intersect(const Drawing::PointF &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void ContainerControl::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();
		
		for (auto it = std::begin(internalControls); it != internalControls.end(); ++it)
		{
			(*it)->CalculateAbsoluteLocation();
		}
	}
	//---------------------------------------------------------------------------
	Control* ContainerControl::GetChildAtPoint(const Drawing::PointF &point) const
	{
		for (auto it = controls.rbegin(); it != controls.rend(); ++it)
		{
			Control *control = *it;

			if (control->GetEnabled() && control->GetVisible() && control->Intersect(point))
			{
				return control;
			}
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	Control* ContainerControl::GetChildByName(const Misc::AnsiString &name) const
	{
		for (auto it = std::begin(controls); it != std::end(controls); ++it)
		{
			Control *control = *it;
			
			if (control->GetName() == name)
			{
				return control;
			}
		}

		return nullptr;
	}
	//---------------------------------------------------------------------------
	ContainerControl::PostOrderIterator ContainerControl::GetPostOrderEnumerator()
	{
		return PostOrderIterator(this);
	}
	//---------------------------------------------------------------------------
	void ContainerControl::Render()
	{
		if (!isVisible)
		{
			return;
		}

		using namespace Drawing;

		RenderContext ctx;
		GetRenderContext(ctx);

		// clear geometry from surface if it's ours
		if (ctx.Owner == this)
		{
			ctx.Surface->Reset();
		}

		// redraw if no surface set, or if surface is invalidated
		if (!surface)// || surface->isInvalidated())
		{
			DrawSelf(ctx);

			Control *focusedControl = nullptr;
			for (auto it = controls.rbegin(); it != controls.rend(); ++it)
			{
				Control *control = *it;
				if (control->GetIsFocused())
				{
					focusedControl = control;
				}
				else
				{
					control->Render();
				}
			}
			if (focusedControl != nullptr)
			{
				focusedControl->Render();
			}
		}

		if (ctx.Owner == this)
		{
			ctx.Surface->Draw();
		}
	}
	//---------------------------------------------------------------------------
	void ContainerControl::ApplyTheme(const Drawing::Theme &theme)
	{
		Control::ApplyTheme(theme);

		auto &controls = this->GetControls();
		for (auto it = std::begin(controls); it != std::end(controls); ++it)
		{
			auto control = *it;
			control->ApplyTheme(theme);
		}
	}
	//---------------------------------------------------------------------------
	//ContainerControl::PostOrderIterator
	//---------------------------------------------------------------------------
	ContainerControl::PostOrderIterator::PostOrderIterator(ContainerControl *start)
	{
		this->start = start;

		if (this->start->internalControls.empty())
		{
			current = this->start;
		}
		else
		{
			LoopThrough(this->start);
			current = controlStack.back();
			controlStack.pop_back();
		}
	}
	//---------------------------------------------------------------------------
	void ContainerControl::PostOrderIterator::LoopThrough(ContainerControl *container)
	{
		controlStack.push_back(container);
		for (auto it = container->internalControls.rbegin(); it != container->internalControls.rend(); ++it)
		{
			Control *control = *it;
			if (control->GetVisible() && control->GetEnabled())
			{
				if (control->IsContainer())
				{
					LoopThrough(static_cast<ContainerControl*>(control));
				}
				else
				{
					controlStack.push_back(control);
				}
			}
		}
	}
	//---------------------------------------------------------------------------
	void ContainerControl::PostOrderIterator::operator++()
	{
		current = controlStack.back();
		controlStack.pop_back();
	}
	//---------------------------------------------------------------------------
	bool ContainerControl::PostOrderIterator::operator()()
	{
		return !controlStack.empty();
	}
	//---------------------------------------------------------------------------
	Control* ContainerControl::PostOrderIterator::operator*()
	{
		return current;
	}
	//---------------------------------------------------------------------------
}