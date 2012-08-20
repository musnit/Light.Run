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
	bool IsTouched();
	void IsTouched(bool touched);
protected:
	bool _touched;
};
#endif