#ifndef PLAYER
#define PLAYER

#include <map>
using namespace std;

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

//Origin: top-left
class Player
{
public:
	Player(map<string, sf::Texture> &textures, sf::Vector2u windowSize);
	void update(float dt, float y);
	void draw(sf::RenderWindow &window);
	
private:
	sf::Sprite sprite_;
	sf::Vector2f position_;
};

#endif