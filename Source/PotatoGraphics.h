#include "stdafx.h"

#define GRAPH_CONFIG "graphics.config"
#define ENG_BACKGROUND 0
#define ENG_FOREGROUND 1
#define ENG_MENU 2
#define ENG_MENU_BUTTONS 3

namespace Potato{
	struct RenderObject{
		int state;
		string name;
		int animTime;
		bool isSprite;
		Pair coordinate;
		vector<int> times;
		Uint32 lastUpdate;
		vector<Pair> sizes;
		SDL_Surface* sprite;
		vector<Pair> locations;


		void Animate(int increment=1); // Done
	};

	class Graphics{
	public:
		SDL_Surface* display;
		std::map<string, const RenderObject*> assets;

	private:
		bool update;
		bool kill;
		bool app;
		int period;
		bool running;
		int frameRate;

		Pair res;
		Uint32 IMG_FLAGS;
		Uint32 INIT_FLAGS;
		std::mutex locked;
		Uint32 VIDEO_FLAGS;
		std::thread* renderThread;
		vector<RenderObject> rawAssets;
		vector<vector<RenderObject*>> current;
		vector<vector<RenderObject*>> pending;

	public:
		Graphics(void);
		~Graphics(void);

		void Cleanup(void); // Done
		bool Initialise(void); // Done
		RenderObject* CreateObject(string asset);
		void Set(vector<vector<RenderObject*>> renderLists); // Done
		void Configure(map<string, vector<string>> options); // Done
		void Update(RenderObject* obj, int level=ENG_FOREGROUND); // Done
		void Update(vector<RenderObject*> list, int level=ENG_FOREGROUND); // Done

	private:
		void Delay(void); // Done
		void LoadConfig(void); // Done
		void LoadAssets(void); // Done
		void Renderloop(void); // Done
		bool Render(RenderObject *object); // Done
		map<string, vector<string>> ConfigSegments(void); // Done
	};
}