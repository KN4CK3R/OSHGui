/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "IBeam.hpp"
#include "../Drawing/Graphics.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void IBeamCursor::Initialize()
	{
		using namespace Drawing;

		Graphics g(geometry);

		PointF offset(-4, -7);

		g.FillRectangle(Color::White(), PointF(0, 0) + offset, SizeF(7, 1));
		g.FillRectangle(Color::White(), PointF(0, 13) + offset, SizeF(7, 1));
		g.FillRectangle(Color::White(), PointF(3, 1) + offset, SizeF(1, 12));
	}
	//---------------------------------------------------------------------------
}