#include "set.h"
#include "deconstructer.h"

//Coordinates

Coordinates::Coordinates() { }

Coordinates::Coordinates(const Coordinates& coordinates)
{
	number = coordinates.number;
	lenght = number;
	this->coordinates = new Coordinate[lenght];
	for (unsigned char i = 0; i < number; i++) this->coordinates[i] = coordinates[i];
}

Coordinates::~Coordinates()
{
	number = 0;
	lenght = 0;
	if (coordinates != nullptr) delete[] coordinates;
}

Coordinates& Coordinates::operator=(Coordinates coordinates)
{
	number = coordinates.number;

	if (number >= lenght)
	{
		//重新分配内存
		lenght = number;
		if (this->coordinates != nullptr) delete[] this->coordinates;
		this->coordinates = new Coordinate[lenght];
	}

	for (unsigned char i = 0; i < number; i++) this->coordinates[i] = coordinates[i];

	return *this;
}

Coordinates& Coordinates::operator+=(Coordinate coordinate)
{
	if (number >= lenght)
	{
		//重新分配内存
		lenght += CooridinateAllocateStep;
		Coordinate* temp = new Coordinate[lenght];
		for (unsigned char i = 0; i < number; i++)
		{
			temp[i] = this->coordinates[i];
		}
		delete[] this->coordinates;
		this->coordinates = temp;
		temp = nullptr;
	}
	
	this->coordinates[number] = coordinate;
	number++;

	return *this;
}

Coordinates Coordinates::operator+(Coordinate coordinate)
{
	Coordinates ret;
	ret = *this;
	ret += coordinate;
	return ret;
}

unsigned char Coordinates::getNumber()
{
	return number;
}

//line

Line::~Line()
{
	number = 0;

	Point* target = head.getNext();
	head.setNext(nullptr);
	if (target != nullptr)
	{
		while (target->getNext() != nullptr)
		{
			target = target->getNext();
			delete target->getLast();
		}
		delete target;
		target = nullptr;
	}
	head.setLast(nullptr);
}

void Line::setLineNumber(unsigned char lineNumber)
{
	this->lineNumber = lineNumber;
}

unsigned char Line::getLineNumber()
{
	return lineNumber;
}

void Line::add(std::wstring name, unsigned char weight)
{
	number++;

	Point* target = &head;
	while (target->getNext() != nullptr) target = target->getNext();
	target->setNext(new Point);
	target->getNext()->setLast(target);
	target = target->getNext();
	head.setLast(target);
	target->setWeight(weight);
	target->setName(name);
	target->setLastLine(lineNumber);
	target->setLastPosition(totolWeight);
	totolWeight += weight;
}

void Line::clear()
{
	if (number == 0) return;

	Point* target = head.getNext();
	head.setNext(nullptr);
	while (target->getNext() != nullptr)
	{
		target = target->getNext();
		delete target->getLast();
	}
	delete target;
	target = nullptr;
	head.setLast(nullptr);
}

void Line::rand()
{
	//Point重载“|”运算符为交换数值

	using Pointer = Point*;

	Pointer* targetPointer = new Pointer[number];
	targetPointer[0] = head.getNext();
	for (unsigned char i = 1; i < number; i++)
		targetPointer[i] = targetPointer[i - 1]->getNext();

	for (unsigned char i = 0; i < number; i++)
		*targetPointer[i] | *targetPointer[::rand() % number]; //交换值

	delete[] targetPointer;
}

void Line::exchange(Coordinate selfCoordinate, Line& targetLine, Coordinate targetCoordinate)
{
	totolWeight = 0; //使缓存失效

	Point* beside = nullptr;
	Point* self = head.getNext();
	Point* target = targetLine.head.getNext();

	//左端
	for (unsigned char i = 0; i < selfCoordinate.position; i++) self = self->getNext(); //假定坐标合法
	for (unsigned char i = 0; i < targetCoordinate.position; i++) target = target->getNext();

	if (self->getLast() != nullptr) self->getLast()->setNext(target);
	if (target->getLast() != nullptr) target->getLast()->setNext(self);

	beside = self->getLast();
	self->setLast(target->getLast());
	target->setLast(beside);

	//右端
	for (unsigned char i = 0; i < selfCoordinate.lenght - 1; i++) self = self->getNext(); //假定坐标合法
	for (unsigned char i = 0; i < targetCoordinate.lenght - 1; i++) target = target->getNext();

	if (self->getNext() != nullptr) self->getNext()->setLast(target);
	if (target->getNext() != nullptr) target->getNext()->setLast(self);

	beside = self->getNext();
	self->setNext(target->getNext());
	target->setNext(beside);

	char delta = targetCoordinate.lenght - selfCoordinate.lenght;
	number += delta;
	targetLine.exchanged(-delta);
}

