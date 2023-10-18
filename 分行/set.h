#include "Button.h"
#include "deconstructer.h"

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

	void setLast(Point* lastPoint) { this->lastPoint = lastPoint; }
	void setNext(Point* nextPoint) { this->nextPoint = nextPoint; }
	Point* getLast() { return this->lastPoint; }
	Point* getNext() { return this->nextPoint; }

	Point& operator=(Point point) { weight = point.weight; name = point.name; }
	void operator|(Point& point) { unsigned char selfweight = weight; weight = point.weight; point.weight = selfweight; name.swap(point.name); } //交换数值

private:
	unsigned char weight = 1;
	std::wstring name;
	Point* nextPoint = nullptr;
	Point* lastPoint = nullptr;
};

class Line
{
public:
	~Line();

	void add(std::wstring name, unsigned char weight);
	void clear();
	void rand();
	void exchange(Coordinate selfCoordinate, Line& targetLine, Coordinate targetCoordinate);
	Coordinates find(unsigned short weight, unsigned char offset = 0);
	unsigned short getTotolWeight();
	unsigned char getNumber();

	Point& operator[](unsigned char index);

private:
	unsigned short totolWeight = 0;
	unsigned char number = 0;
	Point head; //链表
};

class Set
{
public:
	~Set();

	void setLineNumber(unsigned char number);
	unsigned char getLineNumber();
	unsigned short getTotolWeight();
	unsigned short getTotolNumber();
	void setPointReserverNumber(unsigned char number);
	void add(std::wstring name, unsigned char weight = 1);
	void build();
	void rand();

	Line& operator[](unsigned char lineIndex);

private:
	bool checkBalance();
	bool balance();
	void swap(Coordinates* coordinates, unsigned char* lineCooridinateNumber, unsigned short& totolCoordinateNumber, unsigned short weight);

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

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	Button_Text text;
};

class ElementLine : public sf::Drawable
{
public:
	~ElementLine();
	void load(Line& line);

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	unsigned char elementNumber = 0;
	Element* elements = nullptr;
	sf::Transformable transform;
};

class ElementSet : public sf::Drawable
{
public:
	~ElementSet();
	void setPosition(sf::Vector2f position);
	void load(Set& set);

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	unsigned short totolWeight = 0;
	unsigned short totolNumber = 0;
	unsigned char lineNumber = 0;
	ElementLine* elementLines = nullptr;
	sf::Transformable transform;
};
