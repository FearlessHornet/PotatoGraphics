// PotatoGraphics.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "PotatoGraphics.h"

namespace Potato{
	void RenderObject::Animate(int increment=1)
		/* Checks is the last animation has played and increments accoringly */
	{

		Uint32 thisTick;
		int timeElapsed, net;
		if (isSprite)
		{
			thisTick = SDL_GetTicks();
			timeElapsed = thisTick - lastUpdate;
			if (timeElapsed >= animTime)
			{
				net = state + increment;
				state = (net % surfaces.size()) + 1;
				sprite = surfaces[state-1];
				animTime = times[state-1];
				lastUpdate = thisTick;
			}
		}

	}


	void Graphics::LoadConfig(void)
		/* Loads configuration from graphics.config file */
	{
		std::ifstream configFile;
		std::stringstream buffer;
		string data;
		vector<string>* temp;
		vector<string>* segments;
		vector<string>* FLAGS;
		map<string, Uint32> optionFlags;

		optionFlags["EVERYTHING"] = SDL_INIT_EVERYTHING;
		optionFlags["AUDIO"] = SDL_INIT_AUDIO;
		optionFlags["VIDEO"] = SDL_INIT_VIDEO;
		optionFlags["CDROM"] = SDL_INIT_CDROM;
		optionFlags["JOYSTICK"] = SDL_INIT_JOYSTICK;
		optionFlags["NOPARACHUTE"] = SDL_INIT_NOPARACHUTE;
		optionFlags["EVENTTHREAD"] = SDL_INIT_EVENTTHREAD;
		optionFlags["FULLSCREEN"] = SDL_FULLSCREEN;
		optionFlags["HWSURFACE"] = SDL_HWSURFACE;
		optionFlags["ASYNCBLIT"] = SDL_ASYNCBLIT;
		optionFlags["HWPALETTE"] = SDL_HWPALETTE;
		optionFlags["DOUBLEBUF"] = SDL_DOUBLEBUF;
		optionFlags["SWSURFACE"] = SDL_SWSURFACE;
		optionFlags["RESIZEABLE"] = SDL_RESIZABLE;
		optionFlags["NOFRAME"] = SDL_NOFRAME;

		configFile.open("graphics.config");
		buffer << configFile.rdbuf;
		data = buffer.str();
		configFile.close();
		segments = Strings::Split(data,"[", false);
		for (auto i : *segments)
		{
			temp = Strings::Split(i,"]",true);

		}
	}


	bool Graphics::Initialise(void)
		/* Initialises the graphics engine with the configuration in graphics.config */	
	{
		while (running){
			// Possible freeze spot if renderloop stops responding
			if (!locked && !kill)
			{
				locked = true;
				kill = true;
				locked = false;
			}
			Delay();
		}

		LoadConfig();
		SDL_Init(INIT_FLAGS);
		SDL_SetVideoMode(res.x, res.y, 0, VIDEO_FLAGS);
		LoadAssets();

		// Need to start renderloop thread
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

