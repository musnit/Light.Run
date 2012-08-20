#include "Light.h"

using namespace cocos2d;

Light::Light()
{
	CCSprite::CCSprite();

}
	Light* Light::retainedLight(){
	Light* l = new Light();
	l->setContentSize(CCSize(100,5));
	return l;
}


void Light::draw(){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	glLineWidth( 5.0f );
	glColor4ub(255.0f,0.0f,0.0f,255.0f);
	ccDrawLine( ccp(-50,0), ccp(MIN(winSize.width*0.9-getPositionX(),50),0));
	glLineWidth(1);
	glColor4ub(255,255,255,255);

}

void Light::removeFromParentAndCleanup(){
	CCSprite::removeFromParentAndCleanup(true);
}

bool Light::IsTouched(){
	return _touched;
}

void Light::IsTouched(bool touched){
	 _touched = touched;
}