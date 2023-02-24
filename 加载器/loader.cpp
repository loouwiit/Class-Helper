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
void event_Mouse(sf::Event::MouseButtonEvent mouse);

using Function = Interface::Function;

constexpr int text_Nunber = 3;

Interface_Dll* self = nullptr;
sf::RenderTexture* texture;
Button_Text text[text_Nunber];
sf::Color background_Color = sf::Color(0x99FFFFFF);

DLL void* init(void* self)
{
	printf("loader::init\n");

	::self = (Interface_Dll*)self;
	texture = &::self->get_Texture();

	text[0].get_Text().setString(L"退出程序");
	text[0].get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text[0].get_Text().setPosition((float)(1920 - 50), (float)50);
	text[0].set_Alignment(Button_Text::Alignment::Right | Button_Text::Alignment::Top); //右上对齐
	text[0].set_Default_Color(sf::Color(0x0));
	text[0].set_High_Light_Color(sf::Color(0x33333366));
	text[0].init();

	text[1].get_Text().setString(L"加载seat.dll");
	text[1].get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text[1].get_Text().setPosition((float)(1920 / 2), (float)(1080 / 2 - 50));
	text[1].set_Default_Color(sf::Color(0x0));
	text[1].set_High_Light_Color(sf::Color(0x33333366));
	text[1].init();

	text[2].get_Text().setString(L"加载sanitation.dll");
	text[2].get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text[2].get_Text().setPosition((float)(1920 / 2), (float)(1080 / 2 + 50));
	text[2].set_Default_Color(sf::Color(0x0));
	text[2].set_High_Light_Color(sf::Color(0x33333366));
	text[2].init();

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
	case Event::MouseButtonPressed:
	{
		event_Mouse(event.mouseButton);
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
	for (int i = 0; i < text_Nunber; i++)
		texture->draw(text[i]);
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

	printf("loader::ened\n");
}

void event_Key(sf::Event::KeyEvent key)
{
	using Key = sf::Keyboard;

	switch (key.code)
	{
	case Key::Escape:
	{
		self->set_Next_Dll(nullptr);
		ened();
		break;
	}
	case Key::Space:
	{
		self->set_Next_Dll(".\\dll\\loader.dll");
		ened();
		break;
	}
	default:
	{
		break;
	}
	}
}

void event_Mouse(sf::Event::MouseMoveEvent mouse)
{
	for (int i = 0; i < text_Nunber; i++)
		text[i].set_High_Light(text[i].is_Clicked({ (float)mouse.x,(float)mouse.y }));
}

void event_Mouse(sf::Event::MouseButtonEvent mouse)
{
	if (text[0].is_Clicked({ (float)mouse.x,(float)mouse.y }))
	{
		self->set_Next_Dll(nullptr);
		ened();
	}

	if (text[1].is_Clicked({ (float)mouse.x,(float)mouse.y }))
	{
		self->set_Next_Dll(".\\dll\\seat.dll");
		ened();
	}

	if (text[2].is_Clicked({ (float)mouse.x,(float)mouse.y }))
	{
		self->set_Next_Dll(".\\dll\\sanitation.dll");
		ened();
	}
}
