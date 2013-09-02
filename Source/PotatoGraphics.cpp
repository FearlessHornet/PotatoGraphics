// PotatoGraphics.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "PotatoGraphics.h"
#include <fstream>


std::ofstream logFile("graphics.log");

void LogMessage(std::string message)
{
	logFile << message << std::endl;
}

void LogMessage(int val)
{
	logFile << val << std::endl;
}

vector<vector<Potato::RenderObject*>> makeTestObject(Potato::RenderObject* obj) {
	vector<Potato::RenderObject*> list;
	list.push_back(obj);
	vector<vector<Potato::RenderObject*>> list2;
	list2.push_back(list);
	return list2;
}

namespace Potato{

	Graphics::Graphics(void)
		/* Constructor */
	{
		display = NULL;
		frameRate = 60;
		update = false;
		kill = false;
		locked = false;
		period = (int)((1.0/frameRate)*1000);
		res.x=1920;
		res.y=1080;
		running = false;
	}

	Graphics::~Graphics(void)
		/* Destructor */
	{
		Cleanup();
	}

	void Graphics::Delay(void)
		/* Waits until the the next tick */ 
	{
		Uint32 startTick, thisTick;
		bool wait = true;
		float timeElapsed;
		int ticks;

		period = (int)((1.0/frameRate)*1000);
		ticks = SDL_GetTicks() / period;
		// Number of ticks past
		thisTick = 0;

		while (thisTick < period)
		{
			thisTick = SDL_GetTicks() - (ticks * period); 
			// Get time (ms) since the last tick
		}
	}

