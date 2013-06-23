#include "Player.h"

Player::Player(map<string, sf::Texture> &textures, sf::Vector2u windowSize):
moveSpeed_(150)
{
	string name="player.png";
	sf::Texture texture;
	texture.loadFromFile(name);
	textures.insert(pair<string, sf::Texture>(name, texture));
	sprite_.setTexture(textures[name]);
	
	position_.x=windowSize.x/2;
	position_.y=windowSize.y-texture.getSize().y/2-64; //64 is the size of the ground
	sprite_.setPosition(position_);
	sprite_.setOrigin(sprite_.getGlobalBounds().width/2, sprite_.getGlobalBounds().height/2);
}

sf::FloatRect Player::getRect()
{
	return sprite_.getGlobalBounds();
}

void Player::update(float dt, float y, sf::View &view)
{
	sf::Vector2f velocity;
	if(position_.y-getRect().height/2>0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		velocity.y-=1;
	}
	if(position_.y+getRect().height/2<view.getSize().y && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		velocity.y+=1;
	}
	if(position_.x-getRect().width/2>0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		velocity.x-=1;
	}
	if(position_.x+getRect().width/2<view.getSize().x && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		velocity.x+=1;
	}
	float length=sqrt(pow(velocity.x, 2)+pow(velocity.y, 2));
	if(length!=0)
	{
		velocity/=length;
		velocity*=moveSpeed_;
		velocity*=dt;
		position_+=velocity;
	}

	sprite_.setPosition(position_.x, position_.y-y);
}

void Player::draw(sf::RenderWindow &window)
{
	window.draw(sprite_);
}