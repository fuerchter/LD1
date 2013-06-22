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

//Origin of each sprite: top-left
class Map
{
public:
	Map(string folder, string name, map<string, sf::Texture> &textures);
	void draw(sf::RenderWindow &window);
private:
	sf::Vector2i size_;
	vector<sf::Sprite> sprites_;
};

#endif