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
Set::RandRisist randRisist = Set::RandRisist::DifferNone;
Set set;
ElementSet elementSet;
bool autoRanding = false;

void ened();
void event_Key(sf::Event::KeyEvent key);
void event_Mouse(sf::Event::MouseMoveEvent mouse);
void event_Mouse(sf::Event::MouseButtonEvent mouse);

//Element test[3];
Button_Text text_Exit;
Button_Text text_Rand;
Button_Text text_Auto;
Button_Text text_Big;
Button_Text text_Small;
Button_Text text_Save;
Button_Text text_Mode;

DLL void* init(void* self)
{
	printf("line::init\n");

	srand((unsigned)time(nullptr));

	::self = (Interface_Dll*)self;
	texture = &::self->get_Texture();

	text_Exit.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Exit.set_Position((float)(1920 - 50), (float)(1080 - 50));
	text_Exit.set_Alignment(Button_Text::Alignment::Right); //右对齐
	text_Exit.set_Default_Color(sf::Color(0x000000FF));
	text_Exit.set_High_Light_Color(sf::Color(0x666666FF));
	text_Exit.set_Text(L"退出line.dll");

	text_Rand.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Rand.set_Position((float)(1920 - 50), (float)(1080 - 100));
	text_Rand.set_Alignment(Button_Text::Alignment::Right); //右对齐
	text_Rand.set_Default_Color(sf::Color(0x000000FF));
	text_Rand.set_High_Light_Color(sf::Color(0x666666FF));
	text_Rand.set_Text(L"随机排列");

	text_Auto.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Auto.set_Position((float)(1920 - 50), (float)(1080 - 150));
	text_Auto.set_Alignment(Button_Text::Alignment::Right); //右对齐
	text_Auto.set_Default_Color(sf::Color(0x000000FF));
	text_Auto.set_High_Light_Color(sf::Color(0x666666FF));
	text_Auto.set_Text(L"自动重排");

	text_Big.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Big.set_Position((float)(1920 - 50), (float)(1080 - 200));
	text_Big.set_Alignment(Button_Text::Alignment::Right); //右对齐
	text_Big.set_Default_Color(sf::Color(0x000000FF));
	text_Big.set_High_Light_Color(sf::Color(0x666666FF));
	text_Big.set_Text(L"增大字号");

	text_Small.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Small.set_Position((float)(1920 - 50), (float)(1080 - 250));
	text_Small.set_Alignment(Button_Text::Alignment::Right); //右对齐
	text_Small.set_Default_Color(sf::Color(0x000000FF));
	text_Small.set_High_Light_Color(sf::Color(0x666666FF));
	text_Small.set_Text(L"减少字号");

	text_Save.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Save.set_Position((float)(1920 - 50), (float)(1080 - 300));
	text_Save.set_Alignment(Button_Text::Alignment::Right); //右对齐
	text_Save.set_Default_Color(sf::Color(0x000000FF));
	text_Save.set_High_Light_Color(sf::Color(0x666666FF));
	text_Save.set_Text(L"保存图片");

	text_Mode.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Mode.set_Position((float)(1920 - 50), (float)(1080 - 350));
	text_Mode.set_Alignment(Button_Text::Alignment::Right); //右对齐
	text_Mode.set_Default_Color(sf::Color(0x000000FF));
	text_Mode.set_High_Light_Color(sf::Color(0x666666FF));
	text_Mode.set_Text(L"模式:纯随机");

	//test[0].setName(L"100,100");
	//test[0].setPosition({ 100,100 });
	//test[1].setName("100,200");
	//test[1].setPosition({ 100,200 });
	//test[2].setName("200,100");
	//test[2].setPosition({ 200,100 });

	printf("line::init: set initing\n");
	set.setLineNumber(3);
	set.add(L"0", 3);
	set.add(L"8", 3);
	set.add(L"1", 2);
	set.add(L"3", 1);
	set.add(L"2", 2);
	set.add(L"4", 2);
	set.add(L"5", 2);
	set.add(L"6", 2);
	set.add(L"7", 2);
	set.build();
	//set.rand();
	printf("line::init: set randed\n");
	
	elementSet.load(set);
	elementSet.setPosition({ 100,100 });
	elementSet.setSize({ 1720 - 200, 1080 - 200 });
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

DLL void* compute(void* null)
{
	if (autoRanding)
	{
		unsigned short randTime = 0;
		wchar_t name[12] = L"上次自动:失败";
		randTime = set.rand(randRisist, 128);
		if (randTime != 128)
			swprintf(name, sizeof(name) / sizeof(name[0]), L"上次自动:%d次", randTime);
		elementSet.load(set);
		text_Rand.set_Text(name);
	}
	return nullptr;
}

DLL void* draw(void* null)
{
	texture->clear(background_Color);
	texture->draw(text_Exit);
	texture->draw(text_Rand);
	texture->draw(text_Auto);
	texture->draw(text_Big);
	texture->draw(text_Small);
	texture->draw(text_Save);
	texture->draw(text_Mode);
	texture->draw(elementSet);
	//for (unsigned char i = 0; i < 3; i++) texture->draw(test[i]);
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
	text_Rand.set_High_Light(text_Rand.is_Clicked(mouse_f));
	text_Auto.set_High_Light(text_Auto.is_Clicked(mouse_f) ^ autoRanding);
	text_Big.set_High_Light(text_Big.is_Clicked(mouse_f));
	text_Small.set_High_Light(text_Small.is_Clicked(mouse_f));
	text_Save.set_High_Light(text_Save.is_Clicked(mouse_f));
	text_Mode.set_High_Light(text_Mode.is_Clicked(mouse_f));
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

	if (text_Rand.is_Clicked(mouse_f))
	{
		unsigned short randTime = 0;
		wchar_t name[12] = L"上次重排:失败";
		randTime = set.rand(randRisist, 1024);
		if (randTime != 1024)
			swprintf(name, sizeof(name) / sizeof(name[0]), L"上次重排:%d次", randTime);
		text_Rand.set_Text(name);
		elementSet.load(set);
	}

	if (text_Big.is_Clicked(mouse_f))
	{
		elementSet.setTextSize(elementSet.getTextSize() + 10);
	}

	if (text_Small.is_Clicked(mouse_f))
	{
		if (elementSet.getTextSize() > 20) elementSet.setTextSize(elementSet.getTextSize() - 10);
	}

	if (text_Auto.is_Clicked(mouse_f))
	{
		autoRanding = !autoRanding;
		printf("Set::event: auto randing is %s\n", autoRanding ? "open" : "close");
	}

	if (text_Save.is_Clicked(mouse_f))
	{
		sf::RenderTexture save;
		sf::Sprite sprite;
		sprite.setTexture(texture->getTexture());
		save.create(1920, 1080);
		save.draw(sprite);
		save.getTexture().copyToImage().saveToFile(".\\resources\\line\\line.png");
		printf("Set::event: save\n");
	}

	if (text_Mode.is_Clicked(mouse_f))
	{
		switch (randRisist)
		{
		case Set::RandRisist::DifferNone:
		{
			randRisist = Set::RandRisist::DifferLine;
			text_Mode.set_Text(L"模式:不同列");
			break;
		}
		case Set::RandRisist::DifferLine:
		{
			randRisist = Set::RandRisist::DifferRow;
			text_Mode.set_Text(L"模式:不同行");
			break;
		}
		case Set::RandRisist::DifferRow:
		{
			randRisist = Set::RandRisist::DifferLineAndRow;
			text_Mode.set_Text(L"模式:异行列");
			break;
		}
		case Set::RandRisist::DifferLineAndRow:
		{
			randRisist = Set::RandRisist::DifferNone;
			text_Mode.set_Text(L"模式:纯随机");
			break;
		}
		default:
		{
			printf("event_Mouse: textMode change: error mode %llx", (long long)randRisist);
			text_Mode.set_Text(L"模式:错误");
			break;
		}
		}
	}
}
