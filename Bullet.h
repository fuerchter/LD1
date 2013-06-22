#ifndef BULLET
#define BULLET

#include <iostream>
#include <map>
using namespace std;

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

class Bullet
{
public:
	Bullet(map<string, sf::Texture> &textures, sf::Vector2f velocity, sf::Vector2i size, float time);
	void setPosition(sf::Vector2f position);
	bool operator<(const Bullet &bullet) const;
	void update(float dt, float y);
	void draw(sf::RenderWindow &window);
	
	float time;
private:
	sf::Sprite sprite_;
	sf::Vector2f position_;
	sf::Vector2f velocity_;
	
};

#endif