Coordinates Line::find(unsigned short weight, unsigned char offset)
{
	Coordinates ret;
	unsigned short totolWeight = 0;
	Point* start = head.getNext();
	Point* end = nullptr;
	unsigned char lenght = 1;

	for (unsigned char i = 0; i < offset; i++)
		start = start->getNext();
	end = start;
	totolWeight = start->getWeight();

	while (offset < number)
	{
		if (totolWeight < weight)
		{
			lenght++;
			end = end->getNext();
			if (end == nullptr)
			{
				//越界 不再可能有解 直接退出遍历
				offset = number;
				continue;
			}
			totolWeight += end->getWeight();
		}
		else if (totolWeight > weight)
		{
			lenght--;
			offset++;
			totolWeight -= start->getWeight();
			start = start->getNext();
			if (start == nullptr)
			{
				//越界
				offset = number;
				continue;
			}
		}
		else //totolWeight == weight
		{
			ret += {offset, lenght};
			offset++;
			totolWeight -= start->getWeight();
			start = start->getNext();
			end = end->getNext();
			if (start == nullptr || end == nullptr)
			{
				//越界
				offset = number;
				continue;
			}
			totolWeight += end->getWeight();
		}
	}

	return ret;
}

unsigned short Line::getTotolWeight()
{
	if (totolWeight != 0) return totolWeight;
	for (Point* i = head.getNext(); i != nullptr; i = i->getNext())
		totolWeight += i->getWeight();
	return totolWeight;
}

unsigned char Line::getNumber()
{
	return number;
}

void Line::debug()
{
	//printf("Line::debug: start\n");
	Point* point = head.getNext();
	for (unsigned char i = 0; i < number; i++)
	{
		wprintf(L" (%d,\"%s\")", point->getWeight(), point->getName().c_str());
		point = point->getNext();
	}
	//printf("Line::debug: finish\n");
}

Point& Line::operator[](unsigned char index)
{
	Point* target = head.getNext();
	while (index > 0)
	{
		target = target->getNext();
		index--;
	}
	return *target;
}

File& operator<<(File& stream, Line& line)
{
	constexpr wchar_t space = L' ';
	constexpr wchar_t endl = L'\n';
	Point* point = line.head.getNext();
	while (point != nullptr)
	{
		stream << point->getWeight() << space << point->getName() << endl;
		point = point->getNext();
	}
	return stream;
}

//std::wifstream& operator>>(std::wifstream& stream, Line& line)
//{
//	int weight = 0;
//	std::wstring name;
//
//	stream >> weight;
//	stream >> name;
//
//	line.add(name, (unsigned char)weight);
//	return stream;
//}

void Line::exchanged(char deltaNumber)
{
	number += deltaNumber;
}

//set

Set::~Set()
{
	lineNumber = 0;
	pointNumber = 0;
	pointLenght = 0;
	delete[] lines;
	delete[] names;
	delete[] weights;
}

void Set::setLineNumber(unsigned char number)
{
	if (lineNumber != 0) delete[] lines;
	lineNumber = number;
	lines = new Line[lineNumber];
	for (unsigned char i = 0; i < lineNumber; i++)
		lines[i].setLineNumber(i);
}

unsigned char Set::getLineNumber()
{
	return lineNumber;
}

unsigned short Set::getTotolWeight()
{
	return totolWeight;
}

unsigned short Set::getTotolNumber()
{
	return pointNumber;
}

void Set::setPointReserverNumber(unsigned char number)
{
	if (pointLenght != 0)
	{
		delete[] names;
		delete[] weights;
	}
	pointLenght = number;
	names = new std::wstring[pointLenght];
	weights = new unsigned char[pointLenght];
}

