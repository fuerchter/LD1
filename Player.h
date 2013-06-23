#ifndef PLAYER
#define PLAYER

#include <cmath>
#include <map>
using namespace std;

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

class Player
{
public:
	Player(map<string, sf::Texture> &textures, sf::Vector2u windowSize);
	sf::FloatRect getRect();
	void update(float dt, float y, sf::View &view);
	void draw(sf::RenderWindow &window);
	
private:
	sf::Sprite sprite_;
	sf::Vector2f position_;
	float moveSpeed_;
};

#endif