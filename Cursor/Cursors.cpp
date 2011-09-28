#include "Cursors.h"
#include "Cursor.h"
#include "IBeam.h"

namespace OSHGui
{
	std::vector<std::shared_ptr<Cursor> > Cursors::cursors(3, 0);

	Cursor Cursors::Default()
	{
		return Cursor();
	}
	
	Cursor Cursors::IBeam()
	{
		return IBeam();
	}
}