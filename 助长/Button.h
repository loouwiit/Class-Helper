#pragma once
#include <SFML/Graphics.hpp>

class Botton
{
public:
	virtual void set_Hight_Light(bool flag);
	virtual bool set_Hight_Light(sf::Vector2f position);
	virtual void draw_To(sf::RenderTarget target);
	virtual bool is_Cilcked(sf::Vector2f position);
private:
	bool self_High_Light = false;
};

class Botton_Text : public Botton
{

};

class Botton_Texture : public Botton
{

};
