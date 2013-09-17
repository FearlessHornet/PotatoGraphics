// PotatoGraphics.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "PotatoGraphics.h"

void LogVectors(vector<vector<Potato::RenderObject*>> lists, Potato::Debug logger)
{
	for (auto list : lists){
		logger.LogMessage("LIST:");
		for (auto obj : list){
			logger.LogMessage(obj->name);
		}
	}
}

namespace Potato{

	Graphics::Graphics(void)
		/* Constructor (default values) */
	{
		display = NULL;
		frameRate = 60;
		updatePending = false;
		kill = false;
		period = (int)((1.0/frameRate)*1000);
		res.x=1920;
		res.y=1080;
		running = false;
		vector<RenderObject*> empty;
		for (int i=0; i<=4; i++){
			current.push_back(empty);
			pending.push_back(empty);
		}
	}

	Graphics::~Graphics(void)
		/* Destructor */
	{
		cleanup();
	}

	void Graphics::Delay(void)
		/* Waits until the the next tick */ 
	{
		Uint32 thisTick;
		bool wait = true;
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
		
		logger.LogMessage("LOADING CONFIG FILE...");
		data = Strings::ReadFile(GRAPH_CONFIG);
		logger.LogMessage("SPLITTING DATA...");
		blocks = Strings::Split(data,"[");
		logger.LogMessage("CHECKING SEGMENTS");
		for (auto i : *blocks)
		{
			temp = Strings::Split(i,"]",true);
			FLAGS = Strings::Split((*temp)[1]);
			logger.LogMessage("APPLYING SEGMENT...");
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
				logger->LogMessage("ANIMATING...");
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

		logger.LogMessage("CREATING FLAG TABLE...");
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

		logger.LogMessage("LOADING CONFIG SEGMENTS...");
		config = ConfigSegments();
		logger.LogMessage("APPLYING FLAGS...");

		for (auto j : config)
		{

			if (j.first == "INITIALISE")
				// SDL flags for SDL_Init
			{
				logger.LogMessage("BASIC SEGMENTS...");
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
				logger.LogMessage("VIDEO SEGMENTS...");
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
				logger.LogMessage("IMAGE SEGMENTS...");
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
				logger.LogMessage("MISC SEGMENTS...");
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

	bool Graphics::initialise(void)
		/* Initialises the graphics engine with the configuration in graphics.config */	
	{
		logger = Debug(GRAPH_CONFIG);
		logger.LogMessage("INITIALISING...");

		if (running) {
			logger.LogMessage("KILLING RENDER THREAD...");
			locked.lock();
			kill = true;
			locked.unlock();
		}

		logger.LogMessage("LOADING CONFIG...");
		IMG_FLAGS = 0xFFFFFFFF;
		INIT_FLAGS = 0xFFFFFFFF;
		VIDEO_FLAGS = 0xFFFFFFFF;
		LoadConfig();
		if (SDL_Init(INIT_FLAGS) == -1)
		{
			logger.LogMessage("SDL FAILED TO INITIALIZE!");
			logger.LogMessage(INIT_FLAGS);
			return false;
		}

		logger.LogMessage("CONFIGURING SDL...");
		IMG_Init(IMG_FLAGS);
		LoadAssets();
		display = SDL_SetVideoMode(res.x, res.y, 0, VIDEO_FLAGS);
		if (display == NULL) {
			logger.LogMessage("FAILED TO SET VIDEO MODE!");
			logger.LogMessage(VIDEO_FLAGS);
			return false;
		}
		logger.LogMessage("STARTING RENDER THREAD...");
		renderThread = new std::thread(&Potato::Graphics::Renderloop, this);
		renderThread->detach();
		logger.LogMessage("INITIALIZED!");
		return true;
	}

	void Graphics::configure(map<string, vector<string>> options)
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
		for (auto newOptions : options)
		{
			segment = "[";
			segment += newOptions.first;
			segment += "]\n";
			for (auto flag : newOptions.second)
			{
				segment += flag;
				segment += "\n";
			}
			configFile.write(segment.c_str(), segment.size());
		}

		configFile.close();
		LoadConfig();
	}

	void Graphics::update(vector<RenderObject*> list, int level)
		/* Update the render vectors */
	{
		while (updatePending || !locked.try_lock())
		{
			Delay();
		}
		logger.LogMessage("UPDATING...");
		app = true;

		pending[level] = list;
		updatePending = true;
		locked.unlock();
	}

	void Graphics::update(RenderObject* obj, int level)
	{
		vector<RenderObject*> single;
		single.push_back(obj);
		update(single, level);
	}

	Pair Graphics::getCoord(BasicObject* obj, Pair cameraGrid, Pair cameraPos)
	{
		return (obj->grid - cameraGrid)*gridSize + obj->position - cameraPos;
	}

	void Graphics::update(MapNode grid, Pair camera[2], Pair size)
		/* Update render lists according to grid */
	{
		// Collect everything into vector first to reduce update hogging
		vector<RenderObject*> sprites; 

		for (auto obj : grid.contents)
		{
			obj->sprite->coordinate = getCoord(obj, camera[0], camera[1]);
		}
	}

	void Graphics::set(vector<vector<RenderObject*>> renderLists)
		/* Set the render vectors */
	{
		while (updatePending || !locked.try_lock())
		{
			Delay();
		}
		logger.LogMessage("SETTING...");
		pending = renderLists;
		updatePending = true;
		locked.unlock();
	}

	bool Graphics::Render(RenderObject *object)
		/* This method renders an object on the screen with the given parameters */
	{
		SDL_Rect output;
		SDL_Rect input;

		if (object == NULL){ //End of list
			return false;
		}

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
		running = true;
		while (!kill)
		{
			if (updatePending && locked.try_lock())
			{
				logger.LogMessage("RENDER THREAD: UPDATING...");
				if (!app)
				{
					current.clear();
				}
				app = false;
				for (int i=0; i<4; i++){
					current[i].insert(current[i].end(), pending[i].begin(), pending[i].end());
				}
				updatePending = false;
				locked.unlock();
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
			if ((startTick - endTick) != 0) {
				Delay();
			}
		}
		running = false;
	}

	void Graphics::LoadAssets(void)
		/* This method loads all art assets */
	{
		vector<string> info, animation;
		RenderObject* object;
		string asset;
		Pair pair;

		logger.LogMessage("LOADING ASSETS...");

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
			object->state=0;
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
			// Add to rawAssets pool and reset the dummy object pointer
			rawAssets.push_back(*object);
			assets[object->name] = &rawAssets.back();
			object = NULL;
		}
	}

	RenderObject* Graphics::createObject(string asset) {
		RenderObject *newObj;
		if (assets.find(asset) != assets.end()) {
			newObj = new RenderObject;
			*newObj = *assets[asset];
			return newObj;
		}
		return NULL;
	}

	void Graphics::cleanup(void)
		/* Cleans & kills the graphics engine and its processes */
	{
		logger.LogMessage("KILLING CHILD...");
		kill = true;
		while (running) { // If render thread is unresponsive this will never terminate
			Delay();
		}
		kill = false;

		logger.LogMessage("CLEANING...");
		for (auto object : rawAssets)
		{
			SDL_FreeSurface(object.sprite);
		}

		SDL_FreeSurface(display);

		IMG_Quit();
		SDL_Quit();

		logger.LogMessage("PURGING VECTORS...");

		rawAssets.clear();
		current.clear();
		pending.clear();

		logger.LogMessage("RESETTING VARIABLES...");

		display = NULL;
		frameRate = 60;
		updatePending = false;
		period = (int)((1.0/frameRate)*1000);
		res.x=1920;
		res.y=1080;

		IMG_FLAGS = 0x00000000;
		INIT_FLAGS = 0x00000000;
		VIDEO_FLAGS = 0x00000000;

		logger.LogMessage("CLEANED!");
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	Potato::Graphics engine;
	return 0;
}

