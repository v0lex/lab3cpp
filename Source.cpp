#include <iostream>

class Point {
public:
	Point();
	Point(float);
	Point(float, float);
	float getX();
	float getY();
	void printPoint();
private:
	float x = 0;
	float y = 0;
};

class Vector {
public:
	Vector();
	Vector(float);
	Vector(Point, Point);
	float length();
	Point coordinates();
	void printVector();
private:
	Point begin = 0;
	Point end = 0;
};

Point::Point(float a) : x(a), y(a) {};

Point::Point(float x, float y) : x(x), y(y) {};

float Point::getX() {
	return x;
}

float Point::getY() {
	return y;
}

void Point::printPoint() {
	std::cout << "(" << x << "; " << y << ")" << std::endl;
}

Vector::Vector(float a) : begin(a), end(a) {};

Vector::Vector(Point begin, Point end) : begin(begin), end(end) {};

float Vector::length() {
	Point vectorCoordinates = coordinates();
	float vectorXCoordinate = vectorCoordinates.getX();
	float vectorYCoordinate = vectorCoordinates.getY();
	return sqrt(pow(vectorXCoordinate, 2) + pow(vectorYCoordinate, 2));
}

Point Vector::coordinates() {
	float beginX = begin.getX();
	float beginY = begin.getY();
	float endX = end.getX();
	float endY = end.getY();
	return Point(endX - beginX, endY - beginY);
}

void Vector::printVector() {
	std::cout << "Begin: ";
	begin.printPoint();
	std::cout << "End: ";
	end.printPoint();
}
