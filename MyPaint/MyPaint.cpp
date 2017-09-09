#include "Resource.h"

#include "WindowItem.h"

#include "Core.h"


int CALLBACK WinMain(					//Main func
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	Core c = Core(L"My paint",1280,720);	//Creating main class object

	return c.run();						// Run main loop
}