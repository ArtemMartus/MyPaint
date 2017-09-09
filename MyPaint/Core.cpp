#include "Resource.h"

#include "WindowItem.h"

#include "DrawItem.h"

#include "Core.h"

Core* core;					// Local pointer to main object so static wndproc can use it
DrawItem *buffer;			// drawitems buffer
int currentPoint;			// current offset for buffer to draw

POINT mousePos;

bool mouseButtonDown = false;

LRESULT CALLBACK _WndProc(HWND hWnd, UINT msg, WPARAM wPar, LPARAM lParam);			// wndproc
bool SaveHDC(const char* path, HDC _hdc, RECT _area, uint16_t BitsPerPixel = 24);	// save window to picture.bmp

LRESULT Core::WndProc(HWND hWnd, UINT msg, WPARAM wPar, LPARAM lParam)				// in class static wndproc to use private class properties
{
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		core->width = LOWORD(lParam);		// window resize handling
		core->height = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
	{										// start painting
		if (mouseButtonDown)				// press LMB->move->LMB_up = how to draw one picture
			break;

		mousePos.x = LOWORD(lParam);
		mousePos.y = HIWORD(lParam);


		if (!buffer[currentPoint].addPos(mousePos)) {	// if we cannot add next mouse pos to current drawitem -> move next
			currentPoint++;
			if (currentPoint == PAINT_BUFFER_SIZE)		// overflow check
				currentPoint = 0;
			buffer[currentPoint].clear();				// clear old item and fill it with new data
			buffer[currentPoint].addPos(mousePos);
		}

		try
		{
			std::wstring t = core->typeBox.getText();		// read type of objects to be painted
			if (t.compare(TEXT("line")) == 0)
				buffer[currentPoint].type = DT_LINE;
			else if (t.compare(TEXT("box")) == 0)
				buffer[currentPoint].type = DT_BOX;
			else if (t.compare(TEXT("ellipse")) == 0)
				buffer[currentPoint].type = DT_ELLIPS;
			else {
				core->typeBox.setText(TEXT("line"));
				buffer[currentPoint].type = DT_LINE;
			}
		}
		catch (std::exception& e)
		{
			core->typeBox.setText(TEXT("line"));			// wrong text in text box
			buffer[currentPoint].type = DT_LINE;
		}

		try {
			buffer[currentPoint].color.r = std::stoi(core->redBox.getText());		//get color and check whether it  0<=color<=255
			if (buffer[currentPoint].color.r > 255) {	//red
				core->redBox.setText(TEXT("255"));
				buffer[currentPoint].color.r = 255;
			}
			else if (buffer[currentPoint].color.r < 0) {
				core->redBox.setText(TEXT("0"));
				buffer[currentPoint].color.r = 0;
			}

			buffer[currentPoint].color.g = std::stoi(core->greenBox.getText());
			if (buffer[currentPoint].color.g > 255) {	//green
				core->greenBox.setText(TEXT("255"));
				buffer[currentPoint].color.g = 255;
			}
			else if (buffer[currentPoint].color.g < 0) {
				core->greenBox.setText(TEXT("0"));
				buffer[currentPoint].color.g = 0;
			}

			buffer[currentPoint].color.b = std::stoi(core->blueBox.getText());
			if (buffer[currentPoint].color.b > 255) {	//blue
				core->blueBox.setText(TEXT("255"));
				buffer[currentPoint].color.b = 255;
			}
			else if (buffer[currentPoint].color.b < 0) {
				core->blueBox.setText(TEXT("0"));
				buffer[currentPoint].color.b = 0;
			}
		}
		catch (std::exception&ia) {				
			core->redBox.setText(TEXT("0"));
			core->greenBox.setText(TEXT("0"));
			core->blueBox.setText(TEXT("0"));
			buffer[currentPoint].color.r = 0;
			buffer[currentPoint].color.g = 0;
			buffer[currentPoint].color.b = 0;
		}

		try {
			buffer[currentPoint].width = std::stoi(core->widthBox.getText());		// get 1<=(width) for pen
			if (buffer[currentPoint].width <= 0)
			{
				core->widthBox.setText(TEXT("1"));
				buffer[currentPoint].width = 1;
			}
		}
		catch (std::exception&e) {
			core->widthBox.setText(TEXT("1"));
			buffer[currentPoint].width = 1;
		}

		mouseButtonDown = true;

		break;
	}
	case WM_LBUTTONUP:
	{												//finish painting
		if (!mouseButtonDown)						// have it started??
			break;

		mousePos.x = LOWORD(lParam);
		mousePos.y = HIWORD(lParam);

		if (!buffer[currentPoint].addPos(mousePos)) {		// add second pos to draw item
			currentPoint++;
			if (currentPoint == PAINT_BUFFER_SIZE)
				currentPoint = 0;
			buffer[currentPoint].clear();
			buffer[currentPoint].addPos(mousePos);
		}

		InvalidateRect(hWnd, 0, 1);				// send WM_PAINT message

		mouseButtonDown = false;			// now we can draw again

		break;
	}
	case WM_PAINT:
	{
		core->hdc = BeginPaint(hWnd, &ps);


		core->hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));		// write at bottom right the number of draw items left until buffer overwriting 
		SelectObject(core->hdc, core->hPen);

		std::wstring greet = std::to_wstring(PAINT_BUFFER_SIZE - currentPoint);
		TextOut(core->hdc, core->width - 100, core->height - 80, greet.c_str(), greet.size());

		for (int i = 0; i < PAINT_BUFFER_SIZE; i++)		// draw items
			buffer[i].Draw(core->hdc);

		ValidateRect(hWnd, NULL);
		EndPaint(hWnd, &ps);
		
		break;
	}
	case WM_COMMAND: //handle button press
	{
		if (wPar == core->clearButton.getId())
		{
			memset(buffer, 0, sizeof(DrawItem)*PAINT_BUFFER_SIZE);		// clear buffer from any data
			InvalidateRect(core->wndDescr, 0, 1);
			currentPoint = 0;

		}

		if (wPar == core->saveButton.getId())		//save picture
		{
			HWND win = core->wndDescr;
			HDC dc = GetDC(win);
			SaveHDC("./picture.bmp", dc, {0,0,core->width,core->height});
			ReleaseDC(win, dc);

		}

		break;
	}
	}

	return DefWindowProc(hWnd, msg, wPar, lParam);
}


