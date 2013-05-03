/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

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
	void ContainerControl::SetSize(const Drawing::Size &size)
	{
		Drawing::Size offset = size - GetSize();

		Control::SetSize(size);

		for (auto it = std::begin(controls); it != std::end(controls); ++it)
		{
			auto control = *it;
			AnchorStyle anchor = control->GetAnchor();

			if (anchor != (AnchorTop|AnchorLeft))
			{
				if (anchor == (AnchorTop|AnchorLeft|AnchorBottom|AnchorRight))
				{
					control->SetSize(control->GetSize() + offset);
				}
				else if (anchor == (AnchorTop|AnchorLeft|AnchorRight) || anchor == (AnchorBottom|AnchorLeft|AnchorRight))
				{
					control->SetLocation(control->GetLocation() + Drawing::Point(0, offset.Height));
					control->SetSize(control->GetSize() + Drawing::Size(offset.Width, 0));
				}
				else if (anchor == (AnchorTop|AnchorRight) || anchor == (AnchorBottom|AnchorRight))
				{
					control->SetLocation(control->GetLocation() + Drawing::Point(offset.Width, offset.Height));
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
			throw Misc::ArgumentNullException("control", __FILE__, __LINE__);
		}
		#endif

		if (control->GetType() == CONTROL_FORM)
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
			for (auto it = std::begin(controls); it != std::end(controls); )
			{
				if (*it == control)
				{
					it = controls.erase(it);
				}
				else
				{
					++it;
				}
			}
			for (auto it = std::begin(internalControls); it != std::end(internalControls); )
			{
				if (*it == control)
				{
					it = controls.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	}
	//---------------------------------------------------------------------------
	void ContainerControl::AddSubControl(Control* subcontrol)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (subcontrol == nullptr)
		{
			throw Misc::ArgumentNullException("subcontrol", __FILE__, __LINE__);
		}
		#endif

		if (subcontrol->GetType() == CONTROL_FORM)
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
	bool ContainerControl::Intersect(const Drawing::Point &point) const
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
	Control* ContainerControl::GetChildAtPoint(const Drawing::Point &point) const
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
	ContainerControl::PostOrderVisibleIterator ContainerControl::GetPostOrderVisibleEnumerator()
	{
		return PostOrderVisibleIterator(this);
	}
	//---------------------------------------------------------------------------
	void ContainerControl::Render(Drawing::IRenderer *renderer)
	{
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
				control->Render(renderer);
			}
		}
		if (focusedControl != nullptr)
		{
			focusedControl->Render(renderer);
		}
	}
	//---------------------------------------------------------------------------
	void ContainerControl::ApplyTheme(const Drawing::Theme &theme)
	{
		Control::ApplyTheme(theme);

		auto &controls = this->GetControls();
		for (auto it = std::begin(controls); it != std::end(controls); ++it)
		{
			Control *control = *it;
			control->ApplyTheme(theme);
		}
	}
	//---------------------------------------------------------------------------
	//ContainerControl::PostOrderVisibleIterator
	//---------------------------------------------------------------------------
	ContainerControl::PostOrderVisibleIterator::PostOrderVisibleIterator(ContainerControl *start)
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
	void ContainerControl::PostOrderVisibleIterator::LoopThrough(ContainerControl *container)
	{
		controlStack.push_back(container);
		for (auto it = container->internalControls.rbegin(); it != container->internalControls.rend(); ++it)
		{
			Control *control = *it;
			if (control->GetVisible())
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
	void ContainerControl::PostOrderVisibleIterator::operator++()
	{
		current = controlStack.back();
		controlStack.pop_back();
	}
	//---------------------------------------------------------------------------
	bool ContainerControl::PostOrderVisibleIterator::operator()()
	{
		return !controlStack.empty();
	}
	//---------------------------------------------------------------------------
	Control* ContainerControl::PostOrderVisibleIterator::operator*()
	{
		return current;
	}
	//---------------------------------------------------------------------------
}