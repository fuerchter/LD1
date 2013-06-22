#include "Map.h"

Map::Map(string name)
{
	name+=".xml";
	file<> xmlFile(name.c_str());
	using namespace rapidxml;
	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(xmlFile.data());    // 0 means default parse flags
	
	xml_node<> *mapSize=doc.first_node()->first_node("size");
	size.x=atoi(mapSize->first_attribute("x")->value());
	size.y=atoi(mapSize->first_attribute("y")->value());
	
	xml_node<> *images=doc.first_node()->first_node("images");
	xml_node<> *image=images->first_node();
	while(image)
	{
		string textureName=image->first_attribute("texture")->value();
		
		//If the texture is not in the list already
		if(textures.find(textureName)==textures.end())
		{
			//Create and insert it
			sf::Texture texture;
			texture.loadFromFile(textureName);
			textures.insert(pair<string, sf::Texture>(textureName, texture));
		}
		
		//Create and insert the sprite
		sf::Sprite sprite(textures[textureName]);
		int x=atoi(image->first_attribute("x")->value());
		int y=-(atoi(image->first_attribute("y")->value()))+720; //Reversing y axis
		sprite.setPosition(x, y);
		if(image->first_attribute("xs"))
		{
			float xs=atoi(image->first_attribute("xs")->value());
			sprite.setScale(xs/sprite.getGlobalBounds().width, sprite.getScale().y);
		}
		if(image->first_attribute("ys"))
		{
			float ys=atoi(image->first_attribute("ys")->value());
			sprite.setScale(sprite.getScale().x, ys/sprite.getGlobalBounds().height);
		}
		sprites.push_back(sprite);
		
		image=image->next_sibling();
	}
}

void Map::draw(sf::RenderWindow &window)
{
	for(std::vector<sf::Sprite>::iterator it=sprites.begin(); it!=sprites.end(); ++it)
	{
		window.draw(*it);
	}
}