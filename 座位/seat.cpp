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

Interface_Dll* self = nullptr;
sf::RenderTexture* texture;
sf::Color background_Color = sf::Color(0x000000FF);

Button_Text text_Exit;
Button_Text text_Resume;
Button_Text text_Random;
Button_Text text_Apply;

constexpr unsigned un1 = (unsigned)-1;
unsigned clicked_Index = un1;
wchar_t clicked_Last_String[50] = L"";

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

	text_Apply.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Apply.set_Position((float)(1920 - 50), (float)(1080 - 200));
	text_Apply.set_Alignment(Button_Text::Alignment::Right); //右对齐
	text_Apply.set_Default_Color(sf::Color(0x000000FF));
	text_Apply.set_High_Light_Color(sf::Color(0x666666FF));
	text_Apply.set_Text(L"应用更改");

	text_Random.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Random.set_Position((float)(1920 - 50), (float)(1080 -150));
	text_Random.set_Alignment(Button_Text::Alignment::Right); //右对齐
	text_Random.set_Default_Color(sf::Color(0x000000FF));
	text_Random.set_High_Light_Color(sf::Color(0x666666FF));
	text_Random.set_Text(L"随机重排");

	text_Resume.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Resume.set_Position((float)(1920 - 50), (float)(1080 - 100));
	text_Resume.set_Alignment(Button_Text::Alignment::Right); //右对齐
	text_Resume.set_Default_Color(sf::Color(0x000000FF));
	text_Resume.set_High_Light_Color(sf::Color(0x666666FF));
	text_Resume.set_Text(L"恢复原位");

	text_Exit.get_Text().setFillColor(sf::Color(0xFFFFFFFF));
	text_Exit.set_Position((float)(1920 - 50), (float)(1080 - 50));
	text_Exit.set_Alignment(Button_Text::Alignment::Right); //右对齐
	text_Exit.set_Default_Color(sf::Color(0x000000FF));
	text_Exit.set_High_Light_Color(sf::Color(0x666666FF));
	text_Exit.set_Text(L"退出seat.dll");

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

	for (unsigned i = 0; i < seat_Number; i++)
		texture->draw(seats[i]);

	texture->draw(text_Exit);
	texture->draw(text_Resume);
	texture->draw(text_Random);
	texture->draw(text_Apply);
	return nullptr;
}

//DLL void* sleep(void* null)
//{
//	sf::sleep(sf::milliseconds(16));
//	return nullptr;
//}

