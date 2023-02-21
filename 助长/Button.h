#pragma once
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable
{
public:
	void set_Hight_Light(bool flag);
	bool get_Hight_Light();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual bool is_Cilcked(sf::Vector2f position) = 0;
protected:
	bool self_High_Light = false;
};

class Botton_Text : public Button
{
public:
	void set_Hight_Light(bool flag);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool is_Cilcked(sf::Vector2f position);
	void set_Rect();
	void set_Height_Light_Color(sf::Color color);
	void set_Default_Color(sf::Color color);
	sf::Text& get_Text();

protected:
	sf::Text self_Text;
	sf::RectangleShape self_Rectangle;
	sf::Color self_Default_Color = sf::Color(0x0);
	sf::Color self_Height_Light_Color = sf::Color(0x0);
private:
	void set_Rectangle_Color();
};

class Botton_Texture : public Button
{

};
