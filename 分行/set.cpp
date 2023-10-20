#include "set.h"

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
		//���·����ڴ�
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
		//���·����ڴ�
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

void Line::add(std::wstring name, unsigned char weight)
{
	number++;
	totolWeight = 0;

	Point* target = &head;
	while (target->getNext() != nullptr) target = target->getNext();
	target->setNext(new Point);
	target->getNext()->setLast(target);
	target = target->getNext();
	head.setLast(target);
	target->setWeight(weight);
	target->setName(name);
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
	//Point���ء�|�������Ϊ������ֵ

	using Pointer = Point*;

	Pointer* targetPointer = new Pointer[number];
	targetPointer[0] = head.getNext();
	for (unsigned char i = 1; i < number; i++)
		targetPointer[i] = targetPointer[i - 1]->getNext();

	for (unsigned char i = 0; i < number; i++)
		*targetPointer[i] | *targetPointer[::rand() % number]; //����ֵ

	delete[] targetPointer;
}

void Line::exchange(Coordinate selfCoordinate, Line& targetLine, Coordinate targetCoordinate)
{
	totolWeight = 0; //ʹ����ʧЧ

	Point* beside = nullptr;
	Point* self = head.getNext();
	Point* target = targetLine.head.getNext();

	//���
	for (unsigned char i = 0; i < selfCoordinate.position; i++) self = self->getNext(); //�ٶ�����Ϸ�
	for (unsigned char i = 0; i < targetCoordinate.position; i++) target = target->getNext();

	if (self->getLast() != nullptr) self->getLast()->setNext(target);
	if (target->getLast() != nullptr) target->getLast()->setNext(self);

	beside = self->getLast();
	self->setLast(target->getLast());
	target->setLast(beside);

	//�Ҷ�
	for (unsigned char i = 0; i < selfCoordinate.lenght - 1; i++) self = self->getNext(); //�ٶ�����Ϸ�
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
				//Խ�� ���ٿ����н� ֱ���˳�����
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
				//Խ��
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
				//Խ��
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
	printf("Line::debug: start\n");
	Point* point = head.getNext();
	for (unsigned char i = 0; i < number; i++)
	{
		printf("\tpoint[%d] = { %d, %s }\n", i, point->getWeight(), (const char*)point->getName().c_str());
		point = point->getNext();
	}
	printf("Line::debug: finish\n");
}

Point& Line::operator[](unsigned char index)
{
	Point* target = head.getNext();
	while (index > 0)
		target = target->getNext();
	return *target;
}

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
		//���·����ڴ�
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

	//���line
	for (unsigned char i = 0; i < lineNumber; i++)
		lines[i].clear();

	//���ÿһ����
	for (unsigned short i = 0; i < pointNumber; i++)
	{
		lines[i % lineNumber].add(names[i], weights[i]);
	}

	if (!checkBalance()) balance();
	printf("Set::build: build finish\n");
}

void Set::rand()
{
	unsigned short swapWeight = 0;
	unsigned short totolCoordinateNumber = 0;
	unsigned char* lineCooridinateNumber = new unsigned char[lineNumber]; //��deconstructLineCooridinateNumber����
	Coordinates* coordinates = new Coordinates[lineNumber]; //��deconstructCoordinates����

	Deconstructer deconstructCoordinates{ [&coordinates]()->void { delete[] coordinates; coordinates = nullptr; } };
	Deconstructer deconstructLineCooridinateNumber{ [&lineCooridinateNumber]()->void { delete[] lineCooridinateNumber; lineCooridinateNumber = nullptr; } };

	//Ѱ�����ƽ��ֵ
	unsigned short lineTotolNumber = 0;
	for (unsigned char i = 0; i < lineNumber; i--)
	{
		lineTotolNumber = lines[i].getTotolWeight();
		if (lineTotolNumber > swapWeight) swapWeight = lineTotolNumber;
	}
	swapWeight--;

	while (swapWeight > 0)
	{
		totolCoordinateNumber = 0;
		//Ѱ�����п��ܵĽ�
		for (unsigned char i = 0; i < lineNumber; i++)
		{
			coordinates[i] = lines[i].find(swapWeight);
			lineCooridinateNumber[i] = coordinates[i].getNumber();
			totolCoordinateNumber += lineCooridinateNumber[i];
		}

		//����coordinates���н���
		swap(coordinates, lineCooridinateNumber, totolCoordinateNumber, swapWeight); //�ڼ�ǰ�������������ܷ����ı� ����֤һ����

		swapWeight--;
	}
}

