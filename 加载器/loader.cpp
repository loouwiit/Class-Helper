#include "Interface.h"

#define DLL extern "C" __declspec(dllexport)

DLL void* init(void* self);
DLL void* event(void* param);
DLL void* compute(void* null);
DLL void* draw(void* null);
DLL void* sleep(void* null);

void ened();
void event_Key(sf::Event::KeyEvent key);

using Function = Interface::Function;

sf::Font font;
Interface* self = nullptr;
sf::RenderTexture* texture;

DLL void* init(void* self)
{
	::self = (Interface*)self;
	texture = &::self->get_Texture();

	texture->create(1920, 1080);
	if (!font.loadFromFile("C:\\Windows\\Fonts\\msyh.ttc")) font.loadFromFile("C:\\Windows\\Fonts\\msyh.ttf");

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
	case Event::KeyReleased:
	{
		event_Key(event.key);
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
	return nullptr;
}

DLL void* sleep(void* null)
{
	sf::sleep(sf::milliseconds(16));
	return nullptr;
}

void ened()
{
	self->set_Running(false);
	self->set_Next_Dll(nullptr);
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
