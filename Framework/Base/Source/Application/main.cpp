#include "Application.h"
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

void RunApplication() {
	Application* app = &Application::GetInstance();
	app->Init();
	app->Run();
	app->Exit();
	app->Destroy();
}

void main(void) {
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	RunApplication();
	_CrtDumpMemoryLeaks();
}