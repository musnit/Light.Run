#include "Player.h"


Player::Player(float acceleration){
	GameObject::GameObject();
	this->_acceleration = acceleration;
}

void Player::jump(){
	b2Vec2 impulse = b2Vec2(0.0f, 2.5f);
	this->body->ApplyLinearImpulse(impulse, this->body->GetWorldCenter());		    
}

void Player::Accelerate(){
	this->_velocity += this->_acceleration;
}

void Player::SetVelocity(float _velocity){
	this->_velocity = _velocity;
}