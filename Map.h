#ifndef MAP
#define MAP

#include <iostream>
#include <map>
#include <vector>
using namespace std;

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
using namespace rapidxml;

class Map
{
public:
	Map(string name);
	void draw(sf::RenderWindow &window);
	
private:
	sf::Vector2i size;
	map<string, sf::Texture> textures;
	vector<sf::Sprite> sprites;
};

#endif