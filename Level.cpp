#include "Level.h"

Level::Level(string levelName, sf::Vector2u windowSize, map<string, sf::Texture> &textures, int power):
player_(textures, windowSize, power), status_(Playing), lockY_(false)
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
	
	if(doc.first_node()->first_node("lockY"))
	{
		lockY_=true;
	}
	
	xml_node<> *pbc=doc.first_node()->first_node("playerBulletColor");
	sf::Color playerBulletColor;
	playerBulletColor.r=atoi(pbc->first_attribute("r")->value());
	playerBulletColor.g=atoi(pbc->first_attribute("g")->value());
	playerBulletColor.b=atoi(pbc->first_attribute("b")->value());
	playerBulletColor.a=atoi(pbc->first_attribute("a")->value());
	
	player_.setBulletColor(playerBulletColor);
	
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
		float health=atof(enemy->first_attribute("health")->value());
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
		xml_node<> *bullet=bullets->first_node();
		
		while(bullet)
		{
			if(strcmp(bullet->name(), "bullet")==0)
			{
				float time=atof(bullet->first_attribute("time")->value());
				sf::Vector2f velocity;
				velocity.x=atof(bullet->first_attribute("xVel")->value());
				velocity.y=atof(bullet->first_attribute("yVel")->value());
				sf::Vector2i size;
				size.x=atoi(bullet->first_attribute("xs")->value());
				size.y=atoi(bullet->first_attribute("ys")->value());
				sf::Color bulletColor;
				bulletColor.r=atoi(bullet->first_attribute("r")->value());
				bulletColor.g=atoi(bullet->first_attribute("g")->value());
				bulletColor.b=atoi(bullet->first_attribute("b")->value());
				bulletColor.a=atoi(bullet->first_attribute("a")->value());
				bool homing=false;
				if(bullet->first_attribute("homing"))
				{
					homing=true;
				}
				
				enemyBullets.push_back(Bullet(textures, velocity, size, time, 0, bulletColor, homing));
			}
			else if(strcmp(bullet->name(), "bulletpattern")==0)
			{
				if(strcmp(bullet->first_attribute("type")->value(), "spiral")==0)
				{
					float time=atof(bullet->first_attribute("time")->value());
					float timeOffset=atof(bullet->first_attribute("timeOffset")->value());
					string cwise=bullet->first_attribute("clockwise")->value();
					bool clockwise=true;
					if(cwise=="false")
					{
						clockwise=false;
					}
					float startingDegrees=atof(bullet->first_attribute("startingDegrees")->value());
					float degreesPerBullet=atof(bullet->first_attribute("degreesPerBullet")->value());
					float totalDegrees=atof(bullet->first_attribute("totalDegrees")->value());
					float speed=atof(bullet->first_attribute("speed")->value());
					sf::Vector2i size;
					size.x=atoi(bullet->first_attribute("xs")->value());
					size.y=atoi(bullet->first_attribute("ys")->value());
					sf::Color bulletColor;
					bulletColor.r=atoi(bullet->first_attribute("r")->value());
					bulletColor.g=atoi(bullet->first_attribute("g")->value());
					bulletColor.b=atoi(bullet->first_attribute("b")->value());
					bulletColor.a=atoi(bullet->first_attribute("a")->value());
					
					vector<Bullet> bulletpattern=Bulletpattern::createSpiral(textures, time, timeOffset, clockwise, startingDegrees, degreesPerBullet, totalDegrees, speed, size, bulletColor);
					enemyBullets.insert(enemyBullets.end(), bulletpattern.begin(), bulletpattern.end());
				}
			}
			
			bullet=bullet->next_sibling();
		}
		
		//Sorting the bullets by time
		sort(enemyBullets.begin(), enemyBullets.end());
		enemies_.insert(pair<int, Enemy>(y, Enemy(name, textures, moveSpeed, wayPoints, enemyBullets, health)));
		enemy=enemy->next_sibling();
	}
	
	string win=doc.first_node()->first_node("win")->value();
	if(win=="y")
	{
		winCondition_=Y;
	}
	else if(win=="enemies")
	{
		winCondition_=Enemies;
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

Player Level::getPlayer()
{
	return player_;
}

/**
* Update sequence: Enemies, Bullets, Player
*/
void Level::update(float dt, map<string, sf::Texture> &textures)
{
	//Calculating the current y coordinate of view
	y_=-(view_.getCenter().y-view_.getSize().y/2);
	
	switch(winCondition_)
	{
	case Y:
		if(y_+view_.getSize().y>=maps_[0].getSize().y)
		{
			status_=Win;
		}
		break;
	case Enemies:
		if(enemies_.empty())
		{
			status_=Win;
		}
		break;
	}
	
	//A new scrollingSpeed has been assigned
	if(!scrollingSpeeds_.empty() && y_>=scrollingSpeeds_.begin()->first)
	{
		scrollingSpeed_=scrollingSpeeds_.begin()->second;
		scrollingSpeeds_.erase(scrollingSpeeds_.begin());
	}
	
	//Updates the view
	sf::Vector2f change(0, -scrollingSpeed_*dt);
	view_.move(change);
	
	if(lockY_) /* && getViewBounds().left+getViewBounds().width<maps_[0].getSize().x*/
	{
		//If view is not at left or right border
		if(player_.getRect().left+player_.getRect().width/2>getViewBounds().width/2 && player_.getRect().left+player_.getRect().width/2<maps_[0].getSize().x-getViewBounds().width/2)
		{
			change.x=view_.getCenter().x-(player_.getRect().left+player_.getRect().width/2);
			view_.setCenter(player_.getRect().left+player_.getRect().width/2, view_.getCenter().y);
		}
	}
	
	for(int i=1; i<maps_.size(); i++)
	{
		maps_[i].move(-change, 1.0/((float)i+1));
	}
	
	//cout << enemies_.size() << endl;
	for(map<int, Enemy>::iterator it=enemies_.begin(); it!=enemies_.end();)
	{
		//Only enemies which exist on the screen are to be updated
		if(y_>=it->first)
		{
			it->second.update(dt, y_, bullets_, player_.getPosition());
			
			//Enemy hits player
			if(it->second.getRect().intersects(player_.getRect()))
			{
				status_=Lose;
			}
			//An enemy has left the screen (and has lived for at least 2 seconds)
			if((it->second.getLifeTime()>2 && !getViewBounds().intersects(it->second.getRect()))
			|| it->second.getHealth()<=0)
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
	
	player_.update(dt, y_, view_, playerBullets_, textures, lockY_, maps_[0].getSize().x);
	
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

	for(vector<Bullet>::iterator it=playerBullets_.begin(); it!=playerBullets_.end(); ++it)
	{
		it->update(dt, y_);
		bool erase=false;
		
		//Bullet hits Enemy
		for(map<int, Enemy>::iterator et=enemies_.begin(); et!=enemies_.end(); ++et)
		{
			if(it->getRect().intersects(et->second.getRect()))
			{
				et->second.setHealth((et->second.getHealth())-(it->getDamage()));
				if(et->second.getHealth()<=0)
				{
					player_.incPower();
				}
				erase=true;
			}
		}

		//A bullet has left the screen
		if(!getViewBounds().intersects(it->getRect()))
		{
			erase=true;
		}
		
		if(erase)
		{
			playerBullets_.erase(it);
			it--;
		}
	}
}

/**
* Draw sequence: Map, Enemies, Bullets, Player
*/
void Level::draw(sf::RenderWindow &window)
{
	window.setView(view_);
	
	for(vector<Map>::reverse_iterator it=maps_.rbegin(); it!=maps_.rend(); ++it)
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
	
	for(vector<Bullet>::iterator it=playerBullets_.begin(); it!=playerBullets_.end(); ++it)
	{
		it->draw(window);
	}
}