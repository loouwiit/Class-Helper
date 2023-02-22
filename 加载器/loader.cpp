#include "Interface.h"
#include "Button.h"

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
Interface_Dll* self = nullptr;
sf::RenderTexture* texture;
Button_Text text;
Button_Texture image;
sf::Color background_Color = sf::Color(0x99FFFFFF);

DLL void* init(void* self)
{
	printf("loader::init\n");

	::self = (Interface_Dll*)self;
	texture = &::self->get_Texture();

	texture->create(1920, 1080);
	if (!font.loadFromFile("C:\\Windows\\Fonts\\msyh.ttc")) font.loadFromFile("C:\\Windows\\Fonts\\msyh.ttf");
	texture->clear(background_Color);

	text.get_Text().setFont(font);
	text.get_Text().setString(L"test");
	text.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text.get_Text().setPosition(1920 / 2, 1080 / 2);
	text.get_Text().setOrigin(text.get_Text().getGlobalBounds().width / 2, text.get_Text().getGlobalBounds().height / 2);
	text.init();
	text.set_Default_Color(sf::Color(0x0));
	text.set_High_Light_Color(sf::Color(0x33333366));

	image.set_Texture(".\\resources\\test.png");
	image.set_Postion({ 50, 50 });
	image.set_High_Light_Color(sf::Color(0xFFFFFF99));
	image.init();

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
	texture->draw(image);
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
	text.set_High_Light(text.is_Clicked({ (float)mouse.x,(float)mouse.y }));
	image.set_High_Light(image.is_Clicked({ (float)mouse.x,(float)mouse.y }));
	//printf("loader::event_Mouse:(%d,%d):%d\n", mouse.x, mouse.y, image.get_Hight_Light());
}
