// PotatoGraphics.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "PotatoGraphics.h"

namespace Potato{

	void Graphics::Delay(void)
		/* Waits until the the next tick */ 
	{
		Uint32 startTick, thisTick;
		bool wait = true;
		float timeElapsed;
		int ticks;
		
		ticks = SDL_GetTicks() / period;
		// Number of ticks past

		while (thisTick < period)
		{
			thisTick = SDL_GetTicks() % ticks;
			// Get time (ms) since the tick "ticks"
		}
	}

	map<string, vector<string>> Graphics::ConfigSegments(void)
		/* Parses the config file */
	{
		std::ifstream configFile;
		string data;
		vector<string>* blocks;
		map<string, vector<string>> segments;
		vector<string>* FLAGS;
		std::stringstream buffer;
		vector<string>* temp;

		configFile.open("graphics.config");
		buffer << configFile.rdbuf;
		data = buffer.str();
		configFile.close();
		blocks = Strings::Split(data,"[");
		for (auto i : *blocks)
		{
			temp = Strings::Split(i,"]",true);
			FLAGS = Strings::Split((*temp)[1]);
			segments[(*temp)[0]] = *FLAGS;
		}

		return segments;
	}

	void RenderObject::Animate(int increment)
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
		map<string, vector<string>> config;
		map<string, Uint32> optionFlags;
		vector<string> buffer;

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

		config = ConfigSegments();

		for (auto j : config)
		{
			if (j.first == "INITIALISE")
			{
				for (auto i : j.second)
				{
					INIT_FLAGS = INIT_FLAGS | optionFlags[i];
					// Edge case, will the OR ADD work when INIT_FLAGS isn't initialised with NULL?
				}
			}
			else if (j.first == "VIDEO")
			{
				for (auto i : j.second)
				{
					VIDEO_FLAGS = VIDEO_FLAGS | optionFlags[i];
					// Edge case as above
				}
			}
			else if (j.first == "MISC")
			{
				for (auto i : j.second)
				{
					buffer = *Strings::Split(i, "=");
					if (buffer[0] == "FrameRate")
					{
						frameRate = atoi(buffer[1].c_str());
						period = (int)((1.0/frameRate)*1000);
					}
					else if (buffer[0] == "Resolution")
					{
						buffer = *Strings::Split(buffer[1],"x");
						res.x = atoi(buffer[0].c_str());
						res.y = atoi(buffer[1].c_str());
					}
				}
			}
			
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

	void Graphics::Configure(map<string, vector<string>> options)
		/* Allows engine to be configured, new config is save to config file */
	{
		map<string, vector<string>> config;
		std::ofstream configFile;
		string segment;

		config = ConfigSegments();
		for (auto i : options)
		{
			config[i.first] = i.second;
		}

		configFile.open("graphics.config");
		for (auto j : options)
		{
			segment = "[";
			segment += j.first;
			segment += "]\n";
			for (auto k : j.second)
			{
				segment += k;
				segment += "\n";
			}
			configFile.write(segment.c_str(), segment.size());
		}

		configFile.close();
		LoadConfig();
	}

	void Graphics::Update(vector<vector<RenderObject*>> renderLists)
		/* Updates the render vectors */
	{
		while (update && locked)
		{
			Delay();
		}
		locked = true;
		pending = renderLists;
		update = true;
		locked = false;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

