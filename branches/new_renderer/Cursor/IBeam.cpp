/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "IBeam.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	IBeamCursor::IBeamCursor()
	{
		offset = Drawing::PointF(-4, -7);
		CreateCursor();
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void IBeamCursor::CreateCursor()
	{
		/*cursor->BeginUpdate();
		cursor->Clear();
		
		cursor->Fill(0, 0, 7, 1, Drawing::Color::White());
		cursor->Fill(0, 13, 7, 1, Drawing::Color::White());
		cursor->Clear(3, 0, 1, 14);
		cursor->Fill(3, 1, 1, 12, Drawing::Color::White());
		
		cursor->EndUpdate();*/
	}
	//---------------------------------------------------------------------------
}