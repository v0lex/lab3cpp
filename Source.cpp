#include <iostream>
#include <vector>
#include <set>
#include<algorithm>

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
	void add(const Point&);
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
	GroupOfPoints getPoints() const;
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

std::vector<GroupOfPoints> pocketSortLinearEnvelope(LinearEnvelope);
Point getLeftInGroup(GroupOfPoints);
Point getRightInGroup(GroupOfPoints);
std::vector<Point> getLeftsInLinearEnvelope(std::vector<GroupOfPoints>);
std::vector<Point> getRightsInLinearEnvelope(std::vector<GroupOfPoints>);
std::vector<Vector> getLeftPartOfEnvelope(std::vector<Point>);
std::vector<Vector> getRightPartOfEnvelope(std::vector<Point>);
float getScalar(Vector v1, Vector v2);
float getAngle(Vector v1, Vector v2);

FindLinearEnvelope* LinearEnvelope::findLinearEnvelope = new Kirkpatrick();

int main() {
	Point p1(1, 2), p2(2,1), p3(3,2);
	GroupOfPoints gop(std::vector<Point>{p1, p2, p3});
	LinearEnvelope le(gop);
	std::vector<GroupOfPoints> vec = pocketSortLinearEnvelope(le);
	for (GroupOfPoints gop : vec) {
		gop.printGroupOfPoints();
	}
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

void GroupOfPoints::add(const Point& p) {
	points.push_back(p);
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

GroupOfPoints LinearEnvelope::getPoints() const {
	return points;
}

GroupOfVectors Kirkpatrick::findLinearEnvelope(GroupOfPoints) {
	std::cout << "Kirkpatrick: " << std::endl;
	std::vector<Vector>vec = { Vector(3.) };
	return GroupOfVectors(vec);
}

std::vector<GroupOfPoints> pocketSortLinearEnvelope(LinearEnvelope le) {
	GroupOfPoints startedPoints = le.getPoints();
	std::set<int> yCoordinates;
	for (Point p : startedPoints.getPoints()) {
		yCoordinates.insert((int)p.getY());
	}
	std::vector<GroupOfPoints> sortedPoints;
	sortedPoints.resize(*std::max_element(yCoordinates.begin(), yCoordinates.end()));
	for (Point p : startedPoints.getPoints()) {
		sortedPoints.at(p.getY() - 1).add(p);
	}
	return sortedPoints;
}

std::vector<Point> getLeftsInLinearEnvelope(std::vector<GroupOfPoints> vgop) {
	std::vector<Point> lefts;
	for (GroupOfPoints gop : vgop) {
		lefts.push_back(getLeftInGroup(gop));
	}
	return lefts;
}

Point getLeftInGroup(GroupOfPoints gop) {
	std::vector<Point> gopPoints = gop.getPoints();
	Point left = gopPoints.at(0);
	for (int i = 1; i < gopPoints.size(); ++i) {
		if (gopPoints.at(i).getX() < left.getX()) {
			left = gopPoints.at(i);
		}
	}
	return left;
}

std::vector<Point> getRightsInLinearEnvelope(std::vector<GroupOfPoints> vgop) {
	std::vector<Point> rights;
	for (GroupOfPoints gop : vgop) {
		rights.push_back(getRightInGroup(gop));
	}
	return rights;
}

Point getRightInGroup(GroupOfPoints gop) {
	std::vector<Point> gopPoints = gop.getPoints();
	Point right = gopPoints.at(0);
	for (int i = 1; i < gopPoints.size(); ++i) {
		if (gopPoints.at(i).getX() > right.getX()) {
			right = gopPoints.at(i);
		}
	}
	return right;
}

std::vector<Vector> getLeftPartOfEnvelope(std::vector<Point> vp) {
	
}

float getScalar(Vector v1, Vector v2) {
	float v1X = v1.coordinates().getX();
	float v1Y = v1.coordinates().getY();
	float v2X = v2.coordinates().getX();
	float v2Y = v2.coordinates().getY();
	return v1X * v2X + v1Y * v2Y;
}

float getAngle(Vector v1, Vector v2) {
	return acos((getScalar(v1, v2)) / (v1.length() * v2.length()));
}

