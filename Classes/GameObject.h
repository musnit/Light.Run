// GameObject.h
#ifndef GAMEOBJECT
#define GAMEOBJECT

#include "cocos2d.h"
#include "box2d.h"
 using namespace cocos2d;
 #define PTM_RATIO 32

class GameObject : public CCNode {
public:
	CCSprite* getSprite();

	b2Body* getBody();
	void createBox2dObject(b2World* world);
	//used so i can easily identify a gameObject in the world
	string id;
	GameObject();
	static GameObject* retainedObjectWithSpriteFrameName(const char *pszSpriteFrameName);
	static GameObject* retainedObjectWithSpriteFrame(CCSpriteFrame *pSpriteFrame );
	static GameObject* retainedObjectWithSprite(CCSprite *pSprite );
	void updateTrail(float dt);
	bool canBeOffScreen();
	bool isOffScreen();
		virtual void removeFromParentAndCleanup();

private:
	int colourmode;
		_ccColor3B colour;
		float newtrail;
		_ccColor3B nextColour();
protected:
	    b2Body *body;
		CCSprite* sprite;
};
#endif