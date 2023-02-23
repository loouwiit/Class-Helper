#include <fstream>
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

void seats_Load(const char Path[]);
void seats_Clicked(unsigned index);

using Function = Interface::Function;

sf::Font font;
Interface_Dll* self = nullptr;
sf::RenderTexture* texture;
Button_Text text_Exit;
Button_Text* seats;
unsigned seats_Number = 0;
sf::Color background_Color = sf::Color(0x99FFFFFF);

DLL void* init(void* self)
{
	printf("seat::init\n");

	::self = (Interface_Dll*)self;
	texture = &::self->get_Texture();

	if (!font.loadFromFile("C:\\Windows\\Fonts\\msyh.ttc")) font.loadFromFile("C:\\Windows\\Fonts\\msyh.ttf");

	text_Exit.get_Text().setFont(font);
	text_Exit.get_Text().setString(L"�˳�seat.dll");
	text_Exit.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Exit.get_Text().setPosition((float)(1920 - 50), (float)50);
	text_Exit.get_Text().setOrigin(text_Exit.get_Text().getGlobalBounds().width, 0.0f); //���϶���
	text_Exit.init();
	text_Exit.set_Default_Color(sf::Color(0x0));
	text_Exit.set_High_Light_Color(sf::Color(0x33333366));

	setlocale(LC_ALL, "chs");
	seats_Load(".\\test.test�������̼�");

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

	for (unsigned i = 0; i < seats_Number; i++)
		texture->draw(seats[i]);

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
	if (seats != nullptr)
	{
		seats_Number = 0;
		delete[] seats;
	}

	self->set_Running(false);

	printf("seat::ened\n");
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
	sf::Vector2f mouse_f{ (float)mouse.x,(float)mouse.y };

	text_Exit.set_High_Light(text_Exit.is_Clicked(mouse_f));

	for (unsigned i = 0; i < seats_Number; i++)
		seats[i].set_High_Light(seats[i].is_Clicked(mouse_f));
}

void event_Mouse(sf::Event::MouseButtonEvent mouse)
{
	sf::Vector2f mouse_f{ (float)mouse.x,(float)mouse.y };

	if (text_Exit.is_Clicked(mouse_f))
	{
		self->set_Next_Dll(".\\dll\\loader.dll");
		ened();
	}

	for (unsigned i = 0; i < seats_Number; i++)
		if (seats[i].is_Clicked(mouse_f)) seats_Clicked(i);
}

void seats_Load(const char Path[])
{
	std::ifstream file;
	file.open(Path);
	if (!file.is_open())
	{
		printf("seat::seats_Load:����%sʧ��\n", Path);

		if (seats != nullptr)
		{
			seats_Number = 0;
			delete[] seats;
		}

		wchar_t buffer[200] = L"";
		mbstowcs_s(NULL, buffer, sizeof(buffer) / sizeof(wchar_t), Path, _TRUNCATE);

		seats_Number = 2;
		seats = new Button_Text[seats_Number];

		seats[0].get_Text().setFont(font);
		seats[0].get_Text().setString(L"�ļ���ȡʧ��,·��:");
		seats[0].get_Text().setFillColor(sf::Color(0xFFFFFFFF));
		seats[0].get_Text().setPosition((float)(1920 / 2), (float)(1080 / 2));
		seats[0].get_Text().setOrigin(seats[0].get_Text().getGlobalBounds().width / 2, seats[0].get_Text().getGlobalBounds().height); //�϶���
		seats[0].init();
		seats[0].set_Default_Color(sf::Color(0x0));
		seats[0].set_High_Light_Color(sf::Color(0x33333366));

		seats[1].get_Text().setFont(font);
		seats[1].get_Text().setString(buffer);
		seats[1].get_Text().setFillColor(sf::Color(0xFFFFFFFF));
		seats[1].get_Text().setPosition((float)(1920 / 2), (float)(1080 / 2));
		seats[1].get_Text().setOrigin(seats[1].get_Text().getGlobalBounds().width / 2, 0.0f);//�¶���
		seats[1].init();
		seats[1].set_Default_Color(sf::Color(0x0));
		seats[1].set_High_Light_Color(sf::Color(0x33333366));
		return;
	}

	file.close();
}

void seats_Clicked(unsigned index)
{
	return;
}
