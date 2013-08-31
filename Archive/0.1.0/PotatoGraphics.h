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
		void Animate(int increment);
	};

	class Graphics{
	public:
		vector<SDL_Surface*> pendingBG;
		vector<SDL_Surface*> pendingFG;
		vector<SDL_Surface*> pendingOBG;
		vector<SDL_Surface*> pendingOFG;

	private:
		bool kill;
		bool locked;
		bool update;
		bool running;

		Pair res;
		Uint32 lastFrame;
		Uint32 INIT_FLAGS;
		Uint32 VIDEO_FLAGS;
		vector<SDL_Surface*> backGround;
		vector<SDL_Surface*> foreGround;
		vector<SDL_Surface*> optionsBackGround;
		vector<SDL_Surface*> optionsForeGround;

	public:
		Graphics(void);
		~Graphics(void);

		bool Initialise(void);
		void Update(void);
		void Configure(map<string, vector<string>> options);

	private:
		bool Delay(void);
		void LoadConfig(void);
		void LoadAssets(void);
		void Render(void);
		void Renderloop(void);
		void Cleanup(void);

	};
}