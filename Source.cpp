#include <iostream>
#include <vector>

class Point {
public:
	Point() {};
	Point(float);
	Point(float, float);
	float getX() const;
	float getY() const;
	void printPoint() const;
private:
	float x = 0;
	float y = 0;
};

class Vector {
public:
	Vector() {};
	Vector(float);
	Vector(Point, Point);
	float length() const;
	Point coordinates() const;
	void printVector() const;
private:
	Point begin = 0;
	Point end = 0;
};

class GroupOfPoints {
public: 
	GroupOfPoints() {};
	GroupOfPoints(std::vector<Point>);
	std::vector<Point> getPoints() const;
	void printGroupOfPoints() const;
private:
	std::vector<Point> points;
};

class GroupOfVectors {
public:
	GroupOfVectors() {};
	GroupOfVectors(std::vector<Vector>);
	std::vector<Vector> getVectors() const;
	void printGroupOfVectors() const;
private: 
	std::vector<Vector> vectors;
};

class FindLinearEnvelope;

class LinearEnvelope {
public:
	LinearEnvelope() {};
	LinearEnvelope(GroupOfPoints);
	static void setFindLinearEnvelope(FindLinearEnvelope*);
	GroupOfVectors findLinearEnvelopeFunc() const;
	void printLinearEnvelope() const;
private:
	GroupOfPoints points;
	static FindLinearEnvelope* findLinearEnvelope;
};

class FindLinearEnvelope {
public:
	virtual GroupOfVectors findLinearEnvelope(GroupOfPoints) = 0;
};

class Kirkpatrick : public FindLinearEnvelope {
public: 
	GroupOfVectors findLinearEnvelope(GroupOfPoints);
};

std::vector<GroupOfPoints> pocketSortLinearEnvelope();

FindLinearEnvelope* LinearEnvelope::findLinearEnvelope = new Kirkpatrick();

int main() {
	Point p1(1, 2), p2(2,3), p3(3,4);
	GroupOfPoints gop(std::vector<Point>{p1, p2, p3});
	LinearEnvelope le(gop);
	GroupOfVectors gov = le.findLinearEnvelopeFunc();
	GroupOfVectors gov1 = gov;
	gov1.printGroupOfVectors();
}

Point::Point(float a) : x(a), y(a) {};

Point::Point(float x, float y) : x(x), y(y) {};

float Point::getX() const{
	return x;
}

float Point::getY() const{
	return y;
}

void Point::printPoint() const{
	std::cout << "(" << x << "; " << y << ")" << std::endl;
}

Vector::Vector(float a) : begin(a), end(a) {};

Vector::Vector(Point begin, Point end) : begin(begin), end(end) {};

float Vector::length() const {
	Point vectorCoordinates = coordinates();
	float xCoordinate = vectorCoordinates.getX();
	float yCoordinate = vectorCoordinates.getY();
	return sqrt(pow(xCoordinate, 2) + pow(yCoordinate, 2));
}

Point Vector::coordinates() const {
	float beginX = begin.getX();
	float beginY = begin.getY();
	float endX = end.getX();
	float endY = end.getY();
	return Point(endX - beginX, endY - beginY);
}

void Vector::printVector() const {
	std::cout << "Begin: ";
	begin.printPoint();
	std::cout << "End:   ";
	end.printPoint();
}

GroupOfPoints::GroupOfPoints(std::vector<Point> groupOfPoints) : points(groupOfPoints) {};

std::vector<Point> GroupOfPoints::getPoints() const {
	return points;
}

void GroupOfPoints::printGroupOfPoints() const {
	for (int i = 0; i < points.size(); ++i) {
		std::cout << "Point " << i + 1 << ": " << std::endl;
		points.at(i).printPoint();
	}
}

GroupOfVectors::GroupOfVectors(std::vector<Vector> groupOfVectors) : vectors(groupOfVectors) {};

std::vector<Vector> GroupOfVectors::getVectors() const {
	return vectors;
}

void GroupOfVectors::printGroupOfVectors() const {
	for (int i = 0; i < vectors.size(); ++i) {
		std::cout << "Vector " << i + 1 << ": " << std::endl;
		vectors.at(i).printVector();
	}
}

LinearEnvelope::LinearEnvelope(GroupOfPoints groupOfPoints) : points(groupOfPoints) {};

void LinearEnvelope::setFindLinearEnvelope(FindLinearEnvelope* pfindLinearEnvelope) {
	findLinearEnvelope = pfindLinearEnvelope;
}

GroupOfVectors LinearEnvelope::findLinearEnvelopeFunc() const {
	return findLinearEnvelope->findLinearEnvelope(this->points);
}

GroupOfVectors Kirkpatrick::findLinearEnvelope(GroupOfPoints) {
	std::cout << "Kirkpatrick: " << std::endl;
	std::vector<Vector>vec = { Vector(3.) };
	return GroupOfVectors(vec);
}