	map<string, vector<string>> Graphics::ConfigSegments(void)
		/* Parses the config file */
	{
		string data;
		vector<string>* blocks;
		map<string, vector<string>> segments;
		vector<string>* FLAGS;
		vector<string>* temp;
		
		LogMessage("LOADING CONFIG FILE...");
		data = Strings::ReadFile(GRAPH_CONFIG);
		LogMessage("SPLITTING DATA...");
		blocks = Strings::Split(data,"[");
		LogMessage("CHECKING SEGMENTS");
		for (auto i : *blocks)
		{
			temp = Strings::Split(i,"]",true);
			FLAGS = Strings::Split((*temp)[1]);
			LogMessage("APPLYING SEGMENT...");
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
				LogMessage("ANIMATING...");
				net = state + increment;
				state = (net % times.size()); // Possible skipping edge case
				animTime = times[state];
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

		LogMessage("CREATING FLAG TABLE...");
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
		optionFlags["JPG"] = IMG_INIT_JPG;
		optionFlags["PNG"] = IMG_INIT_PNG;
		optionFlags["TIF"] = IMG_INIT_TIF;

		LogMessage("LOADING CONFIG SEGMENTS...");
		config = ConfigSegments();
		LogMessage("APPLYING FLAGS...");

		for (auto j : config)
		{

			if (j.first == "INITIALISE")
				// SDL flags for SDL_Init
			{
				LogMessage("BASIC SEGMENTS...");
				for (auto i : j.second)
				{
					if (INIT_FLAGS == 0xFFFFFFFF)
					{
						INIT_FLAGS = optionFlags[i];
					}
					else
					{
						INIT_FLAGS = INIT_FLAGS | optionFlags[i];
					}
				}
			}
			
			else if (j.first == "VIDEO")
				// SDL flags for SDL_SetVideoMode
			{
				LogMessage("VIDEO SEGMENTS...");
				for (auto i : j.second)
				{
					if (VIDEO_FLAGS == 0xFFFFFFFF)
					{
						VIDEO_FLAGS = optionFlags[i];
					}
					else
					{
						VIDEO_FLAGS = VIDEO_FLAGS | optionFlags[i];
					}
				}
			}

			else if (j.first == "IMGLOADER")
				// SDL_image flags
			{
				LogMessage("IMAGE SEGMENTS...");
				for (auto i : j.second)
				{
					if (IMG_FLAGS == 0xFFFFFFFF)
					{
						IMG_FLAGS = optionFlags[i];
					}
					else
					{
						IMG_FLAGS = IMG_FLAGS | optionFlags[i];
					}
				}
			}

			else if (j.first == "MISC")
			{
				LogMessage("MISC SEGMENTS...");
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
		LogMessage("INITIALISING...");

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

		LogMessage("LOADING CONFIG...");
		IMG_FLAGS = 0xFFFFFFFF;
		INIT_FLAGS = 0xFFFFFFFF;
		VIDEO_FLAGS = 0xFFFFFFFF;
		LoadConfig();
		if (SDL_Init(INIT_FLAGS) == -1)
		{
			LogMessage("SDL FAILED TO INITIALIZE!");
			LogMessage(INIT_FLAGS);
			return false;
		}

		LogMessage("CONFIGURING SDL...");
		IMG_Init(IMG_FLAGS);
		LoadAssets();
		display = SDL_SetVideoMode(res.x, res.y, 0, VIDEO_FLAGS);
		if (display == NULL) {
			LogMessage("FAILED TO SET VIDEO MODE!");
			LogMessage(VIDEO_FLAGS);
			return false;
		}
		LogMessage("STARTING RENDER THREAD...");
		renderThread = new std::thread(&Potato::Graphics::Renderloop, this);
		renderThread->detach();
		LogMessage("INITIALIZED!");
		return true;
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
		/* Update the render vectors */
	{
		int i=0;
		while (update && locked)
		{
			Delay();
		}
		locked = true;
		pending = current;
		for (auto list : renderLists){
			if (i < pending.size()){
				pending[i].insert(pending[i].end(), list.begin(), list.end());
				i++;
			}
			else {
				pending.push_back(list);
			}
		}
		update = true;
		locked = false;
	}

	void Graphics::Set(vector<vector<RenderObject*>> renderLists)
		/* Set the render vectors */
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

	bool Graphics::Render(RenderObject *object)
		/* This method renders an object on the screen with the given parameters */
	{
		SDL_Rect output;
		SDL_Rect input;

		object->Animate(); // Check if the objects sprite is up-to-date

		if (object->sprite == NULL)
		{
			return false;
		}

		output.x = object->coordinate.x;
		output.y = object->coordinate.y;

		input.x = object->locations[object->state].x;
		input.y = object->locations[object->state].y;
		input.w = object->sizes[object->state].x;
		input.h = object->sizes[object->state].y;

		SDL_BlitSurface(object->sprite, &input, display, &output);

		return true;
	}

	void Graphics::Renderloop(void)
		/* This is a constant loop run by a child thread, renders everything in
		the render lists */
	{
		Uint32 startTick, endTick;
		while (!kill)
		{
			if (update && !locked)
			{
				LogMessage("RENDER THREAD: UPDATING...");
				locked = true;
				current.clear();
				current = pending;
				locked = false;
				update = false;
			}

			startTick = (int)(SDL_GetTicks()/period);

			for (auto renderlist : current)
			{
				for (auto object : renderlist)
				{
					Render(object); // Possible DLL error?
				}
			}
			SDL_Flip(display);
			endTick = (int)(SDL_GetTicks()/period);
			if ((startTick - endTick) == 0) {
				Delay();
			}
		}
	}

	void Graphics::LoadAssets(void)
		/* This method loads all art assets */
	{
		vector<string> info, animation;
		RenderObject* object;
		SDL_Rect* area;
		string asset;
		Pair pair;
		SDL_Surface* temp;

		LogMessage("LOADING ASSETS...");

		for (auto line : *Strings::GetLines("manifest"))
			/* Each Asset */
		{

			// Create a dummy object
			object = new RenderObject;

			// Get the all data and extract the basic data (first two elements)
			info = *Strings::Split(line, " ");
			object->name = info[0];
			info.erase(info.begin());
			asset = info[0];
			info.erase(info.begin());

			// If there's only one animation state set isSprite to false else isSprite is true
			object->isSprite = !(info.size() == 1);

			// Default data
			object->state=1;
			object->coordinate.x=0;
			object->coordinate.y=0;
			object->lastUpdate=0;

			// Loading the SDL_Surface
			object->sprite = IMG_Load(asset.c_str());

			for (auto anim : info)
				/* Each animation state */
			{

				// Parsing <x>,<y>,<w>,<h>
				animation = *Strings::Split(anim,",");
				pair.x = atoi(animation[0].c_str());
				pair.y = atoi(animation[1].c_str());
				object->locations.push_back(pair);
				pair.x = atoi(animation[2].c_str());
				pair.y = atoi(animation[3].c_str());
				object->sizes.push_back(pair);

				// <TIME>
				object->times.push_back(atoi(animation[4].c_str()));

			}

			object->animTime = object->times[0];
			// Add to assets pool and reset the dummy object pointer
			assets.push_back(*object);
			object = NULL;
		}
	}

	void Graphics::Cleanup(void)
		/* Cleans & kills the graphics engine and its processes */
	{
		LogMessage("KILLING CHILD...");
		kill = true;
		Delay();
		Delay();
		kill = false;

		LogMessage("CLEANING...");
		for (auto object : assets)
		{
			SDL_FreeSurface(object.sprite);
		}

		SDL_FreeSurface(display);

		IMG_Quit();
		SDL_Quit();

		LogMessage("PURGING VECTORS...");

		assets.clear();
		current.clear();
		pending.clear();

		LogMessage("RESETTING VARIABLES...");

		display = NULL;
		frameRate = 60;
		update = false;
		locked = false;
		period = (int)((1.0/frameRate)*1000);
		res.x=1920;
		res.y=1080;

		IMG_FLAGS = 0x00000000;
		INIT_FLAGS = 0x00000000;
		VIDEO_FLAGS = 0x00000000;

		LogMessage("CLEANED!");
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	Potato::Graphics engine;
	if (engine.Initialise()) {
		Potato::RenderObject bg, test;
		//bg = engine.assets[0];
		test = engine.assets[1];
		//engine.Update(makeTestObject(&bg));
		engine.Update(makeTestObject(&test));
		Sleep(4000);
		test.coordinate.x = 150;
		test.coordinate.y = 150;
		//engine.Update(makeTestObject())
		Sleep(6000);
	}
	engine.Cleanup();
	logFile.close();
	return 0;
}

