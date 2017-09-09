#pragma once

class Core
{
protected:
	int width;			// Window properties
	int height;
	TCHAR* title;
	HWND wndDescr;		// Window descriptor
	HPEN hPen; 
	HDC hdc;

	EditTextBox redBox;		// Window editboxes and buttons
	EditTextBox greenBox;
	EditTextBox blueBox;

	EditTextBox typeBox;
	EditTextBox widthBox;

	Button clearButton;
	Button saveButton;
	
public:

	Core(TCHAR* title,int width,int height);	
	int run();

	~Core();

	static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wPar, LPARAM lParam);
};

