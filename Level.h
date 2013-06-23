#ifndef LEVEL
#define LEVEL

#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
using namespace std;

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
using namespace rapidxml;

#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

class Level
{
public:
	enum Status
	{
		Playing,
		Win,
		Lose
	};
	
	Level(string levelName, sf::Vector2u windowSize, map<string, sf::Texture> &textures);
	sf::FloatRect getViewBounds();
	Level::Status getStatus();
	void update(float dt);
	void draw(sf::RenderWindow &window);
private:
	vector<Map>	maps_;
	map<int, float> scrollingSpeeds_; //y coordinate of the top of view_, scrollingSpeed from that point on
	float scrollingSpeed_;
	float y_;
	sf::View view_;
	Player player_;
	map<int, Enemy> enemies_;
	vector<Bullet> bullets_;
	Status status_;
};

#endif