void Set::debug()
{
	printf("Set::debug: start\n");
	for (unsigned char i = 0; i < lineNumber; i++)
	{
		printf("Set::debug: line %d\n", i);
		lines[i].debug();
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

	//�Զ�ƽ��
	unsigned short target = totolWeight / lineNumber;
	unsigned short lineWeight = 0;
	signed short* delta = new signed short[lineNumber]; //��destructWeight����
	signed balanceWeight = 0;

	Deconstructer destructWeight{ [&delta]()->void { delete[] delta; delta = nullptr; } };

	Coordinates negtiveCoordinates;
	Coordinates positiveCoordinates;

	//���Ҳ�ֵ
	for (unsigned char i = 0; i < lineNumber; i++)
		delta[i] = target - lines[i].getTotolWeight();

	//����ƽ��
	unsigned char negtive = 0;
	unsigned char positive = 0;
	for (negtive = 0; negtive < lineNumber; negtive++)
	{
		if (delta[negtive] >= 0) continue; //Ѱ�Ҹ�ֵ
		for (positive = 0; positive < lineNumber; positive++)
		{
			if (delta[positive] <= 0) continue; //Ѱ����ֵ
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
				//ʧ��
				//Ѱ����һ����ܵ���ֵ
				continue;
			}
			//delta[Negtive] += balanceWeight;
			//delta[j] -= balanceWeight;

		}

		if (delta[negtive] < 0 || delta[positive] > 2)
		{
			//�˴���������ʧ��
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

	printf("Set::swap: swap start\n");

	debug();

	for (unsigned char i = 0; i < lineNumber; i++)
	{
		printf("Set::swap: swap in %d line\n", i);

		//�����Լ���ÿһ��
		//Coordinates& lineCoordinate = coordinates[i];
		//unsigned char& lineCoordinateCount = lineCooridinateNumber[i];

		//����Ǳ�ڵĽ������� 1���Լ�����
		coordinateAviliableCount = totolCoordinateNumber - lineCooridinateNumber[i] + 1;
		if (coordinateAviliableCount == 0) continue;
		
		//���Խ���
		for (unsigned char j = 0; j < lineCooridinateNumber[i]; j++)
		{
			//����ÿһ���ж���ĵ�

			//���ɽ���Ŀ�� 
			randIndex = ::rand() % coordinateAviliableCount; //�������Լ����е�
			if (randIndex == 0) continue; //�Լ������ö�
			else randIndex--;

			//Ѱ����������
			if (i == 0) lineIndex = 1; //�����Լ�
			else lineIndex = 0;
			while (randIndex >= lineCooridinateNumber[lineIndex])
			{
				randIndex -= lineCooridinateNumber[lineIndex];
				lineIndex++;
				if (i == lineIndex) lineIndex++; //�����Լ�
			}
			targetIndex = (unsigned char)randIndex; //��ʱrandIndexһ������unsigned char

			//��������
			lines[i].exchange(coordinates[i][j], lines[lineIndex], coordinates[lineIndex][targetIndex]);

			printf("Set::swap: exchange at [%d][%d] in %d elements\n\t\t  with [%d][%d] in %d elements\n", i, coordinates[i][j].position, coordinates[i][j].lenght, lineIndex, coordinates[lineIndex][targetIndex].position, coordinates[lineIndex][targetIndex].lenght);

			//���¼�����ߵĽ�������
			totolCoordinateNumber -= lineCooridinateNumber[i];
			coordinates[i] = lines[i].find(weight);
			lineCooridinateNumber[i] = coordinates[i].getNumber();
			totolCoordinateNumber += lineCooridinateNumber[i];

			totolCoordinateNumber -= lineCooridinateNumber[lineIndex];
			coordinates[lineIndex] = lines[lineIndex].find(weight);
			lineCooridinateNumber[lineIndex] = coordinates[lineIndex].getNumber();
			totolCoordinateNumber += lineCooridinateNumber[lineIndex];

			coordinateAviliableCount = totolCoordinateNumber - lineCooridinateNumber[i];

			debug();

			//[optimize][�Ż�]
			//�˴���find����Ѱ�����н�
			//���Զ���Ϊ�ض��⣬ͬʱ��������ѡ������������ظ���
			//���Ż��Ժ���˵������������Ӳ��Ǵ�����
		}
	}

	debug();

	printf("Set::swap: swap finish\n");
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

void ElementLine::load(Line& line)
{
	elementNumber = line.getTotolWeight();
	unsigned char linePointNumber = line.getNumber();
	unsigned short index = 0;

	if (elements != nullptr) delete[] elements;
	elements = new Element[elementNumber];

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
	sf::Vector2f delta = { size.x,size.y / elementNumber };

	for (unsigned char i = 0; i < elementNumber; i++)
	{
		elements[i].setPosition(position);
		//elements[i].setPosition({ (float)(::rand() % 1000), (float)(::rand() % 1000) });
		position.y += delta.y;
	}
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
	lineNumber = set.getLineNumber();

	delete[] elementLines;
	elementLines = new ElementLine[lineNumber];
	for (unsigned char i = 0; i < lineNumber; i++)
		elementLines[i].load(set[i]);
	arrange();
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
	sf::Vector2f delta = { size.x / lineNumber,size.y };

	for (unsigned char i = 0; i < lineNumber; i++)
	{
		elementLines[i].setPosition(position);
		elementLines[i].setSize(delta);
		position.x += delta.x;
	}
}
