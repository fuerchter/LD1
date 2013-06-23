#include "Bullet.h"

Bullet::Bullet(map<string, sf::Texture> &textures, sf::Vector2f velocity, sf::Vector2i size, float time, float damage):
time(time), velocity_(velocity), damage_(damage)
{
	sprite_.setTexture(textures["bullet.png"]);
	sprite_.setScale(size.x, size.y);
	sprite_.setColor(sf::Color(255, 0, 0, 255));
	sprite_.setOrigin(0.5, 0.5); //Having to ignore scale the center of the 1x1 image is 0.5x0.5
}

sf::Vector2f Bullet::getPosition()
{
	return position_;
}

void Bullet::setPosition(sf::Vector2f position)
{
	position_=position;
}

bool Bullet::operator<(const Bullet &bullet) const
{
	return time<bullet.time;
}

sf::FloatRect Bullet::getRect()
{
	return sprite_.getGlobalBounds();
}

float Bullet::getDamage()
{
	return damage_;
}

/**
* Adjusts its position with its velocity
*/
void Bullet::update(float dt, float y)
{
	position_+=velocity_*dt;
	sprite_.setPosition(position_.x, position_.y-y);
}

void Bullet::draw(sf::RenderWindow &window)
{
	window.draw(sprite_);
}