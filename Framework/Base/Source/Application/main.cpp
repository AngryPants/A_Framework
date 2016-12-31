#include "Application.h"
#include <stdlib.h>
#include <crtdbg.h>
#include <exception>
#include <iostream>

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

	try {
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
		RunApplication();
		_CrtDumpMemoryLeaks();

	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}

	//system("pause");
	
}