#include "Map.h"

Map::Map(string folder, string name, map<string, sf::Texture> &textures)
{
	name+=".xml";
	file<> xmlFile((folder+name).c_str());
	using namespace rapidxml;
	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(xmlFile.data());    // 0 means default parse flags
	
	xml_node<> *images=doc.first_node()->first_node("images");
	xml_node<> *image=images->first_node();
	
	//Moves through each of the images in the xml and pushes them onto the vector
	while(image)
	{
		string textureName=image->first_attribute("texture")->value();
		
		//If the texture is not in the list already
		if(textures.find(textureName)==textures.end())
		{
			//Create and insert it
			sf::Texture texture;
			texture.loadFromFile(folder+textureName);
			textures.insert(pair<string, sf::Texture>(textureName, texture));
		}
		
		//Create and insert the sprite
		sf::Sprite sprite(textures[textureName]);
		int x=atoi(image->first_attribute("x")->value());
		int actualY=atoi(image->first_attribute("y")->value());
		int y=-(actualY)+720; //Reversing y axis
		if(actualY>size_.y)
		{
			size_.y=actualY;
		}
		sprite.setPosition(x, y);
		if(image->first_attribute("xs"))
		{
			float xs=atoi(image->first_attribute("xs")->value());
			if(x+xs>size_.x)
			{
				size_.x=x+xs;
			}
			sprite.setScale(xs/sprite.getGlobalBounds().width, sprite.getScale().y);
		}
		if(image->first_attribute("ys"))
		{
			float ys=atoi(image->first_attribute("ys")->value());
			sprite.setScale(sprite.getScale().x, ys/sprite.getGlobalBounds().height);
		}
		sprites_.push_back(sprite);
		
		image=image->next_sibling();
	}
}

sf::Vector2i Map::getSize()
{
	return size_;
}

void Map::move(sf::Vector2f direction, float factor)
{
	for(vector<sf::Sprite>::iterator it=sprites_.begin(); it!=sprites_.end(); ++it)
	{
		it->move(direction*factor);
	}
}

void Map::draw(sf::RenderWindow &window)
{
	for(vector<sf::Sprite>::iterator it=sprites_.begin(); it!=sprites_.end(); ++it)
	{
		window.draw(*it);
	}
}