void ened()
{
	//seat_Save(".\\resources\\seat\\seat.txt");

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
	text_Resume.set_High_Light(text_Resume.is_Clicked(mouse_f));
	text_Random.set_High_Light(text_Random.is_Clicked(mouse_f));
	text_Apply.set_High_Light(text_Apply.is_Clicked(mouse_f));

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

	if (text_Apply.is_Clicked(mouse_f))
	{
		printf("event_Mouse:save\n");
		seat_Save(".\\resources\\seat\\seat.txt");
	}

	if (text_Resume.is_Clicked(mouse_f))
	{
		printf("event_Mouse:resume\n");
		for (unsigned i = 0; i < seat_Active_Number; i++)
		{
			seats[seat_Active_Indexs[i]].set_Text(seat_Strings[seat_Active_Indexs[i]]);
		}
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

		seats[0].get_Text().setFillColor(sf::Color(0xFFFFFFFF));
		seats[0].set_Position((float)(1920 / 2), (float)(1080 / 2));
		seats[0].set_Alignment(Button_Text::Alignment::Top); //上对齐
		seats[0].set_Default_Color(sf::Color(0x000000FF));
		seats[0].set_High_Light_Color(sf::Color(0x666666FF));
		seats[0].set_Text(L"文件读取失败,路径:");

		seats[1].get_Text().setFillColor(sf::Color(0xFFFFFFFF));
		seats[1].set_Position((float)(1920 / 2), (float)(1080 / 2));
		seats[1].set_Alignment(Button_Text::Alignment::Botton); //下对齐
		seats[1].set_Default_Color(sf::Color(0x000000FF));
		seats[1].set_High_Light_Color(sf::Color(0x666666FF));
		seats[1].set_Text(buffer);
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
		seats[i].get_Text().setStyle(sf::Text::Bold);
		seats[i].get_Text().setCharacterSize(72);
		//seats[i].set_Position((float)(1920 / 2 + 100 * (int)(i % 3 - 1)), (float)(1080 / 2 + 100 * (i / 3 - 1)));
		seats[i].set_Default_Color(sf::Color(0x000000FF));
		seats[i].set_High_Light_Color(sf::Color(0x666666FF));
		seats[i].set_Text(L"test");
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
				seats[index].set_Text(L"");
			else
			{
				size_t size = strlen(string) + 1;
				seat_Strings[index] = new wchar_t[size];
				seat_Active_Indexs[seat_Active_Number] = index;
				seat_Active_Number++; //遇到存在数值后加一
				seat_Active_Rows[line]++; //对应行加一
				mbstowcs_s(NULL, seat_Strings[index], size, string, _TRUNCATE);
				seats[index].set_Text(seat_Strings[index]);
			}
			seats[index].set_Position(X, Y);

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
	if (clicked_Index == un1)
	{
		printf("seat::seat_Clicked:ID = %d\n", index);

		clicked_Index = index;
		mbstowcs_s(NULL, clicked_Last_String, sizeof(clicked_Last_String) / sizeof(wchar_t), seats[clicked_Index].get_Text().getString().toAnsiString().c_str(), _TRUNCATE);
		seats[clicked_Index].set_Text("?");
		return;
	}

	//已点击
	printf("seat::seat_Clicked:swap %d and %d\n", clicked_Index, index);

	seats[clicked_Index].set_Text(seats[index].get_Text().getString().toWideString());
	seats[index].set_Text(clicked_Last_String);

	clicked_Index = un1;
	return;
}

void seat_Random()
{
	printf("seat::seat_Random:called\n");

	unsigned change = 0;
	unsigned rand_Position = 0;
	unsigned rand_Times = 0;

	//生成随机
	do
	{
		rand_Times++;

		for (unsigned i = 0; i < seat_Active_Number; i++)
		{
			rand_Position = rand() % seat_Active_Number;

			change = seat_Random_Indexs[i];
			seat_Random_Indexs[i] = seat_Random_Indexs[rand_Position];
			seat_Random_Indexs[rand_Position] = change;
		}
	} while (!seat_Is_Avilible()); //符合条件

	//应用随机
	for (unsigned i = 0; i < seat_Active_Number; i++)
	{
		seats[seat_Active_Indexs[i]].set_Text(seat_Strings[seat_Active_Indexs[seat_Random_Indexs[i]]]);
	}

	printf("seat::seat_Random:rand_Times = %d\n", rand_Times);

	wchar_t buffer[20] = L"";
	swprintf_s(buffer, L"上次随机重排:%d次计算", rand_Times);
	text_Random.set_Text(buffer);

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
	for (unsigned i = 0 ; i < seat_Lines; i++)
		range[i + 1] = range[i] + seat_Active_Rows[i];

	//检验合格性
	unsigned old_Row = 1;
	unsigned now_Row = 1;
	unsigned index_Now = 0;

	for (unsigned index = 0; index < seat_Active_Number; index++)
	{
		//检验每一个的合格性

		old_Row = 1;
		now_Row = 1;
		index_Now = 0;

		while (seat_Random_Indexs[index_Now] != index) index_Now++; //寻找自己的位置
		wprintf(L"seat::seat_Is_Avilible: %s move from %d to %d\n", seat_Strings[seat_Active_Indexs[index]], index, index_Now );

		while (!(range[old_Row - 1] <= index && index < range[old_Row])) old_Row++;
		while (!(range[now_Row - 1] <= index_Now && index_Now < range[now_Row])) now_Row++;

		if (old_Row == now_Row)
		{
			wprintf(L"seat::seat_Is_Avilible: %s is same row:%d\n", seat_Strings [seat_Active_Indexs[index]], old_Row);
			avilible = false;
			break;
		}
	}

	delete[] range;
	range = nullptr;

	return avilible;
}
