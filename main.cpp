#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "Level.h"

int main()
{	
	sf::RenderWindow window(sf::VideoMode(1280, 720), "test");
	
	map<string, sf::Texture> textures;
	int levelCounter=0;
	int power=0;
	
	
	ifstream save ("save.txt");
	if (save.is_open())
	{
		string line;
		getline(save,line);
		levelCounter=atoi(line.c_str());
		getline(save,line);
		power=atoi(line.c_str());
		save.close();
	}
	stringstream s;
	s << levelCounter;
	Level level(s.str(), window.getSize(), textures, power);
	
	
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
		if(level.getStatus()==Level::Win)
		{
			levelCounter++;
			if(levelCounter>3)
			{
				levelCounter=0;
			}
			ofstream save;
			save.open ("save.txt");
			stringstream s;
			s << levelCounter;
			save << s.str()+ "\n";
			save << level.getPlayer().getPower();
			save.close();
		}
		if(level.getStatus()!=Level::Playing)
		{
			ifstream save ("save.txt");
			if (save.is_open())
			{
				string line;
				getline(save,line);
				levelCounter=atoi(line.c_str());
				getline(save,line);
				power=atoi(line.c_str());
				save.close();
			}
			stringstream s;
			s << levelCounter;
			textures.clear();
			level=Level(s.str(), window.getSize(), textures, power);
		}
		//UPDATE SECTION
		
		window.clear();
		
		//DRAW SECTION
		level.draw(window);
		//DRAW SECTION
		
		window.display();
	}
}