#include <string>
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

void seat_Load(const char Path[]);
void seat_Save(const char Path[]);
bool seat_Is_Good();
void seat_Clicked(unsigned index);
void seat_Random();
bool seat_Is_Avilible();

using Function = Interface::Function;

Interface_Dll* self = nullptr;
sf::RenderTexture* texture;
sf::Color background_Color = sf::Color(0x000000FF);

Button_Text text_Exit;
Button_Text text_Random;
Button_Text* seats;
unsigned seat_Number = 0;
unsigned seat_Lines = 0;
unsigned* seat_Rows = nullptr; //seat_Lines个
wchar_t** seat_Strings = nullptr; //seat_Number个

unsigned seat_Active_Number = 0; //有数据的数量
unsigned* seat_Active_Indexs = nullptr; //有数据的映射表
unsigned* seat_Active_Rows = nullptr; //每行中有数据的个数
unsigned* seat_Random_Indexs = nullptr; //随机用的映射表


DLL void* init(void* self)
{
	printf("seat::init\n");

	::self = (Interface_Dll*)self;
	texture = &::self->get_Texture();

	text_Exit.get_Text().setString(L"退出seat.dll");
	text_Exit.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Exit.get_Text().setPosition((float)(1920 - 50), (float)50);
	text_Exit.set_Alignment(Button_Text::Alignment::Right | Button_Text::Alignment::Top); //右上对齐
	text_Exit.init();
	text_Exit.set_Default_Color(sf::Color(0x000000FF));
	text_Exit.set_High_Light_Color(sf::Color(0x666666FF));

	text_Random.get_Text().setString(L"随机重排");
	text_Random.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Random.get_Text().setPosition((float)(1920 - 50), (float)100);
	text_Random.set_Alignment(Button_Text::Alignment::Right | Button_Text::Alignment::Top); //右上对齐
	text_Random.init();
	text_Random.set_Default_Color(sf::Color(0x000000FF));
	text_Random.set_High_Light_Color(sf::Color(0x666666FF));

	srand((unsigned)time(nullptr));
	setlocale(LC_ALL, "chs");
	seat_Load(".\\resources\\seat\\seat.txt");

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

	for (unsigned i = 0; i < seat_Number; i++)
		texture->draw(seats[i]);

	texture->draw(text_Exit);
	texture->draw(text_Random);
	return nullptr;
}

//DLL void* sleep(void* null)
//{
//	sf::sleep(sf::milliseconds(16));
//	return nullptr;
//}

