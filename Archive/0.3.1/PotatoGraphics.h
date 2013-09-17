#include "stdafx.h"

#define GRAPH_CONFIG "graphics.config"
#define ENG_BACKGROUND 0
#define ENG_FOREGROUND 1
#define ENG_MENU 2
#define ENG_MENU_BUTTONS 3

namespace Potato{
	struct RenderObject {
		int state;
		int level;
		string name;
		int animTime;
		Debug *logger;
		bool isSprite;
		Pair coordinate;
		vector<int> times;
		Uint32 lastUpdate;
		vector<Pair> sizes;
		SDL_Surface* sprite;
		vector<Pair> locations;

		void Animate(int increment=1); // Done
	};
////////////////////// Under Construction ////////////////////////

	struct BasicObject
		/* Basic object pointed to by map node */
	{
		RenderObject* sprite;
		Pair grid; // x,y index of gridbox
		Pair position; // x,y coord of position in gridbox
		Pair squareSize;
		bool collision;
	};

	struct MapNode
		/* Linked grid node */
	{
		// Define a vector node mask by index:
		// 0 - NE, 1 - N, 2 - NW
		// 3 - E,         4 - W
		// 5 - SE, 6 - S, 7 - SW
		vector<MapNode*> mask;
		vector<BasicObject*> contents;
		bool hasCollision;

		bool traversable(Pair size); // Can object of square size walk through?
		void addObject(BasicObject* obj);
	};


	class Camera{
	public:
		MapNode[][] map;

	private:

	public:
		void moveCamera(Pair position);
	};
//////////////////////////////////////////////////////////////////


	class Graphics{
	public:
		SDL_Surface* display;
		std::map<string, const RenderObject*> assets;

	private:
		Pair gridSize;
		bool app;
		bool kill;
		bool updatePending;
		bool running;
		int frameRate;
		unsigned int period;

		Pair res;
		Debug logger;
		Uint32 IMG_FLAGS;
		std::mutex locked;
		Uint32 INIT_FLAGS;
		Uint32 VIDEO_FLAGS;
		std::thread* renderThread;
		vector<RenderObject> rawAssets;
		vector<vector<RenderObject*>> current;
		vector<vector<RenderObject*>> pending;

	public:
		Graphics(void);
		~Graphics(void);

		void cleanup(void); // Done
		bool initialise(void); // Done
		RenderObject* createObject(string asset);
		void set(vector<vector<RenderObject*>> renderLists); // Done
		void configure(map<string, vector<string>> options); // Done
		void update(RenderObject* obj, int level=ENG_FOREGROUND); // Done
		void update(vector<RenderObject*> list, int level=ENG_FOREGROUND); // Done
		void update(MapNode grid, Pair camera[2], Pair size);

	private:
		void Delay(void); // Done
		void LoadConfig(void); // Done
		void LoadAssets(void); // Done
		void Renderloop(void); // Done
		bool Render(RenderObject *object); // Done
		map<string, vector<string>> ConfigSegments(void); // Done
		Pair getCoord(BasicObject *obj, Pair camera[2]);
	};
}