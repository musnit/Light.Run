#include "Player.h"


Player::Player(void)
{
	GameObject::GameObject();
}


void Player::jump(){
    b2Vec2 impulse = b2Vec2(0.0f, 2.5f);
    this->body->ApplyLinearImpulse(impulse, this->body->GetWorldCenter());		    
}