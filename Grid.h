#ifndef INCLUDED_GRID_H_
#define INCLUDED_GRID_H_

#include "Vector2.h"

namespace Grid
{
	Vector2 GetSize();
	void SetSize( Vector2 wholeSize );

	int GetRowMax();
	int GetColumnMax();

	void Draw( Vector2 shake );
}

#endif //INCLUDED_GRID_H_
