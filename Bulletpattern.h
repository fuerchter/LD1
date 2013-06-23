#ifndef BULLETPATTERN
#define BULLETPATTERN

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
using namespace std;

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include "Bullet.h"

class Bulletpattern
{
public:
	static vector<Bullet> createSpiral(map<string, sf::Texture> &textures, float startingTime, float timeOffset, bool clockwise, float startingDegrees, float degreesPerBullet, float totalDegrees, float speed, sf::Vector2i size, sf::Color color);
};

#endif