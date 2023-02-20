#include "Botton.h"

void Botton::set_Hight_Light(bool flag)
{
	self_High_Light = flag;
}

bool Botton::get_Hight_Light()
{
	return self_High_Light;
}

void Botton_Text::set_Hight_Light(bool flag)
{
	self_High_Light = flag;
	set_Rectangle_Color();
}

void Botton_Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(self_Rectangle);
	target.draw(self_Text);
}

bool Botton_Text::is_Cilcked(sf::Vector2f position)
{
	return self_Rectangle.getGlobalBounds().contains(position);
}

void Botton_Text::set_Rect()
{
	sf::FloatRect rect;
	rect = self_Text.getGlobalBounds();
	self_Rectangle.setPosition(rect.left, rect.top);
	self_Rectangle.setSize({ rect.width,rect.height });
}

void Botton_Text::set_Height_Light_Color(sf::Color color)
{
	self_Height_Light_Color = color;
}

void Botton_Text::set_Default_Color(sf::Color color)
{
	self_Default_Color = color;
}

sf::Text& Botton_Text::get_Text()
{
	return self_Text;
}

void Botton_Text::set_Rectangle_Color()
{
	if (self_High_Light)
		self_Rectangle.setFillColor(self_Height_Light_Color);
	else
		self_Rectangle.setFillColor(self_Default_Color);
}