void Set::add(std::wstring name, unsigned char weight)
{
	if (pointNumber >= pointLenght)
	{
		//重新分配内存
		pointLenght += CooridinateAllocateStep;
		std::wstring* tempName = new std::wstring[pointLenght];
		unsigned char* tempWeight = new unsigned char[pointLenght];
		for (unsigned char i = 0; i < pointNumber; i++)
		{
			tempName[i] = names[i];
			tempWeight[i] = weights[i];
		}
		delete[] names;
		delete[] weights;
		names = tempName;
		weights = tempWeight;
		tempName = nullptr;
		tempWeight = nullptr;
	}

	names[pointNumber] = name;
	weights[pointNumber] = weight;

	pointNumber++;
	totolWeight += weight;
}

void Set::build()
{
	printf("Set::build: build start\n");

	//清除line
	for (unsigned char i = 0; i < lineNumber; i++)
		lines[i].clear();

	//添加每一个点
	for (unsigned short i = 0; i < pointNumber; i++)
	{
		lines[i % lineNumber].add(names[i], weights[i]);
	}

	if (!checkBalance()) balance();
	printf("Set::build: build finish\n");
}

void Set::load(const char path[])
{
	wchar_t inputs[10] = L"";
	unsigned short input = 0;
	unsigned char* points = 0;
	Deconstructer deconstructPoints{ [&points]()->void {delete[] points; } };

	File file;
	file.open(path);

	file.ignore(2);//UTF16的字节序
	
	//读取总行数
	file.getline(inputs, 10, L' ');
	setLineNumber((unsigned char)_wtoi(inputs));

	//读取各行数
	points = new unsigned char[lineNumber];
	for (unsigned char i = 0; i < lineNumber; i++)
	{
		file.getline(inputs, 10, L' ');
		points[i] = (unsigned char)_wtoi(inputs);
	}

	//读取各行
	unsigned char weight = 0;
	std::wstring name;
	for (unsigned char i = 0; i < lineNumber; i++)
	{
		lines[i].clear(); //非必要，但冗余
		for (unsigned char j = 0; j < points[i]; j++)
		{
			file.getline(inputs, 10, L' ');
			weight = (unsigned char)_wtoi(inputs);
			file.getline(inputs, 10, L'\n');
			name = inputs;

			lines[i].add(name, weight);
		}
	}

	file.close();
}

void Set::save(const char path[])
{
	constexpr wchar_t space = L' ';
	constexpr wchar_t endl = L'\n';
	File file;
	file.open(path);
	file.addBE();
	file << lineNumber << space;
	for (unsigned char i = 0; i < lineNumber; i++)
		file << lines[i].getNumber() << space;
	file << endl;
	for (unsigned char i = 0; i < lineNumber; i++)
		file << lines[i] << endl;
	file.close();
}

unsigned short Set::rand(RandRisist randRisist, unsigned short maxTimes)
{
	if (maxTimes == 0) return 0; //非法参数

	bool randFinish = false;
	unsigned short swapTimes = 1; //因为下面是小等于
	unsigned short maxSwapWeight = 0;
	unsigned short swapWeight = 0;
	unsigned short totolCoordinateNumber = 0;
	unsigned char* lineCooridinateNumber = new unsigned char[lineNumber]; //由deconstructLineCooridinateNumber回收
	Coordinates* coordinates = new Coordinates[lineNumber]; //由deconstructCoordinates回收

	Deconstructer deconstructCoordinates{ [&coordinates]()->void { delete[] coordinates; coordinates = nullptr; } };
	Deconstructer deconstructLineCooridinateNumber{ [&lineCooridinateNumber]()->void { delete[] lineCooridinateNumber; lineCooridinateNumber = nullptr; } };

	//寻找最大平衡值
	unsigned short lineTotolNumber = 0;
	for (unsigned char i = 0; i < lineNumber; i--)
	{
		lineTotolNumber = lines[i].getTotolWeight();
		if (lineTotolNumber > maxSwapWeight) maxSwapWeight = lineTotolNumber;
	}
	maxSwapWeight--;

	while (!randFinish && swapTimes <= maxTimes)
	{
		swapWeight = maxSwapWeight;
		//进行每一种可能性的交换
		while (swapWeight > 0)
		{
			totolCoordinateNumber = 0;
			//寻找所有可能的解
			for (unsigned char i = 0; i < lineNumber; i++)
			{
				coordinates[i] = lines[i].find(swapWeight);
				lineCooridinateNumber[i] = coordinates[i].getNumber();
				totolCoordinateNumber += lineCooridinateNumber[i];
			}

			//根据coordinates进行交换
			swap(coordinates, lineCooridinateNumber, totolCoordinateNumber, swapWeight); //期间前三个参数均可能发生改变 不保证一致性

			swapWeight--;
		}

		debug();
		//判断是否合理
		swapTimes++;
		randFinish = checkRand(randRisist);
	}

	swapTimes--;
	if (!randFinish) printf("Set::rand: rand filed in %d times\n", swapTimes);
	return swapTimes;
}

