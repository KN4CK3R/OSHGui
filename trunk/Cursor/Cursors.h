#ifndef __OSHGUI_CURSORS_H__#define __OSHGUI_CURSORS_H__#include <vector>#include <memory>namespace OSHGui{	/**	 * Stellt Tastencodes bereit.	 */	class Cursors	{	public:		static Cursor Default();		static Cursor IBeam();			private:		static vector<std::shared_ptr<Cursor> > cursors;	};}#endif