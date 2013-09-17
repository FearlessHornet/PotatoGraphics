#include "stdafx.h"
#include "PotatoGraphics.h"

namespace Potato {
	MapNode::MapNode(void){
		vector<BasicObject*> empty;
		empty.push_back(NULL);
		for (int i=0; i<ENG_MAP_DEPTH; i++){
			contents.push_back(empty);
		}

		for (int i=0; i<8; i++){
			mask.push_back(NULL);
		}
	}

	void MapNode::addObject(BasicObject* obj, int level)
		/* Add an object to the node */
	{
		if (contents[level][0] == NULL) {
			contents[level].clear();
		}
		contents[level].push_back(obj);
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

}