void Set::debug()
{
	printf("Set::debug: start\n");
	for (unsigned char i = 0; i < lineNumber; i++)
	{
		printf("\tline %d:", i);
		lines[i].debug();
		printf("\n");
	}
	printf("Set::debug: finish\n");
}

Line& Set::operator[](unsigned char lineIndex)
{
	return lines[lineIndex];
}

bool Set::checkBalance()
{
	unsigned short minimum = totolWeight / lineNumber;
	unsigned short maxmum = minimum + 1;
	unsigned short lineWeight = 0;

	for (unsigned char i = 0; i < lineNumber; i++)
	{
		lineWeight = lines[i].getTotolWeight();
		if (minimum <= lineWeight && lineWeight <= maxmum) continue;
		else return false;
	}
	return true;
}

bool Set::balance()
{
	printf("Set::balance: balance start\n");

	//自动平衡
	unsigned short target = totolWeight / lineNumber;
	unsigned short lineWeight = 0;
	signed short* delta = new signed short[lineNumber]; //由destructWeight回收
	signed balanceWeight = 0;

	Deconstructer destructWeight{ [&delta]()->void { delete[] delta; delta = nullptr; } };

	Coordinates negtiveCoordinates;
	Coordinates positiveCoordinates;

	//查找差值
	for (unsigned char i = 0; i < lineNumber; i++)
		delta[i] = target - lines[i].getTotolWeight();

	//进行平衡
	unsigned char negtive = 0;
	unsigned char positive = 0;
	for (negtive = 0; negtive < lineNumber; negtive++)
	{
		if (delta[negtive] >= 0) continue; //寻找负值
		for (positive = 0; positive < lineNumber; positive++)
		{
			if (delta[positive] <= 0) continue; //寻找正值
			balanceWeight = std::min((signed short)-delta[negtive], delta[positive]);
			negtiveCoordinates = lines[negtive].find(balanceWeight);
			positiveCoordinates = lines[positive].find(balanceWeight);
			while (balanceWeight > 0 &&
				(negtiveCoordinates.getNumber() == 0 || positiveCoordinates.getNumber() == 0))
			{
				balanceWeight--;
				negtiveCoordinates = lines[negtive].find(balanceWeight);
				positiveCoordinates = lines[positive].find(balanceWeight);
			}
			if (balanceWeight == 0)
			{
				//失败
				//寻找下一项可能的正值
				continue;
			}
			//delta[Negtive] += balanceWeight;
			//delta[j] -= balanceWeight;

		}

		if (delta[negtive] < 0 || delta[positive] > 2)
		{
			//此次搜索出现失败
			printf("Set::balance: balance failed\n");
			return false;
		}
	}

	printf("Set::balance: balance succuses\n");
	return true;
}

