#include "stdafx.h"

namespace Potato{
	struct RenderObject{
		int animTime;
		SDL_Surface* sprite;
		int state;
		vector<SDL_Surface*> surfaces;
		vector<int> times;
		Uint32 lastUpdate;
		bool isSprite;


		RenderObject(void);
		~RenderObject(void);
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
		Uint32 INIT_FLAGS;
		Uint32 VIDEO_FLAGS;
		vector<SDL_Surface*> assets;
		vector<vector<RenderObject*>> current;
		vector<vector<RenderObject*>> pending;

	public:
		Graphics(void);
		~Graphics(void);

		bool Initialise(void); // In progress, thread to start Renderloop missing
		void Update(void); // Done
		void Configure(map<string, vector<string>> options); // Done

	private:
		void Delay(void); // Done
		void LoadConfig(void); // Done
		void LoadAssets(void);
		void Render(void);
		void Renderloop(void);
		void Cleanup(void);
		void Update(vector<vector<RenderObject*>> renderLists); // Done
		map<string, vector<string>> ConfigSegments(void); // Done
	};
}