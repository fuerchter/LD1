#include <iostream>
#include <fstream>
using namespace std;

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "Level.h"

int main()
{	
	sf::RenderWindow window(sf::VideoMode(1280, 720), "test");
	
	map<string, sf::Texture> textures;
	Level level("ground", window.getSize(), textures);
	
	sf::Clock clock;
	sf::Time previousTick=clock.getElapsedTime();
	sf::Time deltaTime;
	
	sf::Time counter=clock.getElapsedTime();
	int fps=0;
	
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			switch(event.type)
			{
				case sf::Event::Closed:
					window.close();
				default:
					break;
			}
		}
		
		deltaTime=clock.getElapsedTime()-previousTick;
		previousTick=clock.getElapsedTime();
		
		fps++;
		counter+=deltaTime;
		if(counter.asSeconds()>=1)
		{
			//cout << fps << endl;
			fps=0;
			counter=sf::Time::Zero;
		}
		
		//UPDATE SECTION
		level.update(deltaTime.asSeconds(), textures);
		if(level.getStatus()==Level::Lose)
		{
			level=Level("ground", window.getSize(), textures);
		}
		//UPDATE SECTION
		
		window.clear();
		
		//DRAW SECTION
		level.draw(window);
		//DRAW SECTION
		
		window.display();
	}
}