void Set::swap(Coordinates* coordinates, unsigned char* lineCooridinateNumber, unsigned short& totolCoordinateNumber, unsigned short weight)
{
	unsigned short coordinateAviliableCount = 0;
	unsigned short randIndex = 0;
	unsigned char lineIndex = 0;
	unsigned char targetIndex = 0;

	//printf("Set::swap: swap start\n");

	//debug();

	for (unsigned char i = 0; i < lineNumber; i++)
	{
		//printf("Set::swap: swap in %d line\n", i);

		//对于自己的每一行
		//Coordinates& lineCoordinate = coordinates[i];
		//unsigned char& lineCoordinateCount = lineCooridinateNumber[i];

		//计算潜在的交换数量 1是自己不动
		coordinateAviliableCount = totolCoordinateNumber - lineCooridinateNumber[i] + 1;
		if (coordinateAviliableCount == 0) continue;
		
		//可以交换
		for (unsigned char j = 0; j < lineCooridinateNumber[i]; j++)
		{
			//对于每一行有对象的点

			//生成交换目标 
			randIndex = ::rand() % coordinateAviliableCount; //不包含自己行中的
			if (randIndex == 0) continue; //自己，不用动
			else randIndex--;

			//寻找所在行数
			if (i == 0) lineIndex = 1; //跳过自己
			else lineIndex = 0;
			while (randIndex >= lineCooridinateNumber[lineIndex])
			{
				randIndex -= lineCooridinateNumber[lineIndex];
				lineIndex++;
				if (i == lineIndex) lineIndex++; //跳过自己
			}
			targetIndex = (unsigned char)randIndex; //此时randIndex一定属于unsigned char

			//交换二者
			lines[i].exchange(coordinates[i][j], lines[lineIndex], coordinates[lineIndex][targetIndex]);

			//printf("Set::swap: exchange at [%d][%d] in %d elements\n\t\t  with [%d][%d] in %d elements\n", i, coordinates[i][j].position, coordinates[i][j].lenght, lineIndex, coordinates[lineIndex][targetIndex].position, coordinates[lineIndex][targetIndex].lenght);

			//重新计算二者的交换数据
			totolCoordinateNumber -= lineCooridinateNumber[i];
			coordinates[i] = lines[i].find(weight);
			lineCooridinateNumber[i] = coordinates[i].getNumber();
			totolCoordinateNumber += lineCooridinateNumber[i];

			totolCoordinateNumber -= lineCooridinateNumber[lineIndex];
			coordinates[lineIndex] = lines[lineIndex].find(weight);
			lineCooridinateNumber[lineIndex] = coordinates[lineIndex].getNumber();
			totolCoordinateNumber += lineCooridinateNumber[lineIndex];

			coordinateAviliableCount = totolCoordinateNumber - lineCooridinateNumber[i];

			//debug();

			//[optimize][优化]
			//此处的find用来寻找所有解
			//可以定制为特定解，同时还可以有选择的跳过部分重复项
			//但优化以后再说，慢个三五分钟不是大问题
		}
	}

	//debug();

	//printf("Set::swap: swap finish\n");
}

bool Set::checkRand(RandRisist randRisist)
{
	//列检测
	if (randRisist && RandRisist::DifferLine)
	{
		unsigned char linePointNumber = 0;
		for (unsigned char i =0;i<lineNumber;i++)
		{
			//每一行
			linePointNumber = lines[i].getNumber();
			Line& line = lines[i];
			for (unsigned char j = 0; j < linePointNumber; j++)
			{
				//每一个的列不同于上一次
				if (line[j].getLastLine() == i)
				{
					printf("Set::checkRand: line test filed at (%d, %d)\n", i, j);
					return false;
				}
			}
		}
	}

	//行检测
	if (randRisist && RandRisist::DifferRow)
	{
		unsigned char weight = 0;
		unsigned char linePointNumber = 0;
		unsigned short lastPosition = 0;
		unsigned short nowPosition = 0;
		unsigned short nextPosition = 0;
		for (unsigned char i = 0; i < lineNumber; i++)
		{
			//每一行
			linePointNumber = lines[i].getNumber();
			nowPosition = 0; //位置归零
			nextPosition = 0;
			Line& line = lines[i];

			Point* point = &line[0];
			for (unsigned char j = 0; j < linePointNumber; j++)
			{
				weight = point->getWeight();
				nowPosition = nextPosition; //上一次的next是这一次的now
				nextPosition += weight;
				lastPosition = point->getLastPosition();
				point = point->getNext(); //切到下一个

				//每一个的行不于上一次相交
				//if (lastPosition + weight < nowPosition ||
				//	nextPosition < lastPosition ||
				//	weight == 0) continue;
				if (lastPosition != nowPosition) continue;
				else
				{
					printf("Set::checkRand: row test filed at (%d, %d)\n", i, j);
					return false;
				}
			}
		}
	}

	return true;
}

