#include "stdafx.h"

#define GRAPH_CONFIG "graphics.config"
#define ENG_BACKGROUND 0
#define ENG_FOREGROUND 1
#define ENG_ACTIVE 2
#define ENG_MAP_DEPTH 3

namespace Potato{
	struct RenderObject {
		int state;
		string name;
		int animTime;
		Debug *logger;
		bool isSprite;
		vector<int> times;
		Uint32 lastUpdate;
		vector<Pair> sizes;
		SDL_Surface* sprite;
		vector<Pair> locations;

		void Animate(int increment=1); // Done
	};

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
		// 0 - NW, 1 - N, 2 - NE
		// 3 - W,         4 - E
		// 5 - SW, 6 - S, 7 - SE
		vector<MapNode*> mask;
		vector<vector<BasicObject*>> contents;
		bool hasCollision;
		Pair coordinate;

		MapNode(void);
		bool traversable(Pair size); // Can object of square size walk through?
		void addObject(BasicObject* obj, int level=ENG_FOREGROUND);
		void removeObject(BasicObject* obj, int level=ENG_FOREGROUND);
	};


	class Graphics{
	public:
		SDL_Surface* display;
		std::map<string, const RenderObject*> assets;
		Pair camera;
		Pair cameraSize; // Pixel size of the camera

	private:
		MapNode* gameMap;
		Pair gridSize; // Pixel size of each grid cell
		bool app;
		bool kill;
		bool updatePending;
		bool running;
		int frameRate;
		unsigned int period;

		Pair res;
		Debug logger;
		Uint32 IMG_FLAGS;
		Uint32 INIT_FLAGS;
		std::mutex padlock;
		Uint32 VIDEO_FLAGS;
		std::thread* renderThread;
		vector<RenderObject> rawAssets;

	public:
		Graphics(void);
		~Graphics(void);

		void cleanup(void); // Done
		bool initialise(void); // Done
		RenderObject* createObject(string asset);
		//void set(vector<vector<RenderObject*>> renderLists); // Removed
		void configure(map<string, vector<string>> options); // Done
		void renderMap(MapNode* grid);
		void renderRow(MapNode* row);

	private:
		void delay(void); // Done
		void loadConfig(void); // Done
		void loadAssets(void); // Done
		void renderloop(void); // Done
		map<string, vector<string>> configSegments(void); // Done

		bool renderObj(RenderObject *object, Pair coordinate);
		void renderRow(MapNode* row, int width);
		void renderMap(MapNode* grid, int depth);
		MapNode* getNode(MapNode* grid, Pair location);
		Pair getCameraGridSize(void);
		Pair getCameraGrid(void);
		void update(BasicObject* obj, int level=ENG_FOREGROUND);
	};
}