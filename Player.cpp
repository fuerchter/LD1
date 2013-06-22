#include "Player.h"

Player::Player(map<string, sf::Texture> &textures, sf::Vector2u windowSize)
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

void Player::update(float dt, float y)
{
	sprite_.setPosition(position_.x, position_.y-y);
}

void Player::draw(sf::RenderWindow &window)
{
	window.draw(sprite_);
}