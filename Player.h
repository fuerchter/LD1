#ifndef PLAYER
#define PLAYER

#include <iostream>
#include <cmath>
#include <map>
using namespace std;

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "Bullet.h"

class Player
{
public:
	Player(map<string, sf::Texture> &textures, sf::Vector2u windowSize, int power);
	sf::FloatRect getRect();
	sf::Vector2f getPosition();
	int getPower();
	void incPower();
	void setBulletColor(sf::Color color);
	void update(float dt, float y, sf::View &view, vector<Bullet> &bullets, map<string, sf::Texture> &textures, bool lockY, int mapX);
	void draw(sf::RenderWindow &window);
private:
	sf::Sprite sprite_;
	sf::Vector2f position_;
	float maxMoveSpeed_;
	float moveSpeed_;
	bool canShoot_;
	float cooldown_;
	float currentCooldown_;
	float stamina_;
	float lossPerShot_;
	float regen_;
	int power_;
	sf::Color bulletColor_;
};

#endif