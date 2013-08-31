#include "stdafx.h"

#define GRAPH_CONFIG "graphics.config"

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

	private:
		bool update;
		bool kill;
		int period;
		bool locked;
		bool running;
		int frameRate;

		Pair res;
		Uint32 IMG_FLAGS;
		Uint32 INIT_FLAGS;
		Uint32 VIDEO_FLAGS;
		SDL_Surface* display;
		std::thread* renderThread;
		vector<RenderObject> assets;
		vector<vector<RenderObject*>> current;
		vector<vector<RenderObject*>> pending;

	public:
		Graphics(void);
		~Graphics(void);

		bool Initialise(void); // Done
		void Update(void); // Done
		void Configure(map<string, vector<string>> options); // Done
		void Cleanup(void); // Done

	private:
		void Delay(void); // Done
		void LoadConfig(void); // Done
		void LoadAssets(void); // Done
		bool Render(RenderObject *object); // Done
		void Renderloop(void); // Done
		void Update(vector<vector<RenderObject*>> renderLists); // Done
		map<string, vector<string>> ConfigSegments(void); // Done
	};
}