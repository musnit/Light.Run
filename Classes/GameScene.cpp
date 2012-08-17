#include "GameScene.h"
#include "Light.h"
#include "cocos2d.h"
#include "b2debugDraw.h"

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
world->SetDebugDraw(debugDraw);
uint32 flags = 0;
flags += b2Draw::e_shapeBit;
flags += b2Draw::e_jointBit;
flags += b2Draw::e_centerOfMassBit;
flags += b2Draw::e_aabbBit;
flags += b2Draw::e_pairBit;
debugDraw->SetFlags(flags);

 //END
	//Create Individual sprites for objects that will persist all game
	 winSize = CCDirector::sharedDirector()->getWinSize();
		
	//player
	_player = (Player*) GameObject::retainedObjectWithSpriteFrameName("stander.png");
	_player->getSprite()->setPosition(ccp(winSize.width * 0.1, winSize.height * 0.5));
	_batchNode->addChild(_player->getSprite(), 1);
	_player->createBox2dObject(world);
	

	//animation
	CCArray* frames = CCArray::create(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("runner1.png"),CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("runner2.png"),CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("runner3.png"),NULL);
	CCAnimation *animation = CCAnimation::create(frames,0.2f);
	_player->getSprite()->runAction(CCRepeatForever::create(CCAnimate::create(animation)));   

	//boss
	_boss = GameObject::retainedObjectWithSpriteFrameName("boss2.png");
	_boss->getSprite()->setPosition(ccp(winSize.width * 0.9, winSize.height * 0.5));
	_batchNode->addChild(_boss->getSprite(), 1);
	_boss->createBox2dObject(world);
	//Register for touches and gameloop
	this->setTouchEnabled(true) ;
	this->scheduleUpdate();
	move=true;
	spawnrate=3;

	//floor
	_floor = GameObject::retainedObjectWithSpriteFrameName("floor.png");
	_floor->getSprite()->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.1));
	_batchNode->addChild(_floor->getSprite(), 1);
	_floor->createBox2dObject(world);
	_floor->getBody()->SetType(b2_staticBody);

	return true;
}
void Game::update(float dt) {

	_player->updateTrail();
	_boss->getBody()->ApplyForce(-1 * _boss->getBody()->GetMass() * world->GetGravity(), _boss->getBody()->GetWorldCenter());

	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful
	//http://gafferongames.com/game-physics/fix-your-timestep/
 
	int32 velocityIterations = 8;
	int32 positionIterations = 1;
 
	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	world->Step(dt, velocityIterations, positionIterations);
 
 
	//Iterate over the bodies in the physics world
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != NULL) {
			//Synchronize the AtlasSprites position and rotation with the corresponding body
			GameObject *myActor = (GameObject*)b->GetUserData();
			myActor->getSprite()->setPosition(CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
			myActor->getSprite()->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}	
	}	
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
			Light* test = Light::retainedLight();
			test->setPosition(ccp(_boss->getSprite()->getPositionX()+50, _boss->getSprite()->getPositionY()));
			this->addChild(test);	
			test->runAction(CCSequence::create(CCMoveBy::create( 4 ,ccp(-winSize.width*1.5, 0)),CCCallFunc::create(test,callfunc_selector(Light::removeFromParentAndCleanup)), NULL));
			spawnrate=0;
			CCDirector::sharedDirector()->getActionManager()->pauseTarget(_boss->getSprite());
			move=false;
			CCLog("%i", this->getChildrenCount());//err:gameobjects arent autoreleased(coz it doesnt work dunno wtf- cocos releases them too early maybe?) so makesure to release when done - check/confirm memusage with this print
		}
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
