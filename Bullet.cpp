#include "Bullet.h"

Bullet::Bullet(map<string, sf::Texture> &textures, sf::Vector2f velocity, sf::Vector2i size, float time):
time(time), velocity_(velocity)
{
	sprite_.setTexture(textures["bullet.png"]);
	sprite_.setScale(size.x, size.y);
	sprite_.setColor(sf::Color(255, 0, 0, 255));
}

void Bullet::setPosition(sf::Vector2f position)
{
	position_=position;
}

bool Bullet::operator<(const Bullet &bullet) const
{
	return time<bullet.time;
}

void Bullet::update(float dt, float y)
{
	position_+=velocity_*dt;
	sprite_.setPosition(position_.x, position_.y-y);
}

void Bullet::draw(sf::RenderWindow &window)
{
	window.draw(sprite_);
}