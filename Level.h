#ifndef LEVEL
#define LEVEL

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
using namespace rapidxml;

#include "Map.h"

class Level
{
public:
	Level(sf::Vector2u windowSize);
	void update(float dt);
	void draw(sf::RenderWindow &window);
private:
	map<string, sf::Texture> textures_;
	vector<Map>	maps_;
	map<int, float> scrollingSpeeds_; //y coordinate of the top of view_, scrollingSpeed from that point on
	float scrollingSpeed_;
	sf::View view_;
};

#endif