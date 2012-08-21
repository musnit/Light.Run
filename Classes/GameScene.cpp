#include "GameScene.h"
#include "Light.h"
#include "cocos2d.h"
#include "b2debugDraw.h"
#include "ContactListener.h"

using namespace cocos2d;

CCScene* Game::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		Game *layer = Game::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}
void Game::draw(){
	CCLayer::draw();
ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
kmGLPushMatrix();
world->DrawDebugData();
kmGLPopMatrix();

}
// on "init" you need to initialize your instance
bool Game::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	//Load Spritesheets
	_batchNode = CCSpriteBatchNode::create("runner.png");
	this->addChild(_batchNode);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("runner.plist");
 
	// Construct a world object, which will hold and simulate the rigid bodies.
b2Vec2 gravity;
gravity.Set(0.0f, -10.0f);
world = new b2World(gravity);
 world->SetContinuousPhysics(true);
 //DEBUG
b2DebugDraw *debugDraw = new b2DebugDraw(PTM_RATIO);
//world->SetDebugDraw(debugDraw);
uint32 flags = 0;
flags += b2Draw::e_shapeBit;
flags += b2Draw::e_jointBit;
flags += b2Draw::e_centerOfMassBit;
flags += b2Draw::e_aabbBit;
flags += b2Draw::e_pairBit;
debugDraw->SetFlags(flags);
 //END
//setting ContactListner
//ContactListener listener = new ContactListener();
//world->SetContactListener(listener);
//Preping Vector
platforms.reserve(10);
//prep stats
_stats =  Statistics();
	 winSize = CCDirector::sharedDirector()->getWinSize();
//THIS IS THE LABEL SPAMMAGE BEGIN DONT FRET ILL CLEAN IT UP
_multiplier = CCLabelTTF::labelWithString("_multiplier", CCSizeMake(300, 50), CCTextAlignment::kCCTextAlignmentLeft, "Arial", 9);
_score = CCLabelTTF::labelWithString("_score", CCSizeMake(300, 50), CCTextAlignment::kCCTextAlignmentLeft, "Arial", 9);
_multiplier->setPosition(ccp(winSize.width * 0.1, winSize.height * 0.5));
_multiplier->setVisible(true);
_multiplier->setColor(ccc3(0,0,0));
this->addChild(_multiplier);
	//Create Individual sprites for objects that will persist all game
	//player
	 this->_label = CCLabelTTF::labelWithString("MASSING OF TEXT ATM", "Arial", 12);
this->_label->retain();
this->_label->setColor(ccc3(234,0,12));
this->_label->setPosition(ccp(winSize.width/2, winSize.height/2));
this->addChild(_label);
_multiplier = CCLabelTTF::labelWithString("Multiplier", "Arial", 12);
_score = CCLabelTTF::labelWithString("Score", "Arial", 12);
_multiplier->setPosition(ccp(50, winSize.height-50));
_multiplier->setVisible(true);
_multiplier->setColor(ccc3(234,0,12));
this->addChild(_multiplier);
///LABEL SPAMMAGE IS NOW COMPLETE
	_player = (Player*) GameObject::retainedObjectWithSpriteFrameName("stander.png");
	_player->getSprite()->setPosition(ccp(winSize.width * 0.1, winSize.height * 0.5));
	_batchNode->addChild(_player->getSprite(), 1);
	_player->createBox2dObject(world);
	//_player->getBody()->SetLinearVelocity(b2Vec2(1.0f, 0.0f));
	//_player->getBody()->SetType(b2_kinematicBody);
///THIS IS BREAKING CODE 
	/*
	float pEyeX = 10;
	float pEyeY= 10;
	float pEyeZ= 10;
	this->m_pCamera->getEyeXYZ (&pEyeX,&pEyeY,&pEyeZ);
	*/
