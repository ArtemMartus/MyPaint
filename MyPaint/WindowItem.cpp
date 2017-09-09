#include "Resource.h"

#include "WindowItem.h"



WindowItem::WindowItem()
{
}

//Initializing, random id
WindowItem::WindowItem(int _width, int _height, int _x, int _y, TCHAR *_className, TCHAR* _text) :
	 width(_width), height(_height), x(_x), y(_y), id(rand()), text(_text),className(_className)
{
	if (text == 0)
		text = TEXT("no name");
	if (className == 0)
		throw std::runtime_error("empty class name");
}


WindowItem::~WindowItem()
{
	CloseWindow(wndDescr);
}

int WindowItem::getId()
{
	return id;
}

void WindowItem::setText(TCHAR * newText)
{
	SetWindowText(wndDescr, newText);
	text = newText;
}

TCHAR * WindowItem::getText()
{
	TCHAR buff[1024];
	memset(buff, 0, sizeof(buff));
	GetWindowText(wndDescr, buff, 1024);
	text =  buff;
	return text;
}

void WindowItem::Create(HWND parent)
{
	parentDescr = parent;

	RECT parentRect;
	GetWindowRect(parentDescr, &parentRect);
	POINT parentSize;
	parentSize.x = parentRect.right - parentRect.left;
	parentSize.y = parentRect.bottom - parentRect.top;

	// check for correct width & height

	width = width < 20 ? 20 : width > parentSize.x ? parentSize.x : width;
	height = height < 20 ? 20 : height > parentSize.y ? parentSize.y : height;

	// check for correct position

	x = x < 0 ? 0 : x > parentSize.x - width ? parentSize.x - width : x;
	y = y < 0 ? 0 : y > parentSize.y - height ? parentSize.y - height : y;

	wndDescr = 0;
	Show();
}

void WindowItem::Show()
{
	if (wndDescr != 0)
		return;
	int flags = WS_CHILD | WS_VISIBLE | WS_BORDER;
	if (_tcscmp(className, TEXT("button")) == 0) // if it's a button assign correct flag
		flags |= BS_PUSHBUTTON;
	wndDescr = CreateWindowEx(WS_EX_STATICEDGE,className, text, flags, x, y, width, height, parentDescr, (HMENU)id, GetModuleHandle(0), 0);
	if (!wndDescr)
		throw std::runtime_error("cannot create window item");
}

void WindowItem::Hide()
{
	if (wndDescr == 0)
		return;
	CloseWindow(wndDescr);
	wndDescr = 0;
}

EditTextBox::EditTextBox(int _width, int _height, int _x, int _y): WindowItem(_width,_height,_x,_y,TEXT("edit"),TEXT("Type something"))
{
}

Label::Label(int _width, int _height, int _x, int _y, TCHAR * _text) : WindowItem(_width, _height, _x, _y, TEXT("static"), _text)
{
}

Button::Button(int _width, int _height, int _x, int _y, TCHAR * _text):WindowItem(_width, _height, _x, _y, TEXT("button"), _text)
{
}
