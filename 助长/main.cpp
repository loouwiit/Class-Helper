//����

/*
* 0.0.0.1 �����ļ����߼�
* 0.0.0.2 ��Ⱦloader�Ļ���
* 0.0.0.3 ����Interface��dll����
*/

#include "Interface.h"

#pragma region main��������
#ifdef _DEBUG
#define MAIN main(int argc, char* argv[])
#else
#ifdef WIN32
#define MAIN __stdcall WinMain(void* hInstance, void* hPrevInstance, void* lpCmdLine, int nCmdShow)
#else
#define MAIN WinMain(int argc, char* argv[])
#endif
#endif
#pragma endregion

constexpr wchar_t window_Name[] = L"����";

sf::RenderWindow window;
sf::View view;
sf::Sprite fream;
sf::Font font;
Interface_Dll menu;

int MAIN;
void init();
void event(sf::Event event);
void compute();
void draw();

void draw_Loading();

int MAIN
{
	init();
	while (window.isOpen())
	{
		static sf::Event _event;
		while (window.pollEvent(_event)) event(_event);
		compute();
		draw();

		menu.sleep();
	}
	return 0;
}

void init()
{
	if (!font.loadFromFile("C:\\Windows\\Fonts\\msyh.ttc")) font.loadFromFile("C:\\Windows\\Fonts\\msyh.ttf");
	window.create(sf::VideoMode(1280, 720), window_Name);
	view.setSize(1920, 1080);
	view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
	window.setView(view);

	fream.setScale(1, -1);
	fream.setPosition(0, 1080);
	fream.setTexture(menu.get_Texture().getTexture());

	draw_Loading();
	
	menu.load(".\\dll\\loader.dll");
	menu.init();
}

void event(sf::Event event)
{
	using Event = sf::Event;
	switch (event.type)
	{
	case Event::Closed:
	{
		window.close();
		break;
	}
	case Event::Resized:
	{
		sf::Vector2u size{ event.size.width / 2,event.size.height / 2 };
		size.x += event.size.height / 9 * 16 / 2;
		size.y += event.size.width / 16 * 9 / 2;
		window.setSize(size);
		return;
	}
	case Event::MouseMoved:
	{
		sf::Vector2f temp = window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y });
		event.mouseMove.x = (int)temp.x;
		event.mouseMove.y = (int)temp.y;
		break;
	}
	case Event::MouseButtonPressed:
	case Event::MouseButtonReleased:
	{
		sf::Vector2f temp = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
		event.mouseButton.x = (int)temp.x;
		event.mouseButton.y = (int)temp.y;
		break;
	}
	case Event::MouseWheelMoved:
	case Event::MouseWheelScrolled:
	{
		sf::Vector2f temp = window.mapPixelToCoords({ event.mouseWheel.x, event.mouseWheel.y });
		event.mouseWheel.x = (int)temp.x;
		event.mouseWheel.y = (int)temp.y;
		break;
	}
	default:
	{
		break;
	}
	}

	menu.event(event); //���¼����͸�dll�������跢������swtich��ֱ��return
}

void compute()
{
	menu.compute();

	if (menu.is_Running() == false)
	{
		//��������
		if (menu.get_Next_Dll() == nullptr)
		{
			//û����һ��Dll
			printf("main::compute:δָ����һ��dll�������˳�\n");
			window.close();
			menu.load(nullptr);
		}
		else
		{
			//������һ��Dll
			draw_Loading();
			menu.load(menu.get_Next_Dll());
			menu.set_Next_Dll(nullptr);
			menu.init();
			menu.compute(); //��֤��drawǰ��һ��compute
		}
	}
}

void draw()
{
	menu.draw();
	//window.clear();
	window.draw(fream);
	window.display();
}

void draw_Loading()
{
	sf::Text text;
	text.setFont(font);
	text.setFillColor(sf::Color(0xFFFFFFFF));
	text.setString(L"�����С���");
	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	text.setPosition((float)(view.getSize().x / 2), (float)(view.getSize().y / 2));

	//sf::RectangleShape rectangle;
	//sf::FloatRect rect = text.getGlobalBounds();
	//rectangle.setPosition(rect.left, rect.top);
	//rectangle.setSize({ rect.width, rect.height });
	//rectangle.setFillColor(sf::Color(0x777777FF));

	window.clear();
	window.draw(text);
	window.display();
}
