#ifndef ENEMY
#define ENEMY

#include <vector>
#include <map>
using namespace std;

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

//Origin: top-left
class Enemy
{
public:
	Enemy(string name, map<string, sf::Texture> &textures, float moveSpeed, vector<sf::Vector2i> wayPoints); //Bullets
	void update(float dt, float y);
	void draw(sf::RenderWindow &window);
	
private:
	sf::Sprite sprite_;
	sf::Vector2f position_;
	float moveSpeed_;
	vector<sf::Vector2i> wayPoints_;
};

#endif