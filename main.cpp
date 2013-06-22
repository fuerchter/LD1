#include <iostream>
#include <fstream>
using namespace std;

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "Level.h"

int main()
{
	/*file<> xmlFile("level.xml");
	using namespace rapidxml;
	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(xmlFile.data());    // 0 means default parse flags
	
	xml_node<> *level=doc.first_node();
	xml_node<> *config=level->first_node("config");
	xml_node<> *colorscheme=config->first_node("colorscheme");
	
	xml_node<> *colors=doc.first_node()->first_node("config")->first_node("colorscheme")->first_node();
	while(colors)
	{
		cout << colors->name() << endl;
		xml_attribute<> *rgb=colors->first_attribute();
		while(rgb)
		{
			cout << rgb->name() << " " << rgb->value() << endl;
			rgb=rgb->next_attribute();
		}
		colors=colors->next_sibling();
	}*/
	
	sf::RenderWindow window(sf::VideoMode(1280, 720), "test");
	Level level(window.getSize());
	
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
		level.update(deltaTime.asSeconds());
		//UPDATE SECTION
		
		window.clear();
		
		//DRAW SECTION
		level.draw(window);
		//DRAW SECTION
		
		window.display();
	}
}