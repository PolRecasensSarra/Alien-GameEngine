#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "Color.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();
	void LoadConfig(JSONfilepack*& config);
	void SaveConfig(JSONfilepack*& config);
	void SetTitle(const char* title);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	int width = 0;
	int height = 0;
	float brightness = 0.0f;
	bool fullscreen = false;
	bool full_desktop = false;
	bool borderless = false;
	bool resizable = false;
	int style = 0;
	char* window_name = "";
	char* organitzation_name = "";

};

#endif // __ModuleWindow_H__