void ened()
{
	seat_Save(".\\resources\\seat\\seat.txt");

	if (seats != nullptr)
	{
		seat_Number = 0;
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
	text_Random.set_High_Light(text_Random.is_Clicked(mouse_f));

	for (unsigned i = 0; i < seat_Number; i++)
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

	if (text_Random.is_Clicked(mouse_f))
	{
		seat_Random();
	}

	for (unsigned i = 0; i < seat_Number; i++)
		if (seats[i].is_Clicked(mouse_f)) seat_Clicked(i);
}

void seat_Load(const char Path[])
{
	std::ifstream file;

		if (seats != nullptr) delete[] seats;
		if (seat_Active_Indexs != nullptr) delete[] seat_Active_Indexs;
		for (unsigned i = 0; i < seat_Number; i++)
			if (seat_Strings[i] != nullptr) delete[] seat_Strings[i];
		if (seat_Strings != nullptr) delete[] seat_Strings;

	file.open(Path);
	if (!file.is_open())
	{
		printf("seat::seat_Load:加载%s失败\n", Path);

		seat_Number = 2;

		seats = new Button_Text[seat_Number];
		seat_Active_Indexs = new unsigned[seat_Number];
		for (unsigned i = 0; i < seat_Number; i++) seat_Active_Indexs = 0;
		seat_Strings = new wchar_t* [seat_Number];
		for (unsigned i = 0; i < seat_Number; i++) seat_Strings[i] = nullptr;

		seat_Lines = 0;
		seat_Rows = nullptr;

		wchar_t buffer[200] = L"";
		mbstowcs_s(NULL, buffer, sizeof(buffer) / sizeof(wchar_t), Path, _TRUNCATE);

		seats[0].get_Text().setString(L"文件读取失败,路径:");
		seats[0].get_Text().setFillColor(sf::Color(0xFFFFFFFF));
		seats[0].get_Text().setPosition((float)(1920 / 2), (float)(1080 / 2));
		seats[0].set_Alignment(Button_Text::Alignment::Top); //上对齐
		seats[0].init();
		seats[0].set_Default_Color(sf::Color(0x000000FF));
		seats[0].set_High_Light_Color(sf::Color(0x666666FF));

		seats[1].get_Text().setString(buffer);
		seats[1].get_Text().setFillColor(sf::Color(0xFFFFFFFF));
		seats[1].get_Text().setPosition((float)(1920 / 2), (float)(1080 / 2));
		seats[1].set_Alignment(Button_Text::Alignment::Botton); //下对齐
		seats[1].init();
		seats[1].set_Default_Color(sf::Color(0x000000FF));
		seats[1].set_High_Light_Color(sf::Color(0x666666FF));
		return;
	}


	file >> seat_Number;
	if (file.eof())
	{
		printf("文件是空文件\n");
		return;
	}

	seats = new Button_Text[seat_Number];
	seat_Active_Indexs = new unsigned[seat_Number];
	for (unsigned i = 0; i < seat_Number; i++) seat_Active_Indexs[i] = 0;
	seat_Strings = new wchar_t* [seat_Number];
	for (unsigned i = 0; i < seat_Number; i++) seat_Strings[i] = nullptr;

	for (unsigned i = 0; i < seat_Number; i++)
	{
		seats[i].get_Text().setFillColor(sf::Color(0xFFFFFFFF));
		seats[i].get_Text().setString(L"test");
		seats[i].get_Text().setStyle(sf::Text::Bold);
		seats[i].get_Text().setCharacterSize(72);
		//seats[i].get_Text().setPosition((float)(1920 / 2 + 100 * (int)(i % 3 - 1)), (float)(1080 / 2 + 100 * (i / 3 - 1)));
		seats[i].set_Default_Color(sf::Color(0x000000FF));
		seats[i].set_High_Light_Color(sf::Color(0x666666FF));
		seats[i].init();
	}

	char string[50] = "";

	unsigned index = 0;
	float delta_X = 0;
	float delta_Y = 0;
	float X = 100;
	float Y = 100;

	seat_Active_Number = 0;

	if (seat_Rows != nullptr) delete[] seat_Rows;

	file >> seat_Lines;
	seat_Rows = new unsigned[seat_Lines];
	seat_Active_Rows = new unsigned[seat_Lines];
	delta_Y = (float)(1080 / seat_Lines);

	for (unsigned line = 0; line < seat_Lines; line++)
	{
		file >> seat_Rows[line];
		seat_Active_Rows[line] = 0;
		delta_X = (float)(1620 / seat_Rows[line]);
		X = 100;

		for (unsigned row = 0; row < seat_Rows[line]; row++)
		{
			if (index >= seat_Number)
			{
				//异常
				printf("seat::seat_Load:索引错误，seat_Number = %d，而index = %d", seat_Number, index);
				break;
				break;
			}

			file >> string;
			if (string[0] == '-' && string[1] == '\0')
				seats[index].get_Text().setString(L"");
			else
			{
				size_t size = strlen(string) + 1;
				seat_Strings[index] = new wchar_t[size];
				seat_Active_Indexs[seat_Active_Number] = index;
				seat_Active_Number++; //遇到存在数值后加一
				seat_Active_Rows[line]++; //对应行加一
				mbstowcs_s(NULL, seat_Strings[index], size, string, _TRUNCATE);
				seats[index].get_Text().setString(seat_Strings[index]);
			}
			seats[index].get_Text().setPosition(X, Y);
			seats[index].init();

			index++;
			X += delta_X;
		}

		Y += delta_Y;
	}
	
	seat_Random_Indexs = new unsigned[seat_Number];
	for (unsigned i = 0; i < seat_Active_Number; i++) //所有正确的数值
		seat_Random_Indexs[i] = i;
	for (unsigned i = seat_Active_Number; i < seat_Number; i++) //剩下的填0
		seat_Random_Indexs[i] = 0;

	//[debug]
	//wprintf(L"seat::seat_Load:seat_Number = %d\n", seat_Number);
	//for (unsigned i = 0; i < seat_Number; i++)
	//	wprintf(L"seat::seat_Load:seat_strings[%d] = %s\n", i, seat_Strings[i]);
	//for (unsigned i = 0; i < seat_Number; i++)
	//	wprintf(L"seat::seat_Load:seat_Active_Indexs[%d] = %d\n", i, seat_Active_Indexs[i]);
	//for (unsigned i = 0; i < seat_Number; i++)
	//	wprintf(L"seat::seat_Load:seat_Random_Indexs[%d] = %d\n", i, seat_Random_Indexs[i]);

	file.close();
}

void seat_Save(const char Path[])
{
	constexpr char space = ' ';
	constexpr char endl = '\n';

	if (!seat_Is_Good())
	{
		printf("seat::seat_Save:数据结构错误\n");
		return;
	}

	std::ofstream file;

	file.open(Path);
	if (!file.is_open())
	{
		printf("seat::seat_Save:打开%s失败\n", Path);
		return;
	}

	unsigned index = 0;
	std::string string;

	file << seat_Number << space << seat_Lines << endl;
	for (unsigned line = 0; line < seat_Lines; line++)
	{
		file << seat_Rows[line] << space;
		for (unsigned row = 0; row < seat_Rows[line]; row++)
		{
			string = seats[index].get_Text().getString().toAnsiString();
			if (string[0] == '\0') file << '-' << space;
			else file << string << space;
			index++;
		}
		file << endl;
	}

	file.close();
}

bool seat_Is_Good()
{
	unsigned answer = 0;
	for (unsigned i = 0; i < seat_Lines; i++)
		answer += seat_Rows[i];

	return answer == seat_Number;
}

void seat_Clicked(unsigned index)
{
	return;
}

void seat_Random()
{
	printf("seat::seat_Random:called\n");

	unsigned buffer = 0;
	unsigned rand_Position = 0;
	unsigned rand_Times = 0;

	//生成随机
	do
	{
		rand_Times++;

		for (unsigned i = 0; i < seat_Active_Number; i++)
		{
			rand_Position = rand() % seat_Active_Number;

			buffer = seat_Random_Indexs[i];
			seat_Random_Indexs[i] = seat_Random_Indexs[rand_Position];
			seat_Random_Indexs[rand_Position] = buffer;
		}
	} while (!seat_Is_Avilible()); //符合条件

	//应用随机
	for (unsigned i = 0; i < seat_Active_Number; i++)
	{
		seats[seat_Active_Indexs[i]].get_Text().setString(seat_Strings[seat_Active_Indexs[seat_Random_Indexs[i]]]);
	}

	printf("seat::seat_Random:rand_Times = %d\n", rand_Times);

	//[debug]
	//for (unsigned i = 0; i < seat_Number; i++)
	//{
	//	printf("seat_Random:seat_Random_Indexs[%d] = %d\n", i, seat_Random_Indexs[i]);
	//	wprintf(L"seat_Random:随机后的字符;%s\n", seat_Strings[seat_Active_Indexs[seat_Random_Indexs[i]]]);
	//}
}

bool seat_Is_Avilible()
{
	unsigned* range = new unsigned[seat_Lines + 1]; //每行的取值范围
	bool avilible = true;

	//生成对应区间
	range[0] = 0;
	for (unsigned i = 1 ; i < seat_Lines; i++)
		range[i] = range[i - 1] + seat_Active_Rows[i];

	//检验合格性
	unsigned old = 1;
	unsigned now = 1;

	for (unsigned index = 0; index < seat_Active_Number; index++)
	{
		//检验每一个的合格性

		if (seat_Strings[index] == nullptr) continue; //空，不参与运算，直接下一个

		old = 1;
		now = 1;

		while (!(range[old - 1] <= index && index <= range[old])) old++; //区间有交集，防止0出错误，同时不影响后面。
		while (!(range[now - 1] <= seat_Random_Indexs[index] && seat_Random_Indexs[index] <= range[now])) now++;

		if (old == now)
		{
			avilible = false;
			break;
		}
	}

	delete[] range;
	range = nullptr;

	return avilible;
}
