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

using wstring = wchar_t*;

Interface_Dll* self = nullptr;
sf::RenderTexture* texture;
sf::Color background_Color = sf::Color(0x000000FF);

Button_Text text_Exit;

unsigned group_Number = 0;
wstring* group_Names = nullptr;
wstring** group_Member_Names = nullptr;

DLL void* init(void* self)
{
	printf("sanitation::init\n");

	::self = (Interface_Dll*)self;
	texture = &::self->get_Texture();

	text_Exit.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Exit.set_Position((float)(1920 - 50), (float)(1080 - 50));
	text_Exit.set_Alignment(Button_Text::Alignment::Right); //ÓÒ¶ÔÆë
	text_Exit.set_Default_Color(sf::Color(0x000000FF));
	text_Exit.set_High_Light_Color(sf::Color(0x666666FF));
	text_Exit.set_Text(L"ÍË³ösanitation.dll");

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
	texture->draw(text_Exit);
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

	printf("sanitation::ened\n");
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
	text_Exit.set_High_Light(text_Exit.is_Clicked({ (float)mouse.x,(float)mouse.y }));
}

void event_Mouse(sf::Event::MouseButtonEvent mouse)
{
	if (text_Exit.is_Clicked({ (float)mouse.x,(float)mouse.y }))
	{
		self->set_Next_Dll(".\\dll\\loader.dll");
		ened();
	}
}
