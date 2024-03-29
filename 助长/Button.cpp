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


bool Button_Text::self_Font_Loaded = false;
sf::Font Button_Text::self_Font;

void Button_Text::set_High_Light(bool flag)
{
	self_High_Light = flag;
	set_Rectangle_Color();
}

void Button_Text::init()
{
	set_Text_Alignment();

	sf::FloatRect rect;
	rect = self_Text.getGlobalBounds();
	self_Rectangle.setPosition(rect.left, rect.top);
	self_Rectangle.setSize({ rect.width,rect.height });
	set_Rectangle_Color();
}

void Button_Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(self_Rectangle, states);
	target.draw(self_Text, states);
}

bool Button_Text::is_Clicked(sf::Vector2f position)
{
	return self_Rectangle.getGlobalBounds().contains(position);
}

Button_Text::Button_Text()
{
	if (!self_Font_Loaded)
	{
		if (!self_Font.loadFromFile("C:\\windows\\fonts\\msyh.ttc")) self_Font.loadFromFile("C:\\windows\\fonts\\msyh.ttf");
		self_Font_Loaded = true;
	}
	self_Text.setFont(self_Font);
}

void Button_Text::set_Font(sf::Font font)
{
	self_Font = font;
}

unsigned int Button_Text::get_Default_Text_Size()
{
	return 30; //����sf::Text::setCharacterSize��֪
}

sf::Font Button_Text::get_Font()
{
	return self_Font;
}

sf::Text& Button_Text::get_Text()
{
	return self_Text;
}

void Button_Text::set_Text(const sf::String& string)
{
	self_Text.setString(string);
	init();
}

void Button_Text::set_Text_Size(const unsigned int size)
{
	self_Text.setCharacterSize(size);
	init();
}

void Button_Text::set_Position(const sf::Vector2f& position)
{
	self_Text.setPosition(position);
	init();
}

void Button_Text::set_Position(float x, float y)
{
	self_Text.setPosition(x, y);
	init();
}

void Button_Text::set_Alignment(Alignment alignment)
{
	self_Alignment_Flag = alignment;
	set_Text_Alignment();
}

void Button_Text::set_Rectangle_Color()
{
	if (self_High_Light)
		self_Rectangle.setFillColor(self_High_Light_Color);
	else
		self_Rectangle.setFillColor(self_Default_Color);
}

void Button_Text::set_Text_Alignment()
{
	sf::FloatRect rect = self_Text.getGlobalBounds();
	sf::Vector2f origin{ 0,0 };

	if (!(self_Alignment_Flag && Alignment::Horizontal)) origin.x = rect.width / 2;
	else if (self_Alignment_Flag && Alignment::Right) origin.x = rect.width;

	if (!(self_Alignment_Flag && Alignment::Vertical)) origin.y = rect.height / 2;
	else if (self_Alignment_Flag && Alignment::Botton) origin.y = rect.height;

	self_Text.setOrigin(origin.x, origin.y);
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

Button_Text::Alignment operator|(Button_Text::Alignment a, Button_Text::Alignment b)
{
	return (Button_Text::Alignment)((char)a | (char)b);
}

Button_Text::Alignment operator&(Button_Text::Alignment a, Button_Text::Alignment b)
{
	return (Button_Text::Alignment)((char)a & (char)b);
}

bool operator&&(Button_Text::Alignment a, Button_Text::Alignment b)
{
	return (Button_Text::Alignment)((char)a & (char)b) != Button_Text::Alignment::Middle;
}
