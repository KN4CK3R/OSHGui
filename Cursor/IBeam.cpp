#include "IBeam.h"
#include "../Application.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	IBeamCursor::IBeamCursor()
	{
		offset = Drawing::Point(-4, -7);
		CreateCursor();
	}
	//---------------------------------------------------------------------------
	IBeamCursor::~IBeamCursor()
	{
	
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void IBeamCursor::CreateCursor()
	{
		cursor->BeginUpdate();
		cursor->Clear();
		
		cursor->Fill(0, 0, 7, 1, Drawing::Color::White());
		cursor->Fill(0, 13, 7, 1, Drawing::Color::White());
		cursor->Clear(3, 0, 1, 14);
		cursor->Fill(3, 1, 1, 12, Drawing::Color::White());
		
		cursor->EndUpdate();
	}
	//---------------------------------------------------------------------------
}