//element

void Element::setName(sf::String name)
{
	this->text.set_Text(name);
}

sf::String Element::getName()
{
	return text.get_Text().getString();
}

void Element::setPosition(sf::Vector2f position)
{
	text.set_Position(position);
}

void Element::setTextSize(unsigned int size)
{
	text.set_Text_Size(size);
}

void Element::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(text, states);
	//text.draw(target, states);
}

//element line

ElementLine::~ElementLine()
{
	elementNumber = 0;
	if (elements != nullptr)
	{
		delete[] elements;
		elements = nullptr;
	}
}

void ElementLine::load(Line& line, unsigned int textSize)
{
	auto lineNumber = line.getTotolWeight();
	unsigned char linePointNumber = line.getNumber();
	unsigned short index = 0;

	if (elementNumber != lineNumber)
	{
		elementNumber = lineNumber;
		delete[] elements; //delete 支持 nullptr
		elements = new Element[elementNumber];
		for (unsigned char i = 0; i < elementNumber; i++)
			elements[i].setTextSize(textSize);
	}

	Point* target = &line[0];
	unsigned char targetWeight = 0;
	std::wstring targetName;
	for (unsigned char point = 0; point < linePointNumber; point++)
	{
		targetWeight = target->getWeight();
		targetName = target->getName();
		for (unsigned char j = 0; j < targetWeight; j++)
		{
			elements[index].setName(targetName);
			//elements[index].setPosition(sf::Vector2f((float)rand() / RAND_MAX * 10, (float)rand() / RAND_MAX * 10));
			index++;
		}
		target = target->getNext();
	}
}

void ElementLine::setPosition(sf::Vector2f position)
{
	transform.setPosition(position);
}

void ElementLine::setSize(sf::Vector2f size)
{
	sf::Vector2f position = { 0,0 };
	sf::Vector2f delta = { size.x,size.y / (elementNumber - 1) };

	for (unsigned char i = 0; i < elementNumber; i++)
	{
		elements[i].setPosition(position);
		//elements[i].setPosition({ (float)(::rand() % 1000), (float)(::rand() % 1000) });
		position.y += delta.y;
	}
}

void ElementLine::setTextSize(unsigned int size)
{
	for (unsigned char i = 0; i < elementNumber; i++)
		elements[i].setTextSize(size);
}

void ElementLine::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= transform.getTransform();
	for (unsigned char i = 0; i < elementNumber; i++)
		target.draw(elements[i], states);
}

//element set

ElementSet::~ElementSet()
{
	totolWeight = 0;
	totolNumber = 0;
	lineNumber = 0;
	if (elementLines != nullptr)
	{
		delete[] elementLines;
		elementLines = nullptr;
	}
}

void ElementSet::setPosition(sf::Vector2f position)
{
	transform.setPosition(position);
}

void ElementSet::setSize(sf::Vector2f size)
{
	this->size = size;
	arrange();
}

void ElementSet::load(Set& set)
{
	totolWeight = set.getTotolWeight();
	totolNumber = set.getTotolNumber();
	auto LineNumber = set.getLineNumber();

	if (lineNumber != LineNumber)
	{
		lineNumber = LineNumber;
		delete[] elementLines;
		elementLines = new ElementLine[lineNumber];
		printf("ElementSet::load: realloc ram\n");
	}

	for (unsigned char i = 0; i < lineNumber; i++)
		elementLines[i].load(set[i], textSize);
	arrange();
}

void ElementSet::setTextSize(unsigned int size)
{
	textSize = size;
	for (unsigned char i = 0; i < lineNumber; i++)
		elementLines[i].setTextSize(size);
}

unsigned int ElementSet::getTextSize()
{
	return textSize;
}

void ElementSet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= transform.getTransform();
	for (unsigned char i = 0; i < lineNumber; i++)
		target.draw(elementLines[i], states);
}

void ElementSet::arrange()
{
	sf::Vector2f position = { 0,0 };
	sf::Vector2f delta = { size.x / (lineNumber - 1),size.y };

	for (unsigned char i = 0; i < lineNumber; i++)
	{
		elementLines[i].setPosition(position);
		elementLines[i].setSize(delta);
		position.x += delta.x;
	}
}
