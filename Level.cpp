#include "Level.h"

Level::Level(sf::Vector2u windowSize)
{	
	file<> xmlFile("level.xml");
	using namespace rapidxml;
	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(xmlFile.data());    // 0 means default parse flags
	
	//Create a map for each layer in layercount
	int layercount=atoi(doc.first_node()->first_node("layercount")->value());
	for(int i=1; i<=layercount; i++)
	{
		stringstream s;
		s << i;
		Map map("layer" +s.str(), textures_);
		maps_.push_back(map);
	}
	
	//Add each speed to the map
	xml_node<> *speeds=doc.first_node()->first_node("speeds");
	xml_node<> *speed=speeds->first_node();
	while(speed)
	{
		int y=atoi(speed->first_attribute("y")->value());
		float amount=atof(speed->first_attribute("amount")->value());
		scrollingSpeeds_.insert(pair<int, float>(y, amount));
		speed=speed->next_sibling();
	}
	
	//Uses first scrollingSpeed (no matter whether y=0)
	scrollingSpeed_=scrollingSpeeds_.begin()->second;
	scrollingSpeeds_.erase(scrollingSpeeds_.begin());
	
	view_=sf::View(sf::FloatRect(0, 0, windowSize.x, windowSize.y));
}

void Level::update(float dt)
{
	float y=-(view_.getCenter().y-view_.getSize().y/2);
	if(!scrollingSpeeds_.empty() && y>=scrollingSpeeds_.begin()->first)
	{
		scrollingSpeed_=scrollingSpeeds_.begin()->second;
		scrollingSpeeds_.erase(scrollingSpeeds_.begin());
	}
	
	view_.move(0, -scrollingSpeed_*dt);
}

void Level::draw(sf::RenderWindow &window)
{
	window.setView(view_);
	for(vector<Map>::iterator it=maps_.begin(); it!=maps_.end(); ++it)
	{
		it->draw(window);
	}
}