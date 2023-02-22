#pragma once
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable
{
public:
	void set_High_Light(bool flag);
	bool get_Hight_Light();

	void set_High_Light_Color(sf::Color color);
	void set_Default_Color(sf::Color color);

	virtual void init() = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual bool is_Clicked(sf::Vector2f position) = 0;

protected:
	bool self_High_Light = false;
	sf::Color self_Default_Color = sf::Color(0x0);
	sf::Color self_High_Light_Color = sf::Color(0x0);
};

class Button_Text : public Button
{
public:
	sf::Text& get_Text();

	void set_High_Light(bool flag);
	void init();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool is_Clicked(sf::Vector2f position);

protected:
	sf::Text self_Text;
	sf::RectangleShape self_Rectangle;

private:
	void set_Rectangle_Color();
};

class Button_Texture : public Button
{
public:
	Button_Texture();

	void set_Texture(sf::Texture texture);
	void set_Texture(const char path[]);
	void set_Postion(sf::Vector2f position);
	void set_Scale(sf::Vector2f scale);

	const sf::Texture& get_Texture();
	const sf::Vector2f& get_Position();
	const sf::Vector2f& get_Scale();

	void set_High_Light(bool flag);
	void init();
	void draw(sf::RenderTarget& target, sf::RenderStates states)const;
	bool is_Clicked(sf::Vector2f position);

protected:
	sf::Sprite self_Sprite;
	sf::Texture self_Texture;

private:
	void set_Sprite_Color();
};
