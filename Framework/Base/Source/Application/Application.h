#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "SingletonTemplate.h"

class Application : public Singleton<Application> {
	friend class Singleton<Application>;

private:
	//Threads
	enum THREAD {
		THREAD_UPDATE_WINDOW_SIZE,
		NUM_THREAD,
	};
	
	double elapsedTime;
	double accumulatedTime[NUM_THREAD];
	
	//Declare the window width and height as constant integer
	int m_window_width;
	int m_window_height;	
	//Declare a window object
	StopWatch m_timer;
	//Check if we should quit the application
	bool quit;

	//Constructor(s) & Destructor
	Application();
	virtual ~Application();

	void ExitSystems();
	void InitScenes();
	void RunThreads();

public:
	//Interface Function(s)
	int GetWindowWidth();
	int GetWindowHeight();
	void Init();
	void Run();
	void Quit();
	void Exit();
};

#endif