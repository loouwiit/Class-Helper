#include "Button.h"

void Button::set_High_Light(bool flag)
{
	self_High_Light = flag;
}

bool Button::get_Hight_Light()
{
	return self_High_Light;
}

void Button::set_High_Light_Color(sf::Color color)
{
	self_High_Light_Color = color;
}

void Button::set_Default_Color(sf::Color color)
{
	self_Default_Color = color;
}

void Button_Text::set_High_Light(bool flag)
{
	self_High_Light = flag;
	set_Rectangle_Color();
}

void Button_Text::init()
{
	sf::FloatRect rect;
	rect = self_Text.getGlobalBounds();
	self_Rectangle.setPosition(rect.left, rect.top);
	self_Rectangle.setSize({ rect.width,rect.height });
}

void Button_Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(self_Rectangle);
	target.draw(self_Text);
}

bool Button_Text::is_Clicked(sf::Vector2f position)
{
	return self_Rectangle.getGlobalBounds().contains(position);
}

sf::Text& Button_Text::get_Text()
{
	return self_Text;
}

void Button_Text::set_Rectangle_Color()
{
	if (self_High_Light)
		self_Rectangle.setFillColor(self_High_Light_Color);
	else
		self_Rectangle.setFillColor(self_Default_Color);
}

const sf::Texture& Button_Texture::get_Texture()
{
	return self_Texture;
}

Button_Texture::Button_Texture()
{
	self_Default_Color = sf::Color(0xFFFFFFFF);
	self_High_Light_Color = sf::Color(0xFFFFFFFF);
}

void Button_Texture::set_Texture(sf::Texture texture)
{
	self_Texture = texture;
	self_Sprite.setTexture(self_Texture);
}

void Button_Texture::set_Texture(const char path[])
{
	self_Texture.loadFromFile(path);
	self_Sprite.setTexture(self_Texture);
}

void Button_Texture::set_Postion(sf::Vector2f position)
{
	self_Sprite.setPosition(position);
}

void Button_Texture::set_Scale(sf::Vector2f scale)
{
	self_Sprite.setScale(scale);
}

const sf::Vector2f& Button_Texture::get_Position()
{
	return self_Sprite.getPosition();
}

const sf::Vector2f& Button_Texture::get_Scale()
{
	return self_Sprite.getScale();
}

void Button_Texture::set_High_Light(bool flag)
{
	self_High_Light = flag;
	set_Sprite_Color();
}

void Button_Texture::init()
{
	self_Texture.setSmooth(true);
	return;
}

void Button_Texture::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(self_Sprite);
}

bool Button_Texture::is_Clicked(sf::Vector2f position)
{
	return self_Sprite.getGlobalBounds().contains(position);
}

void Button_Texture::set_Sprite_Color()
{
	if (self_High_Light)
		self_Sprite.setColor(self_High_Light_Color);
	else
		self_Sprite.setColor(self_Default_Color);
}
