#ifndef ENEMY
#define ENEMY

#include <queue>
#include <map>
#include <cmath>
#include <iostream>
using namespace std;

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include "Bullet.h"

class Enemy
{
public:
	Enemy(string name, map<string, sf::Texture> &textures, float moveSpeed, queue<sf::Vector3f> wayPoints, vector<Bullet> bullets); //Bullets
	void update(float dt, float y, vector<Bullet> &bullets);
	void draw(sf::RenderWindow &window);
	
private:
	sf::Sprite sprite_;
	sf::Vector2f position_;
	float moveSpeed_;
	float lifeTime_;
	queue<sf::Vector3f> wayPoints_;
	vector<Bullet> bullets_;
};

#endif