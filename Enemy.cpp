#include "Enemy.h"

Enemy::Enemy(string name, map<string, sf::Texture> &textures, float moveSpeed, queue<sf::Vector3f> wayPoints, vector<Bullet> bullets, float health):
moveSpeed_(moveSpeed), wayPoints_(wayPoints), bullets_(bullets), health_(health)
{
	sf::Texture texture;
	texture.loadFromFile(name);
	textures.insert(pair<string, sf::Texture>(name, texture));
	sprite_.setTexture(textures[name]);
	position_.x=wayPoints.front().x;
	position_.y=wayPoints.front().y;
	sprite_.setPosition(position_);
	sprite_.setOrigin(sprite_.getGlobalBounds().width/2, sprite_.getGlobalBounds().height/2);
}

sf::FloatRect Enemy::getRect()
{
	return sprite_.getGlobalBounds();
}

float Enemy::getLifeTime()
{
	return lifeTime_;
}

float Enemy::getHealth()
{
	return health_;
}

void Enemy::setHealth(float health)
{
	health_=health;
}

/**
* Follows its waypoints and waits if a waittime was entered\n
* Currently moves in on a waypoint until the distance is smaller than moveSpeed\n
* Finally sends out all bullets which have to be shot, putting them in the bullets vector given by the Level
*/
void Enemy::update(float dt, float y, vector<Bullet> &bullets, sf::Vector2f playerPosition)
{	
	if(!wayPoints_.empty())
	{
		//We are still waiting
		if(wayPoints_.front().z>0)
		{
			wayPoints_.front().z-=dt;
		}
		else
		{
			sf::Vector2f direction;
			direction.x=wayPoints_.front().x-position_.x;
			direction.y=wayPoints_.front().y-position_.y;
			float distance=sqrt(pow(direction.x, 2)+pow(direction.y, 2));
			//Reached the waypoint, required distance is still arbitrary
			if(distance<moveSpeed_)
			{
				wayPoints_.pop();
			}
			//Move closer to waypoint
			else
			{
				direction/=distance;
				direction*=moveSpeed_;
				direction*=dt;
				position_+=direction;
			}
		}
	}
	
	lifeTime_+=dt;
	while(!bullets_.empty() && lifeTime_>=bullets_[0].time)
	{
		bullets_[0].setPosition(position_);
		if(bullets_[0].isHoming())
		{
			sf::Vector2f velocity=bullets_[0].getVelocity();
			float speed=sqrt(pow(velocity.x, 2)+pow(velocity.y, 2));
			velocity=playerPosition-position_;
			float length=sqrt(pow(velocity.x, 2)+pow(velocity.y, 2));
			velocity/=length;
			velocity*=speed;
			bullets_[0].setVelocity(velocity);
		}
		bullets.push_back(bullets_[0]);
		bullets_.erase(bullets_.begin());
	}
	
	sprite_.setPosition(position_.x, position_.y-y);
}

void Enemy::draw(sf::RenderWindow &window)
{
	window.draw(sprite_);
}