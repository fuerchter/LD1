#include "Player.h"

Player::Player(map<string, sf::Texture> &textures, sf::Vector2u windowSize, int power):
maxMoveSpeed_(150), moveSpeed_(maxMoveSpeed_), canShoot_(true), cooldown_(0.5), currentCooldown_(0), stamina_(100), lossPerShot_(10), regen_(7.5), power_(power)
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

sf::Vector2f Player::getPosition()
{
	return position_;
}

int Player::getPower()
{
	return power_;
}

void Player::incPower()
{
	power_++;
}

void Player::setBulletColor(sf::Color color)
{
	bulletColor_=color;
}

void Player::update(float dt, float y, sf::View &view, vector<Bullet> &bullets, map<string, sf::Texture> &textures, bool lockY, int mapX)
{
	cooldown_=0.5/((power_/20.0)+1.0);
	sf::Vector2f velocity;
	if(!lockY)
	{
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
	}
	else
	{
		if(position_.x-getRect().width/2>0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			velocity.x-=1;
		}
		if(position_.x+getRect().width/2<mapX && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			velocity.x+=1;
		}
	}
	float length=sqrt(pow(velocity.x, 2)+pow(velocity.y, 2));
	if(length!=0)
	{
		velocity/=length;
		velocity*=moveSpeed_;
		velocity*=dt;
		position_+=velocity;
	}
	
	if(canShoot_ && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		sf::Vector2f shotVelocity;
		shotVelocity.y=-175-10*power_;
		sf::Vector2i size;
		size.x=16+4*power_;
		size.y=16+4*power_;
		float damage=10+2*power_;
		Bullet bullet(textures, shotVelocity, size, 0, damage, bulletColor_);
		bullet.setPosition(position_);
		bullets.push_back(bullet);
		canShoot_=false;
		
		stamina_-=lossPerShot_;
		if(stamina_<0)
		{
			stamina_=0;
		}
	}
	if(!canShoot_)
	{
		currentCooldown_+=dt;
		if(currentCooldown_>=cooldown_)
		{
			canShoot_=true;
			currentCooldown_=0;
		}
	}
	
	stamina_+=regen_*dt;
	if(stamina_>100)
	{
		stamina_=100;
	}
	
	moveSpeed_=maxMoveSpeed_*(stamina_/100);
	//cout << "Stamina: " << stamina_ << " Power: " << power_ << " Cooldown: " << cooldown_ << endl;

	sprite_.setPosition(position_.x, position_.y-y);
}

void Player::draw(sf::RenderWindow &window)
{
	stringstream stamina;
	stamina << stamina_;
	stringstream power;
	power << power_;
	stringstream cooldown;
	cooldown << cooldown_;
	window.setTitle("Stamina: " +stamina.str()+ " Power: " +power.str()+ " Cooldown: " +cooldown.str());
	window.draw(sprite_);
}