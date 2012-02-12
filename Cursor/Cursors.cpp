/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "Cursors.hpp"
#include "Cursor.hpp"
#include "IBeam.hpp"
#include "Pipette.hpp"
#include "Hand.hpp"
#include "Circle.hpp"
#include "Wait.hpp"
#include "Pen.hpp"

namespace OSHGui
{
	std::map<int, std::shared_ptr<Cursor> > Cursors::cursors;

	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	const std::shared_ptr<Cursor> Cursors::Get(Cursors::CursorType cursorType)
	{
		auto it = cursors.find((int)cursorType);
		if (it == cursors.end())
		{
			std::shared_ptr<Cursor> cursor;
			switch (cursorType)
			{
				case Cursors::IBeam:
					cursor = std::shared_ptr<Cursor>(new IBeamCursor());
					break;
				case Cursors::Pipette:
					cursor = std::shared_ptr<Cursor>(new PipetteCursor());
					break;
				case Cursors::Hand:
					cursor = std::shared_ptr<Cursor>(new HandCursor());
					break;
				case Cursors::Circle:
					cursor = std::shared_ptr<Cursor>(new CircleCursor());
					break;
				case Cursors::Wait:
					cursor = std::shared_ptr<Cursor>(new WaitCursor());
					break;
				case Cursors::Pen:
					cursor = std::shared_ptr<Cursor>(new PenCursor());
					break;
				default:
					cursor = std::shared_ptr<Cursor>(new Cursor());
					break;
			}
			cursors.insert(std::pair<int, std::shared_ptr<Cursor> >((int)cursorType, cursor));
			return cursor;
		}
		return (*it).second;
	}
	//---------------------------------------------------------------------------
}