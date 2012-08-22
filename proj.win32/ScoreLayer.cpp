#include "ScoreLayer.h"

using namespace cocos2d;

bool ScoreLayer::init(){
	winSize = CCDirector::sharedDirector()->getWinSize();
//THIS IS THE LABEL SPAMMAGE BEGIN DONT FRET ILL CLEAN IT UP
//Create Individual sprites for objects that will persist all game
//player
this->_label = CCLabelTTF::labelWithString("MASSING OF TEXT ATM", "Arial", 12);
this->_label->retain();
this->_label->setColor(ccc3(234,0,12));
this->_label->setPosition(ccp(winSize.width/2, winSize.height/2));
this->addChild(_label);
_multiplier = CCLabelTTF::labelWithString("Multiplier", "Arial", 12);
_score = CCLabelTTF::labelWithString("Score", "Arial", 12);
_multiplier->setPosition(ccp(100, winSize.height-100));
_multiplier->setVisible(true);
_multiplier->setColor(ccc3(234,0,12));
this->addChild(_multiplier);
///LABEL SPAMMAGE IS NOW COMPLETE
return true;
}

 void ScoreLayer::draw(){

 }

void ScoreLayer::update(float dt){

}