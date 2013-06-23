#include "Level.h"

Level::Level(string levelName, sf::Vector2u windowSize, map<string, sf::Texture> &textures):
player_(textures, windowSize), status_(Playing)
{	
	file<> xmlFile(("levels/" +levelName+ "/level.xml").c_str());
	using namespace rapidxml;
	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(xmlFile.data());    // 0 means default parse flags
	
	//Create a map for each layer in layercount
	int layercount=atoi(doc.first_node()->first_node("layercount")->value());
	for(int i=1; i<=layercount; i++)
	{
		stringstream s;
		s << i;
		Map map("levels/" +levelName+ "/", "layer" +s.str(), textures);
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
	
	//Adds enemies (and thereby waypoints and bullets)
	sf::Texture bullet;
	bullet.loadFromFile("bullet.png");
	textures.insert(pair<string, sf::Texture>("bullet.png", bullet));
	
	xml_node<> *enemies=doc.first_node()->first_node("enemies");
	xml_node<> *enemy=enemies->first_node();
	while(enemy)
	{
		//cout << enemy->first_attribute()->name() << endl;
		int y=atoi(enemy->first_attribute("y")->value());
		string name="levels/" +levelName+ "/" +enemy->first_attribute("texture")->value();
		float moveSpeed=atof(enemy->first_attribute("moveSpeed")->value());
		queue<sf::Vector3f> wayPoints;
		
		xml_node<> *waypoints=enemy->first_node("waypoints");
		xml_node<> *waypoint=waypoints->first_node();
		while(waypoint)
		{
			sf::Vector3f point;
			point.x=atoi(waypoint->first_attribute("x")->value());
			point.y=atoi(waypoint->first_attribute("y")->value());
			point.z=atof(waypoint->first_attribute("wait")->value());
			wayPoints.push(point);
			waypoint=waypoint->next_sibling();
		}
		
		vector<Bullet> enemyBullets;
		
		xml_node<> *bullets=enemy->first_node("bullets");
		xml_node<> *bullet=bullets->first_node("bullet");
		
		while(bullet)
		{
			
			float time=atof(bullet->first_attribute("time")->value());
			sf::Vector2f velocity;
			velocity.x=atof(bullet->first_attribute("xVel")->value());
			velocity.y=atof(bullet->first_attribute("yVel")->value());
			sf::Vector2i size;
			size.x=atoi(bullet->first_attribute("xs")->value());
			size.y=atoi(bullet->first_attribute("ys")->value());
			
			enemyBullets.push_back(Bullet(textures, velocity, size, time));
			bullet=bullet->next_sibling();
		}
		
		//Sorting the bullets by time
		sort(enemyBullets.begin(), enemyBullets.end());
		enemies_.insert(pair<int, Enemy>(y, Enemy(name, textures, moveSpeed, wayPoints, enemyBullets)));
		enemy=enemy->next_sibling();
	}
	
}

sf::FloatRect Level::getViewBounds()
{
	sf::Vector2f center=view_.getCenter();
	sf::Vector2f size=view_.getSize();
	return sf::FloatRect(center.x-size.x/2, center.y-size.y/2, size.x, size.y);
}

Level::Status Level::getStatus()
{
	return status_;
}

/**
* Update sequence: Enemies, Bullets, Player
*/
void Level::update(float dt)
{
	//Calculating the current y coordinate of view
	y_=-(view_.getCenter().y-view_.getSize().y/2);
	
	//A new scrollingSpeed has been assigned
	if(!scrollingSpeeds_.empty() && y_>=scrollingSpeeds_.begin()->first)
	{
		scrollingSpeed_=scrollingSpeeds_.begin()->second;
		scrollingSpeeds_.erase(scrollingSpeeds_.begin());
	}
	
	//Updates the view
	view_.move(0, -scrollingSpeed_*dt);
	
	//cout << enemies_.size() << endl;
	for(map<int, Enemy>::iterator it=enemies_.begin(); it!=enemies_.end();)
	{
		//Only enemies which exist on the screen are to be updated
		if(y_>=it->first)
		{
			it->second.update(dt, y_, bullets_);
			
			//Enemy hits player
			if(it->second.getRect().intersects(player_.getRect()))
			{
				status_=Lose;
			}
			//An enemy has left the screen (and has lived for at least 2 seconds)
			if(it->second.getLifeTime()>2 && !getViewBounds().intersects(it->second.getRect()))
			{
				map<int, Enemy>::iterator toerase = it;
				++it;
				enemies_.erase(toerase);
			}
			else
			{
				++it;
			}
		}
		else
		{
			break;
		}
	}
	
	for(vector<Bullet>::iterator it=bullets_.begin(); it!=bullets_.end(); ++it)
	{
		it->update(dt, y_);
		
		//Bullet hits player
		if(it->getRect().intersects(player_.getRect()))
		{
			status_=Lose;
		}
		//A bullet has left the screen
		if(!getViewBounds().intersects(it->getRect()))
		{
			bullets_.erase(it);
			it--;
		}
	}
	
	player_.update(dt, y_, view_);	
}

/**
* Draw sequence: Map, Enemies, Bullets, Player
*/
void Level::draw(sf::RenderWindow &window)
{
	window.setView(view_);
	
	for(vector<Map>::iterator it=maps_.begin(); it!=maps_.end(); ++it)
	{
		it->draw(window);
	}
	
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
	
	player_.draw(window);
	
	for(vector<Bullet>::iterator it=bullets_.begin(); it!=bullets_.end(); ++it)
	{
		it->draw(window);
	}
}