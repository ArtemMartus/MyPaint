#include "Resource.h"

#include "DrawItem.h"



DrawItem::DrawItem():currentState(0)
{
}


DrawItem::~DrawItem()
{
}

bool DrawItem::addPos(POINT & point)		// add new position to draw item
{
	if (currentState < 0)
		currentState = 0;

	switch (currentState) {
	case 0:				// if it's empty assign 'point' to positionStart
	{
		positionStart = point;
		currentState++;
		return true;
	}
	case 1:				// if it's not empty -  to positionEnd
	{
		positionEnd = point;
		currentState++;
		return true;
	}
	default:			// if we have posStart and posEnd - return false
		return false;
	}
}

void DrawItem::clear()
{
	currentState = 0; //reset pos counter
}

void DrawItem::Draw(HDC hdc)
{
	if (currentState < 0)
		currentState = 0;

	if (currentState == 2) // draw only if we have posStart and posEnd
	{
		auto hPen = CreatePen(PS_SOLID, width, RGB(color.r, color.g, color.b)); // create color and width for pen
		SelectObject(hdc, hPen);
		switch (type) {
		case DT_LINE:
		{
			MoveToEx(hdc, positionStart.x, positionStart.y, 0);
			LineTo(hdc, positionEnd.x, positionEnd.y);
			break;
		}
		case DT_BOX:
		{
			Rectangle(hdc, positionStart.x, positionStart.y, positionEnd.x, positionEnd.y);
			break;
		}
		case DT_ELLIPS:
		{
			Ellipse(hdc, positionStart.x, positionStart.y, positionEnd.x, positionEnd.y);
			break;
		}
		}
	}
}
