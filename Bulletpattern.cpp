#include "Bulletpattern.h"

vector<Bullet> Bulletpattern::createSpiral(map<string, sf::Texture> &textures, float startingTime, float timeOffset, bool clockwise, float startingDegrees, float degreesPerBullet, float totalDegrees, float speed, sf::Vector2i size, sf::Color color)
{
	vector<Bullet> res;
	int degrees=0;
	for(int count=0; degrees<=totalDegrees; degrees+=degreesPerBullet, count++)
	{
		sf::Vector2f velocity;
		float startingRadian=-((float)startingDegrees*M_PI/180.0f);
		velocity.x=0*cos(startingRadian)-(-speed)*sin(startingRadian);
		velocity.y=0*sin(startingRadian)+(-speed)*cos(startingRadian);
		sf::Vector2f newVelocity;
		float radian=((float)degrees*M_PI/180.0f);
		if(!clockwise)
		{
			radian*=(-1);
		}
		newVelocity.x=velocity.x*cos(radian)-velocity.y*sin(radian);
		newVelocity.y=velocity.x*sin(radian)+velocity.y*cos(radian);
		float time=startingTime+count*timeOffset;
		res.push_back(Bullet(textures, newVelocity, size, time, 0, color));
	}
	return res;
}