#include "Enemy.h"

Enemy::Enemy(string name, map<string, sf::Texture> &textures, float moveSpeed, vector<sf::Vector2i> wayPoints):
moveSpeed_(moveSpeed), wayPoints_(wayPoints)
{
	sf::Texture texture;
	texture.loadFromFile(name);
	textures.insert(pair<string, sf::Texture>(name, texture));
	sprite_.setTexture(textures[name]);
	position_.x=wayPoints[0].x;
	position_.y=wayPoints[0].y;
	sprite_.setPosition(position_);
}

void Enemy::update(float dt, float y)
{
	sprite_.setPosition(position_.x, position_.y-y);
}

void Enemy::draw(sf::RenderWindow &window)
{
	window.draw(sprite_);
}