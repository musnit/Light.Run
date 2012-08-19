#ifndef LIGHT
#define LIGHT

#include "GameObject.h"
using namespace cocos2d;

class Light : public CCSprite
{
public:
	Light();
	float width;
	static Light* retainedLight();
	virtual void draw();
	virtual void removeFromParentAndCleanup();
};
#endif