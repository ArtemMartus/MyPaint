#pragma once
//Class name for main window
#define CLASS_NAME L"MyPaint core window"    
//Draw items buffer size
#define PAINT_BUFFER_SIZE 36          

//Draw item types
#define DT_LINE 0x1
#define DT_BOX 0x2
#define DT_ELLIPS 0x3

#include <windows.h>
#include <iostream>
#include <map>
#include <tchar.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <cstring>

//struct for handling colors
struct rgba
{
	int r;
	int g;
	int b;
};