///JUST TRYING TO GET SOME DATA FROM THE CAMERA BUT I BELIVE IT REQUIRES SOME SORT OF INITS INITS
	//animation
	CCArray* frames = CCArray::create(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("runner1.png"),CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("runner2.png"),CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("runner3.png"),NULL);
	CCAnimation *animation = CCAnimation::create(frames,0.2f);
	_player->getSprite()->runAction(CCRepeatForever::create(CCAnimate::create(animation)));   

	//boss
	_boss = GameObject::retainedObjectWithSpriteFrameName("boss2.png");
	_boss->getSprite()->setPosition(ccp(winSize.width * 0.9, winSize.height * 0.5));
	_batchNode->addChild(_boss->getSprite(), 1);
	_boss->createBox2dObject(world);
		_boss->getBody()->SetType(b2_kinematicBody);

	//Register for touches and gameloop
	this->setTouchEnabled(true) ;
	this->scheduleUpdate();
	move=true;
	spawnrate=3;
	//get the existing filter
  b2Filter filter = _boss->getBody()->GetFixtureList()->GetFilterData();
  //make no collisions
  filter.maskBits = 0;
  //and set it back
  _boss->getBody()->GetFixtureList()->SetFilterData(filter);

	//floor
	_floor = GameObject::retainedObjectWithSpriteFrameName("floor.png");
	_floor->getSprite()->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.1));
	_batchNode->addChild(_floor->getSprite(), 1);
	_floor->createBox2dObject(world);
	_floor->getBody()->SetType(b2_staticBody);

	return true;
}
void Game::update(float dt) {
	//clean out out of bounds platforms
	int platCount = platforms.size();
	if(platCount > 0){
		GameObject *obj =  platforms.front();
		b2Vec2  pos = obj->getBody()->GetPosition();
		if(obj->isOffScreen()){
			Light* l = (Light*)obj;
			if(!l->IsTouched()){
				_stats.IncrementMultiplier(1);
				_stats.IncrementScore(10);
			}
			//obj->removeFromParentAndCleanup();
			platforms.erase(platforms.begin());
		}
	}
	_player->updateTrail(dt);
	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful
	//http://gafferongames.com/game-physics/fix-your-timestep/
 
	int32 velocityIterations = 8;
	int32 positionIterations = 1;
 
	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	world->Step(dt, velocityIterations, positionIterations);
	CleanWorld();
	if(move==false){
		spawnrate+=dt;
		if(spawnrate>=1){
			spawnrate=0.5;
			CCDirector::sharedDirector()->getActionManager()->resumeTarget(_boss->getSprite());
			move=true;
		}
	}
	else{
		spawnrate-=dt;
		if((spawnrate<=0)&&(_boss->getSprite()->getPositionY()<winSize.height*0.1+0.5*winSize.height)){//err:doesn't account for change in players height on new platform
			GameObject* test = GameObject::retainedObjectWithSprite(Light::retainedLight());
			test->getSprite()->setPosition(ccp(_boss->getSprite()->getPositionX()+50, _boss->getSprite()->getPositionY()));
			this->addChild(test->getSprite());	
			test->createBox2dObject(world);
			test->getBody()->SetLinearVelocity(b2Vec2(-5.0f, 0.0f));
			test->getBody()->SetType(b2_kinematicBody);
			//test->getSprite()->runAction(CCMoveBy::create( 4 ,ccp(-winSize.width*1.5, 0)));
			spawnrate=0;
			platforms.push_back(test);
			CCDirector::sharedDirector()->getActionManager()->pauseTarget(_boss->getSprite());
			move=false;
			CCLog("%i", _batchNode->getChildrenCount());//err:gameobjects arent autoreleased(coz it doesnt work dunno wtf- cocos releases them too early maybe?) so makesure to release when done - check/confirm memusage with this print
		}
	}
}

void Game::CleanWorld(){
		vector<GameObject*> objectsToClean(0);
	//Iterate over the bodies in the physics world
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != NULL) {
			GameObject *myActor = (GameObject*)b->GetUserData();
			if(myActor->isOffScreen()&&!myActor->canBeOffScreen()){
				//if not player sprite dont add to clean arr
				if(_player != myActor){
					objectsToClean.push_back(myActor);	
				}else{
					//place in screen again
				}
			}
			else {
			//Synchronize the AtlasSprites position and rotation with the corresponding body
				myActor->getSprite()->setPosition(CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
				myActor->getSprite()->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
			}
		}	
	}
	for(int i =0;i<objectsToClean.size();i++){
		objectsToClean.at(i)->removeFromParentAndCleanup();
	}
}
void Game::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	//if(_player->getSprite()->getPositionY()==0.1*winSize.height){//err:doesn't account for change in players height on new platform
		_player->jump();
	//}
}

void Game::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
