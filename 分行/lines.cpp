#include "Interface.h"
#include "Button.h"
#include "set.h"

#define DLL extern "C" __declspec(dllexport)

DLL void* init(void* self);
DLL void* event(void* param);
DLL void* compute(void* null);
DLL void* draw(void* null);

Interface_Dll* self = nullptr;
sf::RenderTexture* texture;
sf::Color background_Color = sf::Color(0x000000FF);
Set set;
ElementSet elementSet;

void ened();
void event_Key(sf::Event::KeyEvent key);
void event_Mouse(sf::Event::MouseMoveEvent mouse);
void event_Mouse(sf::Event::MouseButtonEvent mouse);

Element test[3];
Button_Text text_Exit;

DLL void* init(void* self)
{
	printf("line::init\n");

	srand((unsigned)time(nullptr));

	::self = (Interface_Dll*)self;
	texture = &::self->get_Texture();

	text_Exit.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Exit.set_Position((float)(1920 - 50), (float)(1080 - 50));
	text_Exit.set_Alignment(Button_Text::Alignment::Right); //ÓÒ¶ÔÆë
	text_Exit.set_Default_Color(sf::Color(0x000000FF));
	text_Exit.set_High_Light_Color(sf::Color(0x666666FF));
	text_Exit.set_Text(L"ÍË³öline.dll");

	test[0].setName(L"TEST");
	test[0].setPosition({ 900,100 });
	test[1].setName("1st");
	test[1].setPosition({ 900,150 });
	test[2].setName("2nd");
	test[2].setPosition({ 900,200 });

	printf("line::init: set initing\n");
	set.setLineNumber(3);
	set.add(L"A", 3);
	set.add(L"B", 2);
	set.add(L"C", 2);
	set.add(L"D", 1);
	set.add(L"E", 1);
	set.add(L"F", 2);
	set.build();
	set.rand();
	printf("line::init: set randed\n");
	
	elementSet.load(set);
	printf("line::init: set loaded\n");
	
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

DLL void* compute(void* null) { return nullptr; }

DLL void* draw(void* null)
{
	texture->clear(background_Color);
	texture->draw(text_Exit);
	texture->draw(elementSet);
	for (unsigned char i = 0; i < 3; i++) texture->draw(test[i]);
	return nullptr;
}

void ened()
{
	self->set_Running(false);

	printf("line::ened\n");
}

void event_Key(sf::Event::KeyEvent key)
{
	using Key = sf::Keyboard;

	switch (key.code)
	{
	case Key::Escape:
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
	//move
	sf::Vector2f mouse_f{ (float)mouse.x, (float)mouse.y };

	text_Exit.set_High_Light(text_Exit.is_Clicked(mouse_f));
}

void event_Mouse(sf::Event::MouseButtonEvent mouse)
{
	//click
	sf::Vector2f mouse_f{ (float)mouse.x, (float)mouse.y };

	if (text_Exit.is_Clicked(mouse_f))
	{
		self->set_Next_Dll(".\\dll\\loader.dll");
		ened();
	}
}