// Initializing class with data, creating edit boxes and buttons
Core::Core(TCHAR* _title, int _width, int _height) : width(_width), height(_height), title(_title),
		redBox(200, 30, 20, 10), greenBox(200, 30, 20, 50), blueBox(200, 30, 20, 90),
		typeBox(190, 30, width - 210, 10), widthBox(190, 30, width - 210, 50),
		clearButton(190, 30, width - 210, 90,TEXT("Clear")),saveButton(190,30,20,130,TEXT("Save pic"))
{
	core = this;		//assign pointer to this object so static func can use it
	buffer = new DrawItem[PAINT_BUFFER_SIZE];	//allocate memory for buffer

	WNDCLASSEX wcex;		//usual creation of window

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = _WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(0);
	wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = CLASS_NAME;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
		throw std::runtime_error("Cannot register class");

	 wndDescr = CreateWindow(wcex.lpszClassName,title,WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, CW_USEDEFAULT,width, height,NULL,NULL,wcex.hInstance,NULL);
	if (!wndDescr)
		throw std::runtime_error("Cannot create window");

	ShowWindow(wndDescr,SW_SHOWDEFAULT);
	UpdateWindow(wndDescr);

	// Creating sub-windows, assign names for some of them

	redBox.Create(wndDescr);
	greenBox.Create(wndDescr);
	blueBox.Create(wndDescr);
	typeBox.Create(wndDescr);
	widthBox.Create(wndDescr);
	clearButton.Create(wndDescr);
	saveButton.Create(wndDescr);

	redBox.setText(TEXT("0"));
	greenBox.setText(TEXT("0"));
	blueBox.setText(TEXT("0"));
	typeBox.setText(TEXT("line"));
	widthBox.setText(TEXT("2"));

}

//Main loop
int Core::run() 
{
	MSG msg = { 0 };
	while (msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}


Core::~Core()
{
	CloseWindow(wndDescr);
	UnregisterClass(CLASS_NAME, GetModuleHandle(0));
	delete[]buffer;
}


LRESULT CALLBACK _WndProc(HWND hWnd, UINT msg, WPARAM wPar, LPARAM lParam) { // link to same function inside class
	return Core::WndProc(hWnd, msg, wPar, lParam);
}

bool SaveHDC(const char* path, HDC _hdc, RECT _area, uint16_t BitsPerPixel) // save window func
{
	uint32_t Width = _area.right - _area.left;
	uint32_t Height = _area.bottom - _area.top;

	BITMAPINFO Info;
	BITMAPFILEHEADER Header;
	memset(&Info, 0, sizeof(Info));
	memset(&Header, 0, sizeof(Header));
	Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	Info.bmiHeader.biWidth = Width;
	Info.bmiHeader.biHeight = Height;
	Info.bmiHeader.biPlanes = 1;
	Info.bmiHeader.biBitCount = BitsPerPixel;
	Info.bmiHeader.biCompression = BI_RGB;
	Info.bmiHeader.biSizeImage = Width * Height * (BitsPerPixel > 24 ? 4 : 3);
	Header.bfType = 0x4D42;
	Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


	char* Pixels = NULL;		//get pixels into array
	HDC MemDC = CreateCompatibleDC(_hdc);
	HBITMAP Section = CreateDIBSection(_hdc, &Info, DIB_RGB_COLORS, (void**)&Pixels, 0, 0);
	DeleteObject(SelectObject(MemDC, Section));
	BitBlt(MemDC, 0, 0, Width, Height, _hdc, _area.left, _area.top, SRCCOPY);
	DeleteDC(MemDC);

	std::fstream hFile(path, std::ios::out | std::ios::binary); //and save them
	if (hFile.is_open())
	{
		hFile.write((char*)&Header, sizeof(Header));
		hFile.write((char*)&Info.bmiHeader, sizeof(Info.bmiHeader));
		hFile.write(Pixels, (((BitsPerPixel * Width + 31) & ~31) / 8) * Height);
		hFile.close();
		DeleteObject(Section);
		return true;
	}

	DeleteObject(Section);
	return false;
}