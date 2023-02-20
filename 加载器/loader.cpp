#include "Interface.h"
#include "Botton.h"

#define DLL extern "C" __declspec(dllexport)

DLL void* init(void* self);
DLL void* event(void* param);
DLL void* compute(void* null);
DLL void* draw(void* null);
//DLL void* sleep(void* null);

void ened();
void event_Key(sf::Event::KeyEvent key);
void event_Mouse(sf::Event::MouseMoveEvent mouse);

using Function = Interface::Function;

sf::Font font;
Interface* self = nullptr;
sf::RenderTexture* texture;
Botton_Text text;
sf::Color background_Color = sf::Color(0x99FFFFFF);

DLL void* init(void* self)
{
	printf("loader::init\n");

	::self = (Interface*)self;
	texture = &::self->get_Texture();

	texture->create(1920, 1080);
	if (!font.loadFromFile("C:\\Windows\\Fonts\\msyh.ttc")) font.loadFromFile("C:\\Windows\\Fonts\\msyh.ttf");
	texture->clear(background_Color);

	text.get_Text().setFont(font);
	text.get_Text().setString(L"test");
	text.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text.get_Text().setPosition(1920 / 2, 1080 / 2);
	text.get_Text().setOrigin(text.get_Text().getGlobalBounds().width / 2, text.get_Text().getGlobalBounds().height / 2);
	text.set_Rect();
	text.set_Default_Color(sf::Color(0x000000FF));
	text.set_Height_Light_Color(sf::Color(0x333333FF));

	return nullptr;
}

DLL void* event(void* param)
{
	using Event = sf::Event;
	Event& event = *(Event*)param;

	switch (event.type)
	{
	case Event::Closed:
	{
		ened();
		break;
	}
	case Event::KeyPressed:
	{
		event_Key(event.key);
		break;
	}
	case Event::MouseMoved:
	{
		event_Mouse(event.mouseMove);
		break;
	}
	default:
	{
		break;
	}
	}
	return nullptr;
}

DLL void* compute(void* null)
{
	return nullptr;
}

DLL void* draw(void* null)
{
	texture->clear(background_Color);
	texture->draw(text);
	return nullptr;
}

//DLL void* sleep(void* null)
//{
//	sf::sleep(sf::milliseconds(16));
//	return nullptr;
//}

void ened()
{
	self->set_Running(false);
	self->set_Next_Dll(nullptr);

	printf("loader::ened\n");
}

void event_Key(sf::Event::KeyEvent key)
{
	using Key = sf::Keyboard;

	switch (key.code)
	{
	case Key::Escape:
	{
		ened();
	}
	default:
	{
		break;
	}
	}
}

void event_Mouse(sf::Event::MouseMoveEvent mouse)
{
	text.set_Hight_Light(text.is_Cilcked({ (float)mouse.x,(float)mouse.y }));
	printf("loader::ebent_Mouse:hight_Light=%d", text.get_Hight_Light());
}
