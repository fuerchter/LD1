#include "Level.h"

Level::Level(sf::Vector2u windowSize):
player_(textures_, windowSize) //Create Player
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
	
	//Create View
	view_=sf::View(sf::FloatRect(0, 0, windowSize.x, windowSize.y));
	
	xml_node<> *enemies=doc.first_node()->first_node("enemies");
	xml_node<> *enemy=enemies->first_node();
	while(enemy)
	{
		//cout << enemy->first_attribute()->name() << endl;
		int y=atoi(enemy->first_attribute("y")->value());
		string name=enemy->first_attribute("texture")->value();
		float moveSpeed=atof(enemy->first_attribute("moveSpeed")->value());
		vector<sf::Vector2i> wayPoints;
		
		xml_node<> *waypoints=enemy->first_node();
		xml_node<> *waypoint=waypoints->first_node();
		while(waypoint)
		{
			//Wait? 3rd coordinate
			sf::Vector2i point;
			point.x=atoi(waypoint->first_attribute("x")->value());
			point.y=atoi(waypoint->first_attribute("y")->value());
			wayPoints.push_back(point);
			waypoint=waypoint->next_sibling();
		}
		
		//Bullets
		
		enemies_.insert(pair<int, Enemy>(y, Enemy(name, textures_, moveSpeed, wayPoints)));
		enemy=enemy->next_sibling();
	}
	
}

void Level::update(float dt)
{
	y_=-(view_.getCenter().y-view_.getSize().y/2);
	if(!scrollingSpeeds_.empty() && y_>=scrollingSpeeds_.begin()->first)
	{
		scrollingSpeed_=scrollingSpeeds_.begin()->second;
		scrollingSpeeds_.erase(scrollingSpeeds_.begin());
	}
	
	view_.move(0, -scrollingSpeed_*dt);
	player_.update(dt, y_);
	
	for(map<int, Enemy>::iterator it=enemies_.begin(); it!=enemies_.end(); ++it)
	{
		if(y_>=it->first)
		{
			it->second.update(dt, y_);
		}
		else
		{
			break;
		}
	}
}

void Level::draw(sf::RenderWindow &window)
{
	window.setView(view_);
	for(vector<Map>::iterator it=maps_.begin(); it!=maps_.end(); ++it)
	{
		it->draw(window);
	}
	
	player_.draw(window);
	
	for(map<int, Enemy>::iterator it=enemies_.begin(); it!=enemies_.end(); ++it)
	{
		if(y_>=it->first)
		{
			it->second.draw(window);
		}
		else
		{
			break;
		}
	}
}