#pragma once
class WindowItem		// sub-window class  
{
protected:
	int width;
	int height;
	int x;
	int y;
	TCHAR* text;
	TCHAR* className;
	int id;
	HWND wndDescr;
	HWND parentDescr;
public:
	WindowItem();
	WindowItem(int width,int height, int x, int y,TCHAR *className,TCHAR* text);
	~WindowItem();
	int getId();
	void setText(TCHAR*newText);
	TCHAR* getText();
	void Create(HWND parent);
	void Show();
	void Hide();
};

class EditTextBox : public WindowItem
{
public:
	EditTextBox(int width, int height, int x,int y);
};

class Label : public WindowItem
{
public:
	Label(int width, int height, int x, int y,TCHAR*text);
};

class Button : public WindowItem
{
public:
	Button(int width, int height, int x, int y, TCHAR*text);
};