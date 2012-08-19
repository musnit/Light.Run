#include "GameObject.h"

using namespace cocos2d;

GameObject::GameObject()
{
		this->newtrail=0.05f;
		_ccColor3B c =  {255,0,0};
		 this->colour =c;
		 this->colourmode=0;

    CCNode::CCNode();
}
	bool GameObject::canBeOffScreen(){
	return false;
	}
	bool GameObject::isOffScreen(){
					CCSize winSize = CCDirector::sharedDirector()->getWinSize();
					if(this->sprite->getPositionX()<-50||this->sprite->getPositionX()>winSize.width+50||this->sprite->getPositionY()<-50||this->sprite->getPositionY()>winSize.height+50){
					return true;
					}
	return false;
	}

GameObject* GameObject::retainedObjectWithSpriteFrameName(const char *pszSpriteFrameName )
{
	GameObject *obj = new GameObject();
    if (obj->sprite=CCSprite::createWithSpriteFrameName(pszSpriteFrameName))
    {
		obj->newtrail=0.05f;
		_ccColor3B c =  {255,0,0};
		 obj->colour =c;
		 		 obj->colourmode=0;

		 return obj;
    }
    CC_SAFE_DELETE(obj);
	return NULL;
}

GameObject* GameObject::retainedObjectWithSpriteFrame(CCSpriteFrame *pSpriteFrame )
{
	GameObject *obj = new GameObject();
    if (obj->sprite=CCSprite::create(pSpriteFrame))
    {
		obj->newtrail=0.05f;
		_ccColor3B c =  {255,0,0};
		 obj->colour =c;
		 		 obj->colourmode=0;

		 return obj;
    }
    CC_SAFE_DELETE(obj);
	return NULL;
}

GameObject* GameObject::retainedObjectWithSprite(CCSprite *pSprite )
{
	GameObject *obj = new GameObject();
    obj->sprite=pSprite;
    obj->newtrail=0.05f;
	_ccColor3B c =  {255,0,0};
	 obj->colour =c;
	 obj->colourmode=0;
	 return obj;
    
}

	CCSprite* GameObject::getSprite(){
		return this->sprite;
	}

	b2Body* GameObject::getBody(){
		return this->body;	
	}
	_ccColor3B GameObject::nextColour(){
		if(colourmode==0){
			colour.b+=51;
			if(colour.b==255){
			colourmode=1;
			}
		}
		if(colourmode==1){
			colour.r-=51;
			if(colour.r==0){
			colourmode=2;
			}
		}
		if(colourmode==2){
			colour.g+=51;
			if(colour.g==255){
			colourmode=3;
			}
		}
		if(colourmode==3){
			colour.b-=51;
			if(colour.b==0){
			colourmode=4;
			}
		}
		if(colourmode==4){
			colour.r+=51;
			if(colour.r==255){
			colourmode=5;
			}
		}	
		if(colourmode==5){
			colour.g-=51;
			if(colour.g==0){
			colourmode=0;
			}
		}	
		return this->colour;
	}

	void GameObject::createBox2dObject(b2World* world) {
    b2BodyDef playerBodyDef;
	playerBodyDef.type = b2_dynamicBody;
	playerBodyDef.position.Set(this->sprite->getPosition().x/PTM_RATIO, this->sprite->getPosition().y/PTM_RATIO);
	playerBodyDef.userData = this;
	playerBodyDef.fixedRotation = true;
 
	this->body = world->CreateBody(&playerBodyDef);
 
	   b2PolygonShape dynamicBox;
	   dynamicBox.SetAsBox((this->sprite->getContentSize().width/PTM_RATIO)/2, (this->sprite->getContentSize().height/PTM_RATIO)/2);//These are mid points for our 1m box err:if object is scaled/rotated this will bug out

//	b2CircleShape circleShape;
	//circleShape.m_radius = 0.7f; 
	   //err:change collision&other paramaters
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution =  0.0f;
	this->body->CreateFixture(&fixtureDef);
}

	void GameObject::updateTrail(float dt){
		this->newtrail-=dt;
		if(this->newtrail<=0){
			this->newtrail=0.05f;
		GameObject* trail = GameObject::retainedObjectWithSpriteFrame(this->sprite->displayFrame()); 
		trail->sprite->setPosition(ccp(this->sprite->getPositionX(), this->sprite->getPositionY()));
		trail->createBox2dObject(this->body->GetWorld());
		trail->body->ApplyLinearImpulse(b2Vec2(-0.5f, 0.0f) ,this->body->GetWorldCenter());
		this->sprite->getParent()->addChild(trail->getSprite());
		trail->body->SetType(b2_kinematicBody);
		//get the existing filter
  b2Filter filter = trail->body->GetFixtureList()->GetFilterData();
  //make no collisions
  filter.maskBits = 0;
  //and set it back
  trail->body->GetFixtureList()->SetFilterData(filter);

		trail->sprite->setColor(nextColour());
		}

	

}
 
	void GameObject::removeFromParentAndCleanup(){
	   this->body->GetWorld()->DestroyBody( this->body );
	   this->sprite->removeFromParentAndCleanup(true);
	  // CCNode::removeFromParentAndCleanup(true);
}