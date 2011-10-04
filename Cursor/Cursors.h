#ifndef OSHGUI_CURSOR_CURSORS_H_
#define OSHGUI_CURSOR_CURSORS_H_

#include <map>
#include <memory>
#include "Cursor.h"

namespace OSHGui
{
	/**
	 * Stellt Tastencodes bereit.
	 */
	class Cursors
	{
	public:
		enum CursorType
		{
			Default,
			IBeam,
			Pipette,
			Hand,
			Circle,
			Wait
		};

		static const std::shared_ptr<Cursor> Get(CursorType cursorType);

	private:
		static std::map<int, std::shared_ptr<Cursor> > Cursors::cursors;
	};
}

#endif