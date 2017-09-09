#pragma once
class DrawItem
{
	int currentState;	
	POINT positionStart;
	POINT positionEnd;
public:
	DrawItem();
	~DrawItem();
	bool addPos(POINT& point);
	void clear();
	void Draw(HDC hdc);

	int width;
	int type;
	
	rgba color;
};

