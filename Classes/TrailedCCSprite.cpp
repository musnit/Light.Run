#include "TrailedCCSprite.h"

using namespace cocos2d;

TrailedCCSprite::TrailedCCSprite()
{
	CCSprite::CCSprite(); 
	}
void TrailedCCSprite::createWithSpriteFrameName(const char* pszSpriteFrameName){
	colourer=1;
	CCSprite::createWithSpriteFrameName(pszSpriteFrameName);
}

void TrailedCCSprite::draw(){
	CCSprite::draw();
		}

void TrailedCCSprite::updateTrail(){
		this->removeAllChildrenWithCleanup(1);
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCSprite* head = CCSprite::create( this-> displayFrame()); 
		head->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
		this->addChild(head, 9999);
for(int i=1;i<40;i++){//needs to loop to end of screen or based on motion
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCSprite* trail = CCSprite::create( this->displayFrame()); 
		trail->setPosition(ccp(this->getContentSize().width/2-i, this->getContentSize().height/2));
		_ccColor3B color = {20*(colourer+i),10*(colourer+i),50*(colourer+i)};
		trail->setColor(color);
		this->addChild(trail, 20-i);
		if (colourer==8){
			colourer=1;
			}
		else{
				colourer++;
		}
		}



}
