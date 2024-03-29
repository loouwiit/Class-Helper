#include "Button.h"
#include "file.h"
#include <fstream>

constexpr unsigned char CooridinateAllocateStep = 4;
constexpr unsigned char LineBufferAllocateStep = 8;
constexpr unsigned char SetPointAllocateStep = 4;

struct Coordinate
{
	unsigned char position = 0;
	unsigned char lenght = 0;

	Coordinate() = default;
	Coordinate(unsigned char position, unsigned char lenght) { this->position = position; this->lenght = lenght; }
};

class Coordinates
{
public:
	Coordinates();
	Coordinates(const Coordinates& coordinates);
	~Coordinates();

	Coordinates& operator=(Coordinates coordinates);
	Coordinates& operator+=(Coordinate coordinate);
	//Coordinates& operator+=(Coordinates coordinates);
	Coordinates operator+(Coordinate coordinate);
	//Coordinates operator+(Coordinates coordinates);

	const Coordinate& operator[](unsigned char index) const { return coordinates[index]; }
	Coordinate& operator[](unsigned char index) { return coordinates[index]; }
	unsigned char getNumber();

private:
	unsigned char number = 0;
	unsigned char lenght = 0;
	Coordinate* coordinates = nullptr; //数组指针
};

class Point
{
public:
	unsigned char getWeight() { return weight; }
	void setWeight(unsigned char weight) { this->weight = weight; }

	std::wstring getName() { return name; }
	void setName(std::wstring name) { this->name = name; }

	void setLastLine(unsigned char lastLine) { this->lastLine = lastLine; }
	unsigned char getLastLine() { return lastLine; }

	void setLastPosition(unsigned short lastPosition) { this->lastPosition = lastPosition; }
	unsigned short getLastPosition() { return lastPosition; }

	void setLast(Point* lastPoint) { this->lastPoint = lastPoint; }
	void setNext(Point* nextPoint) { this->nextPoint = nextPoint; }
	Point* getLast() { return this->lastPoint; }
	Point* getNext() { return this->nextPoint; }

	Point& operator=(Point point) { weight = point.weight; name = point.name; lastLine = point.lastLine; lastPosition = point.lastPosition; }
	void operator|(Point& point)
	{
		unsigned char selfweight = weight;
		unsigned char selfLastLine = lastLine;
		unsigned short selfLastPosition = lastPosition;

		weight = point.weight; point.weight = selfweight;
		name.swap(point.name);
		lastLine = point.lastLine; point.lastLine = selfLastLine;
		lastPosition = point.lastPosition; point.lastPosition = selfLastPosition;
	} //交换数值

private:
	unsigned char weight = 0;
	std::wstring name{ L"noname" };
	unsigned char lastLine = -1;
	unsigned short lastPosition = -1;
	Point* nextPoint = nullptr;
	Point* lastPoint = nullptr;
};

class Line
{
public:
	~Line();

	void setLineNumber(unsigned char lineNumber);
	unsigned char getLineNumber();

	void add(std::wstring name, unsigned char weight);
	void clear();
	void rand();
	void exchange(Coordinate selfCoordinate, Line& targetLine, Coordinate targetCoordinate);
	Coordinates find(unsigned short weight, unsigned char offset = 0);
	unsigned short getTotolWeight();
	unsigned char getNumber();

	void debug();

	Point& operator[](unsigned char index);

	friend File& operator<<(File& stream, Line& line);
	//friend std::wifstream& operator>>(std::wifstream& stream, Line& line);

protected:
	void exchanged(char deltaNumber);

private:
	unsigned short totolWeight = 0;
	unsigned char lineNumber = -1;
	unsigned char number = 0;
	Point head; //链表
};

class Set
{
public:
	~Set();

	enum class RandRisist : unsigned char
	{
		//[future]后期功能，以后再说
		//InLine = 1 << 0,	//启动列交换
		//InRow = 1 << 1,		//启动行交换
		//InLineAndRow = InLine | InRow,
		//InAll = InLineAndRow,

		DifferNone = 0,
		DifferLine = 1 << 2,	//启动同列检测
		DifferRow = 1 << 3,		//启动同行检测
		DifferLineAndRow = DifferLine | DifferRow,
		DifferAll = DifferLineAndRow,
	};

	constexpr static unsigned short RandMaxTime = (unsigned short)-1 - 1;

	void setLineNumber(unsigned char number);
	unsigned char getLineNumber();
	unsigned short getTotolWeight();
	unsigned short getTotolNumber();
	void setPointReserverNumber(unsigned char number);
	void add(std::wstring name, unsigned char weight = 1);
	void build();

	void load(const char path[]);
	void save(const char path[]);

	unsigned short rand(RandRisist randRisist, unsigned short maxTimes = RandMaxTime);
	void debug();

	Line& operator[](unsigned char lineIndex);

private:
	bool checkBalance();
	bool balance();
	void swap(Coordinates* coordinates, unsigned char* lineCooridinateNumber, unsigned short& totolCoordinateNumber, unsigned short weight);
	bool checkRand(RandRisist randRisist);

	unsigned char lineNumber = 0;
	unsigned short totolWeight = 0;
	unsigned char pointNumber = 0;
	unsigned char pointLenght = 0;
	Line* lines = nullptr; //数组指针
	std::wstring* names = nullptr;
	unsigned char* weights = nullptr;
};

class Element : public sf::Drawable
{
public:
	void setName(sf::String name);
	sf::String getName();

	void setPosition(sf::Vector2f position);
	void setTextSize(unsigned int size);

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	Button_Text text;
};

class ElementLine : public sf::Drawable
{
public:
	~ElementLine();
	void load(Line& line, unsigned int textSize);
	void setPosition(sf::Vector2f position);
	void setSize(sf::Vector2f size);
	void setTextSize(unsigned int size);

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	unsigned short elementNumber = 0;
	Element* elements = nullptr;
	sf::Transformable transform;
};

class ElementSet : public sf::Drawable
{
public:
	~ElementSet();
	void setPosition(sf::Vector2f position);
	void setSize(sf::Vector2f size);
	void load(Set& set);
	void setTextSize(unsigned int size);
	unsigned int getTextSize();

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	void arrange();

	sf::Vector2f size;
	unsigned int textSize = Button_Text::get_Default_Text_Size();

	unsigned short totolWeight = 0;
	unsigned short totolNumber = 0;
	unsigned char lineNumber = 0;
	ElementLine* elementLines = nullptr;
	sf::Transformable transform;
};

#include "set.inl"
