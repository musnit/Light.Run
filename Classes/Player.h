// Player.h

#ifndef PLAYER
#define PLAYER

#include "cocos2d.h"
#include "Box2D.h"
#include "GameObject.h"
 
class Player : public GameObject {
private:
	float _velocity;
	float _acceleration;
public:
	Player(float acceleration);
	void jump();
	void Accelerate();
	void SetVelocity(float velocity);
};
  
#endif